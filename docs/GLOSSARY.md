# Glossary

Adapted from the page of the same name in llc00lt's tree (LLCOOLT/ttyd), used with permission, and checked against this tree.

The original source names things in romanised Japanese, after the game's Japanese names. This page maps the names you meet in the code to the English release, and explains the area codes and common abbreviations.

## Area codes

Every area of the world is one REL, `rels/<code>/`, with maps named `<code>_NN` (`gor_00`, `gor_01`, ...). The chapter column comes from the message ids the area's scripts use (`stg1_...`, `stg4_...`).

| Code | Area | Chapter | Note |
|---|---|---|---|
| `aaa` | Mario Bros.' house | prologue | the opening scene |
| `aji` | X-Naut Fortress | 7 | *ajito*, hideout |
| `bom` | Fahr Outpost | 7 | after the Bob-ombs who live there |
| `dig` | digest (ダイジェスト): plays demo.thp with characters; nothing in the game enters it | | JP, the JP store demo and PAL; one map |
| `dmo` | opening demo (オープニングデモ): the storybook prologue after the boot logos | | |
| `dou` | Pirate's Grotto | 5 | *dōkutsu*, cave |
| `eki` | Riverside Station | 6 | *eki*, station |
| `end` | ending and credits | | |
| `gon` | Hooktail Castle | 1 | after *Gonbaba* (Hooktail) |
| `gor` | Rogueport | | *Gorotsuki* town |
| `gra` | Twilight Trail | 4 | |
| `hei` | Petal Meadows | 1 | *heigen*, plain |
| `hom` | the Riverside Station platform (hom_00) and the Excess Express ride scenes (hom_10..hom_12) | 6 | *hōmu*, platform |
| `jin` | Creepy Steeple | 4 | |
| `jon` | Pit of 100 Trials | | |
| `kpa` | Bowser's interludes: castle scenes and side-scrolling stages | | *Koopa*, Bowser |
| `las` | Palace of Shadow | 8 | *last* |
| `moo` | the Moon | 7 | |
| `mri` | The Great Tree | 2 | *mori*, forest |
| `muj` | Keelhaul Key | 5 | *mujintō*, desert island |
| `nok` | Petalburg | 1 | after the Koopas (*Nokonoko*) who live there |
| `pik` | Poshley Heights | 6, 7 | |
| `qiz` | test map (init script only) | | JP, the JP store demo and PAL; one map |
| `rsh` | Excess Express | 6 | *ressha*, train |
| `sys` | system maps: `sys_00` (an empty map) and `sys_01` (the chapter title screens) | | |
| `tik` | Rogueport Sewers | | *chika*, underground |
| `tou` | Glitzville and the Glitz Pit | 3 | *tōgijō*, arena |
| `tou2` | Glitz Pit arena (`tou_03`, the fights) | 3 | |
| `tst` | developer test REL (test battles and units, and the GLU tessellator) | | the JP store demo and PAL |
| `usu` | Twilight Town | 4 | *usugurai*, dim |
| `win` | Boggly Woods | 2 | |
| `yuu` | Pianta Parlor games | | *yūgijō*, amusement hall |

## Characters

Every NPC carries an English note where it is defined: its map's setup entry (`"モニー", // name (Francesca)`) or the `evt_npc_entry` call that creates it, and every NPC kind and field-AI kind in `src/data/npc_data.c`. The notes come from the US text (mostly Goombella's tattle line for that NPC, which is stored under the NPC's name), the battle-unit names, and the scripts. A note that starts with "lit." is a literal translation of a Japanese name the English game never names.

### Partners

| In the code | English | Field code | Battle unit |
|---|---|---|---|
| `christine`, `kuribo` (field) | Goombella | `src/party/party_kuribo.c` | `src/unit/party/unit_party_christine.c` |
| `nokotarou`, `nokonoko` (field) | Koops | `src/party/party_nokonoko.c` | `src/unit/party/unit_party_nokotarou.c` |
| `clauda`, `cloud` (field) | Flurrie | `src/party/party_cloud.c` | `src/unit/party/unit_party_clauda.c` |
| `yoshi` | Yoshi | `src/party/party_yoshi.c` | `src/unit/party/unit_party_yoshi.c` |
| `vivian` | Vivian | `src/party/party_vivian.c` | `src/unit/party/unit_party_vivian.c` |
| `sanders`, `bomhei` (field) | Admiral Bobbery | `src/party/party_bomhei.c` | `src/unit/party/unit_party_sanders.c` |
| `chuchurina`, `chuchu` (field) | Ms. Mowz | `src/party/party_chuchu.c` | `src/unit/party/unit_party_chuchurina.c` |

### Bosses and story characters

