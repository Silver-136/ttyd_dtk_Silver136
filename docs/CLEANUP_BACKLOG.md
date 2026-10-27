# Cleanup backlog

Everything left to clean up, in one place. Both builds (US G8ME01, JP G8MJ01) must stay byte-identical to the
retail game through every step; nothing here changes behaviour. Per-function detail for the fakes lives in
[FAKES.md](FAKES.md); invented names are listed in [NAME_PROVENANCE.md](NAME_PROVENANCE.md). Counts are from
2026-10-25 (game code unless marked; the "at adoption" column is the LLCOOLT tree at `85ac972b`).

## 1. Matching tricks (debt by policy)

| Item | Now | At adoption | Notes |
| --- | --- | --- | --- |
| FAKES.md rows | 212 (189 both, 20 US, 3 JP) | 214 | the count rises when hidden tricks are found and documented (2026-10-26 readability sweep: 15 unmarked one-use inline/macro devices) |
| FAKE comment sites | 306 in 102 files (libs 18) | 320 (libs 94) | |
| `#pragma opt_*` / `optimize` / `dont_inline` / `auto_inline` lines | 143 / 4 / 112 / 4 (libs: `dont_inline` 6, VI's three pairs) | 259 / 71 / 358 / 9 | whole-file settings moved to per-unit flags (Codex round 14: five more units; round 16: OSLink); the rest are per-function pairs with no whole-unit equivalent |
| `__declspec(section)` placements | 18 (libs 35) | 434 | template Vecs in .rodata, .sdata2 constants, a few real-storage objects; the library sites are mostly genuine (see section 7) |
| `#pragma force_active` | 15 lines in 7 files (libs 10) | 776 | the library pairs keep the `gap_*` objects |
| `gap_*` padding objects | 8 (libs 7) | 142 | eff_kemutest's gaps are real storage; the library gaps are scaffolding (section 7) |
| `fd_*` math-static data | 28 lines, end_00 only | 16,730 lines in 607 files | end_00 is built without the REL precompiled header |
| inline asm | 6 (libs 284) | 19 | library sites sorted in section 7: mostly genuine assembly, three TRK candidates for C |
| raw pointer arithmetic | 26 (libs 15) | 28 | all documented fakes |

FAKES.md rows by kind (approximate): spare temporaries or copies that steer register choice ~65, byte-offset
struct views 21, dead stores / self-assignments / dead conditions 20, comma expressions `(0, x)` 19, placement or
data order 10, sound-name integer round trips (`SFX_NAME_WORD`) 9, no-op casts 6, others ~45.

## 2. Types and declarations

- Calls through function-pointer casts: 40 (icon drawing family 18, window frame 4, party slit-floor motion 3,
  sound pitch 2, party menu setup 2, puni map data getters 3, single sites). Each needs a prototype that the
  definition and every caller compile identically with. Tested without a fit (retail mismatches, Codex rounds 12-13,
  cleanup_notes/codex_round13_intake.md): iconDispGx (best 14/22), iconDispGx2 (JP definition), windowDispGX_Waku_col
  float-first (fits all but msgdrv), BtlUnit_snd_se_pos word pitch, typed puni getidx calls (the casts cross
  pointer/scalar argument shapes; worth a look as a possible retail ABI mismatch).
- Disagreeing private declarations: FontDrawString (14 position-first copies fit; JP gor_irai needs its
  string-first calls and includes fontmgr.h, so no header prototype yet), psndSFX_pit 9 (no width fits all).
  Settled: BtlUnit_SetAnim, partyChgPose, effMarioBalloonEntry, spline_getvalue / spline_maketable, envAddTev,
  envSetYamiView, iconDispGxCol, iconGetTexObj (header prototypes); BattleAcDrawGauge (callers agree with the
  definition except ac_repeatedly, unprototyped, and ac_repeatedly_lv, ratio first: both change code otherwise).
- Private function declarations without a header: 404 (85 are each REL's own `_prolog`/`_epilog`/`_unresolved`;
  114 are tou_03, which cannot include the evt headers because it defines its own out-of-line GXSetTexCoordGen).
  Most of the rest are retail mismatches the bytes prove: different argument counts (partyWalkInit, winBadgeExit,
  effStageClearEndChk, effThunderflareN64Entry), narrowed parameters or returns (BattleStageSet*Dark*,
  psndSFX_pit/vol, msgWindow_Entry, hankakuSearch per version), Vec/GXColor by value vs pointer
  (cloudGetBreathPower, mapObjSetColor, mapSetBlend, partyNrmToAngle, marioCreviceWallChk), argument order that
  changes code (intpl_sub, marioSetForceMove, gor_irai's FontDrawString).
- `itemEntry` has no header prototype because evt_mobj needs a float-first private declaration (FAKES row).
- Double casts `(s32)(u8)x` / `(s32)(s8)x`: the 11 in equality tests against small constants were removed
  (2026-10-27, no byte change). Left: evt_mobj's four `(s32)(s8)linkedName[0]` truth tests (char is unsigned
  here; the plain test compiles differently), and the value conversions (damageCode, attackDirection, Mario's
  stick directions). u8 casts on BOOL audience getters: kept until an accessor or prototype explains them.
- Data declared through private views of another unit's struct (a few effect and map work structs).

## 3. Names

- Battle unit / part work slots (suggested by jdaster64): named in every unit script (file-local UNIT_WORK_* /
  PARTS_WORK_*), with shared names in include/battle/unit_work.h (C-read slots, the sisters' line, Barriern
  satellites, Bobbery's bomb, Mario, the lecture units, Vivian's Veil) and family headers
  rels/rsh|tik|muj|las/include/unit_boss_*.h. 46 numeric slot arguments left (cleanup_tools/work_slots_left.py),
  all write-only (no reader to name them from).
- Placeholder struct fields: the remaining `field_0x..` / `unk..` uses (about 60) are all writes; write-only or
  unused fields keep offset names unless their meaning is clear (then `_UNUSED`).
- Padding arrays (`u8 pad_..[n]`, `u8 field_..[n]`) in struct layouts: 455; many are views that should become the
  real struct type.
- Names come only from the game's own data (symbols, strings, labels, message keys, model and event names) or
  from what our code does; other people's or other projects' names are not used (2026-10-26). Inherited struct
  field names are credited in NAME_PROVENANCE.md.
- Icon ids: every icon the game code uses has a name; unused placeholders stay ICON_xxx. The US-only per-language
  icon tables (statuswindow, kpa, evt_shop) still list raw US ids for the non-English variants.
- Saved-game flags and variables: story progress (StoryProgress, 407 game labels), Trouble Center, Star Pieces
  and Shine Sprites (game strings), and every other GSWF/GSW the scripts use (805 names in
  include/evt/evt_save_flags.h, 2026-10-26, from the scripts and room text; tool cleanup_tools/save_flags.py).
  Area-local values (LSWF/LSW): 84 names in the area headers rels/<area>/include/<area>.h (tool
  cleanup_tools/area_flags.py). C code that uses saved values by index (`swSet(0x994)`) cannot use the names;
  each such line ends with the name as a comment (save_flags.py cnotes).
- Names with an address suffix: 5 unreferenced .sbss words with no name evidence (the JP demo map names the
  neighbouring objects only), the JP-only party init copies, jumptable_80374848, and the shadowdrv debug strings
  by address (`str_8...`, 6; open research).
- Ghidra- or m2c-style local names: none left (2026-10-26). JP demo map: every invented function was checked
  against it (cleanup_tools/demo_names.py); no further original name fits (2026-10-26).
- Battle values still unnamed: pose types 32-34 and 53-55 (no code found selects them; only Mario's pose table
  lists 32). Dead bits (no reader): weapon special 0x200, target weighting 0x2000.
- Comments from the game's own data: message paraphrases next to message calls (only 95 of 6,224 have one), place
  comments per room and NPC identities where the game's data (debug story list maps, NPC tribe table, messages)
  shows them.
- Raw flag masks: about 230 bit tests/sets by number are left outside sound tables and counter/byte arithmetic
  (2026-10-26, down from about 1,800). Left: words whose bits mean different things per caller (evt_npc move
  functions' moveFlags), evt_mobj typeData words (per object kind; the switch block's high bits are never read in C),
  tou.c's own FighterData copy (tou2.c has the FighterFlags names), matrix-part masks in animdrv (fine as numbers),
  and bits with no setter or no known meaning (kept numeric: MapObjectFlags 0x20, GlobalWorkFlags 0x40,
  MarioDispFlags bit 29, PartyFlags bits 15/18, MarioTrigFlags bit 15, HitAttributes bit 10, ItemEntry flags bit 7,
  MapObjectFlags 0x20 (set on A-button objects, never read), damage result 0x10000,
  unit attribute 0x100). Tools: flag_names.py (struct fields), word_masks.py (values by source, also locals),
  material_flags.py (pose material word).

## 4. Structure and layout of the source

- Version blocks: 2,155 `#if`/`#ifdef VERSION_*` blocks (1,889 at adoption; more now because duplicated US/JP copies were
  split into small per-difference blocks, which removed about 20,000 duplicated lines).
- Private copies of one struct: done for the star stone block (SacStarStone in sac_common.h: bakugame, muki, suki;
  deka has its own variant), Maya animation, audience sound/member, ChuchuParty, EnvironmentWork views,
  LectButtonDisp, BoobooName. Kept apart on purpose: the per-map crowd works in hei_10 / rsh_00 / tou_03 (same
  `{flag, count, members, defs[32]}` frame, different member structs; rsh_00's MoamoaWork is not the swarm in
  include/moamoa.h) and gor_03's Monte (a different layout from yuu's; only the 0x38-byte piece is shared). Left:
  nothing: mobjdrv's Cube* types are fixed-size static instances of the map-file mesh/joint structs (variable-length
  tail), not views; the ac_*.c views of BattleActions and
  BattleACDispParams and battle_seq_end's SeqEndWork view are gone (real fields, per-command param-slot enums,
  BattleSeqEndWork in its header; 2026-10-27).
