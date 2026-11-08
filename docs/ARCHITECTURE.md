# Architecture

Adapted from the page of the same name in llc00lt's tree (LLCOOLT/ttyd), used with permission, and checked against this tree.

A map of how the game is put together and where each part lives in this tree. File paths are given for the main entry points; most systems have a matching header under `include/` with the same name.

The game is one main executable, `MarioSt.dol` (built from `src/` and `libs/`), plus one relocatable module (REL) per area of the game world (built from `rels/<area>/`). The executable holds the engine, every system and the shared data; an area REL holds that area's maps, scripts, NPC and battle setups, and the enemy units fought there.

```
                         main()                          src/main.c
                           |
              marioStInit, then once per frame:          src/mario/mariost.c
              marioStMain (update) + marioStDisp (draw)
                           |
   +-----------+-----------+-------------+-------------+------------+
   |           |           |             |             |            |
 seqMain    evtmgrMain   drivers       battleMain    psndMain    winMgrMain ...
 sequences  scripts      map, NPC,     battle        sound       menus
 (title,    (field and   Mario, items, (when a       (pmario_    (winmgr.c)
 game, map  battle       effects,      battle runs)  sound.c ->
 change,    scripts)     camera, ...                 sound.c ->
 battle)       |                                     MusyX)
   |           |
   |      user functions (C) called from scripts: src/event/evt_*.c,
   |      src/battle/battle_event_cmd.c, and each REL's own
   |
 map change: load files/rel/<area>.rel, OSLink it, call its prolog,
 which registers each map's init script; run the entered map's script
```

## Boot and the frame loop

- `src/main.c`: `main` calls `marioStInit` and then loops forever: `marioStMain` (game update), `marioStDisp` (draw), with the SDK's `DEMOBeforeRender`/`DEMODoneRender` around each frame and the frame timings stored in `gp`.
- `src/mario/mariost.c`: `marioStInit` sets up the global work `gp` (`include/mario/mariost.h`): video mode, frame rate, language, heaps, the DVD-check thread, and then the init function of every system (sound, files, camera, windows, map, scripts, NPCs, effects, Mario, ...). `marioStMain` reads the controllers (`makeKey`, `src/system.c`) and then calls every system's main function in a fixed order: sequences, memory card, background, shadows, animation, battle, camera, windows, map, collision, scripts, Mario, map objects, NPCs, fades, images, lights, items, effects and so on. It also runs the idle timer that ends a kiosk session after a while without input. `marioStDisp` runs the camera's draw pass (`camDraw`), which draws everything registered for each camera.
- `src/system.c`: controller input (`makeKey`, `keyGet*`), random numbers (`irand`), angles, interpolation (`intplGetValue`), sorting, LZ77 decoding and other small shared helpers.

## Sequences

The game is always in one sequence: logo, title, game, map change, battle, game over, load (file select) and `e3`.

- `src/driver/seqdrv.c`: the sequence driver (`seqSetSeq`, `seqMain`, `seqGetSeq`); each sequence has init, main and exit functions.
- `src/sequence/seqdef.c`: the table of sequences.
- `src/sequence/seq_*.c`: one file per sequence. `src/sequence/seq_mapchange.c` loads areas and maps (below); `src/sequence/seq_battle.c` enters and leaves battles.

## The event (script) system

Most game logic is written as scripts: arrays of 32-bit words that a small interpreter runs, many at once, each with its own local variables. Cutscenes, NPC behaviour, map setup, item and shop logic, and every enemy's battle AI are scripts.

