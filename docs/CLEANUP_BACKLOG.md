# Cleanup backlog

Everything left to clean up, in one place. All seven builds (US, JP, PAL, the two early demos, the two September 2004 demos) must stay byte-identical
to the original games through every step; US retail is the priority for readability; nothing here changes behaviour. Per-function detail for the fakes lives in
[FAKES.md](FAKES.md); invented names are listed in [NAME_PROVENANCE.md](NAME_PROVENANCE.md). Counts are from
<!--n:counted_date-->2026-11-16<!--/n--> (game code unless marked; the "at adoption" column is the LLCOOLT tree at `85ac972b`).

## 1. Matching tricks (debt by policy)

| Item | Now | At adoption | Notes |
| --- | --- | --- | --- |
| FAKES.md rows | <!--n:fakes_rows-->220<!--/n--> (<!--n:fakes_rows_both-->192<!--/n--> both, <!--n:fakes_rows_us-->11<!--/n--> US, <!--n:fakes_rows_jp-->3<!--/n--> JP, <!--n:fakes_rows_demos-->12<!--/n--> demos, <!--n:fakes_rows_pal-->2<!--/n--> PAL) | 214 | the count rises when hidden tricks are found and documented (2026-10-27: certification review marks every device it finds that the bytes need; 2026-10-26 readability sweep: 15 unmarked one-use inline/macro devices) |
| FAKE comment sites | <!--n:all_fake_comment_sites-->297<!--/n--> in <!--n:all_fake_comment_sites_files-->117<!--/n--> files (libs <!--n:libs_fake_comment_sites-->25<!--/n-->; up from 306 because the certification review marked existing tricks, 2026-10-27) | 320 (libs 94) | |
| `#pragma opt_*` / `optimize` / `dont_inline` / `auto_inline` lines | <!--n:game_pragma_opt_star-->32<!--/n--> / <!--n:game_pragma_optimize-->0<!--/n--> / <!--n:game_pragma_dont_inline-->28<!--/n--> / <!--n:game_pragma_auto_inline-->2<!--/n--> (libs: `dont_inline` <!--n:libs_pragma_dont_inline-->4<!--/n-->, VI's two pairs) | 259 / 71 / 358 / 9 | whole-file settings moved to per-unit flags (Codex round 14: five more units; round 16: OSLink); the rest are per-function pairs with no whole-unit equivalent |
| `__declspec(section)` placements | <!--n:game_section_placement-->4<!--/n--> (libs <!--n:libs_section_placement-->30<!--/n-->) | 434 | template Vecs in .rodata, .sdata2 constants, a few real-storage objects; the library sites are mostly genuine (see section 7) |
| `#pragma force_active` | <!--n:game_force_active-->0<!--/n--> lines in <!--n:game_force_active_files-->0<!--/n--> files (libs <!--n:libs_force_active-->0<!--/n-->) | 776 | the game pairs keep the game `gap_*` objects; no library pairs are left |
| `gap_*` padding objects | <!--n:game_gap_padding-->0<!--/n--> (libs <!--n:libs_gap_padding-->0<!--/n-->) | 142 | game only: dispdrv and eff_gonbaba_breath (real space), eff_kemutest (JP), itemdrv_pad, party_vivian (section 7) |
| `fd_*` math-static data | <!--n:game_fake_data_fd-->28<!--/n--> lines, end_00 only | 16,730 lines in 607 files | end_00 is built without the REL precompiled header |
| inline asm | <!--n:game_inline_asm-->78<!--/n--> (libs <!--n:libs_inline_asm-->276<!--/n-->) | 19 | library sites sorted in section 7: mostly genuine assembly (the five TRK functions with C in other decompilations are C now) |
| raw pointer arithmetic | <!--n:game_raw_pointer_math-->10<!--/n--> (libs <!--n:libs_raw_pointer_math-->15<!--/n-->) | 28 | all documented fakes |

FAKES.md rows by kind: no current breakdown (the old one counted about 195 rows); the classes that remain are listed
in `docs/FAKES.md` and in the mweppc review record.

## 2. Types and declarations

- Raw numbers with no name yet: certification `magic` notes, outcome per note in `cleanup_notes/magic_notes_outcome.tsv`
  (431, all rechecked against the code 2026-11-05: 189 named, 136 settled or already named, 45 kept as numbers on
  purpose, 13 partly named; left: 32 with no evidence in the code (write-only values, nothing reads them) and 16
  small ones a name would barely help: element counts, a few one-site flags).

- Calls through function-pointer casts to named functions: 0 in game code (37 in the earlier count; the last ones,
  iconDispGx2 x5 and iconDispGx in zubastar_disp2D, became plain calls with a block-scope prototype in the caller,
  GA8 2026-11-17). Where the caller's signature differs from the definition (a retail ABI mismatch: iconDispGx2's
  u16 id before US, zubastar's s32 icon id) the prototype says so. Left on purpose: real function-pointer values
  (system.c interpolation modes, REL prolog/epilog, evt_ext/evt_nannpc script functions) and the tst REL's
  dbgDraw* macros, which call `_unresolved` for functions not in the build.
- Disagreeing private declarations: FontDrawString (14 position-first copies fit; JP gor_irai needs its
  string-first calls and includes fontmgr.h, so no header prototype yet), psndSFX_pit 9 (no width fits all),
  and two block-scope prototypes that disagree with a header the same file includes (GA8, 2026-11-17):
  zubastar_disp2D's iconDispGx with an s32 id (header u16) and the pre-US iconDispGx2 callers' u16 id (header s16
  before US). The bytes need the callers' types; MWCC accepts the second declaration.
  Settled: BtlUnit_SetAnim, partyChgPose, effMarioBalloonEntry, spline_getvalue / spline_maketable, envAddTev,
  envSetYamiView, iconDispGxCol, iconGetTexObj (header prototypes); BattleAcDrawGauge (callers agree with the
  definition except ac_repeatedly, unprototyped, and ac_repeatedly_lv, ratio first: both change code otherwise).
- Private function declarations of other units' functions (file scope, 2026-11-17): 227. 87 are each REL's own
  `_prolog`/`_epilog`/`_unresolved`; 111 are tou_03, which cannot include the evt headers because it defines its
  own out-of-line GXSetTexCoordGen; 9 are compiler intrinsics (`__fabsf`, `__abs`, `abs`), glu's DebugEvent and
  tou_03's macro-defined evt functions; glu/normal.c keeps its MSL `sqrt` (SGI code). The other 19 are retail
  mismatches the bytes prove: unprototyped calls with arguments the definition does not take or types it does not
  have (partyWalkInit x2, winBadgeExit, winMarioMain2, dou10_yoko_yari3, msgWindow_Add, FontDrawStringCenterMtx,
  winLogDisp: a win_log.h prototype changes the US DOL, tried 2026-11-17), narrowed parameters or returns
  (BattleStage*Dark* x6, hankakuSearch per version, iconDispGxCol in evt_yuucounter, iconGetTexObj in win_main,
  pouchEquipBadge in winmgr), and FontDrawStringVecPitch in win_main (Vec by pointer, pitch last).
- From the certification pilot (2026-10-27), all settled: qqsort has one prototype in system.h (`QSortCompare`,
  `const void*` arguments; eca5d4b2b); fbatNpcTalkCheck returns NpcEntry* everywhere (npcdrv.h);
  animPoseDisp_MakeExtTexture has one animdrv.h prototype; GlobalWork.language is u32 with no casts left
  (EffectWork.language, its copy in effdrv, is u32 too, GA14 2026-11-17); effGetTexObj takes `s32 id` (the retail
  test is signed; no `(s32)id` cast).
- `itemEntry` has no header prototype because evt_mobj needs a float-first private declaration (FAKES row).
- Double casts `(s32)(u8)x` / `(s32)(s8)x`: none left (2026-11-17). What remains are float truncations
  `(f32)(s32)x` and conversions of u32 results, which are real conversions. Audience getters GetExist, GetSysCtrl,
  GetEscapeChangeOK and GetWaiting return u8 (0de024340).
- Data declared through private views of another unit's struct: none left that has an owner type (2026-11-17,
  `cleanup_tools/struct_views.py`: clang record layouts of every US unit, each .c / REL-local struct compared with
  every header struct and with the other local structs; plus `private_views.py` and extern/cast scans). Fixed:
  tst unit_yoshi writes EffSweatN64Work, moo.h's rock_main is EvtWord, aji_06 uses MapEntryAnimData, evt_kinopio
  uses data_tables.h's KinopioData, hitdrv reads MapAnimJointList (MapAnimJoint now in mapdrv.h), mri_puni builds
  punidrv's PunidrvTexInfo. Same-layout local structs left on purpose: each REL's own copy of copied code (the ext
  crowd pose tables FanAnim / KyakuAnim / MoamoaPoseDef and KyakuWork / MoamoaSpriteWork, gor_03 GMonteSub / yuu
  MonteSub, battle_item_data ThrowWork / unit_boss_koopa.h IconJumpWork, punidrv's copies of the extdrv types):
  no code reaches the other copy's data, so a shared type would be a guess. Kept: koopa.c's unsized `extern f32 marioJumpData[]` (incomplete type gives the retail
  lis/addi addressing; comment there) and tst unit_yoshi's YoshiSlideAcWork (BattleActions.work of an AC with no
  struct of its own yet).

