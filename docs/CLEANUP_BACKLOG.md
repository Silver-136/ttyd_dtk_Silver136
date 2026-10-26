# Cleanup backlog

Everything left to clean up, in one place. Both builds (US G8ME01, JP G8MJ01) must stay byte-identical to the
retail game through every step; nothing here changes behaviour. Per-function detail for the fakes lives in
[FAKES.md](FAKES.md); invented names are listed in [NAME_PROVENANCE.md](NAME_PROVENANCE.md). Counts are from
2026-10-25 (game code unless marked; the "at adoption" column is the LLCOOLT tree at `85ac972b`).

## 1. Matching tricks (debt by policy)

| Item | Now | At adoption | Notes |
| --- | --- | --- | --- |
| FAKES.md rows | 195 (173 both, 20 US, 2 JP) | 214 | the count rose for a while because hidden tricks were documented |
| FAKE comment sites | 306 in 102 files (libs 18) | 320 (libs 94) | |
| `#pragma opt_*` / `optimize` / `dont_inline` / `auto_inline` lines | 143 / 21 / 144 / 4 | 259 / 71 / 358 / 9 | whole-file settings moved to per-unit flags; the rest are per-function pairs |
| `__declspec(section)` placements | 18 (libs 35) | 434 | template Vecs in .rodata, .sdata2 constants, a few real-storage objects |
| `#pragma force_active` | 15 lines in 7 files (libs 12) | 776 | |
| `gap_*` padding objects | 8 (libs 6) | 142 | eff_kemutest's gaps are real storage |
| `fd_*` math-static data | 28 lines, end_00 only | 16,730 lines in 607 files | end_00 is built without the REL precompiled header |
| inline asm | 6 (libs 284) | 19 | most library sites are probably genuine assembly functions; to be sorted, not removed |
| raw pointer arithmetic | 26 (libs 15) | 28 | all documented fakes |

FAKES.md rows by kind (approximate): spare temporaries or copies that steer register choice ~65, byte-offset
struct views 21, dead stores / self-assignments / dead conditions 20, comma expressions `(0, x)` 19, placement or
data order 10, sound-name integer round trips (`SFX_NAME_WORD`) 9, no-op casts 6, others ~45.

## 2. Types and declarations

- Calls through function-pointer casts: 42 (icon drawing family 20, window frame 4, party slit-floor motion 3,
  sound pitch 2, party menu setup 2, puni map data getters 3, single sites). Each needs a prototype that the
  definition and every caller compile identically with. Tested without a fit so far: windowDispGX_Waku_col
  float-first (fits all but msgdrv), BtlUnit_snd_se_pos word pitch, typed puni getidx calls (the casts cross
  pointer/scalar argument shapes; worth a look as a possible retail ABI mismatch).
- Largest families of disagreeing private declarations (review priorities): BtlUnit_SetAnim 18, FontDrawString 15,
  BattleAcDrawGauge 9, partyChgPose 9, psndSFX_pit 9, effMarioBalloonEntry 8, spline_getvalue / spline_maketable 8,
  envAddTev 7, envSetYamiView 7.
- Private function declarations: about 650 still have no header to move into, and a few hundred disagree with the
  function's definition (type questions, one family at a time).
- `itemEntry` has no header prototype because evt_mobj needs a float-first private declaration (FAKES row).
- Signed-byte casts `(s32)(u8)` on character and party ids, and u8 casts on BOOL audience getters: kept until an
  accessor or prototype explains them.
- Data declared through private views of another unit's struct (a few effect and map work structs).

## 3. Names

- Placeholder struct fields: the remaining `field_0x..` / `unk..` uses (about 60) are all writes; write-only or
  unused fields keep offset names unless their meaning is clear (then `_UNUSED`).
- Padding arrays (`u8 pad_..[n]`, `u8 field_..[n]`) in struct layouts: 455; many are views that should become the
  real struct type.