| In the code | English |
|---|---|
| `koopa` | Bowser |
| `peach` | Princess Peach |
| `black_peach` | the Shadow Queen (possessing Peach) |
| `kamec_obaba` | Kammy Koopa |
| `gonbaba`, `bunbaba`, `zonbaba` | Hooktail, Gloomtail, Bonetail |
| `gesso` | Blooper |
| `rampell` | Doopliss |
| `majolyne`, `marilyn`, `vivian` | Beldam, Marilyn, Vivian (the Shadow Sirens) |
| `batten` | the X-Nauts (*batten*, the X mark) |
| `batten_leader` | Sir Grodus |
| `kanbu` | Lord Crump |
| `gundan_zako` | X-Naut (rank and file) |
| `magnum_battender` | Magnus von Grapple |
| `macho_gance` | Macho Grubba |
| `champion` | Rawk Hawk |
| `cortez` | Cortez |
| `moamoa` | Smorg |
| `atmic_teresa` | Atomic Boo |
| `frankli` | Professor Frankly |
| `kinopio`, `kinopiko` | Toad, Toadette |
| `monban` | Gus (*monban*, gatekeeper) |
| `puni` | the Punies |

### Enemies

| In the code | English | | In the code | English |
|---|---|---|---|---|
| `kuriboo` | Goomba | | `karon` | Dry Bones |
| `patakuri` | Paragoomba | | `honenoko` | Dull Bones |
| `togekuri` | Spiky Goomba | | `red_honenoko` | Red Bones |
| `nokonoko` | Koopa Troopa | | `black_karon` | Dark Bones |
| `patapata` | Koopa Paratroopa | | `teresa` | Boo |
| `togezo` | Spiny | | `heiho` | Shy Guy |
| `met` | Buzzy Beetle | | `bomhei` | Bob-omb |
| `togemet` | Spike Top | | `pakkun` | Piranha Plant |
| `patamet` | Parabuzzy | | `chorobon` | Fuzzy |
| `jyugem` | Lakitu | | `wanwan` | Chain Chomp |
| `kamec` | Magikoopa | | `killer`, `killer_cannon` | Bullet Bill, Bill Blaster |
| `barriern` | Yux | | `sambo` | Pokey |
| `mahorn` | Wizzerd | | `basabasa` | Swooper |
| `pansy` | Crazee Dayzee | | `borodo`, `badge_borodo` | Bandit, Badge Bandit |

Prefixes: `pata-` is the winged (Para-) form, `toge-` the spiked form, `hyper_` the Hyper form, `yami_` the dark form of the Pit of 100 Trials (`yami_kuriboo`, Gloomba), `ura_` the Shady form (`ura_noko`, Shady Koopa), `monochrome_` the Pale form.

## Abbreviations

| Term | Meaning |
|---|---|
| `evt`, event | a script (see [ARCHITECTURE.md](ARCHITECTURE.md#the-event-script-system)); `EVT_BEGIN`/`EVT_END` define one |
| `USER_FUNC` | a C function that scripts call (`CALL(...)`); its signature is `s32 f(EventEntry* event, BOOL isFirstCall)` |
| `LW`, `LF` | a script's local words and flags |
| `GW`, `GF` | global words and flags, shared by all scripts and not saved |
| `GSW`, `GSWF` | global saved words (bytes) and flags: story progress, kept in the save file (`GSW(0)` is the main story counter) |
| `LSW`, `LSWF` | saved words and flags local to an area |
| `UW`, `UF` | a script's user words and flags |
| `EvtWord` | the type of a script word |
| `bero` | a map entrance or exit (doors, pipes, map edges) |
| `mobj` | map object: blocks, save blocks, switches, item boxes |
| `fbat` | field battle: an enemy encounter on the field, before the battle starts |
| `btlevtcmd` | battle event command: the user functions battle scripts call |
| `ac` | action command (`src/battle/ac/`) |
| `sac` | special action command: the special moves (`src/battle/sac/`) |
| unit | a battle participant: Mario, a partner, an enemy or an object (`unit_*.c`) |
| `seq` | sequence (logo, title, game, battle, ...) |
| `drv`, `mgr` | driver, manager: the engine systems in `src/driver/` and `src/manager/` |
| `gp` | the global work struct (`include/mario/mariost.h`) |
| `wp` | a file's own work struct ("work pointer") |
| pouch | the inventory (`src/mario/mario_pouch.c`) |
| `psnd` | the game's sound API (`src/pmario_sound.c`) |
| `eff` | an effect (`src/effect/`); `n64` effects follow the designs of the first Paper Mario |
| `stg` | stage: a chapter (the digit after `stg`, as in `stg0_05_init`) |
| `MarioSt` | the main executable's name (*Mario Story*) |
| DOL, REL | the main executable and a relocatable module (one per area) |
| TPL, THP | GameCube texture files and movie files |