## 3. Names

- L_/U_/N_ function names (Zephiles, 2026-10-27; mostly done 2026-10-28, see the commits): 70 functions carried Zephiles' confidence prefix (L_ likely correct,
  U_ likely wrong, N_ named from the code). Judged 2026-11-08 against the JP demo map: `marioChkRub` (same
  address in the JP demo build), `allPartyRideOn` (its stripped slot) and `yoshi_hitobj_ride` (was L_sweatEntry:
  the slot holds both names, the body records Yoshi's stand-on hit object) are original; `marioChkUseParty` is new
  in JP, so its name stays descriptive (NAME_PROVENANCE.md). Two keep the prefix: `L_getTargetPos` is a global copy
  (called from party_motion) of the static getTargetPos that party_move's callers inline; one function for both
  makes three of those callers 0x18 bytes shorter (tested), so the name stays taken, and
  `L_psndBGM_stop` sits in a slot of four stripped demo functions while its body stops battle-start sound effects,
  not the BGM (new in JP; the demos clear the flag inline). Rechecked 2026-11-17: both still stand as described;
  both now have NAME_PROVENANCE.md rows ("Names added in this fork", GB9).
- DONE (2026-10-27): descriptive function names (user): each of the 147 functions whose name is not in the symbol maps
  (docs/NAME_PROVENANCE.md, kind `function`) gets a comment at its definition saying the name is descriptive and
  giving the placeholder it replaced and its JP address (e.g. `unk_8013be14`, JP 0x8013BE14), so readers can search
  by the names other projects use. Mechanical from NAME_PROVENANCE.md; comment only. First done by hand:
  marioUsePartyMain (268027141).
- Battle unit / part work slots (suggested by jdaster64): named in every unit script (file-local UNIT_WORK_* /
  PARTS_WORK_*), with shared names in include/battle/unit_work.h (C-read slots, the sisters' line, Barriern
  satellites, Bobbery's bomb, Mario, the lecture units, Vivian's Veil) and family headers
  rels/rsh|tik|muj|las/include/unit_boss_*.h; units built in several RELs (60) share include/battle/unit/<unit>.h
  (0f45e78cd, cleanup_tools/unit_work_headers.py). 53 numeric slot arguments left (cleanup_tools/work_slots_left.py,
  rechecked 2026-11-17); slots no script reads have nothing to name them from.
- Placeholder struct fields: the remaining `field_0x..` / `unk..` uses (83 in 35 files, debt_inventory 2026-11-07;
  65 `field_0x..`/`unk_0x..`/`unkNN` tokens in 29 files by a plain grep, 2026-11-17); write-only or
  unused fields keep offset names unless their meaning is clear (then `_UNUSED`). Checked 2026-11-08: the 20 code
  uses left are all one-time writes at setup that nothing reads (effects, gor_irai, jin_07, las_04, muj, tou_03,
  sac_deka); none of the written values shows what the field is, so they keep offset names.
- Padding arrays (`u8 pad_..[n]`, `u8 field_..[n]`) in struct layouts: 394 (2026-11-08); 292 `pad_`/`field_`/`unk_`
  member arrays of any type in src/include/rels on 2026-11-17 (GB8 also dropped cam_shift's two pads when
  CamShiftWork moved to its header). The views among them were
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
  itemHitCheckGround/Ceil (then itemHitCheckY/Up), breakableMobj, effMugiEntry, evt_mobj_rideswitch, mobj_sign,
  mobj_koopa_stone_blk, THPSimpleGetFrameRate/Volume, rndi; itemdrv.h now includes hitdrv.h for HitEntry).
  Not covered: FAKE stand-ins (`*Stripped*`, forceSdata), non-static inline helpers, partyNrmToAngle (its
  pointer prototype in party.c is a FAKES row); cleanup_tools/uncalled_protos.py lists the rest. 2026-11-17 (GB8):
  camShiftGetWorkPtr (CamShiftWork moved into cam_shift.h; cam_shift.c now includes its header), getAnimPose,
  shadowGetProjShadowWork/DepthShadowWork, and yuu.h's yuucounter_set_data/flag_off/flag and
  yuunpc_wp_flag_on/check got prototypes. Left in the tool's no-declaration list: the FAKE `*StrippedPool`
  stand-ins, the battleAc*_ functions (declared by ac_table.h's token-pasting macro, which the tool cannot see),
  three empty BattleCommandDisplay_* stubs, the JP+ yuu_02_icon_monte_on/off and the two
  `*yuuranking_draw_msg_fit_right` copies. Seven dead header externs of objects that are now local initialisers
  were removed (vec3_802f972c.., vec3_802fe200, magic1_zero_color, mapchange_zero_color).
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
- Names with an address suffix (rechecked 2026-11-17): left in code are three unreferenced .sbss words with no
  name evidence (`unk_8041840C` hitdrv, `unk_804186EC` mario, `unk_804189f4` party_motion; the JP demo map names
  the neighbouring objects only), party_move's two unused zero vectors `vec3_802ce0ac/b8` (names invented) and
  win_main's FAKE stand-in `float_0p5_8041dc98` (FAKES row). Gone: the JP-only party init copies (efed9b727),
  jumptable_80374848 and the shadowdrv `str_8...` strings. Other `unk_8...` text is the placeholder named in
  descriptive-name comments, on purpose.
- Ghidra- or m2c-style local names: none left (2026-10-26). JP demo map: every invented function was checked
  against it (cleanup_tools/demo_names.py); no further original name fits (2026-10-26).
- Battle values still unnamed: pose types 32-34 and 53-55 (no code found selects them; only Mario's pose table
  lists 32). Dead bits (no reader): weapon special 0x200, target weighting 0x2000. Still open 2026-11-17 (the
  PoseType enum in battle_unit.h jumps from 31 to 35 and from 52 to 56).
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
  damage result 0x10000, unit attribute 0x100; MapObjectFlags 0x20 is set on A-button objects and never read).
  Tools: flag_names.py (struct fields), word_masks.py (values by source, also locals), material_flags.py (pose
  material word). Rechecked 2026-11-17 (GB14): flag_names.py over every header field whose comment names its enum
  finds nothing left; numbers reached through casts, getters and array elements now use the names
  (EffStarStoneWork, EffectEntry, ExtdrvPose, PouchPartyData, SoundStream, BattleWork.dispFlags, AC doneFlags), and
  new names with a setter and a reader: BreakSlotWork / BreakSlotReel flags (Shadow Queen bits, reel draw / hide),
  ShopWork flags (item selected, buying), EnvBlurWork FADE_OUT, PART_BLUR_SKIP_FRAME_UNUSED. Script CALL masks
  (flag_onoff, attribute and unit-flag commands) carry no hex masks. A scan for `x.flags/attr/status OP number`
  (outside FAKE lines and shifts) counted 218 left after GB14 (297 before). GB17 (2026-11-17) named the file-local
  work-struct bits with a setter and a reader (sac_bakugame/muki/scissor, evt_twoddisp, gor_03 slot, MiniGameWork,
  aji_17 dance and hologram, the yuu minigames, yuu counter / ranking / title, Senkaron, las_04 fire bars, Smorg
  swarm and sprites, tou_02 fans, punidrv poses, nok_00 poster, bom_01 crowd) and used existing names where the bits
  already had them (CHOROGUN_FLAG_ALIVE, NAMEENT_FLAG_CANCELLED, PUNI_TARGET_FLAG_*, LECT_BUTTON_FLAG_HIDDEN,
  STAGE_FLAG_NO_IRON_FRAME, LECTURE_FLAG_CONTINUE_WINDOW, NANNPC_WORK_DEPTH_SORT, EXT_POSE_FLAG_NO_SHADOW). GB17's
  scan (also matching dispFlags/hitFlags, 226 at its start) counts 40 left (16 in src/). Kept numeric with reasons:
  mot_ship MotShipWork.flags (assigned whole direction codes 0/1/2), cam_road CamRoadHit flags (curve hit bits,
  meaning unclear), las_04 fire bar bits 0x1/0x2 and MoamoaSprite 0x2 and PunidrvPose 0x4/0x10 (read, never set),
  Senkaron `&= 0xFFFE` (u32 field: also clears the upper half, not `~1`), YuuRankingWork dispFlags 0x2 (never
  read), tou_03 kyaku 0x1 and evt_yuunpc pose 0x1 (only set), rsh_06 0x18, eff_peach_fukidashi `& 3` (a button
  index), tst unit_yoshi ac flags. Also kept numeric with reasons: EventEntry 0x80 (tested only) and 0x40 (set by inline/sibling events, never
  read), SeqEndDispFlags 0x1 (only cleared), BattleWorkStatusIcon.flags 0x1 (read only), FieldBattleData
  firstAttackFlags (may be written by fbatHitCheck's 16-byte copy), party_bomhei 0x4 (read only).
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
- Offset-only field names: 89 named from their uses (2026-10-26); the rest are write-only or unclear (about 120
  then; see the placeholder-field count in section 3 for the current number).
- Hand-expanded code that a macro or inline probably produced: open. psndENVMain's US+ arm still expands its four
  cases through the local `ENVAPPLY` macro (comment there: a static inline over `ent` changes the object);
  psndENVOn_f_d's offset sum; the Smorg hop code's temporaries (hand-inlined `randf` in map units: settled as
  written, Codex round 9 found the inline call changes literal-pool order).
- Zero/template vector globals placed by hand: 4 game section placements are left (2026-11-17): seq_load's US+
  `seq_logo_zero_color` (.sdata2) and the three `moamoa_zero_vec` copies in eki_00, rsh_05, rsh_06 (.rodata), all
  FAKE-commented and tried many times (bf logs DM, T1, T6, W1b, X2b, Y2, Y7, Z4). The others became local
  initialisers or literals.
- JP-only local duplicates of party init functions (`*_init_800...` names): done (efed9b727, "JP init aliases gone").
- Long one-line statements in map units: done (split_blocks.py, 2026-11-08). Left on purpose: psndENVMain's four
  `case N: switch (...) { case M: ENVAPPLY } break;` lines (they go with the macro) and union/struct one-liners.

## 5. Open research (outside review)

Prepared as questions for an outside review: sound-name temporaries (one source shape for ~30 rows), the last
placements and force_active blocks, end_00's math statics, evt_mobj's itemEntry order, the cast-call families,
psndENVMain, why scripts pass the fade-out bit to the BGM start call, the shadowdrv name table, and a review of the
latest naming batches.

State 2026-11-17: Codex no longer works in the repo, so this list is a record, not a queue. Settled since:
sound-name temporaries (SFX option C, 455bb2877; the integer round trip is original, cleanup_notes/bf/SFX), the
force_active blocks (0 left), the shadowdrv name table (`str_8...` names gone). Still open: the 4 game section
placements (section 4), end_00's math statics (FAKES row `fd_end_00_*`), psndENVMain (section 4). Not rechecked
on 2026-11-17: evt_mobj's itemEntry order, the cast-call families (FAKES rows), the BGM fade-out bit.

## 6. Readability ideas beyond the debt list (no byte change)

Status rechecked 2026-11-17 in brackets.

- Name the saved-game flags (`GSWF`/`GSW`) from the scripts that set them, per chapter. [Done: section 3.]
- Turn private struct views and padding views into the real struct types where the layout is known. [Done for
  every view with an owner type: sections 2 and 4.]
- Replace remaining raw case labels and comparisons with the enums that now exist (states, kinds, sequence ids).
  [Ongoing on contact; no count.]
- Split very long functions' nested one-line blocks into normal statements (whitespace-only, checked per unit).
  [Done: section 4.]
- Comment retail bugs and oddities met during cleanup with `// BUG:` (the code keeps the retail behaviour).
  [Standing practice; notable ones in KNOWN_GAME_BUGS.md, records in cleanup_notes/bugs.]
- Certification pass per object against the cleanup standard once the classes above are done. [Open: waits for
  the FAKES rows.]
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
  cleanup_notes/hw_register_names.md; undocumented MI offsets stay raw). Open: DSP mail words (15 raw
  `DSPSendMailToDSP(0x...)` words left on 2026-11-17). TRK codes
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
  in dispdrv (28, JP+) and eff_gonbaba_breath (28/20, JP+) are gone too (ab2d48dfe, PD: split alignment instead of
  padding objects; no `gap_*` object is left anywhere, rechecked 2026-11-17).
  The synth_dbtab (24) and snd_service (16) gaps are gone
  (2026-11-07): compressorTable is 32-byte aligned as in AxioDL's 2.0.2-2.0.3 source; the vi gap (20) was only the
  alignment pad before DEMOFontBitmap, which the linker inserts itself.