- Offset-only field names: 89 named from their uses (2026-10-26); the rest are write-only or unclear (about 120).
- Hand-expanded code that a macro or inline probably produced: psndENVMain's `ENVAPPLY` cases and psndENVOn_f_d's
  offset sum, the Smorg hop code's temporaries (hand-inlined `randf` in map units: settled as written, Codex round 9
  found the inline call changes literal-pool order).
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

## 7. Libraries (SDK, runtime, TRK, MusyX, THP), sorted by Codex round 16

Round 16 compared the library sources with the original SDK/runtime material the user has locally (evidence only,
never copied in). What the raw counts above mean for the libraries:

- **Genuine assembly, keep.** runtime.c and the SDK math files (mtx, mtx44, mtx44vec, mtxvec, psmtx, quat, vec): all
  70 assembly functions are assembly in the original sources too. The hardware/ABI routines (PPCArch, OS cache /
  context / exception / interrupt / time / startup, AI stack switch, DB, TRK MSR/cache, GX paired-single and FIFO
  helpers, OSFastCast) are real assembly by what they do; their exact original spelling is not proven.
- **Candidates for C (open):** TRK `support.c` HandleOpenFileSupportRequest, TRKRequestSend, TRKSuppAccessFile
  (ordinary protocol and file-request code). Needs the TRK version and API settled first; no form passed yet.