- `include/evt/evt_cmd.h`: the script language as C macros (`EVT_BEGIN`, `SET`, `IF_*`, `LOOP`, `WAIT_FRAMES`, `USER_FUNC`/`CALL`, `CALL_CHILD`, `SPAWN_EVENT`, ...) and the variable spaces (`LW`, `GW`, `LSW`, `GSW`, `GSWF`, `UW`, ...; see [GLOSSARY.md](GLOSSARY.md#abbreviations)).
- `include/evt/evt_word.h`: `EvtWord`, the type of a script word, which can hold a number, a variable reference, a float encoding or an address.
- `src/manager/evtmgr.c`: the scheduler. `evtEntry` starts a script, `evtmgrMain` runs every active script each frame by priority, with child and sibling scripts, pause/resume by type and per-script speed.
- `src/manager/evtmgr_cmd.c`: the interpreter (`evtmgrCmd`), one function per opcode, plus `evtGetValue`/`evtSetValue` for the variable spaces.
- **User functions** are C functions a script calls (defined with `USER_FUNC(name)` in C, called with `CALL(name, ...)` in a script). The shared library is `src/event/evt_*.c` (NPCs, camera, messages, Mario, party, map, items, sound, shops, ...); battle scripts use `src/battle/battle_event_cmd.c` (`btlevtcmd_*`); each REL adds its own.
- `USER_FUNC(name)` expands to `s32 name(EventEntry* event, BOOL isFirstCall)`: `event->args` holds the script arguments (read them with `evtGetValue`), and `isFirstCall` is TRUE on the first call for this command. The argument comment above a user function lists the arguments in order: a pointer-looking entry such as `s32* battleId` or `const char* name` is a value the function reads with `evtGetValue`, and a plain entry such as `s32 indexX` is a script variable the function writes its result into (with `evtSetValue`).
 The return value tells the interpreter what to do next: `EVT_RETURN_DONE` goes on to the next command in the same frame, `EVT_RETURN_BLOCK` stops for this frame and calls the function again next frame (a wait), `EVT_RETURN_YIELD` goes on to the next command next frame, `EVT_RETURN_REPEAT` runs the same command again at once, and `EVT_RETURN_FINISH` ends the script.
- Story progress and flags: `src/driver/swdrv.c` (the `GSW`/`GSWF`/`LSW`/`LSWF` saved words and flags), `src/event.c` (the story-event table).

## Reading the code

- **Finding a function.** `config/<version>/symbols.txt` lists every symbol with its address, and `config/<version>/splits.txt` says which source file owns each address range; REL symbols are under `config/<version>/<rel>/`.
- **Section markers.** A comment such as `//.sbss` or `//.data` before a group of variables says which section they go to in the original object. Definitions are ordered to reproduce the original layout, so a variable can sit far from the code that uses it; do not move one without building every version.
- **Version blocks.** Ordered tests such as `#if VERSION >= VERSION_US` (`include/version.h`) select code by version: that one covers US and every later build (the September 2004 demos and PAL). See [VERSIONS.md](VERSIONS.md) for the order.
- **Fakes and bugs.** A `// FAKE:` comment marks C written in an unnatural way to reproduce the original bytes ([FAKES.md](FAKES.md)); a `// BUG:` comment marks a bug of the original game that the code keeps ([KNOWN_GAME_BUGS.md](KNOWN_GAME_BUGS.md)).

## Maps, areas and RELs

An **area** (`gor`, `hei`, `tik`, ...; see [GLOSSARY.md](GLOSSARY.md#area-codes)) is a group of maps (`gor_00`, `gor_01`, ...) sharing one REL.

- `src/sequence/seq_mapchange.c`: when the player enters a map in another area, it loads `files/rel/<area>.rel` into the map heap, links it with `OSLink`, and calls the REL's `_prolog`. Then it runs the init script of the map being entered.
- `rels/<area>/src/<area>.c`: the REL's `_prolog`, which registers each map's init script (`relSetEvtAddr`) and the area's battle setups (`relSetBtlAddr`).
- `rels/<area>/src/<area>_NN.c`: one file per map: its init script, NPC setups, cutscenes and map-specific user functions. Larger areas also have `event/` (shared area scripts), `data/` (NPC and battle data) and `unit/` (enemies fought there).
- `src/data/mapdata.c`: the world, area and map tables (`mapDataPtr`, `areaDataPtr`, `relSetEvtAddr`, `relSetBtlAddr`).
- `src/driver/mapdrv.c`: map geometry, map file loading and drawing; `src/driver/hitdrv.c`: collision with the map; `src/event/evt_bero.c`: map exits and entrances ("bero").

## Drivers and managers

Each driver owns one kind of object, with `...Init`, `...Main` (per frame) and usually `...Entry` and `...Delete` functions.

| Driver | File | Owns |
|---|---|---|
| Animation | `src/driver/animdrv.c` | character models and their animation poses |
| Camera | `src/driver/camdrv.c` | cameras and the draw-callback list |
| Display | `src/driver/dispdrv.c` | per-camera draw callbacks, sorted by layer |
| Effects | `src/driver/effdrv.c` | effect entries; the effects themselves are `src/effect/eff_*.c` and `src/effect/n64/` |
| Map objects | `src/driver/mobjdrv.c` | blocks, save blocks, switches, item boxes (`mobj`) |
| NPCs | `src/driver/npcdrv.c` | NPCs and enemies on the field, and field-battle encounters (`fbat*`) |
| Items | `src/driver/itemdrv.c` | items lying on the field |
| Messages | `src/driver/msgdrv.c` | message text: loading, control tags, text boxes |
| Windows | `src/driver/windowdrv.c` | message windows |
| Icons | `src/driver/icondrv.c` | 2D icons |
| Fades | `src/driver/fadedrv.c` | screen transitions |
| Lights, shadows, background | `src/driver/lightdrv.c`, `src/driver/shadowdrv.c`, `src/driver/bgdrv.c` | |
| Flags | `src/driver/swdrv.c` | saved story words and flags |

| Manager | File | Owns |
|---|---|---|
| Scripts | `src/manager/evtmgr.c`, `src/manager/evtmgr_cmd.c` | see above |
| Menus | `src/manager/winmgr.c` | pause menu and menu windows; the pages are `src/window/win_*.c` |
| Files | `src/manager/filemgr.c`, `src/manager/dvdmgr.c` | game files on the disc, textures (TPL) |
| ARAM | `src/manager/arammgr.c` | the auxiliary RAM |
| Memory card | `src/manager/cardmgr.c` | save files |
| Fonts | `src/manager/fontmgr.c` | the font and text drawing |

## Mario and partners

- `src/mario/mario.c`: Mario's state and per-frame update; `src/mario/mario_motion.c` switches between motions, each in `src/motion/mot_*.c` (walk, jump, hammer, and the paper modes such as plane, boat and slit).
- `src/mario/mario_hit.c`, `src/mario/mario_cam.c`, `src/mario/mario_party.c`: collision, camera and partner handling for Mario.
- `src/mario/mario_pouch.c`: the inventory ("pouch"): items, badges, coins, star pieces, HP/FP.
- `src/party.c` and `src/party/party_*.c`: partners on the field: following, field abilities, each partner in its own file (see [GLOSSARY.md](GLOSSARY.md#characters)).

## Battle

- `src/sequence/seq_battle.c`: the battle sequence; `src/battle/battle.c`: `BattleInit` and `battleMain`.
- `src/battle/battle_seq.c`, `src/battle/battle_seq_command.c`, `src/battle/battle_seq_end.c`: battle phases, the command menu, the end of battle (experience, level up).
- `src/battle/battle_unit.c`: battle units (Mario, partners, enemies) and their parts; `src/battle/battle_damage.c`: damage and status; `src/battle/battle_event_cmd.c`: the `btlevtcmd_*` user functions that battle scripts call.
- `src/battle/ac/`: action commands; `src/battle/sac/`: special moves (star powers); `src/battle/battle_audience.c`: the audience.
- `src/unit/`: Mario's and the partners' battle units and other shared units; each enemy is a `unit_*.c` file under the REL of the area where it appears (`rels/<area>/src/unit/`), and battle setups are in the area's `data/`.
- `src/data/battle/`, `src/data/item_data.c`: item, weapon and audience tables.

## Memory

`src/memory.c` sets up the heaps at boot (`memInit`): the default heap, an effect heap, a battle heap (JP and the early demos; later versions take battle memory from a sub-heap of the map heap), and the **smart heap** (`smartAlloc`), which holds textures and loaded files and is compacted by `smartGarbage`. The map heap (`_mapAlloc`, inside the `gp->mapAlloc` block) holds the current area's REL and map data. `__memAlloc(heap, size)` is the general allocator.

## Sound

- `src/pmario_sound.c`: the game-level sound API (`psnd*`): sound effects, music and environment sounds by name, 3D positioning, fades.
- `src/sound.c`: the low-level layer over MusyX (`SoundInit`, `SoundMain`, streams, ARAM sound data).
- `libs/musyx/`: the MusyX sound library.

## Libraries

- `libs/dolsdk2004/`: the Dolphin SDK (OS, GX graphics, DVD, VI, PAD, CARD, AR/ARQ, THP, ...).
- `libs/msl/`: the Metrowerks C library (MSL), `libs/runtime/`: the compiler runtime, `libs/trk/`: the debugger kernel.
- `src/sdk/`: SDK demo-library code the game compiled into itself (`DEMOInit.c`, `DEMOPad.c`) and the archive and texture-palette helpers.
