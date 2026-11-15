#!/usr/bin/env python3
"""Export CodeWarrior symbol maps for the built game files, in the format of the JP demo's dbg/*.MAP files.

For each version, builds with the linker's own map output (configure.py --map, which also lists the functions the
linker strips as UNUSED) and copies the maps next to each other:

    build/maps/<version>/MarioSt.MAP        main program (main.dol), absolute addresses
    build/maps/<version>/<rel>.plf.MAP      one per REL; every section starts at address 0

The maps come straight from mwldeppc, with library members named "archive.a source" as in the original maps
(e.g. "os.a OSInit.c", no trailing space; plain objects keep the linker's "x.o " with one trailing space, as in the
original maps). The build links the objects directly, so the member names come from build.ninja (each object's
source file); objects whose file name occurs twice in a link (main.o) are told apart by link order. In the
"Memory map" block the File Offset column gives the section's offset in the shipped file (main.dol or <rel>.rel,
from that file's own section table) rather than in the linker's intermediate ELF, the empty debug-section lines are
left out, and the block has two empty lines above and below it. Sections without file data (.bss, .sbss, .sbss2)
get the offset where the file data before them ends (in a REL rounded up to the header's bss alignment). Empty
section layout blocks the original maps print but our link scripts do not produce (the DOL's extab and
extabindex; .ctors/.dtors in a REL if ever missing) are added, with their Memory map rows for the DOL.

Names: most are the original names; compiler-generated objects (string literals, float constants) carry the
compiler's labels (@N), whose numbers depend on header contents and need not match Nintendo's. Some names were
made up by this project; docs/NAME_PROVENANCE.md lists them.

Each map starts with one stamp line naming the ttyd commit it was generated from (and "with local changes" when
the working tree differed from it); tools/MAPS_CHANGELOG.md says which pushes made older maps obsolete.

The build outputs are unchanged by --map (the map is a side file). Afterwards the tree is configured again for the
last version without --map.

usage: python tools/export_maps.py [--version G8ME01 ...] [--out DIR] [--no-build] [--ninja PATH]
"""
import argparse
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
VERSIONS = ["G8ME01", "G8MJ01", "G8MP01", "G8ME01_DEMO", "G8MJ01_DEMO", "G8ME01_DEMO2", "G8MP01_DEMO"]
NOFILE = (b".bss", b".sbss", b".sbss2")


def dol_sections(data):
    """[(address, size, file offset)] for the DOL's 7 text and 11 data sections."""
    out = []
    for i in range(18):
        off = struct.unpack(">I", data[i * 4:i * 4 + 4])[0]
        addr = struct.unpack(">I", data[0x48 + i * 4:0x4C + i * 4])[0]
        size = struct.unpack(">I", data[0x90 + i * 4:0x94 + i * 4])[0]
        if off and size:
            out.append((addr, size, off))
    return out


def rel_sections(data):
    """[(size, file offset)] for the REL's section table (offset 0 for .bss)."""
    n, tab = struct.unpack(">II", data[0xC:0x14])
    out = []
    for i in range(n):
        off, size = struct.unpack(">II", data[tab + 8 * i:tab + 8 * i + 8])
        out.append((size, off & ~1))
    return out


def rewrite_memory_map(text, file_offset_of, nofile_align=1):
    """Replace File Offset values in the 'Memory map:' block, drop empty debug lines, and leave exactly two empty
    lines above and below the block. file_offset_of(name, address, size) returns (offset, size stored in the
    shipped file), or None for no file data. Sections without file data get the end of the file data before them,
    rounded up to nofile_align (a REL's .bss: the REL header's bss alignment, the value ttyd-utils gives)."""
    nl = b"\r\n" if b"\r\n" in text else b"\n"
    lines = text.split(nl)
    try:
        start = next(i for i, l in enumerate(lines) if l.startswith(b"Memory map:"))
    except StopIteration:
        return text
    head = lines[:start]
    while head and not head[-1].strip():
        head.pop()
    block = lines[start:start + 3]
    end_of_data = 0
    i = start + 3
    while i < len(lines) and lines[i].strip():
        parts = lines[i].split()
        if len(parts) == 4:
            name, addr, size, _ = parts
            found = None if name in NOFILE else file_offset_of(name, int(addr, 16), int(size, 16))
            if found is None:
                off = (end_of_data + nofile_align - 1) // nofile_align * nofile_align
            else:
                off = found[0]
                end_of_data = max(end_of_data, found[0] + found[1])
            block.append(lines[i][:lines[i].rindex(parts[3])] + b"%08x" % off)
        elif not parts[0].startswith(b".debug") and parts[0] != b".line":
            block.append(lines[i])
        i += 1
    tail = lines[i:]
    while tail and not tail[0].strip():
        tail.pop(0)
    return nl.join(head + [b"", b""] + block + [b"", b""] + tail)


