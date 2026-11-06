# Versions

Adapted from the page of the same name in llc00lt's tree (LLCOOLT/ttyd), used with permission, and checked against this tree.

One source tree builds seven versions of the game. This page says what differs between them and how the sources express those differences. The build ids and flags come from `configure.py`; the macros are in `include/version.h`.

| Version | `BUILD_VERSION` | What it is | Code line |
|---|---|---|---|
| `G8ME01_DEMO` | 0 | US demo build `MarioSt_MN7_NOA_20040619a` (June 2004), shipped as `zz_PaperMario2_game.tgc` on the Interactive Multi-Game Demo Disc Version 17 (USA) (also 18) | JP plus demo changes |
| `G8MJ01_DEMO` | 1 | Japanese store demo, shipped as `files/local/tgc/PaperMarioRPG.tgc` on Gekkan Nintendo Tentou Demo 2004.7.1 (Japan) (also 2004.8.1 and 2004.9.1) | the demo code, a little later than the US demo |
| `G8MJ01` | 2 | Japanese release, Rev 0 | JP |
| `G8ME01` | 3 | North American release, Rev 0 | US |
| `G8ME01_DEMO2` | 4 | US demo built on 18 September 2004, shipped as `zz_PaperMario2_game.tgc` on the Interactive Multi-Game Demo Disc Version 19 (USA) (also 20-22 and 35) | US plus a few demo changes |
| `G8MP01_DEMO` | 5 | European demo built on 23 September 2004, shipped as `zz_PaperMario2_PALDEMO_20040924.tgc` on the Interactive Multi-Game Demo Disk - November 2004 (Europe) | PAL plus the same demo changes |
| `G8MP01` | 6 | European release, Rev 0: English, German, French, Spanish and Italian, 50 Hz and 60 Hz | US plus PAL changes |

The `BUILD_VERSION` number is the position in the `VERSIONS` list of `configure.py` and is the same as the `VERSION` value in `include/version.h`. The default build is `G8MJ01`.

## Preprocessor macros

`configure.py` passes `-DVERSION_<id>` to every unit and `--defsym BUILD_VERSION=<n>` to assembly files. The game code (`src/`, `rels/`, `include/`) does not test the per-version macros directly: `include/version.h` turns them into `VERSION`, one of seven ordered values, and every version condition compares `VERSION` with them.

| Value | Version |
|---|---|
| `VERSION_DEMO_US_1` (0) | `G8ME01_DEMO` |
| `VERSION_DEMO_JP` (1) | `G8MJ01_DEMO` |
| `VERSION_JP` (2) | `G8MJ01` |
| `VERSION_US` (3) | `G8ME01` |
| `VERSION_DEMO_US_2` (4) | `G8ME01_DEMO2` |
| `VERSION_DEMO_EU` (5) | `G8MP01_DEMO` |
| `VERSION_EU` (6) | `G8MP01` |

The order follows the history of the code, so one comparison names a code line. The June US demo comes before the Japanese store demo. The two September 2004 demos sit between the US and the European release they were built from.

- **US, PAL and the September demos: `#if VERSION >= VERSION_US`.** The European release was made from the US code, so everything US changed applies to PAL as well, and the two September demos were built from the US and PAL code.
- **PAL and the European demo: `#if VERSION >= VERSION_DEMO_EU`**: what PAL changed on top of US, usually nested in or next to a US block. What PAL changed after the European demo is `#if VERSION == VERSION_EU`. The opposite of the first is `#if VERSION < VERSION_DEMO_EU`.
- **US and the September US demo: `#if VERSION == VERSION_US || VERSION == VERSION_DEMO_US_2`**: what US has and PAL replaced.
- **JP and both early demos: `#if VERSION <= VERSION_JP`.** The June US demo is in English, but its code is the Japanese code from before localization.
- **Both early demos: `#if VERSION <= VERSION_DEMO_JP`**, and the releases and the September demos: `#if VERSION >= VERSION_JP`. The two early demos are one code line: the Japanese demo is the demo code a little later, so a demo difference covers both unless the Japanese demo's own code shows otherwise.
- **June US demo only: `#if VERSION == VERSION_DEMO_US_1`**: what the June US demo has on top of the demo code, mostly for its English text.
- **One version: `#if VERSION == VERSION_JP`** (and the same for the others).
- **Every demo: `#if VERSION <= VERSION_DEMO_JP || VERSION == VERSION_DEMO_US_2 || VERSION == VERSION_DEMO_EU`**, and the three releases only: `#if VERSION == VERSION_JP || VERSION == VERSION_US || VERSION == VERSION_EU`.
- **The September demos: `#if VERSION == VERSION_DEMO_US_2 || VERSION == VERSION_DEMO_EU`**.
- **Every version but one: `#if VERSION != VERSION_JP`** covers the early demos and the US code line, since JP is the only version without some changes they share.

