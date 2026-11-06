# Contributing

Adapted from the page of the same name in llc00lt's tree (LLCOOLT/ttyd), used with permission, and checked against this tree.

Contributions are welcome: clearer names and types, comments that explain what code does, documentation, and anything that makes the code easier to read while every version still builds byte-identical. Please open an issue in this repository for questions. [CLEANUP_BACKLOG.md](CLEANUP_BACKLOG.md) lists what is left to do.

## The seven-version check

Every change must keep all seven versions byte-identical to the original games. Before you commit, build each version whose game files you have and confirm the check line:

```sh
for v in G8MJ01 G8ME01 G8MP01 G8ME01_DEMO G8MJ01_DEMO G8ME01_DEMO2 G8MP01_DEMO; do
    python configure.py --version $v && ninja || break
done
```

| Version | Expected |
|---|---|
| `G8MJ01` | `32 files OK` |
| `G8ME01` | `30 files OK` |
| `G8MP01` | `33 files OK` |
| `G8ME01_DEMO` | `7 files OK` |
| `G8MJ01_DEMO` | `33 files OK` |
| `G8ME01_DEMO2` | `8 files OK` |
| `G8MP01_DEMO` | `8 files OK` |

This applies to every kind of change, including comment-only and header-only edits:

- Declaration order and prototype types can change register allocation, so moving or retyping a declaration is checked like a code change.
- Line numbers are part of the output in a few places: the SDK's `ASSERT` macros, and in `rels/tst/` the SGI GLU sources (`rels/tst/src/glu/`) and the test units that call `__assert2("file.c", line, ...)`. Do not reflow or re-indent those files, and do not add or remove lines above an assertion in them.
- `src/` and `rels/` sources are UTF-8 and are converted to Shift-JIS when compiled (see [BUILDING.md](BUILDING.md#troubleshooting)), so keep files UTF-8.
- Only a change that touches nothing the build reads (documentation under `docs/`, notes in `*.md` files) needs no build.

[BUILDING.md](BUILDING.md) explains the builds; objdiff (same page) shows exactly which function or data object differs when a check fails.

## Code standards

The code is meant to read like the original source. In order of importance:

- **Struct fields, not offsets.** Access data through struct members, never pointer arithmetic on raw byte offsets (`*(T*)((u8*)p + 0x1C)`). If a struct is not known yet, define it with the members you can name and padding for the rest. One struct per object: declare it once, in the header of the code that owns it.
- **Typed data.** Tables are arrays of structs, floats are `f32`, strings are strings, flags and ids use the enums and named constants that exist (`include/` has most of them), and scripts use the script macros (`include/evt/evt_cmd.h`) rather than raw words.
- **Story flags by name.** Saved story values and flags use the names in `include/evt/evt_story.h`, written the same way in scripts and in C.
- **The named word types.** A script word is `EvtWord` (`include/evt/evt_word.h`); an integer that holds an address for address arithmetic is `AddrWord` (`include/addr_word.h`); a sound parameter that takes a name or an id is `SfxNameWord` (`include/sfx_name_word.h`). Do not store pointers in `s32`.
- **Names need evidence.** Use the original names where they are known (the symbol maps). A new name must come from the game itself (its data, its strings, its other code) or from how our code uses the thing. Do not guess. Prefer a name to a bare number. Give fields, locals and parameters real names; do not add `unk_`, `field_`, `var_r31` or `temp_f0` placeholders. Never put an address in a name.
- **Names that are not original.** Many function and data names in this tree were invented by earlier contributors to make the code readable. They stay, but [NAME_PROVENANCE.md](NAME_PROVENANCE.md) lists them, so nobody mistakes them for original names. Add a row there when you invent a symbol name or story flag name. Replace an existing one only with stronger evidence, such as an original name.
- **Declarations live in headers.** A function or variable used by more than one file is declared in its owner's header and included, not re-declared with `extern` in each `.c` file.
- **No assembly.** The only assembly is what the original sources had in assembly: routines in `libs/`, the hardware helpers in `include/hwaccess.h` and the word copy `memcpy_as4` in `src/system.c`. Do not add inline asm or asm-backed functions, and do not use the `register` keyword to force a match.
- **Formatting.** Follow the repository's `.clang-format`. Scripts (`EVT_BEGIN` ... `EVT_END`) have one command per line. Do not reformat lines whose text or line number is compiled in (asserts).
- **Comments** say what code does and why, especially for game logic, version differences and anything surprising. Start a description with `Guess:` when it is inferred rather than confirmed in the code, or `Unknown:` followed by what is known. In a file whose line numbers reach the object (it uses `__LINE__` or an assert macro that passes it), a comment must not add lines.
- **Bugs of the original game.** A bug gets a `// BUG:` comment that describes only what the code does and what a fix would be, and a row in [KNOWN_GAME_BUGS.md](KNOWN_GAME_BUGS.md). Do not fix it in the matching code: a fix changes the output. Do not describe glitches, techniques or outside research in the comment; the code and the game's data are the evidence.
- **No game data.** The repository must not contain game files, disassembled game code or proprietary SDK/MSL sources. Only code you wrote or reconstructed belongs here. Game files are used as evidence, never copied.

## Fakes

Sometimes the natural C does not produce the original instructions, and a small behaviour-identical trick does: an extra temporary, a no-op cast, a dead guard, a particular declaration order, a pragma. The tree still has such tricks, and the aim is to remove them. Do not add new ones to code that the US or JP release compiles. A trick that only PAL or a demo needs can be allowed, if:

1. the behaviour is identical: a fake never reads uninitialised memory and never changes what the code does;
2. it is marked at the site with a `// FAKE:` comment that says what the trick does;
3. it has a row in [FAKES.md](FAKES.md) (version, file, function, what the trick is, the natural forms that were tried).

Replacing a fake with natural C that still matches is always welcome; then delete its comment and its row. [FAKES.md](FAKES.md) explains why such tricks change the compiler's output.

## Adding a version difference

When code differs between versions (see [VERSIONS.md](VERSIONS.md)):

- **Pick the right condition.** Compare `VERSION` with the ordered values from `include/version.h`: US-and-PAL code (with the September demos) goes inside `#if VERSION >= VERSION_US`, PAL code the European demo shares inside `#if VERSION >= VERSION_DEMO_EU`, code of JP and both early demos inside `#if VERSION <= VERSION_JP`, code of both early demos inside `#if VERSION <= VERSION_DEMO_JP`, and code for one version inside `#if VERSION == VERSION_EU` (or `_JP`, `_DEMO_US_1` and the others). The two early demos share their code, so a demo difference covers both unless the Japanese demo shows otherwise; `VERSION == VERSION_DEMO_US_1` is for what only the June US demo has, mostly its English text. US-only code is also in the September US demo: `#if VERSION == VERSION_US || VERSION == VERSION_DEMO_US_2`. [VERSIONS.md](VERSIONS.md) lists every pattern. Include `version.h` before the first condition, and never test an ordered value with `#ifdef` or `defined()`. Game code does not use the `VERSION_<id>` macros; only `version.h` and `libs/` do.
- **Keep it small.** Wrap only the statements, members or values that differ, not a whole function, unless most of it differs. For a single value, use `VERSION_PICK(jp, us)` from `include/version.h`; for frame counts and easing that the US code scales by the frame rate, use `FPS_FRAMES(n)` and `FPS_EASE(n)`.
- **Structs.** A member that exists only in some versions sits inside the `#if` in the struct itself, so every field access stays correct for every version. Put the version's offset in the member's comment when it helps.
- **Say why.** Start the block with a short comment when the difference is not obvious.
- **Units and data.** A unit that exists in only some versions is declared in `configure.py`; its address ranges and symbols go in the `config/<version>/splits.txt` and `symbols.txt` of each version that has it. See [VERSIONS.md](VERSIONS.md#which-units-build-for-which-version) for how `configure.py` picks the units of each version.
- **Check all seven builds.**

## Commit messages

One line that says what changed, in the imperative or as a short statement; a leading area helps when the change sits in one place. Add a body when the reason is not obvious. Keep each commit to one topic, and make sure every commit passes the seven-version check on its own.