- **MCC is not built** (no configure entry): its 17 `int unused[N]` frame pads (FAKES.md) are not active debt. The
  original debug records show no extra locals, so a fake buffer would not be a fix either.
- **Padclamp:** done (ab2d48dfe, PD): `ClampRegion` is a plain `static const PADClampRegion`; the wrapper and its
  FAKES row are gone.
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
  Now 8,465 tests in 873 game files (PROGRESS.md, 2026-11-16; identical arms collapsed, `#else`+`#if` -> `#elif`).
- Demo text is imported: `if (x) return` forms, old local names and Ghidra-like shapes sit inside demo branches. Tidy
  where it helps the US reader; demo-only tricks do not count as debt. [Ongoing on contact.]
- `src/manager/cardmgr_demo.inc` holds the demo view of a whole file (1,357 lines on 2026-11-17, included by
  cardmgr.c); per-function guards might be smaller. Still open.
- Tables kept once in `.inc` files for demo data order: still `eff_ice_face`, four `battle_audience_*` and two
  `battle_monosiri_*` files (2026-11-17).
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

State 2026-11-17: no longer a Codex queue (Codex stopped on 2026-11-12; Claude owns the work). Done: C (the
`other` notes, section 6), D (libs/ review, section 6), G (39 removed 2026-11-03, 62 kept, section 7), H
(`cleanup_tools/progress_counts.py` writes PROGRESS.md and this file's counts). E: the game `gap_*` fillers are gone
and 4 section placements are left (sections 4 and 7). F: carried on as the FAKES brute-force waves
(cleanup_notes/bf). B: cardmgr_demo.inc is still one file (section 8); its CARD_FLAG_* duplicates are gone
(2026-11-17, GB8: it uses cardmgr.h's CardFlags names and cardmgr.c's CARD_OP_* names; only the demo's own serial
bit CARD_FLAG_SERIAL 0x4000 stays local). Still open from the comment decision:
comments inside a version side that describe shared flow (about 600 starts then). Recounted 2026-11-17 (GB14)
by three scans: no version block has the same comment opening every arm; 8 comments end an arm right before its
`#endif`/`#else`, all about that version's code; 28 comments open an arm without naming a version, and the ones
checked (evt_npc, seq_title, win_root, memory, tou_02, black_peach) describe the arm's own code. No move was
needed; whatever is left would need reading block by block.

## 10. Comment accuracy review and header placement (2026-11-04; rechecked 2026-11-17)
- Comment review against the code (brief cleanup_notes/rels_comment_brief.md): DONE for src/, include/ and every
  rels/ module (rooms, events, battle units; last rooms 3c0e439d8, aa556d4f0). Logs cleanup_notes/rels_fix_*.tsv,
  unit_fix_*.tsv, src_fix_*.tsv.
- Code oddities: DONE (e09e6f110). All ~300 checked against the code; verdicts cleanup_notes/oddity_groups/
  verdict_*.tsv (bug 172 with BUG comments, harmless 99, intended 22, wrong 11, unsure 14). Left: the 14 unsure
  rows; GSWF_TOU_RAWK_HAWK_OPENING_SEEN (GSWF 2399): corrected 2026-11-17, it is set in one place,
  src/event.c's stg3_30_init (story point STG3_30, with the other Rawk Hawk scene flags), and cleared by stg3_34_init,
  tou.c and tou_01.c; no room script sets it, and tou_04's guard (talk_g_2, the opening scene lines
  stg3_tou_423_01/02 at STG3_21) and tou.c read it. The name stays a reading of those uses, not proven.
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
  Rerun 2026-11-17 (`header_placement_scan.py --detail types`): 9 "shared" hits, all the kept ones above plus
  sac_scissor's ScissorWork, whose hit is only its member name `stars` also used in seq_load (not a shared type).
  GB8 moved CamShiftWork into cam_shift.h (a global, camShiftGetWorkPtr, returns it) and replaced
  cardmgr_demo.inc's private copies of the CardFlags bits with cardmgr.h's names (same bits, same uses).