- Names come only from the game's own data (symbols, strings, labels, message keys, model and event names) or
  from what our code does; other people's or other projects' names are not used (2026-10-26). Inherited struct
  field names are credited in NAME_PROVENANCE.md.
- Icon ids: every icon the game code uses has a name; unused placeholders stay ICON_xxx. The US-only per-language
  icon tables (statuswindow, kpa, evt_shop) still list raw US ids for the non-English variants.
- Saved-game flags and variables by number: `GSWF(n)`, `GSW(n)`, `LSW/LSWF(n)`. Named so far: story progress
  (StoryProgress, 407 game labels), Trouble Center, Star Pieces and Shine Sprites (game strings). About 280 more
  GSWF are item/block collect flags (most used only in the call that creates the item); the rest need evidence from
  the scripts that set and test them.
- Names with an address suffix: 5 unreferenced .sbss words with no name evidence (the JP demo map names the
  neighbouring objects only), the JP-only party init copies, jumptable_80374848, and the shadowdrv debug strings
  by address (`str_8...`, 6; open research).
- Ghidra-style locals left: 14.
- Battle values still unnamed: pose types 32-34 and 53-55 (no code found selects them; only Mario's pose table
  lists 32). Dead bits (no reader): weapon special 0x200, target weighting 0x2000.
- JP demo name leads: partyWalkSe (our partyPlaySfxFootstep), partyPrevRunMode (our partyRestoreRunMode).
- Comments from the game's own data: message paraphrases next to message calls (only 95 of 6,224 have one), place
  comments per room and NPC identities where the game's data (debug story list maps, NPC tribe table, messages)
  shows them.
- Unnamed bits in smaller drivers' flag words (module-local words, a few driver fields).

## 4. Structure and layout of the source

- Version blocks: 2,155 `#if`/`#ifdef VERSION_*` blocks (1,889 at adoption; more now because duplicated US/JP copies were
  split into small per-difference blocks, which removed about 20,000 duplicated lines).
- Private copies of one struct that could share a header: the star "arc block" work in sac_bakugame / deka / muki /
  scissor / suki, the `{?, count, elems}` work in hei_10 / rsh_00 / tou_03, `LectButtonDisp` (11 copies),
  `BoobooName` (2), the Pianta Parlor (Monte) structs (5); private views of BattleActions / BattleACDispParams /
  SequenceWork / MapFileMesh in ac_*.c, seq_battle and mobjdrv.
- Offset-only field names that the placeholder count misses (sac_bakugame `s7c`, `c0`, `cc`, `dc`, `e0`, `f0` ...).
- Hand-expanded code that a macro or inline probably produced: psndENVMain's `ENVAPPLY` cases and psndENVOn_f_d's
  offset sum, hand-inlined `randf` in several map units, the Smorg hop code's temporaries.
- Zero/template vector globals placed by hand (part of the 18 placements).
- JP-only local duplicates of party init functions (`*_init_800...` names), shared US float constants.
- Long one-line statements in some map units (nested blocks on one line).

## 5. Open research (outside review)

Prepared as questions for an outside review: sound-name temporaries (one source shape for ~30 rows), the last
placements and force_active blocks, end_00's math statics, evt_mobj's itemEntry order, the cast-call families,
psndENVMain, why scripts pass the fade-out bit to the BGM start call, the shadowdrv name table, and a review of the
latest naming batches.

## 6. Readability ideas beyond the debt list (no byte change)

- Name the saved-game flags (`GSWF`/`GSW`) from the scripts that set them, per chapter.
- Turn private struct views and padding views into the real struct types where the layout is known.
- Replace remaining raw case labels and comparisons with the enums that now exist (states, kinds, sequence ids).
- Split very long functions' nested one-line blocks into normal statements (whitespace-only, checked per unit).
- Comment retail bugs and oddities met during cleanup with `// BUG:` (the code keeps the retail behaviour).
- Certification pass per object against the cleanup standard once the classes above are done.
