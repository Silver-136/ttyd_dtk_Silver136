# Symbol map changelog

`tools/export_maps.py` writes CodeWarrior symbol maps for the seven versions (see the script's docstring). Maps
describe one state of the source: when a push renames, adds, removes, moves or re-scopes functions or data, maps
made from an older commit no longer match the current build. Each map's first line names the commit it was
generated from (maps made before 2026-11-13 have no such line).

One entry per push, newest first: date, the pushed commit, whether maps from the previous push are now obsolete,
and what changed in plain words.

## 2026-11-14 - pushed commit: the one that adds this entry (maps stamped 4cb898184)

Old maps obsolete: **yes, slightly**. Regenerate them.

- Two data names changed (US and later): `icon_049_lang_tbl` / `icon_067_lang_tbl` are now `power_rush_lang_tbl` /
  `power_rush_p_lang_tbl` (icondrv).
- The rest of this push rearranges version `#if` blocks, string spellings and comments, which renames no
  other symbol; the output files stay byte-identical to the original games.

## 2026-11-13 - pushed commit: the one that adds this entry (maps stamped 7799968b7)

Old maps obsolete: **yes**. Regenerate them.

- Many functions the linker stripped from the original (listed as UNUSED in the JP demo maps) are back as
  ordinary functions under their original names; in the built maps they now appear as UNUSED entries of their
  own instead of being missing. Compared with maps made on 2026-11-09, the JP demo's MarioSt.MAP has 125 more
  UNUSED entries (US: 118), most of them from this push. Several replace names this project had made up, for example `partyWallSlide` is
  now `reflect2`, `partyGetFrontAttrFar` is `getFrontFloorAttr2`, `psndSongFindCh` is `searchEmptySONGCh`,
  `psndEnvVolume` is `psndENV_calcvol`, `statusWinEase` is `winPositionCalc`.
- About 90 functions in over 40 files moved to the order the JP demo maps give, so their addresses inside those
  files changed (the game files themselves are unchanged).
- Small data objects changed: some padding objects are gone (for example the `party_vivian` .sdata2 pad is now
  linker padding) and some file-scope helper objects became plain string or float literals (`@N` labels).
- The output files (main.dol, every .rel) are byte-identical to the original games, as always.

## 2026-11-10 - pushed commit 92702a77e

Old maps obsolete: not checked at the time (layout, comment and small helper cleanups).

## 2026-11-10 - pushed commit b8961ed7f

Old maps obsolete: **yes**. About 310 more file-local functions and objects became `static` (scope local, as in
the JP demo maps), and the library gap filler objects were removed.

## 2026-11-10 - pushed commit a57947705

Old maps obsolete: **yes**. About 850 file-local names became `static`, 40 functions became weak as in the JP
demo maps, unit boundaries were moved to match the JP demo maps, and several arrays got their original sizes.

## 2026-11-09 - pushed commit 66e12ec5c

First push with `tools/export_maps.py`.