def source_stamp():
    """The stamp line: commit hash and date of the checkout the maps were built from."""
    def git(*a):
        return subprocess.run(["git", *a], cwd=ROOT, capture_output=True, text=True).stdout.strip()
    rev = git("rev-parse", "--short=9", "HEAD") or "unknown"
    date = git("log", "-1", "--format=%cs", "HEAD") or "unknown date"
    dirty = " with local changes" if git("status", "--porcelain", "--untracked-files=no") else ""
    return f"generated from ttyd commit {rev} ({date}){dirty}"


def stamped(text, stamp):
    nl = b"\r\n" if b"\r\n" in text else b"\n"
    return stamp.encode("ascii") + nl + text


# Library sources whose name in the original maps differs from our file name: gdev_main.c was renamed here (the
# original TRK has two main.c members, ddh and gdev), and __ppc_eabi_init was a C++ file in the SDK.
SOURCE_NAME_OVERRIDES = {"gdev_main.c": "main.c", "__ppc_eabi_init.c": "__ppc_eabi_init.cpp"}


def ninja_statements():
    """[(outputs, rule, inputs)] from build.ninja, continuation lines joined (implicit/order-only inputs dropped)."""
    text = (ROOT / "build.ninja").read_text(encoding="utf-8", errors="replace").replace("$\n", " ")
    out = []
    for line in text.splitlines():
        if not line.startswith("build "):
            continue
        head, _, tail = line[6:].partition(": ")
        outs = head.replace(" | ", " ").split()
        parts = tail.split()
        if not parts:
            continue
        ins = []
        for t in parts[1:]:
            if t in ("|", "||"):
                break
            ins.append(t)
        out.append((outs, parts[0], ins))
    return out


def library_member_name(src):
    """'archive.a source' for a source under libs/, as the original linker maps name library members ("os.a
    OSInit.c"): the archive is the libs/ folder named *.a, else the SDK library folder (libs/dolsdk2004/src/<lib>/ ->
    <lib>.a), else musyx.a. None for game sources."""
    parts = Path(src.replace("\\", "/")).parts
    if not parts or parts[0] != "libs":
        return None
    rel = parts[1:]
    arch = next((part for part in rel if part.endswith(".a")), None)
    if arch is None and rel[0] == "dolsdk2004" and len(rel) > 3:
        arch = rel[2] + ".a"
    if arch is None and rel[0] == "musyx":
        arch = "musyx.a"
    if arch is None:
        return None
    name = parts[-1]
    return f"{arch} {SOURCE_NAME_OVERRIDES.get(name, name)}"


def elf_section_names(path):
    """Section names of an ELF32 big-endian object."""
    d = path.read_bytes()
    shoff, = struct.unpack(">I", d[0x20:0x24])
    shentsize, shnum, shstrndx = struct.unpack(">HHH", d[0x2E:0x34])
    def sh(i):
        return struct.unpack(">IIIIII", d[shoff + i * shentsize:shoff + i * shentsize + 24])
    stroff = sh(shstrndx)[4]
    names = set()
    for i in range(shnum):
        n = sh(i)[0]
        names.add(d[stroff + n:d.index(b"\0", stroff + n)].decode("latin1"))
    return names


class LinkObjects:
    """The objects of one link (from build.ninja), in link order, with the map name of each: 'archive.a source'
    for library members, the object's file name for game objects."""

    def __init__(self, statements, map_path):
        src_of = {}
        for outs, rule, ins in statements:
            if len(outs) == 1 and outs[0].endswith(".o") and ins:
                src_of[outs[0]] = ins[0]
        target = str(map_path.relative_to(ROOT)).replace("/", "\\")
        objs = next((ins for outs, rule, ins in statements if target in outs), None)
        if objs is None:
            sys.exit(f"{map_path}: no link statement in build.ninja")
        self.paths = [o for o in objs if o.endswith(".o")]
        self.name = {}
        self.by_base = {}
        for o in self.paths:
            lib = library_member_name(src_of.get(o, ""))
            self.name[o] = lib.encode() if lib else None
            self.by_base.setdefault(Path(o.replace("\\", "/")).name.encode(), []).append(o)
        self._sections = {}

    def has_section(self, obj, section):
        if obj not in self._sections:
            try:
                self._sections[obj] = elf_section_names(ROOT / obj)
            except (OSError, struct.error, ValueError):
                self._sections[obj] = None
        names = self._sections[obj]
        return names is None or section in names


