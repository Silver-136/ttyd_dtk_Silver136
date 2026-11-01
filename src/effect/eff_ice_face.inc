// The ice shards' face tables: one block in retail, each after its hermit table in the demos (ICE_FACE selects one).
#pragma once off
#if !defined(ICE_FACE) || ICE_FACE == 1
/** Ice shard 1: the top triangle of the block face's top-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane1_0[] = {
    3, // vertex count
    -1280, 1280, 0, 0, 0,
    -640, 640, 0, 128, 128,
    0, 1280, 0, 256, 0,
};
s16* ice_effFaceDt_pPlane1 = ice_effFace_pPlane1_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 2
/** Ice shard 2: the left triangle of the block face's top-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane2_0[] = {
    3, // vertex count
    -1280, 0, 0, 0, 256,
    -640, 640, 0, 128, 128,
    -1280, 1280, 0, 0, 0,
};
s16* ice_effFaceDt_pPlane2 = ice_effFace_pPlane2_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 3
/** Ice shard 3: the bottom triangle of the block face's top-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane3_0[] = {
    3, // vertex count
    -1280, 0, 0, 0, 256,
    0, 0, 0, 256, 256,
    -640, 640, 0, 128, 128,
};
s16* ice_effFaceDt_pPlane3 = ice_effFace_pPlane3_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 4
/** Ice shard 4: the right triangle of the block face's top-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane4_0[] = {
    3, // vertex count
    0, 1280, 0, 256, 0,
    -640, 640, 0, 128, 128,
    0, 0, 0, 256, 256,
};
s16* ice_effFaceDt_pPlane4 = ice_effFace_pPlane4_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 5
/** Ice shard 5: the top triangle of the block face's top-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane5_0[] = {
    3, // vertex count
    0, 1280, 0, 256, 0,
    640, 640, 0, 384, 128,
    1280, 1280, 0, 512, 0,
};
s16* ice_effFaceDt_pPlane5 = ice_effFace_pPlane5_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 6
/** Ice shard 6: the left triangle of the block face's top-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane6_0[] = {
    3, // vertex count
    0, 0, 0, 256, 256,
    640, 640, 0, 384, 128,
    0, 1280, 0, 256, 0,
};
s16* ice_effFaceDt_pPlane6 = ice_effFace_pPlane6_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 7
/** Ice shard 7: the bottom triangle of the block face's top-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane7_0[] = {
    3, // vertex count
    0, 0, 0, 256, 256,
    1280, 0, 0, 512, 256,
    640, 640, 0, 384, 128,
};
s16* ice_effFaceDt_pPlane7 = ice_effFace_pPlane7_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 8
/** Ice shard 8: the right triangle of the block face's top-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane8_0[] = {
    3, // vertex count
    640, 640, 0, 384, 128,
    1280, 0, 0, 512, 256,
    1280, 1280, 0, 512, 0,
};
s16* ice_effFaceDt_pPlane8 = ice_effFace_pPlane8_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 9
/** Ice shard 9: the top triangle of the block face's bottom-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane9_0[] = {
    3, // vertex count
    -640, -640, 0, 128, 384,
    0, 0, 0, 256, 256,
    -1280, 0, 0, 0, 256,
};
s16* ice_effFaceDt_pPlane9 = ice_effFace_pPlane9_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 10
/** Ice shard 10: the left triangle of the block face's bottom-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane10_0[] = {
    3, // vertex count
    -1280, -1280, 0, 0, 512,
    -640, -640, 0, 128, 384,
    -1280, 0, 0, 0, 256,
};
s16* ice_effFaceDt_pPlane10 = ice_effFace_pPlane10_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 11
/** Ice shard 11: the bottom triangle of the block face's bottom-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane11_0[] = {
    3, // vertex count
    -1280, -1280, 0, 0, 512,
    0, -1280, 0, 256, 512,
    -640, -640, 0, 128, 384,
};
s16* ice_effFaceDt_pPlane11 = ice_effFace_pPlane11_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 12
/** Ice shard 12: the right triangle of the block face's bottom-left quarter, with texture coordinates. */
s16 ice_effFace_pPlane12_0[] = {
    3, // vertex count
    -640, -640, 0, 128, 384,
    0, -1280, 0, 256, 512,
    0, 0, 0, 256, 256,
};
s16* ice_effFaceDt_pPlane12 = ice_effFace_pPlane12_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 13
/** Ice shard 13: the top triangle of the block face's bottom-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane13_0[] = {
    3, // vertex count
    0, 0, 0, 256, 256,
    640, -640, 0, 384, 384,
    1280, 0, 0, 512, 256,
};
s16* ice_effFaceDt_pPlane13 = ice_effFace_pPlane13_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 14
/** Ice shard 14: the left triangle of the block face's bottom-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane14_0[] = {
    3, // vertex count
    0, -1280, 0, 256, 512,
    640, -640, 0, 384, 384,
    0, 0, 0, 256, 256,
};
s16* ice_effFaceDt_pPlane14 = ice_effFace_pPlane14_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 15
/** Ice shard 15: the bottom triangle of the block face's bottom-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane15_0[] = {
    3, // vertex count
    0, -1280, 0, 256, 512,
    1280, -1280, 0, 512, 512,
    640, -640, 0, 384, 384,
};
s16* ice_effFaceDt_pPlane15 = ice_effFace_pPlane15_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 16
/** Ice shard 16: the right triangle of the block face's bottom-right quarter, with texture coordinates. */
s16 ice_effFace_pPlane16_0[] = {
    3, // vertex count
    1280, 0, 0, 512, 256,
    640, -640, 0, 384, 384,
    1280, -1280, 0, 512, 512,
};
s16* ice_effFaceDt_pPlane16 = ice_effFace_pPlane16_0;
#endif
#if !defined(ICE_FACE) || ICE_FACE == 17
/** The frost overlay: one square covering the whole block face, corners with texture coordinates. */
s16 ice_effFace_pPlane17_0[] = {
    4, // vertex count
    -1280, -1280, 0, 0, 512,
    1280, -1280, 0, 512, 512,
    1280, 1280, 0, 512, 0,
    -1280, 1280, 0, 0, 0,
};
s16* ice_effFaceDt_pPlane17 = ice_effFace_pPlane17_0;
#endif