In comments, "the demo" and "the early demos" mean the June US demo and the Japanese store demo (the code both share), "the September demos" means `G8ME01_DEMO2` and `G8MP01_DEMO`. "The releases" and "retail" on code under `VERSION >= VERSION_JP` mean the release code line, which the September demos build as well.

Each file that tests `VERSION` includes `version.h` itself. An undefined `VERSION` would compare as 0 in `#if`, so `version.h` stops the build with an error when no `VERSION_<id>` macro is defined. Do not test an ordered value with `#ifdef` or `defined()`: it is always true.

The library code under `libs/` tests the per-version macros directly. For it, `configure.py` also defines `VERSION_G8ME01` when building `G8MP01` and `G8ME01_DEMO2`, and both `VERSION_G8ME01` and `VERSION_G8MP01` when building `G8MP01_DEMO`.

Assembly files get the same index as `BUILD_VERSION` through `--defsym` (for example the TRK stubs in `libs/trk/src/TRK_MINNOW_DOLPHIN.a/targsupp.s`).

### Helpers in `include/version.h`

- `VERSION_PICK(jp, us)`: the JP value, or the US value when `VERSION >= VERSION_US` (US, PAL and the September demos). The early demos get the JP value. Used for constants that changed between the releases, such as struct sizes, offsets, counts and literal values.
- `FPS_FRAMES(n)`: an integer frame count. In the US code line it is `n * gp->framerate / 60`, so a count written for 60 Hz becomes the right count at 50 Hz; in JP and the early demos it is just `n`.
- `FPS_EASE(n)` and `FPS_EASE_D(us, jp)`: the same scaling for easing divisors. `FPS_EASE_D` covers sites that JP writes with a double literal where US writes a float; note that it takes the US value first, unlike `VERSION_PICK`.

`gp->framerate` is set at boot in `src/mario/mariost.c`. It is always 60 in JP and the early demos. In US it is 60 or 50 from the video format. PAL and the European demo also use 60 for EURGB60.

## What differs

### JP to US (`G8ME01`)

- **Text.** English text, fonts and text layout. Many message and window routines changed with it (`src/driver/msgdrv.c`, `src/manager/fontmgr.c`, `src/manager/winmgr.c`, the `src/window/` files).
- **Frame rate.** Timers, frame counts and easing go through `FPS_FRAMES` and `FPS_EASE`, so the same code runs at 50 Hz in Europe.
- **Data structures.** Some structs gained members; those members sit inside `#if VERSION >= VERSION_US` in the struct, so field accesses are right in every version. Offsets that moved are written with `VERSION_PICK`.
- **Memory.** JP and the early demos have a separate battle heap (`HEAP_BATTLE`). In US and later, battle memory comes from a 0x64000-byte sub-heap of the map heap, and map allocations take the heap base as a parameter (`src/memory.c`, `src/battle/battle.c`).
- **Bug fixes.** A few JP bugs are fixed, for example the key check in `src/event/evt_mobj.c` (see [KNOWN_GAME_BUGS.md](KNOWN_GAME_BUGS.md)).
- **RELs.** `dig` and `qiz` are not on the US disc.

### US to PAL (`G8MP01`)

