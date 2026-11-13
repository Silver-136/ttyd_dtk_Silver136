#!/usr/bin/env python3
"""Export CodeWarrior symbol maps for the built game files, in the format of the JP demo's dbg/*.MAP files.

For each version, builds with the linker's own map output (configure.py --map, which also lists the functions the
linker strips as UNUSED) and copies the maps next to each other:

    build/maps/<version>/MarioSt.MAP        main program (main.dol), absolute addresses
    build/maps/<version>/<rel>.plf.MAP      one per REL; every section starts at address 0

The maps come straight from mwldeppc. One change: in the "Memory map" block the File Offset column gives the
section's offset in the shipped file (main.dol or <rel>.rel, from that file's own section table) rather than in the
linker's intermediate ELF, the empty debug-section lines are left out, and the block has two empty lines above
and below it. Sections without file data (.bss, .sbss, .sbss2) get the offset where the file data before them ends.

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


def rewrite_memory_map(text, file_offset_of):
    """Replace File Offset values in the 'Memory map:' block, drop empty debug lines, and leave exactly two empty
    lines above and below the block. file_offset_of(name, address, size) returns (offset, size stored in the
    shipped file), or None for no file data."""
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
                off = end_of_data
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
    nl = b"
" if b"
" in text else b"
"
    return stamp.encode("ascii") + nl + text


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

    (dst / "MarioSt.MAP").write_bytes(stamped(rewrite_memory_map(dol_map.read_bytes(), dol_offset), stamp))
    count = 1
    for rel in sorted(build.glob("*/*.rel")):
        name = rel.stem
        plf_map = rel.parent / f"{name}.plf.MAP"
        if not plf_map.exists():  # the JP demo's tst is linked in two steps
            plf_map = rel.parent / f"{name}.linked.plf.MAP"
        if not plf_map.exists():
            sys.exit(f"{rel}: no linker map next to it")
        table = rel_sections(rel.read_bytes())
        used = set()

        def rel_offset(name, addr, size, table=table, used=used):
            for k, (s, off) in enumerate(table):
                if k not in used and s == size and off:
                    used.add(k)
                    return off, s
            return None

        (dst / f"{name}.plf.MAP").write_bytes(stamped(rewrite_memory_map(plf_map.read_bytes(), rel_offset), stamp))
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
