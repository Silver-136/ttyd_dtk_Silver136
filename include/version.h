#ifndef VERSION_H
#define VERSION_H

// One source tree for five releases (the ordered model comes from llc00lt's tree). configure.py defines one
// VERSION_<id> macro for the version being built; game code tests the ordered value VERSION instead. Only this
// header and libs/ read the VERSION_<id> macros. The order follows the code's history:
//
//   VERSION_DEMO_JP  Japanese store demo (G8MJ01_DEMO): pre-final JP code
//   VERSION_DEMO_US  US demo (G8ME01_DEMO): the same code with English text and a few kiosk changes
//   VERSION_JP       Japanese release (G8MJ01)
//   VERSION_US       US release (G8ME01)
//   VERSION_EU       European release (G8MP01), built from the US code (it also defines VERSION_G8ME01 for libs/)
//
// So `VERSION >= VERSION_US` means US and PAL, and `VERSION <= VERSION_DEMO_US` both demos. Not every set is a
// range: write `==` / `!=` tests for those. Every file that tests VERSION includes this header itself: an
// undefined VERSION would silently compare as 0.

#define VERSION_DEMO_JP 0
#define VERSION_DEMO_US 1
#define VERSION_JP 2
#define VERSION_US 3
#define VERSION_EU 4

#if defined(VERSION_G8MJ01_DEMO)
#define VERSION VERSION_DEMO_JP
#elif defined(VERSION_G8ME01_DEMO)
#define VERSION VERSION_DEMO_US
#elif defined(VERSION_G8MP01)
#define VERSION VERSION_EU
#elif defined(VERSION_G8ME01)
#define VERSION VERSION_US
#elif defined(VERSION_G8MJ01)
#if VERSION == VERSION_DEMO_JP
#define VERSION VERSION_DEMO_JP
#elif VERSION == VERSION_DEMO_US
#define VERSION VERSION_DEMO_US
#else
#define VERSION VERSION_JP
#endif
#else
#error "no VERSION_<id> defined: build through configure.py"
#endif

// VERSION_PICK(jp, us): the JP value (also the demos'), or the US value (also PAL's).
#if VERSION >= VERSION_US
#define VERSION_PICK(jp, us) (us)
#else
#define VERSION_PICK(jp, us) (jp)
#endif

// FPS_EASE(n): easing divisor. US scales it by the frame rate (gp->framerate / 60); JP uses n as is.
// Needs mario/mariost.h for gp.
#if VERSION >= VERSION_US
#define FPS_EASE(n) ((n) * gp->framerate / 60.0f)
#else
#define FPS_EASE(n) (n)
#endif

// FPS_EASE_D(us, jp): for JP sites written with a double literal (8.0) that US writes as a float (8.0f).
// Note the order: US first, unlike VERSION_PICK.
#if VERSION >= VERSION_US
#define FPS_EASE_D(us, jp) FPS_EASE(us)
#else
#define FPS_EASE_D(us, jp) (jp)
#endif

// FPS_FRAMES(n): integer frame count. US scales it to the frame rate (n * gp->framerate / 60).
#if VERSION >= VERSION_US
#define FPS_FRAMES(n) ((n) * gp->framerate / 60)
#else
#define FPS_FRAMES(n) (n)
#endif

#endif
