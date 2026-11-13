# Cleanup backlog

Everything left to clean up, in one place. All seven builds (US, JP, PAL, the two early demos, the two September 2004 demos) must stay byte-identical
to the original games through every step; US retail is the priority for readability; nothing here changes behaviour. Per-function detail for the fakes lives in
[FAKES.md](FAKES.md); invented names are listed in [NAME_PROVENANCE.md](NAME_PROVENANCE.md). Counts are from
<!--n:counted_date-->2026-11-12<!--/n--> (game code unless marked; the "at adoption" column is the LLCOOLT tree at `85ac972b`).

## 1. Matching tricks (debt by policy)

| Item | Now | At adoption | Notes |
| --- | --- | --- | --- |
| FAKES.md rows | <!--n:fakes_rows-->434<!--/n--> (<!--n:fakes_rows_both-->393<!--/n--> both, <!--n:fakes_rows_us-->17<!--/n--> US, <!--n:fakes_rows_jp-->8<!--/n--> JP, <!--n:fakes_rows_demos-->13<!--/n--> demos, <!--n:fakes_rows_pal-->3<!--/n--> PAL) | 214 | the count rises when hidden tricks are found and documented (2026-10-27: certification review marks every device it finds that the bytes need; 2026-10-26 readability sweep: 15 unmarked one-use inline/macro devices) |
| FAKE comment sites | <!--n:all_fake_comment_sites-->667<!--/n--> in <!--n:all_fake_comment_sites_files-->202<!--/n--> files (libs <!--n:libs_fake_comment_sites-->27<!--/n-->; up from 306 because the certification review marked existing tricks, 2026-10-27) | 320 (libs 94) | |
| `#pragma opt_*` / `optimize` / `dont_inline` / `auto_inline` lines | <!--n:game_pragma_opt_star-->129<!--/n--> / <!--n:game_pragma_optimize-->4<!--/n--> / <!--n:game_pragma_dont_inline-->66<!--/n--> / <!--n:game_pragma_auto_inline-->4<!--/n--> (libs: `dont_inline` <!--n:libs_pragma_dont_inline-->4<!--/n-->, VI's two pairs) | 259 / 71 / 358 / 9 | whole-file settings moved to per-unit flags (Codex round 14: five more units; round 16: OSLink); the rest are per-function pairs with no whole-unit equivalent |
| `__declspec(section)` placements | <!--n:game_section_placement-->15<!--/n--> (libs <!--n:libs_section_placement-->30<!--/n-->) | 434 | template Vecs in .rodata, .sdata2 constants, a few real-storage objects; the library sites are mostly genuine (see section 7) |
| `#pragma force_active` | <!--n:game_force_active-->11<!--/n--> lines in <!--n:game_force_active_files-->5<!--/n--> files (libs <!--n:libs_force_active-->0<!--/n-->) | 776 | the game pairs keep the game `gap_*` objects; no library pairs are left |
| `gap_*` padding objects | <!--n:game_gap_padding-->8<!--/n--> (libs <!--n:libs_gap_padding-->0<!--/n-->) | 142 | game only: dispdrv and eff_gonbaba_breath (real space), eff_kemutest (JP), itemdrv_pad, party_vivian (section 7) |
| `fd_*` math-static data | <!--n:game_fake_data_fd-->28<!--/n--> lines, end_00 only | 16,730 lines in 607 files | end_00 is built without the REL precompiled header |
| inline asm | <!--n:game_inline_asm-->9<!--/n--> (libs <!--n:libs_inline_asm-->276<!--/n-->) | 19 | library sites sorted in section 7: mostly genuine assembly (the five TRK functions with C in other decompilations are C now) |
| raw pointer arithmetic | <!--n:game_raw_pointer_math-->20<!--/n--> (libs <!--n:libs_raw_pointer_math-->15<!--/n-->) | 28 | all documented fakes |

FAKES.md rows by kind: no current breakdown (the old one counted about 195 rows); the classes that remain are listed
in `docs/FAKES.md` and in the mweppc review record.

## 2. Types and declarations

- Raw numbers with no name yet: certification `magic` notes, outcome per note in `cleanup_notes/magic_notes_outcome.tsv`
  (431, all rechecked against the code 2026-11-05: 189 named, 136 settled or already named, 45 kept as numbers on
  purpose, 13 partly named; left: 32 with no evidence in the code (write-only values, nothing reads them) and 16
  small ones a name would barely help: element counts, a few one-site flags).

- Calls through function-pointer casts: 37 (icon drawing 10, window frame 5, party slit-floor motion 4, marioChgMot 2,
  sound pitch 2, cos 2, party menu setup 2, puni map data getters 3, single sites). Each needs a prototype that the
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
  psndSFX_pit/vol, hankakuSearch per version), Vec/GXColor by value vs pointer
  (cloudGetBreathPower, mapObjSetColor, mapSetBlend, partyNrmToAngle, marioCreviceWallChk), argument order that
  changes code (intpl_sub, marioSetForceMove, gor_irai's FontDrawString).
- From the certification pilot (2026-10-27): qqsort has no live prototype (system.h's is commented out with a
  `const void**` comparator; dispdrv calls it undeclared, lightdrv/extdrv carry private `const void*` copies);
  fbatNpcTalkCheck is declared `void*` in four files but returns NpcEntry*; extdrv's private
  animPoseDisp_MakeExtTexture takes `void***` where the definition takes `smartEntry**`; GlobalWork.language is
  compared unsigned in some places (`(u32)` casts) and signed in others; effGetTexObj tests `(s32)id`.
- `itemEntry` has no header prototype because evt_mobj needs a float-first private declaration (FAKES row).
- Double casts `(s32)(u8)x` / `(s32)(s8)x`: the 11 in equality tests against small constants were removed
  (2026-10-27, no byte change). Left: evt_mobj's four `(s32)(s8)linkedName[0]` truth tests (char is unsigned
  here; the plain test compiles differently), and the value conversions (damageCode, attackDirection, Mario's
  stick directions). Audience getters: GetWaiting returns u8 (its callers' casts are gone); GetExist, GetSysCtrl
  and GetEscapeChangeOK return int but every caller casts to u8 (header comment).
- Data declared through private views of another unit's struct (a few effect and map work structs).

## 3. Names

- L_/U_/N_ function names (Zephiles, 2026-10-27; mostly done 2026-10-28, see the commits): 70 functions carried Zephiles' confidence prefix (L_ likely correct,
  U_ likely wrong, N_ named from the code). Judged 2026-11-08 against the JP demo map: `marioChkRub` (same
  address in the JP demo build), `allPartyRideOn` (its stripped slot) and `yoshi_hitobj_ride` (was L_sweatEntry:
  the slot holds both names, the body records Yoshi's stand-on hit object) are original; `marioChkUseParty` is new
  in JP, so its name stays descriptive (NAME_PROVENANCE.md). Two keep the prefix: `L_getTargetPos` is a global copy
  (called from party_motion) of the static getTargetPos that party_move's callers inline; one function for both
  makes three of those callers 0x18 bytes shorter (tested), so the name stays taken, and
  `L_psndBGM_stop` sits in a slot of four stripped demo functions while its body stops battle-start sound effects,
  not the BGM (new in JP; the demos clear the flag inline).
- DONE (2026-10-27): descriptive function names (user): each of the 147 functions whose name is not in the symbol maps
  (docs/NAME_PROVENANCE.md, kind `function`) gets a comment at its definition saying the name is descriptive and
  giving the placeholder it replaced and its JP address (e.g. `unk_8013be14`, JP 0x8013BE14), so readers can search
  by the names other projects use. Mechanical from NAME_PROVENANCE.md; comment only. First done by hand:
  marioUsePartyMain (268027141).
- Battle unit / part work slots (suggested by jdaster64): named in every unit script (file-local UNIT_WORK_* /
  PARTS_WORK_*), with shared names in include/battle/unit_work.h (C-read slots, the sisters' line, Barriern
  satellites, Bobbery's bomb, Mario, the lecture units, Vivian's Veil) and family headers
  rels/rsh|tik|muj|las/include/unit_boss_*.h; units built in several RELs (60) share include/battle/unit/<unit>.h
  (0f45e78cd, cleanup_tools/unit_work_headers.py). 153 numeric slot arguments left (cleanup_tools/work_slots_left.py,
  2026-11-07); slots no script reads have nothing to name them from.
- Placeholder struct fields: the remaining `field_0x..` / `unk..` uses (83 in 35 files, debt_inventory 2026-11-07); write-only or
  unused fields keep offset names unless their meaning is clear (then `_UNUSED`). Checked 2026-11-08: the 20 code
  uses left are all one-time writes at setup that nothing reads (effects, gor_irai, jin_07, las_04, muj, tou_03,
  sac_deka); none of the written values shows what the field is, so they keep offset names.
- Padding arrays (`u8 pad_..[n]`, `u8 field_..[n]`) in struct layouts: 394 (2026-11-08). The views among them were
  turned into the real types earlier (section 4); the large gaps left are documented "never accessed" bytes or
  unread file-data fields (cam_road.h's curve record: one value read per 12-byte group, layout unknown). Alignment pads
  (user: remove, 2026-11-08): 80 hand-written 1-3 byte pads that sat exactly where the compiler pads by itself
  are gone (cleanup_tools/align_pads.py: pad offset + size is the pad offset rounded up to the next member's 4- or
  8-byte alignment, and equals that member's offset comment when it has one). Kept: pads the data tables write out
  in their initialisers (`{0}, // pad_05`: BattleWeapon pad_06/pad_AE, BattleUnitSetup pad_05, three field_0x2),
  and 45 the tool cannot prove: a struct-typed, 1- or 2-byte member next, a version block or the struct's end
  next, or no known offset.
- Prototypes of restored stripped functions (rule, user 2026-11-08): a global function the JP demo map lists as
  stripped gets its prototype in its unit's header, like every other global (the header is the interface); no
  private declarations. Done for the 15 that had none (pouch addHead/addHeadCheck/addTail/pouchGetItemCheck,
  itemHitCheckY/Up, breakableMobj, effMugiEntry, evt_mobj_rideswitch, mobj_sign,
  mobj_koopa_stone_blk, THPSimpleGetFrameRate/Volume, rndi; itemdrv.h now includes hitdrv.h for HitEntry).
  Not covered: FAKE stand-ins (`*Stripped*`, forceSdata), non-static inline helpers, partyNrmToAngle (its
  pointer prototype in party.c is a FAKES row); cleanup_tools/uncalled_protos.py lists the rest.
- One-line brace blocks (`{ decl; stmt; }`): laid out one statement per line (2026-11-08, 36 sites,
  cleanup_tools/split_blocks.py); SGI's GLU code keeps its own style.
- Names come only from the game's own data (symbols, strings, labels, message keys, model and event names) or
  from what our code does (2026-10-26). Later decisions allow, with credit in NAME_PROVENANCE.md: jdaster64's
  pseudocode names, Zephiles' IconType names, the MetroTRK spellings of the community TRK decompilations, AxioDL's
  MusyX state names and YAGCD/Dolphin hardware register names. Inherited struct field names are credited there too.
- Icon ids: done (2026-11-05). Every icon id has a name: ours from the code and item table, the rest (373, incl.
  the US-only language variants the statuswindow / kpa / evt_shop tables use) from Zephiles' IconType enum
  (credited in NAME_PROVENANCE.md; cleanup_tools/icon_names_zephiles.py).
- Saved-game flags and variables: story progress (StoryProgress, 407 game labels), Trouble Center, Star Pieces
  and Shine Sprites (game strings), and every other GSWF/GSW the scripts use (805 names in
  include/evt/evt_save_flags.h, 2026-10-26, from the scripts and room text; tool cleanup_tools/save_flags.py).
  Area-local values (LSWF/LSW): 84 names in the area headers rels/<area>/include/<area>.h (tool
  cleanup_tools/area_flags.py); every script use and comment outside the test map uses them (2026-11-06,
  cleanup_tools/area_flag_uses.py). The tst test rooms keep numbers: each room numbers its map objects' state
  flags from 0, so one number means a different object per room. C code that uses saved values by index (`swSet(0x994)`) cannot use the names;
  each such line ends with the name as a comment (save_flags.py cnotes).
- Names with an address suffix: 5 unreferenced .sbss words with no name evidence (the JP demo map names the
  neighbouring objects only), the JP-only party init copies, jumptable_80374848, and the shadowdrv debug strings
  by address (`str_8...`, 6; open research).
- Ghidra- or m2c-style local names: none left (2026-10-26). JP demo map: every invented function was checked
  against it (cleanup_tools/demo_names.py); no further original name fits (2026-10-26).
- Battle values still unnamed: pose types 32-34 and 53-55 (no code found selects them; only Mario's pose table
  lists 32). Dead bits (no reader): weapon special 0x200, target weighting 0x2000.
- Comments from the game's own data: place comments per room and NPC identities where the game's data (debug story
  list maps, NPC tribe table, messages) shows them. Message paraphrases next to message calls: not added (decided
  2026-11-08 after community feedback: more clutter than help, since text editing tools already show the
  messages; the 95 existing ones stay).
- Raw flag masks: about 230 bit tests/sets by number are left outside sound tables and counter/byte arithmetic
  (2026-10-26, down from about 1,800). Left: words whose bits mean different things per caller (evt_npc move
  functions' moveFlags), evt_mobj typeData words (per object kind; the switch block's high bits are never read in C),
  tou.c's own FighterData copy (tou2.c has the FighterFlags names), matrix-part masks in animdrv (fine as numbers),
  and bits with no setter or no known meaning (kept numeric: MapObjectFlags 0x20, GlobalWorkFlags 0x40,
  MarioDispFlags bit 29, PartyFlags bits 15/18, MarioTrigFlags bit 15, HitAttributes bit 10, ItemEntry flags bit 7,
  MapObjectFlags 0x20 (set on A-button objects, never read), damage result 0x10000,
  unit attribute 0x100). Tools: flag_names.py (struct fields), word_masks.py (values by source, also locals),
  material_flags.py (pose material word).
- **English identity for developer and Japanese names (user idea, 2026-11-08):** the original symbols and the
  Japanese strings name characters, NPCs, objects, items and maps in the developers' own words, which do not always
  map cleanly to the English game (example: the symbols `mony_*`/`peton_*` and the NPC strings モニー/ピートン are
  Francesca and Frankie, the Pianta couple; comments had romanised them as Mony/Peton). Pass: find every
  developer/Japanese name whose English identity the scripts, code and US messages do not make clear (NPC name
  strings, model names, symbol prefixes, unit names), and give each one an English note at its definition or NPC
  table entry (what it is, and its English name when the game gives one). Evidence order: US message text and the
  NPC/model data, then the code; where only game knowledge settles it, ask the user. Original symbol names stay.
  **NPCs done (2026-11-08):** every NpcSetupInfo name (all versions), the first evt_npc_entry per name and file,
  and the 352 NPC kinds and 56 field-AI kinds in src/data/npc_data.c carry a note (record
  cleanup_notes\npc_english_names.md). The gon bone piles (カロン, model c_karon_g) are Dull Bones (user); the gon
  comments that called them Dry Bones are fixed. Items done too (every itemDataTable row carries its US name or a
  note). Checked and not needed: map-object names are English already (box, save, switch ...) and every effect
  file opens with an English description. Done.

## 4. Structure and layout of the source

- Version blocks: 2,155 `#if`/`#ifdef VERSION_*` blocks when US and JP were the only versions (1,889 at adoption; the
  current count of version-test lines, with all seven versions, is in PROGRESS.md; more because duplicated US/JP copies were
  split into small per-difference blocks, which removed about 20,000 duplicated lines). Blocks whose arms were the
  same text (150, mostly function headers left by the demo-map static pass; found from two Zephiles spotted) are
  collapsed (2026-11-05, cleanup_tools/same_arm_scan.py; rerun it after any bulk edit of version blocks).
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
- Zero/template vector globals placed by hand (part of the 17 placements).
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
- Certification review notes rechecked against the code (2026-11-05): `other` (476) and `trick`/`comment`/`dead`/`macro`
  (342) by read-only agents; outcomes in `cleanup_notes/certification/other_notes_outcome.tsv` and
  `trick_notes_safe_log.tsv`. Classes found and done: case bodies at their label's indent (3,114 lines), 1,300
  functions and 6 data tables made static where the original demo maps list them local (`demo_local_scan.py`,
  `demo_local_data_scan.py`; kept global: `mot_cloud`, `pressStartGX`, called from other files), 6,357 redundant
  `PTR()` inside `CALL_CHILD`/`SPAWN_EVENT`/`STRING`, all `*Stripped*` stand-ins probed (all needed) and marked.
  The ~160 "try the plain form" proposals were probed one by one (52 plain forms kept, 108 devices marked FAKE),
  the 564 `name`/`type`/`header` notes rechecked the same way (about 140 renames; 47 type/prototype changes kept,
  24 marked; outcome `name_type_header_outcome.tsv`).
- libs/ review (2026-11-05): read-only review of dvd, pad, si, vi, ai, os, gx, card and MusyX (about 50k lines;
  findings `cleanup_notes/certification/libs_*.tsv`), applied and probed: existing header names for raw numbers,
  new names where the code proves the meaning (GX_DIRTY_*, CARD_STATUS_*, OS_FONT_ENCODE_*), code-only BUG
  comments, plain forms, marked devices; snd3d's SND3D_UNUSED blocks (uncompiled) removed. Done the same evening: MusyX
  SYNTH_CFLAG_*, SYNTH_DIRTY_*, MIXER_CTRL_*, DSP_CHANGED_*, SAMPLE_COMP_TYPE, MACRO_OP_* (from the handlers), TRK
  DSErrorCode/DSReplyError (since renamed to the MetroTRK spellings), 207 address-named asm labels renamed by role (generic loop/done/skip names).
  PI/MI/DI/SI/EXI/AI register indices and bits named from YAGCD and Dolphin (2026-11-07,
  cleanup_notes/hw_register_names.md; undocumented MI offsets stay raw). Open: DSP mail words. TRK codes
  now carry the MetroTRK spellings (DS_*, DSREPLY_*, DSSTEP_*; 0x704-0x706 named, 2026-11-07). Game section placements (18 then, 17 now) retested: all needed, all commented. The rest of
  libs/ (MSL, TRK, runtime, smaller SDK folders) was read the same day: shared MSL/TRK private headers, names,
  BUG comments (libs_review_{msl,trk,sdk1,sdk2}.tsv).

## 7. Libraries (SDK, runtime, TRK, MusyX, THP), sorted by Codex round 16

Round 16 compared the library sources with the original SDK/runtime material the user has locally (evidence only,
never copied in). What the raw counts above mean for the libraries:

- **Genuine assembly, keep.** runtime.c, Gecko_setjmp.c and the SDK math files (mtx, mtx44, mtx44vec, mtxvec, psmtx, quat, vec): all
  70 assembly functions are assembly in the original sources too. The hardware/ABI routines (PPCArch, OS cache /
  context / exception / interrupt / time / startup, AI stack switch, DB, TRK MSR/cache, GX paired-single and FIFO
  helpers, OSFastCast) are real assembly by what they do; their exact original spelling is not proven.
- **TRK assembly turned into C (2026-11-07):** `support.c` HandleOpenFileSupportRequest, TRKRequestSend,
  TRKSuppAccessFile, `dolphin_trk.c` TRK__write_aram and `mem_TRK.c` TRK_fill_mem, after the matched GC 2.6 C of
  pikmin2 and ac-decomp (CC0); TRK is built with `-enum int` (as in those projects; every TRK object stays identical).
- **Unclear, keep:** THPDec IDCT/decoder blocks, AXFX reverb/chorus, MusyX CheapReverb/Chorus/StdReverb and
  snd_service math kernels (optimized paired-single code; the original form is not established).
- **MusyX placeholder names:** done; the branch labels inside the CheapReverb and Chorus assembly kernels are named
  by role (no `lbl_XXXXXXXX` left).
- **Genuine placements, keep:** `__start` / `__ppc_eabi_init` / `__os.h` startup code in .init (many of the 32
  sites are extern declarations), runtime `__mem.c` memcpy/memset in .init, the destructor-chain pointer in .dtors,
  TRK's .init routines.
- **Scaffolding, keep until a natural layout is found:** no library `gap_*` objects are left
  (hw_dspctrl 2 x 4, snd3d 7 and OSError 12 went 2026-11-10: the linker leaves that space itself). The game-code fillers
  in dispdrv (28, JP+) and eff_gonbaba_breath (28/20, JP+) are real space: removing them moves the DOL. The synth_dbtab (24) and snd_service (16) gaps are gone
  (2026-11-07): compressorTable is 32-byte aligned as in AxioDL's 2.0.2-2.0.3 source; the vi gap (20) was only the
  alignment pad before DEMOFontBitmap, which the linker inserts itself.
- **MCC is not built** (no configure entry): its 17 `int unused[N]` frame pads (FAKES.md) are not active debt. The
  original debug records show no extra locals, so a fake buffer would not be a fix either.
- **Padclamp:** the original type is ten one-byte fields (10-byte symbol). The plain type matches US; JP loses two
  bytes, so the wrapper stays (FAKES.md) until the JP object extent is understood.
- **sndSeqPlayEx:** done 2026-11-07: a separate `static inline _seqPlaySong` copy (AxioDL's 2.0.2-2.0.3 form).
- **dont_inline:** OSLink's pair became the unit flag `-inline noauto` (round 16, both versions identical). VI's two
  pairs (`__VIGetLatch0Position`, `__VIGetLatch1Position`) stay: a whole-unit flag changes other helpers.
- **Sources no build uses:** Codex round 22 G recounted 101 library sources that none of the five builds then compiled.
  The 39 it marked for removal are gone (2026-11-03, user OK): the AX sound stack (AX, AXART, AXFX, SYN, MIX, SEQ;
  the game plays sound through MusyX, and the JP demo link map has none of these archives), the SDK copies of
  DEMOInit/DEMOPad and texPalette (the map lists them as unreferenced duplicates of the game's own files), MusyX
  txwin and an empty dsp_perf.c. The other 62 (GD, GXSave/GXVerif*, perf, MCC, extra OS/CARD/SI/VI files, ...)
  stay: the SDK Makefile still builds them as separate archives. Reasons per file: round22_G.tsv (cleanup notes).

## 8. After the three new versions (PAL, US demo, JP demo; 2026-11-01)

- Version tests in game code rose from about 2,700 lines to about 9,800 even after 4,040 no-effect demo guards were removed
  (`demo_collapse.py`). Most that are left are real demo differences; a block with `==` tests or inside a header was not tried.
- Demo text is imported: `if (x) return` forms, old local names and Ghidra-like shapes sit inside demo branches. Tidy
  where it helps the US reader; demo-only tricks do not count as debt.
- `src/manager/cardmgr_demo.inc` holds the demo view of a whole file (about 1,430 lines); per-function guards might be smaller.
- Tables kept once in `.inc` files for demo data order (`eff_ice_face`, `battle_audience_*`, `battle_monosiri_*`).
- A few column-0 statements remain inside guarded or macro text; guard indentation elsewhere was normalised by tool.

## 9. Codex round 20: order of work (decided 2026-11-01)
Part A (comments against the code) is done and applied (2026-11-02: 3,898 comment edits in 587 files; its 3,891
evidence-dependent defers stay open, not certified). Empty lines around file-scope version blocks and above comments that
follow code are in. Open from the comment decision: comments sitting inside a version side that describe shared flow
(about 600 starts) should move out; this goes with part B. The parts below go to Codex in a new brief pinned to the
pushed commit (cleanup_notes, round 22):
- B: integration shapes (includes cardmgr_demo.inc and CARD_FLAG_* duplicates)
- C: the 476 `other` review notes
- D: libs/ review
- E: layout debt in game code
- F: prototypes, casts, placeholder fields, pragmas, FAKE rows
- G: the 107 library sources built in no version
- H: one inventory for the progress numbers

## 10. Comment accuracy review and header placement (2026-11-04)
- Comment review against the code (brief cleanup_notes/rels_comment_brief.md): DONE for src/, include/ and every
  rels/ module (rooms, events, battle units; last rooms 3c0e439d8, aa556d4f0). Logs cleanup_notes/rels_fix_*.tsv,
  unit_fix_*.tsv, src_fix_*.tsv.
- Code oddities: DONE (e09e6f110). All ~300 checked against the code; verdicts cleanup_notes/oddity_groups/
  verdict_*.tsv (bug 172 with BUG comments, harmless 99, intended 22, wrong 11, unsure 14). Left: the 14 unsure
  rows; GSWF_TOU_RAWK_HAWK_OPENING_SEEN (GSWF 2399) is never set anywhere, so its name is unproven.
- Brace blocks (Zephiles/NWPlayer123, 2026-11-04): about 445 block-local declarations hoisted where the bytes allow
  (55c3dfe57..9664a3b09, cleanup_tools/brace_hoist.py, brace_rounds.py); 154 blocks the bytes need stay (authentic
  C89 scoping). Left: ~420 blocks of forms the tool skips (name reused elsewhere in the function, multi-declarator
  lines); brace indentation sites where demo and retail arms open braces differently (cleanup_notes/brace_indent_fix.tsv).
- Header placement (Zephiles): scanner cleanup_tools/header_placement_scan.py. Done: one version numbering,
  HEAP_TYPE, shared types (cloud rooms, minigame driver, Monte tables, Glitz Pit ranking), per-version values in
  owner headers, unit work-slot names (0f45e78cd), unit types repeated across REL copies (a9f65232b,
  221646136, 73cf4945d). Then ZakoGunWork one struct in zakogun.h,
  the rsh battle swarm types renamed ExtMoamoa*, chorobon size padding gone (configs follow), Yoshi work slots shared
  (include/battle/unit/unit_party_yoshi.h; slots 4/5 are pitch: BtlUnit_snd_se arg 4), Senkaron in include/senkaron.h
  (gon_03, las_05). The scanner's other "shared" pairs (Teresa jin_00/07, UranaiWork, ScreenWork, PowerGaugeWork)
  are different file-local structs with one name: kept. glu EdgePair untouched (assert __LINE__). eff_balloon_n64 shares effN64Vtx through
  include/eff/eff_n64_vtx.h (its tri1 calls stay unprototyped: the bytes need it). Also left: kpa/yuucounter icon prototypes (kept, they change code),
  placeholder names in headers: done (part attribute 0x40 = kIntangiblePart; GLU_UNKNOWN is the real GLU name).
  Imported "Names from LLCOOLT's tree" blocks: duplicates folded (f48c01840), the rest moved into their families or
  regrouped one family per enum in value order (tools fold_imported_names.py, regroup_imported_names.py).