- **Unclear, keep:** THPDec IDCT/decoder blocks, AXFX reverb/chorus, MusyX CheapReverb/Chorus/StdReverb and
  snd_service math kernels (optimized paired-single code; the original form is not established).
- **MusyX placeholder names:** the 67 `lbl_XXXXXXXX` occurrences are 31 branch labels inside the CheapReverb (20) and
  Chorus (11) assembly kernels, not unknown fields or locals. Rename only labels whose role the code shows.
- **Genuine placements, keep:** `__start` / `__ppc_eabi_init` / `__os.h` startup code in .init (many of the 35
  sites are extern declarations), runtime `__mem.c` memcpy/memset in .init, the destructor-chain pointer in .dtors,
  TRK's .init routines.
- **Scaffolding, keep until a natural layout is found:** the library `gap_*` objects (vi 20 bytes, hw_dspctrl 2 x 4,
  snd_service 16, synth_dbtab 24, snd3d 7, US OSError 12) and the force_active pairs around them.
- **MCC is not built** (no configure entry): its 17 `int unused[N]` frame pads (FAKES.md) are not active debt. The
  original debug records show no extra locals, so a fake buffer would not be a fix either.
- **Padclamp:** the original type is ten one-byte fields (10-byte symbol). The plain type matches US; JP loses two
  bytes, so the wrapper stays (FAKES.md) until the JP object extent is understood.
- **sndSeqPlayEx:** the plain indexed loop and the natural `seqPlaySong(.., 0, ..)` wrapper both change the code; the
  row stays.
- **dont_inline:** OSLink's pair became the unit flag `-inline noauto` (round 16, both versions identical). VI's three
  pairs (getTiming, latch0/latch1) stay: a whole-unit flag changes other helpers.