FILE_AT_END = re.compile(rb"^(.*?)([ \t])([A-Za-z0-9_.]+\.o)( ?)(\r?)$")
LAYOUT_HEAD = re.compile(rb"^(\S+) section layout")
TREE_SYM = re.compile(rb"^\s*\d+\] (\S+) ")
LAYOUT_SYM = re.compile(rb"^  (?:[0-9a-f]{8} [0-9a-f]{6} [0-9a-f]{8} (?:[ \d]\d )?|UNUSED   [0-9a-f]{6} \.{8} )(\S+)")


def name_libraries(text, objs):
    """Name each library member the way the original maps do ("archive.a source", no trailing space; the linker
    writes plain objects as "x.o " with one trailing space, which the original maps have too). Objects whose file
    name occurs more than once in the link (main.o: the game's and TRK's) are told apart by link order: within a
    section layout, each new object group takes the next object of that name (in link order) that has the section;
    link-tree lines take the object that defines the symbol."""
    lines = text.split(b"\n")
    which = [None] * len(lines)  # resolved object path per line, for names that are not unique
    sym_objs = {}
    section = None
    pos = {}
    prev = None
    for i, line in enumerate(lines):
        h = LAYOUT_HEAD.match(line)
        if h:
            section, pos, prev = h.group(1).decode("latin1"), {}, None
            continue
        m = FILE_AT_END.match(line)
        if not m:
            if not line.strip():
                prev = None
            continue
        base = m.group(3)
        cands = objs.by_base.get(base, [])
        if section is None or len(cands) < 2:
            continue
        group_start = prev is None or prev[0] != base or re.match(
            rb"^  [0-9a-f]{8} [0-9a-f]{6} [0-9a-f]{8}  1 " + re.escape(section.encode("latin1")) + rb" \t", line)
        if group_start:
            k = pos.get(base, -1) + 1
            j = next((j for j in range(k, len(cands)) if objs.has_section(cands[j], section)), min(k, len(cands) - 1))
            pos[base] = j
            prev = (base, cands[j])
        which[i] = prev[1]
        s = LAYOUT_SYM.match(line)
        if s:
            sym_objs.setdefault((base, s.group(1)), set()).add(prev[1])
    last = {}
    out = []
    for i, line in enumerate(lines):
        m = FILE_AT_END.match(line)
        if not m:
            out.append(line)
            continue
        base = m.group(3)
        cands = objs.by_base.get(base, [])
        obj = which[i]
        if obj is None and len(cands) == 1:
            obj = cands[0]
        elif obj is None and cands:
            t = TREE_SYM.match(line)
            found = sym_objs.get((base, t.group(1)), set()) if t else set()
            obj = next(iter(found)) if len(found) == 1 else last.get(base)
            if obj is not None:
                last[base] = obj
        name = objs.name.get(obj) if obj else None
        if name is None:
            out.append(line)
        else:
            out.append(m.group(1) + m.group(2) + name + m.group(5))
    return b"\n".join(out)


EMPTY_LAYOUT = b"%s section layout\r\n  Starting        Virtual\r\n  address  Size   address\r\n  -----------------------\r\n\r\n\r\n"


def add_empty_layouts(text, wanted, before):
    """Insert an empty 'X section layout' block for each section in `wanted` the map lacks, in front of the
    `before` section's block (the original maps print a block for every section of the link, empty ones too)."""
    nl = b"\r\n" if b"\r\n" in text else b"\n"
    missing = [w for w in wanted if (nl + w + b" section layout" + nl) not in text and
               not text.startswith(w + b" section layout")]
    if not missing:
        return text
    anchor = nl + before + b" section layout" + nl
    at = text.find(anchor)
    if at < 0:
        return text
    blocks = b"".join(EMPTY_LAYOUT.replace(b"\r\n", nl) % w for w in missing)
    return text[:at + len(nl)] + blocks + text[at + len(nl):]


