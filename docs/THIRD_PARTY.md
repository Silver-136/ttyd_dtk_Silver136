# Third-party code and tools

The game links libraries that Nintendo, Metrowerks and others wrote, and one REL carries a copy of SGI's GLU tessellator. This repository contains C reconstructions of that code (no original binaries) so that the whole game builds from source. This page lists where each part comes from and under which terms it is here. The rest of the repository is under the MIT licence in [LICENSE](../LICENSE).

## Libraries in `libs/`

| Folder | What it is | Where it comes from | Licence in this tree |
|---|---|---|---|
| `libs/dolsdk2004/` | Nintendo's Dolphin SDK libraries (OS, GX, DVD, VI, PAD, CARD, AR/ARQ, THP, ...), as built into the game | A decompilation of the archive libraries of the 2004-04-20 Dolphin SDK; its own README is `libs/dolsdk2004/README.md`. It came into this tree with the upstream project's setup (2025) and was completed and extended here. | No licence file of its own |
| `libs/musyx/` | MusyX, the sound library (Factor 5) | The MusyX decompilation by Axiomatic Data Laboratories, extended here | MIT, `libs/musyx/LICENSE` (the licence of the copy taken in 2025; AxioDL/musyx is now published under CC0) |
| `libs/msl/` | Metrowerks Standard Library (C library and math) | Decompiled in the upstream project before this fork (2026, l-tata and LLCOOLT), extended here | This repository's MIT licence |
| `libs/runtime/` | Metrowerks PowerPC EABI runtime (`__mem`, `__va_arg`, `runtime.c`, ...) | Decompiled in the upstream project before this fork (2026, l-tata and LLCOOLT), extended here | This repository's MIT licence |
| `libs/trk/` | Metrowerks Target Resident Kernel, the debugger stub linked into the game | Decompiled in the upstream project before this fork (2026, l-tata and LLCOOLT), extended here. The C of five functions (`support.c` TRKSuppAccessFile, TRKRequestSend, HandleOpenFileSupportRequest; `dolphin_trk.c` TRK__write_aram; `mem_TRK.c` TRK_fill_mem) is adapted from the matched decompilations of [Pikmin 2](https://github.com/projectPiki/pikmin2) and [Animal Crossing](https://github.com/ACreTeam/ac-decomp) (both CC0); the DS_* / DSREPLY_* constant names are the MetroTRK spellings those projects share (see NAME_PROVENANCE.md) | This repository's MIT licence |

The assembly in these folders is code that the original libraries shipped as assembly (for example paired-single math, cache and context routines, the TRK exception vectors). It is reproduced as assembly on purpose.

`src/sdk/` holds SDK demo-library code (`DEMOInit.c`, `DEMOPad.c`) and helpers that the game compiled as part of its own source; they are covered by the repository licence like the rest of `src/`.

## SGI GLU tessellator in `rels/tst/`

The developer test REL `tst.rel` (in the European release and the JP store demo) contains a copy of the GLU polygon tessellator (libtess, by Eric Veach) from the OpenGL Sample Implementation by Silicon Graphics, Inc. Its reconstruction is in `rels/tst/src/glu/` with headers in `rels/tst/include/glu/`.

It is under the SGI Free Software License B, version 2.0. All eleven source files carry the full notice at the top; the headers do not repeat it, and the source files cannot gain or lose lines without changing the build (the tessellator's assertions record source line numbers). The notice:

```
SGI FREE SOFTWARE LICENSE B (Version 2.0, Sept. 18, 2008)
Copyright (C) 1991-2000 Silicon Graphics, Inc. All Rights Reserved.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice including the dates of first publication and
either this permission notice or a reference to
http://oss.sgi.com/projects/FreeB/
shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
SILICON GRAPHICS, INC. BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Except as contained in this notice, the name of Silicon Graphics, Inc.
shall not be used in advertising or otherwise to promote the sale, use or
other dealings in this Software without prior written authorization from
Silicon Graphics, Inc.
```

## Build scripts in `tools/`

`tools/project.py`, `tools/download_tool.py`, `tools/decompctx.py`, `tools/transform_dep.py` and `tools/changes_fmt.py` come from [dtk-template](https://github.com/encounter/dtk-template), the decomp-toolkit project template. `tools/ninja_syntax.py` is from the [ninja](https://github.com/ninja-build/ninja) project (Apache License 2.0, notice in the file). The other scripts (`drop_relocations.py`, `progress_treemap.py`, `update_readme.py`, `ttydasm/`) were written for this project.

## Build-time tools

These are not in the repository. The first build downloads them into `build/`, at the versions pinned in `configure.py` (see [BUILDING.md](BUILDING.md#downloaded-on-the-first-build)); each comes under its own licence from its authors.

| Tool | Used for | Source |
|---|---|---|
| decomp-toolkit (`dtk`) | splitting the original files, linking RELs, the hash check | [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit) |
| objdiff-cli | progress reports (`report.json`) | [encounter/objdiff](https://github.com/encounter/objdiff) |
| sjiswrap | converting UTF-8 sources to Shift-JIS for the compiler | [encounter/sjiswrap](https://github.com/encounter/sjiswrap) |
| Metrowerks CodeWarrior compilers and linker (GC/2.6 for the game and the link, GC/1.2.5n for the SDK, GC/1.3.2 for MusyX) | compiling and linking | the decomp.dev compiler archive (`files.decomp.dev`) |
| GNU binutils for PowerPC | assembling the original assembly routines | [encounter/gc-wii-binutils](https://github.com/encounter/gc-wii-binutils) (a build of [GNU binutils](https://www.gnu.org/software/binutils/)) |
| wibo (Linux) | running the Windows compilers | [decompals/wibo](https://github.com/decompals/wibo) |

`tools/ttydasm` (a Rust program) pulls its Rust crates from crates.io when you build it with Cargo.