- **Five languages** chosen from the console language (English, German, French, Spanish, Italian; Dutch falls back to English), a 50/60 Hz choice with PAL and EURGB60 render modes (`src/mario/mariost.c`), and PAL wording of the system messages (`src/romfont.c`).
- **Battle and story changes** found in the European code, for example in `include/battle/battle_event_cmd.h`, `include/battle/battle_unit.h` and `include/evt/evt_sub.h`. Search for `VERSION_DEMO_EU` and `VERSION_EU` for the full set.
- **RELs.** `dig` and `qiz` are back, and `tst.rel`, a developer test REL with test battles and test maps, ships only in this release and the Japanese demo (`rels/tst/`).

### The early demos compared with JP (`G8MJ01_DEMO`, `G8ME01_DEMO`)

- **Kiosk behaviour.** After a minute without input the early demos end the session (the September demos do the same): in the field or in a battle they raise the kiosk timeout flag, and in any other sequence they reset (`src/mario/mariost.c`). The retail games raise the same flag after two minutes in the field or a battle; it is only read in kiosk mode. The demo has an extra sequence, `seq_demo` (`src/sequence/seq_demo.c`, listed in `src/sequence/seqdef.c`).
- **Older code.** Many functions are earlier versions of the JP code. The differences are marked `VERSION <= VERSION_DEMO_JP` (or `VERSION >= VERSION_JP` for what only the releases and the later demos have) throughout `src/`.
- **Memory.** The smart heap is a fixed 7 MiB block from the default heap (`src/memory.c`).
- **Small-data thresholds.** The main executables of the other versions are compiled with `-sdata 48 -sdata2 6`; the early demos with `-sdata 8 -sdata2 4`. RELs and TRK use `-sdata 0 -sdata2 0` in every version (`configure.py`).
- **Precompiled headers.** Main-executable units get the MSL math constants from `include/msl_main.pch` (the `MAIN_PCH` set in `configure.py`). The early demos keep those constants in a different set of units (`MAIN_PCH_DEMO_ADD`), and a few demo units use `include/msl_math.pch`, the math header without the constants (`MATH_PCH_DEMO`). RELs use `include/msl_rel.pch`.
- **RELs.** The June US demo has six: `gon`, `hei`, `kpa`, `mri`, `nok` and `sys`.

### The Japanese demo compared with the US demo (`G8MJ01_DEMO`)

The Japanese store demo is the demo code a little after the US demo, without the US demo's English text. It has all 31 RELs of the JP release and `tst`, 32 in all. Its `config/G8MJ01_DEMO` comes from the original linker maps of this demo, so its object boundaries are the original ones.

- **Build.** Same small-data thresholds and precompiled-header sets as the June US demo. Every unit links from C. `tst.c` registers `tst_nakayama_init_evt`, which this version's `evt_init.c` does not define. The original linker left that reference as 0 with no relocation, and the build reproduces it by dropping the relocation before `dtk rel make` (`config/G8MJ01_DEMO/unresolved_symbols.txt`, see [BUILDING.md](BUILDING.md)).
- LLCOOLT measured how many main executable functions the two demos share; his figures are in his tree's page and are not repeated here.

### The September 2004 demos (`G8ME01_DEMO2`, `G8MP01_DEMO`)

Both are kiosk builds of the release code. The US one was built a few days after the US release and the European one a few days before the European release (build dates are in `src/R_build.c`).

- **`G8ME01_DEMO2` is the US code**, with the demo changes below and none of PAL's changes.
- **`G8MP01_DEMO` is the PAL code**, except for a few PAL changes made after it: they are the `#if VERSION == VERSION_EU` blocks.
- **The demo changes** are `#if VERSION == VERSION_DEMO_US_2` or `VERSION <= VERSION_DEMO_JP || VERSION == VERSION_DEMO_US_2 || VERSION == VERSION_DEMO_EU` blocks. They include the kiosk timeout in `marioStMain` and the idle timer in `marioStInit` (`src/mario/mariost.c`), the logo sequence (`src/sequence/seq_logo.c`) and, in the September US demo, the attract screens (`src/sequence/seq_e3.c`).
- **Build.** Retail flags (`-sdata 48 -sdata2 6`, the retail precompiled-header sets). `VERSION_DEMO_US_2` and `VERSION_DEMO_EU` come after `VERSION_US`, so both take the US code (`>= VERSION_US`); the European demo also takes PAL's changes (`>= VERSION_DEMO_EU`) except the ones it predates (`== VERSION_EU`). The main executables need a `force_active` list in `config.yml`, since their seven RELs leave many of the full game's functions and data unreferenced.
- **RELs.** `gon`, `gor`, `hei`, `kpa`, `mri`, `nok` and `tik`; no `sys`.