def add_empty_memory_rows(text, names, after):
    """Add a Memory map row (size 0, at the end of the `after` section) for each section in `names` the block
    lacks, as the original DOL map lists its empty extab and extabindex sections."""
    m = re.search(rb"^( +)" + re.escape(after) + rb"  ([0-9a-f]{8}) ([0-9a-f]{8}) ([0-9a-f]{8})(\r?\n)", text, re.M)
    if not m:
        return text
    width = len(m.group(1)) + len(after)
    end_addr = int(m.group(2), 16) + int(m.group(3), 16)
    end_off = int(m.group(4), 16) + int(m.group(3), 16)
    rows = b""
    for n in names:
        if re.search(rb"^ +" + re.escape(n) + rb"  [0-9a-f]{8} ", text, re.M):
            continue
        rows += n.rjust(width) + b"  %08x %08x %08x" % (end_addr, 0, end_off) + m.group(5)
    return text[:m.end()] + rows + text[m.end():]


def export_version(version, out_dir, stamp):
    build = ROOT / "build" / version
    dst = out_dir / version
    dst.mkdir(parents=True, exist_ok=True)
    dol_map = build / "MarioSt.elf.MAP"
    if not dol_map.exists():
        sys.exit(f"{dol_map} missing: build {version} with configure.py --map first")
    dol = (build / "MarioSt.dol").read_bytes()
    secs = dol_sections(dol)

    def dol_offset(name, addr, size):
        for a, s, off in secs:
            if a == addr:
                return off, s
        return None

    statements = ninja_statements()
    text = rewrite_memory_map(dol_map.read_bytes(), dol_offset)
    text = add_empty_layouts(text, [b"extab", b"extabindex"], b".text")
    text = add_empty_memory_rows(text, [b"extab", b"extabindex"], b".init")
    (dst / "MarioSt.MAP").write_bytes(stamped(name_libraries(text, LinkObjects(statements, dol_map)), stamp))
    count = 1
    for rel in sorted(build.glob("*/*.rel")):
        name = rel.stem
        plf_map = rel.parent / f"{name}.plf.MAP"
        if not plf_map.exists():  # the JP demo's tst is linked in two steps
            plf_map = rel.parent / f"{name}.linked.plf.MAP"
        if not plf_map.exists():
            sys.exit(f"{rel}: no linker map next to it")
        rel_data = rel.read_bytes()
        table = rel_sections(rel_data)
        version_field, = struct.unpack(">I", rel_data[0x1C:0x20])
        bss_align = struct.unpack(">I", rel_data[0x44:0x48])[0] if version_field >= 2 else 1
        used = set()

        def rel_offset(name, addr, size, table=table, used=used):
            for k, (s, off) in enumerate(table):
                if k not in used and s == size and off:
                    used.add(k)
                    return off, s
            return None

        text = rewrite_memory_map(plf_map.read_bytes(), rel_offset, max(bss_align, 1))
        text = add_empty_layouts(text, [b".ctors", b".dtors"], b".rodata")
        (dst / f"{name}.plf.MAP").write_bytes(
            stamped(name_libraries(text, LinkObjects(statements, plf_map)), stamp))
        count += 1
    print(f"{version}: {count} maps -> {dst}")


def main():
    p = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    p.add_argument("--version", action="append", choices=VERSIONS, help="version(s) to export (default: all)")
    p.add_argument("--out", type=Path, default=ROOT / "build" / "maps")
    p.add_argument("--no-build", action="store_true", help="only collect maps from an existing --map build")
    p.add_argument("--ninja", default=shutil.which("ninja") or str(Path(sys.executable).parent / "Scripts" / "ninja"))
    args = p.parse_args()
    versions = args.version or VERSIONS
    stamp = source_stamp()
    for v in versions:
        if not args.no_build:
            subprocess.run([sys.executable, "configure.py", "--version", v, "--map"], cwd=ROOT, check=True)
            subprocess.run([args.ninja], cwd=ROOT, check=True)
        export_version(v, args.out, stamp)
    if not args.no_build:
        subprocess.run([sys.executable, "configure.py", "--version", versions[-1]], cwd=ROOT, check=True)


if __name__ == "__main__":
    main()
