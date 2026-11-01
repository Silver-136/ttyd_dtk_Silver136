#!/usr/bin/env python3
"""Drop the relocations against chosen symbols from a partially linked module (.plf).

The Japanese store demo's tst.rel was linked with a reference to tst_nakayama_init_evt left unresolved: that
build's evt_init.c has no such event, and the original linker wrote 0 in its place with no relocation. mwld leaves
the same reference undefined in our tst.plf, but `dtk rel make` refuses undefined symbols. This step runs between
the link and `dtk rel make`: it removes every relocation whose target is one of the listed symbols, so the
relocated field keeps the 0 the linker wrote and rel make sees no reference to the missing symbol.

Usage: drop_relocations.py <in.plf> <out.plf> <symbol list>
The symbol list has one name per line; blank lines and lines starting with # are ignored.
Only the standard library is used. The input must be a 32-bit big-endian ELF relocatable (what mwld -r1 writes).
"""

import struct
import sys
from pathlib import Path

SHT_SYMTAB = 2
SHT_RELA = 4
SECTION_HEADER = ">IIIIIIIIII"  # name, type, flags, addr, offset, size, link, info, addralign, entsize
SYMBOL = ">IIIBBH"  # name, value, size, info, other, shndx
RELA = ">III"  # offset, info, addend


def read_symbol_list(path: Path) -> set:
    names = set()
    for line in path.read_text(encoding="utf-8").splitlines():
        line = line.strip()
        if line and not line.startswith("#"):
            names.add(line)
    return names


def c_string(data: bytes, offset: int) -> str:
    return data[offset : data.index(b"\0", offset)].decode("ascii")


def drop_relocations(elf: bytearray, names: set) -> int:
    """Remove the relocations against `names` in place and return how many were removed."""
    if elf[:4] != b"\x7fELF" or elf[4] != 1 or elf[5] != 2:
        sys.exit("not a 32-bit big-endian ELF file")
    shoff, = struct.unpack_from(">I", elf, 0x20)
    shentsize, shnum = struct.unpack_from(">HH", elf, 0x2E)
    headers = [list(struct.unpack_from(SECTION_HEADER, elf, shoff + i * shentsize)) for i in range(shnum)]

    # Find the symbol table indices of the listed names. A listed name that is not referenced is an error: the
    # list should name only the references the original linker really left unresolved.
    dropped_symbols = {}  # symtab section index -> set of symbol indices
    found = set()
    for index, (_, sh_type, _, _, offset, size, link, _, _, entsize) in enumerate(headers):
        if sh_type != SHT_SYMTAB:
            continue
        strtab_offset = headers[link][4]
        for symbol in range(size // entsize):
            name_offset = struct.unpack_from(SYMBOL, elf, offset + symbol * entsize)[0]
            name = c_string(elf, strtab_offset + name_offset)
            if name in names:
                dropped_symbols.setdefault(index, set()).add(symbol)
                found.add(name)
    missing = names - found
    if missing:
        sys.exit(f"symbols not found: {', '.join(sorted(missing))}")

    # Compact each relocation section without the dropped entries and shrink its size; the bytes left past the
    # new end are unused. The relocated fields stay as the linker wrote them (0, since the symbol is undefined).
    removed = 0
    for index, header in enumerate(headers):
        _, sh_type, _, _, offset, size, link, _, _, entsize = header
        if sh_type != SHT_RELA or link not in dropped_symbols:
            continue
        kept = []
        for entry in range(size // entsize):
            raw = bytes(elf[offset + entry * entsize : offset + (entry + 1) * entsize])
            info = struct.unpack(RELA, raw)[1]
            if info >> 8 in dropped_symbols[link]:
                removed += 1
            else:
                kept.append(raw)
        new_size = len(kept) * entsize
        elf[offset : offset + size] = b"".join(kept) + bytes(size - new_size)
        header[5] = new_size
        struct.pack_into(SECTION_HEADER, elf, shoff + index * shentsize, *header)
    if removed == 0:
        sys.exit("no relocations against the listed symbols")
    return removed


def main() -> None:
    if len(sys.argv) != 4:
        sys.exit(__doc__)
    elf = bytearray(Path(sys.argv[1]).read_bytes())
    drop_relocations(elf, read_symbol_list(Path(sys.argv[3])))
    Path(sys.argv[2]).write_bytes(elf)


if __name__ == "__main__":
    main()