## REL sets

| Area | JP | US | PAL | June US demo | JP demo | Sept. demos | | Area | JP | US | PAL | June US demo | JP demo | Sept. demos |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| `aaa` | ✓ | ✓ | ✓ | | ✓ | | | `kpa` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `aji` | ✓ | ✓ | ✓ | | ✓ | | | `las` | ✓ | ✓ | ✓ | | ✓ | |
| `bom` | ✓ | ✓ | ✓ | | ✓ | | | `moo` | ✓ | ✓ | ✓ | | ✓ | |
| `dig` | ✓ | | ✓ | | ✓ | | | `mri` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `dmo` | ✓ | ✓ | ✓ | | ✓ | | | `muj` | ✓ | ✓ | ✓ | | ✓ | |
| `dou` | ✓ | ✓ | ✓ | | ✓ | | | `nok` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| `eki` | ✓ | ✓ | ✓ | | ✓ | | | `pik` | ✓ | ✓ | ✓ | | ✓ | |
| `end` | ✓ | ✓ | ✓ | | ✓ | | | `qiz` | ✓ | | ✓ | | ✓ | |
| `gon` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | | `rsh` | ✓ | ✓ | ✓ | | ✓ | |
| `gor` | ✓ | ✓ | ✓ | | ✓ | ✓ | | `sys` | ✓ | ✓ | ✓ | ✓ | ✓ | |
| `gra` | ✓ | ✓ | ✓ | | ✓ | | | `tik` | ✓ | ✓ | ✓ | | ✓ | ✓ |
| `hei` | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ | | `tou` | ✓ | ✓ | ✓ | | ✓ | |
| `hom` | ✓ | ✓ | ✓ | | ✓ | | | `tou2` | ✓ | ✓ | ✓ | | ✓ | |
| `jin` | ✓ | ✓ | ✓ | | ✓ | | | `tst` | | | ✓ | | ✓ | |
| `jon` | ✓ | ✓ | ✓ | | ✓ | | | `usu` | ✓ | ✓ | ✓ | | ✓ | |
| | | | | | | | | `win` | ✓ | ✓ | ✓ | | ✓ | |
| | | | | | | | | `yuu` | ✓ | ✓ | ✓ | | ✓ | |
| **Total** | 31 | 29 | 32 | 6 | 32 | 7 | | | | | | | | |

The two September 2004 demos (`G8ME01_DEMO2`, `G8MP01_DEMO`) have the same seven RELs.

Each version's set is the list of REL folders in `config/<version>/` and the lines of `config/<version>/build.sha1` (one per REL plus `MarioSt.dol`). [GLOSSARY.md](GLOSSARY.md#area-codes) says what each area is.

## Which units build for which version

In `configure.py`, each object is declared with `Matching` or `MatchingFor(...)`. For `G8MJ01` and `G8ME01` the object is built only when the version is in its `MatchingFor` list. For the other five versions every declared object is marked complete, so the version's `config/<version>/splits.txt` alone decides which units it builds: a version builds only the units its splits name. A unit that exists in only some versions (`battle/battle_ufo.c`, the `dig` and `qiz` units, everything in `tst`) is simply not built for the others.

Per-version data lives in `config/<version>/`: `config.yml` (the files to split and their hashes), `splits.txt` (which source file owns which address range), `symbols.txt` (every symbol with its address) and `build.sha1`.

## Adding a version difference

See [CONTRIBUTING.md](CONTRIBUTING.md#adding-a-version-difference).
