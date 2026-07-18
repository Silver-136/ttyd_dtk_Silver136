#include "party/party_motion.h"

extern f32 float_60_8042318c;
extern f32 float_0_804230bc;
extern f32 float_3_804230dc;
extern f32 float_270_80423104;
extern f32 float_180_80423108;
extern f32 unk_8041ec08;
extern s32 unk_8041ec0c;
f32 touch_wall_ct;

void* camGetPtr(s32 id);
f32 revise360(f32 angle);
s32 marioGetPartyId(void);
s32 marioGetExtraPartyId(void);
void* partyGetPtr(s32 id);
void partyPaperOff(void* party);
void partyChgPoseId(void* party, s32 poseId);
void partyChgMot(void* party, s32 mot);

#define PARTY_CAN_FORCE_SLIT_OFF(party) \
    (((*(u32*)(party) & 0x1000) != 0) || ((*(u32*)((s32)(party) + 4) & 0x100) != 0))

#define PARTY_FORCE_SLIT_OFF_BODY(party) \
    do { \
        *(f32*)((s32)(party) + 0x10C) = \
            revise360(*(f32*)((s32)(party) + 0x100) - *(f32*)((s32)(party) + 0xF8)) >= float_180_80423108 \
                ? float_0_804230bc \
                : float_180_80423108; \
        { \
            f32 dir270 = float_270_80423104; \
        *(f32*)((s32)(party) + 0x110) = *(f32*)((s32)(party) + 0x10C); \
            *(f32*)((s32)(party) + 0xFC) = revise360(dir270 - *(f32*)((s32)(party) + 0x10C)); \
        } \
        *(f32*)((s32)(party) + 0x100) = *(f32*)((s32)(party) + 0xFC); \
        *(u32*)((s32)(party) + 4) &= ~0x3C0; \
        partyPaperOff(party); \
        *(u32*)(party) &= ~0x3000; \
        *(u8*)((s32)(party) + 0x37) = 0; \
        partyChgPoseId(party, 1); \
    } while (0)

void N_partyFollowCloseOff(void* party) {
    *(u32*)((s32)party + 8) &= ~0x20000;
}

void N_partyFollowCloseOn(void* party) {
    *(u32*)((s32)party + 8) |= 0x20000;
}

void unk_80151d20(void) {
    touch_wall_ct = float_60_8042318c;
}

void partyChgMotSlitFloor(void* party) {
    if (!(*(u32*)party & 0x2000)) {
        void* unk = *(void**)((s32)party + 0x160);

        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
        *(u32*)party |= 0x2000;
    }
}

void party_set_behind_dir(void* party, f32 dir) {
    void* cam = camGetPtr(4);

    *(f32*)((s32)party + 0x54) = revise360(dir + *(f32*)((s32)cam + 0x114));
    *(u32*)((s32)party + 8) |= 0x100000;
}

void allPartySlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && (*(u32*)party & 0x1000) && *(u8*)((s32)party + 0x3A) < 0x28) {
        *(u8*)((s32)party + 0x3A) = 0x28;
    }
}


u8 party_motion_homing_walk(s32 pParty) {
    extern s32 partyGetFootmarkId(void* party);
    extern void* partyGetFootmarkPtr(s32 id);
    extern s32 party_slit(void* party);
    extern s32 party_roll(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_800cbfbc(f32 speed, void* party);
    extern void partyMoveBehindMario(void* party, s32 mode);
    extern f32 partyGetTargetDistY(s32 id);
    extern f32 partyGetTargetMovespd2(void* party);
    extern void* gp;
    extern f32 vec3_802f4628[];
    extern s32 strcmp(const char*, const char*);
    extern s32 marioKeyOffChk(void);
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    void* mark;
    f32 targetX;
    f32 targetZ;
    f32 dist;
    f32 speed;
    f32 angle;
    f32 followDist;
    f32 closeDist;
    f32 verticalDist;
    s32 id;

    id = partyGetFootmarkId(party);
    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        *(u32*)pParty &= ~0x80000;
    }
    closeDist = (*(s32*)(pParty + 0x28) != 0) ? 28.0f : 45.0f;
    if ((strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            closeDist = 18.0f;
        } else if (*(s8*)(pParty + 0x31) == 5 || *(s8*)(pParty + 0x31) == 6) {
            closeDist = (*(u32*)(pParty + 8) & 0x00200000) ? 8.0f : 30.0f;
        }
        *(u32*)(pParty + 8) |= 0x00020000;
    }
    followDist = 45.0f;
    verticalDist = partyGetTargetDistY(id);
    if (verticalDist != 0.0f) {
        followDist = 50.0f;
    }
    if (party_slit(party) == 0 || party_roll(party) == 0) return 0;
    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }
    mark = partyGetFootmarkPtr(id);
    targetX = *(f32*)mark;
    targetZ = *(f32*)((s32)mark + 8);
    dist = distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), targetX, targetZ);
    speed = partyGetTargetMovespd2(party);
    if (dist > closeDist) speed = 3.0f;
    else if (dist < 18.0f) speed = 0.0f;
    if (*(s32*)(pParty + 0x20) != 0) speed = 1.0f;
    unk_800cbfbc(speed, party);
    angle = angleABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), targetX, targetZ);
    if (speed != 0.0f) {
        movePos(speed, angle, (f32*)(pParty + 0x58), (f32*)(pParty + 0x60));
        *(f32*)(pParty + 0x100) = angle;
        partyChgPoseId(party, 2);
    } else {
        partyChgPoseId(party, 1);
    }
    return 0;
}

u8 party_motion_homing_fly(s32 pParty) {
    extern s32 partyGetFootmarkId(void* party);
    extern void* partyGetFootmarkPtr(s32 id);
    extern s32 party_slit(void* party);
    extern s32 party_roll(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern void partyChgPoseId(void* party, s32 pose);
    extern void unk_800cbfbc(f32 speed, void* party);
    extern void partyMoveBehindMario(void* party, s32 mode);
    extern f32 partyGetTargetDistY(s32 id);
    extern f32 partyGetTargetMovespd2(void* party);
    extern void* gp;
    extern f32 vec3_802f4628[];
    extern s32 strcmp(const char*, const char*);
    extern s32 marioKeyOffChk(void);
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    void* mark;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 dist;
    f32 speed;
    f32 angle;
    f32 followDist;
    f32 closeDist;
    f32 verticalDist;
    s32 id;

    id = partyGetFootmarkId(party);
    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        *(u32*)pParty &= ~0x80000;
        *(u16*)(pParty + 0x4E) = 0;
    }
    closeDist = (*(s32*)(pParty + 0x28) != 0) ? 28.0f : 45.0f;
    if ((strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x188) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x190) == 0 ||
         strcmp((char*)gp + 0x12C, (char*)vec3_802f4628 + 0x198) == 0) &&
        marioKeyOffChk() == 0) {
        if (*(u8*)(pParty + 0x32) == 0) {
            closeDist = 18.0f;
        } else if (*(s8*)(pParty + 0x31) == 5 || *(s8*)(pParty + 0x31) == 6) {
            closeDist = (*(u32*)(pParty + 8) & 0x00200000) ? 8.0f : 30.0f;
        }
        *(u32*)(pParty + 8) |= 0x00020000;
    }
    followDist = 45.0f;
    verticalDist = partyGetTargetDistY(id);
    if (verticalDist != 0.0f) {
        followDist = 50.0f;
    }
    if (party_slit(party) == 0 || party_roll(party) == 0) return 0;
    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }
    mark = partyGetFootmarkPtr(id);
    targetX = *(f32*)mark;
    targetY = *(f32*)((s32)mark + 4) + 5.0f;
    targetZ = *(f32*)((s32)mark + 8);
    dist = distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), targetX, targetZ);
    if (dist > closeDist) speed = 3.0f;
    else if (dist > 18.0f) speed = partyGetTargetMovespd2(party);
    else speed = 0.0f;
    if ((*(u32*)player & 0x200000) != 0) speed = 1.0f;
    unk_800cbfbc(speed, party);
    angle = angleABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60), targetX, targetZ);
    movePos(speed, angle, (f32*)(pParty + 0x58), (f32*)(pParty + 0x60));
    *(f32*)(pParty + 0x100) = angle;
    *(f32*)(pParty + 0x5C) += (targetY - *(f32*)(pParty + 0x5C)) * 0.1f;
    if (speed == 0.0f) partyChgPoseId(party, 1);
    return 0;
}

u8 party_motion_jump(s32 pParty) {
    typedef struct Vec { f32 x, y, z; } Vec;
    extern s32 partyGetFootmarkId(void*);
    extern void partyNextReadId(s32);
    extern f32 distABf(f32,f32,f32,f32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void movePos(f32,f32,f32*,f32*);
    extern void partyChgPoseId(void*,s32);
    extern void psndSFXOn_3D(s32,void*);
    extern void* partySearchGround(f32,void*);
    extern u32 hitGetAttr(void*);
    extern void partyChgMot(void*,s32);
    extern void N_partyPlaySfxPartyLanding1(void*);
    extern void* partySearchFrontWall(f32,f32,void*,Vec*);
    void* party;
    void* player;
    void* hit;
    Vec pos;
    f32 distance;
    f32 frames;
    f32 yDelta;
    f32 accel;
    f32 vel;
    s32 id;
    s32 timer;

    party=(void*)pParty;
    player=*(void**)((s32)party+0x160);
    id=partyGetFootmarkId(party);
    if((*(u32*)((s32)party+8)&2)!=0){
        *(u32*)((s32)party+8)&=~2;
        *(u8*)((s32)party+0x38)=0;
    }

    if(*(u8*)((s32)party+0x38)==10){
        if((*(u32*)party&0x1000)==0) partyNextReadId(id);
        *(f32*)((s32)party+0x114)+=*(f32*)((s32)party+0x11C);
        *(f32*)((s32)party+0x5C)+=*(f32*)((s32)party+0x114);
        if(*(f32*)((s32)party+0x5C)<-2000.0f){
            *(u32*)party&=~0x80;
            *(f32*)((s32)party+0x114)=0.0f;
            *(f32*)((s32)party+0x100)=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
                *(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94));
            *(f32*)((s32)party+0x58)=*(f32*)((s32)player+0x8C);
            *(f32*)((s32)party+0x5C)=*(f32*)((s32)player+0x90);
            *(f32*)((s32)party+0x60)=*(f32*)((s32)player+0x94);
            partyChgMot(party,0);
            return 0;
        }
        if(*(f32*)((s32)party+0x114)<=0.0f){
            *(u32*)party&=~0x80;
            *(u32*)party|=0x20;
            vel=*(f32*)((s32)party+0x114);
            if(vel<0.0f) vel=-vel;
            hit=partySearchGround(vel,party);
            if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)){
                if(*(s8*)((s32)party+0x4C)==0){
                    *(u32*)((s32)party+8)|=0x01000000;
                    *(u32*)party|=0x02000000;
                }
                *(void**)((s32)party+0x138)=0;
                partyChgMot(party,3);
                *(f32*)((s32)party+0x11C)=-1.0f;
                return 0;
            }
            *(void**)((s32)party+0x138)=hit;
            *(f32*)((s32)party+0x5C)=*(f32*)((s32)party+0x118);
            if(hit!=0){
                *(u32*)party|=0x20;
                *(u32*)((s32)party+8)&=~0x200000;
                *(u32*)party&=~0xE0;
                N_partyPlaySfxPartyLanding1(party);
                *(f32*)((s32)party+0x114)=0.0f;
                partyChgMot(party,0);
                *(u32*)((s32)party+8)|=0x400000;
                return 0;
            }
        }
        pos.x=*(f32*)((s32)party+0x58);
        pos.y=*(f32*)((s32)party+0x5C);
        pos.z=*(f32*)((s32)party+0x60);
        if((*(u32*)party&0x200000)==0){
            hit=partySearchFrontWall(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),party,&pos);
            *(void**)((s32)party+0x13C)=hit;
            if((hit!=0)&&(*(f32*)((s32)party+0x104)!=0.0f)) *(void**)((s32)party+0x140)=hit;
            *(f32*)((s32)party+0x58)=pos.x;
            *(f32*)((s32)party+0x60)=pos.z;
        }else{
            movePos(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),
                (f32*)((s32)party+0x58),(f32*)((s32)party+0x60));
        }
        return 0;
    }

    if(*(u8*)((s32)party+0x38)==0){
        if((*(s16*)((s32)party+0x28)!=0)||(*(u8*)((s32)party+0x34)==9)){
            pos.x=*(f32*)((s32)party+0x58);
            pos.y=*(f32*)((s32)party+0x5C);
            pos.z=*(f32*)((s32)party+0x60);
            movePos(*(f32*)((s32)party+0x104),*(f32*)((s32)party+0x100),&pos.x,&pos.z);
            *(f32*)((s32)party+0x94)=pos.x;
            *(f32*)((s32)party+0x98)=pos.y;
            *(f32*)((s32)party+0x9C)=pos.z;
        }
        distance=distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
            *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C));
        *(f32*)((s32)party+0x128)=distance;
        if(*(s16*)((s32)party+0x28)==0){
            *(f32*)((s32)party+0x100)=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),
                *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C));
            *(f32*)((s32)party+0x104)=2.25f**(f32*)((s32)player+0x228);
        }
        timer=(s32)((9.0f*(distance/ *(f32*)((s32)party+0x104))+0.5f)/10.0f);
        if(timer<12) timer=12;
        if(timer>40) timer=40;
        *(s32*)((s32)party+0x24)=timer;
        frames=0.5f*(f32)timer;
        yDelta=*(f32*)((s32)party+0x98)-*(f32*)((s32)party+0x5C);
        accel=((yDelta>=1.0f)?yDelta:10.0f)*2.0f/(frames*frames-frames);
        *(f32*)((s32)party+0x114)=*(f32*)((s32)player+0x228)*accel*frames;
        if(*(f32*)((s32)party+0x114)>13.0f) *(f32*)((s32)party+0x114)=13.0f;
        if(*(f32*)((s32)party+0x114)<3.0f) *(f32*)((s32)party+0x114)=3.0f;
        *(f32*)((s32)party+0x11C)=-accel**(f32*)((s32)player+0x228);
        *(u32*)party|=0x80;
        *(u32*)party&=~0x20;
        partyChgPoseId(party,4);
        psndSFXOn_3D(0x927,(void*)((s32)party+0x58));
        *(u8*)((s32)party+0x38)=10;
        if((*(u32*)party&0x1000)==0) partyNextReadId(id);
    }
    return 0;
}

u8 party_motion_behind_mario(s32 pParty) {
    typedef struct Vec { f32 x,y,z; } Vec;
    extern f32 toMovedir(f32);
    extern f32 revise360(f32);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32);
    extern f32 partyToMovedir(f32,void*);
    extern void partyGetAppearPos4(void*,Vec*);
    extern s32 marioKeyOffChk(void);
    extern f32 distABf(f32,f32,f32,f32);
    extern f32 angleABf(f32,f32,f32,f32);
    extern void partyChgMoveMode(void*,s32);
    extern void partyChgMot(void*,s32);
    extern void partySetDir(f32,f32,void*);
    extern void unk_800cbf84(f32,void*);
    extern void partyMoveCheckWall(void*);
    extern void unk_800cbb10(f32,void*);
    extern void* partySearchGround(f32,f32,void*);
    extern u32 hitGetAttr(void*);
    void* party;
    void* player;
    void* lead;
    void* hit;
    Vec target;
    f32 dir;
    f32 distance;
    f32 speed;

    party=(void*)pParty;
    player=*(void**)((s32)party+0x160);
    if((*(u32*)((s32)party+8)&2)!=0){
        *(u32*)((s32)party+8)&=~2;
        *(u32*)((s32)party+8)|=0x00040000;
        if((*(u32*)((s32)party+8)&0x00100000)==0){
            if((*(u32*)((s32)party+8)&0x00010000)==0){
                if((*(u32*)party&0x1000)==0){
                    dir=toMovedir(*(f32*)((s32)player+0x1AC));
                }else{
                    lead=partyGetPtr(marioGetPartyId());
                    if(lead==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
                    else dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
                }
                dir=revise360(dir+180.0f);
            }else{
                *(u32*)((s32)party+8)&=~0x00010000;
                dir=revise360(180.0f+angleABf(*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94),
                    *(f32*)((s32)party+0x94),*(f32*)((s32)party+0x9C)));
            }
            *(f32*)((s32)party+0x54)=dir;
        }
        partyGetAppearPos4(party,&target);
        *(s32*)((s32)party+0x24)=(s32)distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
        if(*(s32*)((s32)party+0x24)<1) *(s32*)((s32)party+0x24)=1;
    }

    if(((*(u32*)((s32)party+8)&0x00100000)==0)&&(marioKeyOffChk()!=0)){
        if((*(u32*)party&0x1000)==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
        else{
            lead=partyGetPtr(marioGetPartyId());
            if(lead==0) dir=toMovedir(*(f32*)((s32)player+0x1AC));
            else dir=partyToMovedir(*(f32*)((s32)lead+0x10C),party);
        }
        *(f32*)((s32)party+0x54)=revise360(dir+180.0f);
    }

    partyGetAppearPos4(party,&target);
    dir=angleABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
    distance=distABf(*(f32*)((s32)party+0x58),*(f32*)((s32)party+0x60),target.x,target.z);
    speed=(*(f32*)((s32)player+0x1B0)>1.0f)?10.0f:2.0f;
    --*(s32*)((s32)party+0x24);
    if((distance<speed)||(*(s32*)((s32)party+0x24)<1)){
        *(s32*)((s32)party+0x24)=0;
        if((*(u32*)((s32)party+8)&0x8000)!=0) *(u32*)((s32)party+8)&=~0x20000;
        partyChgMoveMode(party,1);
        partyChgMot(party,0);
        partySetDir(*(f32*)((s32)player+0x8C),*(f32*)((s32)player+0x94),party);
        *(f32*)((s32)party+0x40)=50.0f;
        *(u32*)((s32)party+4)&=~0x100000;
        *(u32*)party|=0x100000;
        *(u32*)((s32)party+8)&=~(0x00100000|0x00040000|0x00008000);
        hit=partySearchGround(100.0f,100.0f,party);
        if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)&&(*(s8*)((s32)party+0x4C)==0)){
            hit=0;
            *(u32*)((s32)party+8)|=0x01000000;
            *(u32*)party|=0x02000000;
        }
        *(void**)((s32)party+0x138)=hit;
        return 0;
    }

    *(f32*)((s32)party+0x100)=dir;
    speed=((*(u32*)((s32)party+8)&0x00100000)!=0)?3.6f:2.0f;
    unk_800cbf84(speed,party);
    partyMoveCheckWall(party);
    unk_800cbb10(*(f32*)((s32)party+0x104),party);
    hit=partySearchGround(10.0f,0.0f,party);
    if((hit!=0)&&((hitGetAttr(hit)&0x200)!=0)&&(*(s8*)((s32)party+0x4C)==0)){
        hit=0;
        *(u32*)((s32)party+8)|=0x01000000;
        *(u32*)party|=0x02000000;
    }
    *(void**)((s32)party+0x138)=hit;
    if(hit==0){
        partyChgMot(party,3);
        *(f32*)((s32)party+0x11C)=-1.0f;
    }
    *(f32*)((s32)party+0x5C)=*(f32*)((s32)party+0x118);
    return 0;
}

u8 party_motion_stay(s32 pParty) {
    extern s32 partyGetFootmarkId(void*);
    extern void partyClearFootmark(void);
    extern void partyClearFootmark2(void);
    extern void partyChgPoseId(void*, s32);
    extern void partyChgMot(void*, s32);
    extern void unk_800cbf84(f32, void*);
    extern s32 marioKeyOffChk(void);
    extern s32 marioChkSlitThrouh(void);
    extern s32 marioChkDeepSleep(void);
    extern f32 distABf(f32, f32, f32, f32);
    extern void partyGetMoveDirSpd(void*, f32*, f32*);
    extern void* partySearchFrontWall(f32, f32, void*, void*);
    extern s32 party_slit(void*);
    extern s32 party_roll(void*);
    extern void unk_800cbc30(void*);
    extern void unk_800c27c0(f32, void*, void*);
    extern void unk_80150fac(void*);
    extern void unk_801512e0(void*);
    extern void party_force_reset_outofscreen(void*);
    extern void partyMoveBehindMario(void*, s32);
    void* party = (void*)pParty;
    void* player = *(void**)(pParty + 0x160);
    f32 pos[3];
    f32 dir;
    f32 speed;
    f32 follow = *(f32*)(pParty + 0x40);

    partyGetFootmarkId(party);
    if ((*(u32*)(pParty + 8) & 2) != 0) {
        *(u32*)(pParty + 8) &= ~2;
        partyChgPoseId(party, 1);
        if ((*(u32*)(pParty + 8) & 0x10000) == 0) {
            if ((*(u32*)pParty & 0x1000) == 0) partyClearFootmark();
            else partyClearFootmark2();
        }
        *(u32*)(pParty + 8) &= ~0x10000;
        *(s32*)(pParty + 0x168) = -1;
        unk_800cbf84(0.0f, party);
    }
    if (*(u16*)((s32)player + 0x2E) != 0xE &&
        (*(u32*)party & 0x80000) != 0) {
        *(u32*)party &= ~0x80000;
    }
    if (*(f32*)((s32)player + 0x1B0) == 0.0f && *(f32*)((s32)player + 0x1B4) < 1.0f)
        follow = 100.0f;
    if (*(u8*)((s32)player + 0x2F) == 2) follow = 70.0f;
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if (*(u16*)((s32)player + 0x2E) == 0xE &&
        (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
        return 0;
    }
    if (*(s32*)(pParty + 0x20) != 0 && marioChkSlitThrouh() != 0 && marioChkDeepSleep() == 0)
        partyChgMot(party, 1);
    if (*(s32*)(pParty + 0x20) != 0 &&
        distABf(*(f32*)(pParty + 0x58), *(f32*)(pParty + 0x60),
                *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) >= follow)
        partyChgMot(party, 1);
    pos[0] = *(f32*)(pParty + 0x58);
    pos[1] = *(f32*)(pParty + 0x5C);
    pos[2] = *(f32*)(pParty + 0x60);
    partyGetMoveDirSpd(party, &dir, &speed);
    *(void**)(pParty + 0x13C) = partySearchFrontWall(speed, dir, party, pos);
    *(f32*)(pParty + 0x58) = pos[0];
    *(f32*)(pParty + 0x60) = pos[2];
    partyGetMoveDirSpd(party, &dir, &speed);
    unk_800c27c0(speed, party, pos);
    *(f32*)(pParty + 0x58) = pos[0];
    *(f32*)(pParty + 0x60) = pos[2];
    if (*(s8*)(pParty + 0x4C) != 0) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        party_force_reset_outofscreen(party);
    }
    return 0;
}

s32 party_slit(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern void partyPaperOn(void* party, char* name);
    extern void partyChgPaper(void* party, char* name);
    extern void unk_800cbf84(f32 value, void* party);
    extern f32 revise360(f32 angle);
    extern s32 unk_800c2010(void* party, VecLocal* pos);
    extern u32 hitGetAttr(void* hit);
    extern void marioClearSlitFloor(void);
    extern void partyPaperOff(void* party);
    extern char str_p_slit_802f4770[];
    extern char str_PM_S_1A_802f4778[];
    extern char str_PM_S_1B_802f4780[];
    extern char str_PM_S_1E_802f4790[];
    extern char str_PM_S_1D_802f4798[];
    extern char str_PM_S_1H_802f47a0[];
    extern char str_PM_S_1C_802f47a8[];
    extern f32 float_260_80423120;
    extern f32 float_280_8042311c;
    extern f32 float_315_80423124;
    extern f32 float_45_804230e0;
    extern f32 float_135_80423128;
    extern f32 float_225_8042312c;
    extern f32 float_neg3_80423110;

    void* player;
    VecLocal pos;
    s32 ret;
    s32 timer;
    f32 dirDiff;

    ret = 1;
    player = *(void**)((s32)pParty + 0x160);
    if (*(u8*)((s32)pParty + 0x37) != 1) {
        if ((*(u32*)player & 0x00100000) == 0) {
            return 1;
        }
        if ((*(u32*)((s32)player + 0xC) & 1) != 0) {
            return 1;
        }
        *(u32*)((s32)pParty + 8) |= 1;
        *(u8*)((s32)pParty + 0x37) = 1;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        *(u32*)((s32)pParty + 4) |= 0x100;
        partyPaperOn(pParty, str_p_slit_802f4770);
        partyChgPaper(pParty, str_PM_S_1A_802f4778);
        *(s32*)((s32)pParty + 0x24) = 0x1E;
        unk_800cbf84(float_0_804230bc, pParty);
        dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
        if (dirDiff > float_180_80423108) {
            *(u32*)((s32)pParty + 4) |= 0x200;
            *(u8*)((s32)pParty + 0x3A) = 2;
        } else {
            *(u32*)((s32)pParty + 4) &= ~0x200;
            *(u8*)((s32)pParty + 0x3A) = 0;
        }
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 0:
        case 2:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 0) {
                *(u32*)pParty |= 0x1000;
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
                *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
                partyChgPaper(pParty, str_PM_S_1B_802f4780);
                if ((*(u32*)pParty & 0x2000) == 0) {
                    ret = 0;
                    *(u8*)((s32)pParty + 0x3A) = 10;
                } else {
                    *(u8*)((s32)pParty + 0x3A) = 20;
                }
            }
            break;
        case 10:
            if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
                partyChgPaper(pParty, str_PM_S_1B_802f4780);
                *(f32*)((s32)pParty + 0x10C) = float_270_80423104;
            } else {
                dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
                if (dirDiff < float_180_80423108) {
                    *(u32*)((s32)pParty + 4) &= ~0x200;
                } else {
                    *(u32*)((s32)pParty + 4) |= 0x200;
                }
                if ((*(u32*)((s32)pParty + 4) & 0x200) != 0) {
                    *(f32*)((s32)pParty + 0x10C) = float_280_8042311c;
                } else {
                    *(f32*)((s32)pParty + 0x10C) = float_260_80423120;
                }
                if (dirDiff >= float_315_80423124 || dirDiff <= float_45_804230e0 ||
                    (dirDiff >= float_135_80423128 && dirDiff <= float_225_8042312c)) {
                    partyChgPaper(pParty, str_PM_S_1E_802f4790);
                } else {
                    partyChgPaper(pParty, str_PM_S_1D_802f4798);
                }
            }
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            if (unk_800c2010(pParty, &pos) == 0 && (*(u32*)player & 0x00100000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x2000) == 0 &&
                (hitGetAttr(*(void**)((s32)pParty + 0x13C)) & 0x2000) == 0) {
                *(u8*)((s32)pParty + 0x3A) = 40;
            }
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 20:
            partyChgPaper(pParty, str_PM_S_1H_802f47a0);
            *(u32*)pParty |= 0x80;
            *(u32*)pParty |= 0x40;
            *(s32*)((s32)pParty + 0x24) = 0x14;
            unk_800cbf84(float_0_804230bc, pParty);
            *(u8*)((s32)pParty + 0x3A) = 21;
            break;
        case 21:
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)pParty + 0xFC));
            if (dirDiff <= float_180_80423108) {
                *(f32*)((s32)pParty + 0x10C) = float_260_80423120;
            } else {
                *(f32*)((s32)pParty + 0x10C) = float_280_8042311c;
            }
            *(f32*)((s32)pParty + 0x5C) += float_neg3_80423110;
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                *(u8*)((s32)pParty + 0x3A) = 22;
            }
            break;
        case 22:
            *(u8*)((s32)pParty + 0x3A) = 10;
            *(u32*)pParty &= ~0x80;
            marioClearSlitFloor();
            break;
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 40:
            partyChgPaper(pParty, str_PM_S_1C_802f47a8);
            *(s32*)((s32)pParty + 0x24) = 0x1E;
            *(u8*)((s32)pParty + 0x3A) = 41;
            dirDiff = revise360(*(f32*)((s32)pParty + 0x100) - *(f32*)((s32)player + 0x19C));
            if (float_180_80423108 <= dirDiff) {
                *(f32*)((s32)pParty + 0x10C) = float_0_804230bc;
            } else {
                *(f32*)((s32)pParty + 0x10C) = float_180_80423108;
            }
            *(f32*)((s32)pParty + 0x110) = *(f32*)((s32)pParty + 0x10C);
        case 41:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer < 1) {
                *(u8*)((s32)pParty + 0x3A) = 42;
            }
            break;
        case 42:
            *(u32*)((s32)pParty + 4) &= ~0x3C0;
            partyPaperOff(pParty);
            *(u32*)pParty &= ~0x3000;
            *(u8*)((s32)pParty + 0x37) = 0;
            break;
    }

    return ret;
}

void party_motion_behind_mario_fly(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 marioKeyOffChk(void);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySetDir(f32 x, f32 z, void* party);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void unk_800cba60(void* party);
    extern void unk_800cbfbc(f32 speed, void* party);
    extern void partyGetMoveDirSpd(void* party, f32* dir, f32* speed);
    extern void* partySearchFrontWall(f32 speed, f32 dir, void* party, VecLocal* pos);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_3p6_80423170;
    extern f32 float_0p05_80423174;
    extern f32 float_2_804230d8;
    extern f32 float_1_804230ec;
    extern f32 float_6_80423100;
    extern f32 float_50_804230b8;
    extern f32 float_100_804230b0;

    void* player;
    void* party;
    void* hit;
    VecLocal pos;
    f32 dir;
    f32 speed;
    f32 dist;
    f32 limit;
    f32 y;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(u32*)((s32)pParty + 8) |= 0x00040000;
        if ((*(u32*)((s32)pParty + 8) & 0x00100000) == 0) {
            if ((*(u32*)((s32)pParty + 8) & 0x00010000) == 0) {
                if ((*(u32*)pParty & 0x1000) == 0) {
                    dir = toMovedir(*(f32*)((s32)player + 0x1AC));
                } else {
                    party = partyGetPtr(marioGetPartyId());
                    if (party == 0) {
                        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
                    } else {
                        dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
                    }
                }
                dir = revise360(dir + float_180_80423108);
            } else {
                *(u32*)((s32)pParty + 8) &= ~0x00010000;
                dir = revise360(float_180_80423108 +
                                angleABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                                         *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C)));
            }
            *(f32*)((s32)pParty + 0x54) = dir;
        }
        partyGetAppearPos4(pParty, (VecLocal*)((s32)pParty + 0x94));
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) < 1) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if ((*(u32*)((s32)pParty + 8) & 0x00100000) == 0 && marioKeyOffChk() != 0) {
        if ((*(u32*)pParty & 0x1000) == 0) {
            dir = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                dir = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(dir + float_180_80423108);
    }

    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
             *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    limit = float_2_804230d8;
    if (*(f32*)((s32)player + 0x1B0) > float_1_804230ec) {
        limit = float_6_80423100;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                   *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    *(s32*)((s32)pParty + 0x24) -= 1;
    y = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
    if ((dist < limit && (y <= float_2_804230d8 && -y <= float_2_804230d8)) ||
        *(s32*)((s32)pParty + 0x24) < 1) {
        *(s32*)((s32)pParty + 0x24) = 0;
        if (((*(u32*)((s32)pParty + 8) & 0x00008000) != 0) && *(u8*)((s32)pParty + 0x32) != 0) {
            *(u32*)((s32)pParty + 8) &= ~0x20000;
            *(u32*)((s32)pParty + 4) &= ~0x100000;
        }
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        unk_800cbf84(float_0_804230bc, pParty);
        *(u32*)pParty |= 0x100000;
        *(u32*)((s32)pParty + 8) &= ~0x00100000;
        *(u32*)((s32)pParty + 8) &= ~0x00040000;
        *(u32*)((s32)pParty + 8) &= ~0x00008000;
        unk_800cba60(pParty);
        if (*(void**)((s32)pParty + 0x13C) != 0) {
            *(u32*)pParty |= 0x100000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x100000;
            *(u32*)((s32)pParty + 8) &= ~0x00040000;
        }
        hit = partySearchGround(float_100_804230b0, float_100_804230b0, pParty);
        if (hit != 0) {
            if ((hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
                hit = 0;
                *(u32*)((s32)pParty + 8) |= 0x01000000;
                *(u32*)pParty |= 0x02000000;
            }
            if (*(f32*)((s32)pParty + 0xF0) < dist) {
                hit = 0;
            }
            y = *(f32*)((s32)pParty + 0x5C);
            *(f32*)((s32)pParty + 0x5C) =
                float_0p05_80423174 * (*(f32*)((s32)pParty + 0x118) - y) + y;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        return;
    }

    *(f32*)((s32)pParty + 0x100) =
        angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                 *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
    speed = float_2_804230d8;
    if ((*(u32*)((s32)pParty + 8) & 0x00100000) != 0) {
        speed = float_3p6_80423170;
    }
    if (dist < limit) {
        unk_800cbf84(float_0_804230bc, pParty);
    }
    unk_800cbfbc(speed, pParty);
    pos.x = *(f32*)((s32)pParty + 0x58);
    pos.y = *(f32*)((s32)pParty + 0x5C);
    pos.z = *(f32*)((s32)pParty + 0x60);
    partyGetMoveDirSpd(pParty, &dir, &speed);
    hit = partySearchFrontWall(speed, dir, pParty, &pos);
    if (hit != 0) {
        *(void**)((s32)pParty + 0x13C) = hit;
        if (*(f32*)((s32)pParty + 0x104) != float_0_804230bc) {
            *(void**)((s32)pParty + 0x13C) = hit;
        }
    }
    *(f32*)((s32)pParty + 0x58) = pos.x;
    *(f32*)((s32)pParty + 0x60) = pos.z;
    y = *(f32*)((s32)pParty + 0x5C);
    *(f32*)((s32)pParty + 0x5C) = float_0p05_80423174 * (*(f32*)((s32)pParty + 0x98) - y) + y;
    unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
}

void party_motion_behind_mario_continue(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern void* anotherPartyGetPtr(s32 slot);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 marioKeyOffChk(void);
    extern void* hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                                void* out0, f32* outY, void* out2, f32* dist,
                                void* out4, void* out5, void* out6);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySetDir(f32 x, f32 z, void* party);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void partyMoveCheckWall(void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern f32 float_1p5_8042316c;
    extern f32 float_1000_804230c4;
    extern f32 float_100_804230b0;
    extern f32 float_0p5_804230d0;
    extern f32 float_2_804230d8;
    extern f32 float_1_804230ec;
    extern f32 float_10_804230a4;
    extern f32 float_50_804230b8;
    extern f32 float_neg1_804230c8;
    extern f32 float_neg2000_804230cc;

    void* player;
    void* party;
    void* hit;
    f32 dir;
    f32 targetX;
    f32 targetY;
    f32 targetZ;
    f32 speed;
    f32 dist;
    f32 out0;
    f32 out2;
    f32 out4;
    f32 out5;
    f32 out6;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)pParty & 0x1000) == 0) {
        dir = toMovedir(*(f32*)((s32)player + 0x1AC));
    } else {
        party = partyGetPtr(marioGetPartyId());
        if (party == 0) {
            dir = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
        }
    }
    *(f32*)((s32)pParty + 0x54) = revise360(dir + float_180_80423108);

    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             targetX, targetZ);
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) < 1) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if (marioKeyOffChk() != 0) {
        if ((*(u32*)pParty & 0x1000) == 0) {
            dir = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                dir = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                dir = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(dir + float_180_80423108);
    }

    if ((*(u32*)pParty & 0x1000) == 0) {
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        targetY = *(f32*)((s32)player + 0x90);
    } else {
        party = anotherPartyGetPtr(*(s8*)((s32)pParty + 0x2F));
        if (party == 0) {
            targetX = *(f32*)((s32)player + 0x8C);
            targetZ = *(f32*)((s32)player + 0x94);
            targetY = *(f32*)((s32)player + 0x90);
        } else {
            targetX = *(f32*)((s32)party + 0x58);
            targetZ = *(f32*)((s32)party + 0x60);
            targetY = *(f32*)((s32)party + 0x5C);
        }
    }

    movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    dist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, targetY + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &out0, &targetY, &out2, &dist, &out4, &out5, &out6);
    speed = float_neg2000_804230cc;
    if (hit != 0) {
        speed = (f32)(s32)(float_100_804230b0 * targetY + float_0p5_804230d0) /
                float_100_804230b0;
        hitGetAttr(hit);
    }
    if (speed <= float_neg2000_804230cc) {
        speed = float_1p5_8042316c * *(f32*)((s32)player + 0x1B8);
        movePos(speed, toMovedir(*(f32*)((s32)player + 0x1AC)), &targetX, &targetZ);
    }

    speed = float_2_804230d8;
    if (*(f32*)((s32)player + 0x1B0) > float_1_804230ec) {
        speed = float_10_804230a4;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist < speed) {
        *(s32*)((s32)pParty + 0x24) = 0;
        if (*(f32*)((s32)player + 0x1B0) == float_0_804230bc) {
            *(u32*)pParty |= 0x100000;
            partyChgMoveMode(pParty, 1);
            partyChgMot(pParty, 0);
            partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
            *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
            *(u32*)((s32)pParty + 4) &= ~0x100000;
        }
    } else {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4;
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
            hit = 0;
            *(u32*)((s32)pParty + 8) |= 0x01000000;
            *(u32*)pParty |= 0x02000000;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    }

    if (*(void**)((s32)pParty + 0x13C) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(targetX, targetZ, pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
    }
}

void party_force_reset_outofscreen(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        char pad[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern s32 marioCtrlOffChk(void);
    extern s32 marioKeyOffChk(void);
    extern void marioGetScreenPos(VecLocal* pos, f32* x, f32* y, f32* z);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 partyGetAppearPos3(void* party, VecLocal* pos);
    extern void partyClearFootmark(void);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern void* partyHitCheck(void* party, VecLocal* pos, VecLocal* dir, VecLocal* outPos, HitWork* work, f32* dist);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_20_80423090;
    extern f32 float_800_80423094;
    extern f32 float_neg100_80423098;
    extern f32 float_700_8042309c;
    extern f32 float_650_804230a0;
    extern f32 float_10_804230a4;
    extern f32 float_200_804230a8;
    extern f32 float_300_804230ac;
    extern f32 float_100_804230b0;
    extern f32 float_74_804230b4;
    extern f32 float_50_804230b8;

    void* player;
    VecLocal screenPos;
    VecLocal pos;
    VecLocal checkPos;
    VecLocal dir;
    VecLocal hitPos;
    HitWork work;
    f32 screenX;
    f32 screenY;
    f32 screenZ;
    f32 dist;
    f32 step;
    f32 angle;
    f32 y;
    f32 oldY;
    void* hit;
    s32 xLimit;
    s32 outerCount;
    s32 innerCount;

    player = *(void**)((s32)pParty + 0x160);
    if (marioCtrlOffChk() != 0) {
        return;
    }
    if (marioKeyOffChk() != 0) {
        return;
    }

    screenPos.x = *(f32*)((s32)pParty + 0x58);
    screenPos.y = *(f32*)((s32)pParty + 0x5C);
    screenPos.z = *(f32*)((s32)pParty + 0x60);
    marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);

    if (distABf(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94),
                *(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60)) < float_20_80423090 &&
        screenY > float_800_80423094) {
        partyGetAppearPos3(pParty, &pos);
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x5C) = pos.y;
        *(f32*)((s32)pParty + 0x60) = pos.z;
        return;
    }

    if (screenX >= float_neg100_80423098 && screenX <= float_700_8042309c &&
        screenY <= float_650_804230a0) {
        return;
    }

    partyClearFootmark();
    player = *(void**)((s32)pParty + 0x160);
    xLimit = 100;
    if (*(u8*)((s32)pParty + 0x30) != 0) {
        if (*(s8*)((s32)pParty + 0x31) == 5) {
            xLimit = 70;
        } else {
            xLimit = 50;
        }
        step = float_10_804230a4;
    } else {
        step = *(f32*)((s32)pParty + 0xF4);
    }

    pos.x = *(f32*)((s32)pParty + 0x58);
    pos.y = *(f32*)((s32)pParty + 0x5C);
    pos.z = *(f32*)((s32)pParty + 0x60);
    dir.x = float_0_804230bc;
    dir.y = -float_300_804230ac;
    dir.z = float_0_804230bc;

    for (outerCount = 0; outerCount <= 100; outerCount++) {
        screenPos = pos;
        marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);
        if (screenX >= (f32)-xLimit && screenX <= (f32)(xLimit + 600)) {
            break;
        }

        angle = angleABf(pos.x, pos.z, *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        movePos(step, angle, &pos.x, &pos.z);
        oldY = pos.y;
        y = oldY;
        for (innerCount = 0; innerCount <= 4; innerCount++) {
            dist = float_300_804230ac;
            checkPos.x = pos.x;
            checkPos.y = pos.y + float_200_804230a8 - (float_50_804230b8 * (f32)innerCount);
            checkPos.z = pos.z;
            hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
            if (hit != 0) {
                y = hitPos.y;
                if ((hitGetAttr(hit) & 0x200) != 0) {
                    y = oldY;
                }
            } else {
                y = *(f32*)((s32)player + 0x90);
            }
            screenPos.x = checkPos.x;
            screenPos.y = y;
            screenPos.z = checkPos.z;
            marioGetScreenPos(&screenPos, &screenX, &screenY, &screenZ);
            if (screenY > float_100_804230b0) {
                break;
            }
        }

        if (y < pos.y + float_74_804230b4) {
            dist = float_100_804230b0;
            checkPos.x = pos.x;
            checkPos.y = pos.y + float_74_804230b4;
            checkPos.z = pos.z;
            hit = partyHitCheck(pParty, &checkPos, &dir, &hitPos, &work, &dist);
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                y = hitPos.y;
            }
        }
        pos.y = y;
    }

    if (outerCount <= 100) {
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x5C) = pos.y;
        *(f32*)((s32)pParty + 0x60) = pos.z;
    }
}

void party_motion_beside_mario(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 toMovedir(f32 dir);
    extern s32 marioGetPartyId(void);
    extern void* partyGetPtr(s32 id);
    extern f32 partyToMovedir(f32 dir, void* party);
    extern f32 revise360(f32 dir);
    extern void movePos(f32 speed, f32 angle, f32* x, f32* z);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern s32 marioKeyOffChk(void);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void* hitCheckFilter(f32 x, f32 y, f32 z, f32 dx, f32 dy, f32 dz, s32 flags,
                                void* out0, f32* outY, void* out2, f32* dist,
                                void* out4, void* out5, void* out6);
    extern u32 hitGetAttr(void* hit);
    extern void unk_800cbf84(f32 speed, void* party);
    extern void partyMoveCheckWall(void* party);
    extern void unk_800cbb10(f32 speed, void* party);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern void partyChgMot(void* party, s32 mot);
    extern void partyChgMoveMode(void* party, s32 mode);
    extern void partySetDir(f32 x, f32 z, void* party);
    extern f32 float_1p5_8042316c;
    extern f32 float_90_80423118;
    extern f32 float_neg1_804230c8;
    extern f32 float_neg2000_804230cc;
    extern f32 float_1000_804230c4;
    extern f32 float_100_804230b0;
    extern f32 float_0p5_804230d0;
    extern f32 float_2_804230d8;
    extern f32 float_1_804230ec;
    extern f32 float_10_804230a4;
    extern f32 float_50_804230b8;

    void* player;
    void* party;
    void* hit;
    f32 x;
    f32 y;
    f32 z;
    f32 dist;
    f32 targetX;
    f32 targetZ;
    f32 speed;
    u32 attr;
    s32 timer;
    f32 out0;
    f32 out2;
    f32 out4;
    f32 out5;
    f32 out6;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        if ((*(u32*)pParty & 0x1000) == 0) {
            speed = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                speed = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
        timer = (s32)distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                             targetX, targetZ);
        *(s32*)((s32)pParty + 0x24) = timer;
        if (*(s32*)((s32)pParty + 0x24) < 1) {
            *(s32*)((s32)pParty + 0x24) = 1;
        }
    }

    if (marioKeyOffChk() != 0) {
        if ((*(u32*)pParty & 0x1000) == 0) {
            speed = toMovedir(*(f32*)((s32)player + 0x1AC));
        } else {
            party = partyGetPtr(marioGetPartyId());
            if (party == 0) {
                speed = toMovedir(*(f32*)((s32)player + 0x1AC));
            } else {
                speed = partyToMovedir(*(f32*)((s32)party + 0x10C), pParty);
            }
        }
        *(f32*)((s32)pParty + 0x54) = revise360(speed + float_90_80423118);
    }

    targetX = *(f32*)((s32)player + 0x8C);
    targetZ = *(f32*)((s32)player + 0x94);
    y = *(f32*)((s32)player + 0x90);
    movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
            *(f32*)((s32)pParty + 0x54), &targetX, &targetZ);
    angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);

    dist = float_1000_804230c4;
    hit = hitCheckFilter(targetX, y + touch_wall_ct, targetZ,
                         float_0_804230bc, float_neg1_804230c8, float_0_804230bc,
                         0, &out0, &y, &out2, &dist, &out4, &out5, &out6);
    z = float_neg2000_804230cc;
    if (hit != 0) {
        z = (f32)(s32)(float_100_804230b0 * y + float_0p5_804230d0) / float_100_804230b0;
        hitGetAttr(hit);
    }
    if (z <= float_neg2000_804230cc) {
        targetX = *(f32*)((s32)player + 0x8C);
        targetZ = *(f32*)((s32)player + 0x94);
        movePos(float_1p5_8042316c * *(f32*)((s32)player + 0x1B8),
                toMovedir(*(f32*)((s32)player + 0x1AC)), &targetX, &targetZ);
    }

    speed = float_2_804230d8;
    if (*(f32*)((s32)player + 0x1B0) > float_1_804230ec) {
        speed = float_10_804230a4;
    }
    dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
    *(s32*)((s32)pParty + 0x24) -= 1;
    if (dist < speed || *(s32*)((s32)pParty + 0x24) < 1) {
        *(s32*)((s32)pParty + 0x24) = 0;
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        *(u32*)pParty |= 0x100000;
    } else {
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60), targetX, targetZ);
        speed = dist / float_10_804230a4;
        if (speed > float_2_804230d8) {
            speed = float_2_804230d8;
        }
        if (speed < float_1_804230ec) {
            speed = float_1_804230ec;
        }
        unk_800cbf84(speed, pParty);
        partyMoveCheckWall(pParty);
        unk_800cbb10(*(f32*)((s32)pParty + 0x104), pParty);
        hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
        if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(s8*)((s32)pParty + 0x4C) == 0) {
            hit = 0;
            *(u32*)((s32)pParty + 8) |= 0x01000000;
            *(u32*)pParty |= 0x02000000;
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
        }
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    }

    if (*(void**)((s32)pParty + 0x13C) != 0) {
        partyChgMoveMode(pParty, 1);
        partyChgMot(pParty, 0);
        partySetDir(*(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94), pParty);
        *(f32*)((s32)pParty + 0x40) = float_50_804230b8;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
    }
}

u32 getFrontFloor(f32 angle, void* pParty, f32* outPos) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    typedef struct HitWork {
        char pad[0xC];
        VecLocal start;
        VecLocal dir;
        VecLocal hitPos;
        VecLocal normal;
        f32 dist;
    } HitWork;
    extern void sincosf(f32 angle, f32* sinOut, f32* cosOut);
    extern s32 hitCheckVecFilter(void* work, void* filter);
    extern u32 hitGetAttr(void* hit);
    extern f32 float_2_804230d8;
    extern f32 float_45_804230e0;
    extern f32 float_100_804230b0;
    extern f32 float_300_804230ac;
    extern f32 float_neg2000_804230cc;
    extern f32 float_0p75_804230e4;
    extern f32 float_0p5_804230d0;
    extern f32 float_neg0p5_804230d4;
    extern f32 float_1000_804230c4;

    HitWork probe;
    HitWork floor;
    f32 s;
    f32 c;
    f32 step;
    f32 bestY;
    f32 xzLen;
    f32 maxY;
    f32 bias;
    s32 i;
    void* hit;

    step = (float_2_804230d8 * *(f32*)((s32)pParty + 0xF4)) / float_3_804230dc;
    sincosf(angle, &s, &c);

    probe.dir.x = float_0_804230bc;
    probe.dir.y = -float_100_804230b0;
    probe.dir.z = float_0_804230bc;
    bestY = float_neg2000_804230cc;
    for (i = 0; i < 3; i++) {
        probe.start.x = *(f32*)((s32)pParty + 0x58) + (f32)i * s * step;
        probe.start.y = *(f32*)((s32)pParty + 0x5C) + *(f32*)((s32)pParty + 0xF0);
        probe.start.z = *(f32*)((s32)pParty + 0x60) + (f32)i * c * step;
        probe.dist = float_100_804230b0;
        hit = (void*)hitCheckVecFilter(&probe, 0);
        if (hit != 0) {
            xzLen = probe.normal.x * probe.normal.x + probe.normal.z * probe.normal.z;
            if (xzLen < float_45_804230e0 * float_45_804230e0) {
                bestY = probe.hitPos.y;
                break;
            }
        }
    }

    maxY = *(f32*)((s32)pParty + 0x5C) + touch_wall_ct;
    if (bestY <= float_neg2000_804230cc) {
        bestY = maxY;
    } else {
        bestY -= float_2_804230d8;
        if (maxY < bestY) {
            bestY = maxY;
        }
    }

    step = float_0p75_804230e4 * *(f32*)((s32)pParty + 0xF4);
    sincosf(angle, &s, &c);
    floor.start.x = *(f32*)((s32)pParty + 0x58) + s * step;
    floor.start.y = bestY;
    floor.start.z = *(f32*)((s32)pParty + 0x60) + c * step;
    floor.dir.x = float_0_804230bc;
    floor.dir.y = -float_300_804230ac;
    floor.dir.z = float_0_804230bc;
    floor.dist = float_300_804230ac;

    hit = (void*)hitCheckVecFilter(&floor, 0);
    if (hit == 0) {
        outPos[0] = *(f32*)((s32)pParty + 0x58);
        outPos[1] = *(f32*)((s32)pParty + 0x5C);
        outPos[2] = *(f32*)((s32)pParty + 0x60);
        return 0;
    }

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.x >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[0] = (f32)(s32)(floor.hitPos.x * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.y >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[1] = (f32)(s32)(floor.hitPos.y * float_1000_804230c4 + bias) / float_1000_804230c4;

    bias = float_neg0p5_804230d4;
    if (floor.hitPos.z >= float_0_804230bc) {
        bias = float_0p5_804230d0;
    }
    outPos[2] = (f32)(s32)(floor.hitPos.z * float_1000_804230c4 + bias) / float_1000_804230c4;
    return hitGetAttr(hit);
}

s32 party_roll(void* pParty) {
    extern void partyPaperOn(void* party, char* name);
    extern void partyChgPaper(void* party, char* name);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void unk_800cbf84(f32 value, void* party);
    extern f32 revise360(f32 angle);
    extern void partyPaperLightOff(void* party);
    extern void partyPaperOff(void* party);
    extern char str_p_roll_802f4750[];
    extern char str_PM_R_1A_802f4758[];
    extern char str_PM_R_1B_802f4760[];
    extern char str_PM_R_1C_802f4768[];
    extern f32 float_neg6_804230fc;
    extern f32 float_6_80423100;
    extern f32 float_2_804230d8;
    extern f32 float_7_8042310c;
    extern f32 float_neg3_80423110;
    extern f32 float_0p1_80423114;
    extern f32 float_90_80423118;
    extern f32 float_1_804230ec;
    extern f32 vec3_802f4670;
    extern f32 DAT_802f4674;
    extern f32 DAT_802f4678;
    extern f32 vec3_802f467c;
    extern f32 DAT_802f4680;
    extern f32 DAT_802f4684;
    f32 value;

    if (*(u8*)((s32)pParty + 0x37) != 2) {
        return 1;
    }

    if ((*(u32*)((s32)pParty + 8) & 1) != 0) {
        *(u32*)((s32)pParty + 8) &= ~1;
        partyPaperOn(pParty, str_p_roll_802f4750);
        partyChgPaper(pParty, str_PM_R_1A_802f4758);
        partyChgPoseId(pParty, 0);
        *(f32*)((s32)pParty + 0x88) = float_neg6_804230fc;
        *(f32*)((s32)pParty + 0x8C) = float_6_80423100;
        *(f32*)((s32)pParty + 0x7C) = float_6_80423100;
        *(f32*)((s32)pParty + 0x80) = float_2_804230d8;
        *(s32*)((s32)pParty + 0x24) = 0x41;
        *(u8*)((s32)pParty + 0x3A) = 0;
        unk_800cbf84(float_0_804230bc, pParty);
    }

    switch (*(u8*)((s32)pParty + 0x3A)) {
        case 0:
            *(s32*)((s32)pParty + 0x24) -= 1;
            if (*(s32*)((s32)pParty + 0x24) <= 0) {
                partyChgPaper(pParty, str_PM_R_1B_802f4760);
                *(u8*)((s32)pParty + 0x3A) = 10;
            }
            break;
        case 10:
            *(f32*)((s32)pParty + 0x10C) =
                revise360(float_270_80423104 - *(f32*)((s32)pParty + 0x100));
            value = revise360(*(f32*)((s32)pParty + 0xFC) + *(f32*)((s32)pParty + 0x10C));
            if (value > float_180_80423108) {
                *(f32*)((s32)pParty + 0x6C) =
                    revise360(float_7_8042310c * *(f32*)((s32)pParty + 0x104) +
                              *(f32*)((s32)pParty + 0x6C));
            } else {
                *(f32*)((s32)pParty + 0x6C) =
                    revise360(*(f32*)((s32)pParty + 0x6C) -
                              float_7_8042310c * *(f32*)((s32)pParty + 0x104));
            }
            break;
    }

    if (*(u8*)((s32)pParty + 0x3A) == 100) {
        *(f32*)((s32)pParty + 0x104) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x64) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x6C) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x88) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x8C) = float_0_804230bc;
        value = float_neg3_80423110;
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) {
            value = float_3_804230dc;
        }
        *(f32*)((s32)pParty + 0x7C) = value;
        *(f32*)((s32)pParty + 0x80) = float_0_804230bc;
        *(s32*)((s32)pParty + 0x24) = 0x41;
        *(u8*)((s32)pParty + 0x3A) = 101;
        partyChgPaper(pParty, str_PM_R_1C_802f4768);
        partyPaperLightOff(pParty);
    } else if (*(u8*)((s32)pParty + 0x3A) != 101) {
        return 1;
    }

    *(s32*)((s32)pParty + 0x24) -= 1;
    if (*(s32*)((s32)pParty + 0x24) <= 0) {
        if (*(f32*)((s32)pParty + 0x100) < float_180_80423108) {
            *(f32*)((s32)pParty + 0x10C) = float_180_80423108;
            *(f32*)((s32)pParty + 0x110) = float_180_80423108;
            *(f32*)((s32)pParty + 0xFC) = float_90_80423118;
            *(f32*)((s32)pParty + 0x100) = float_90_80423118;
        } else {
            *(f32*)((s32)pParty + 0x10C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x110) = float_0_804230bc;
            *(f32*)((s32)pParty + 0xFC) = float_270_80423104;
            *(f32*)((s32)pParty + 0x100) = float_270_80423104;
        }
        *(f32*)((s32)pParty + 0x88) = vec3_802f4670;
        *(f32*)((s32)pParty + 0x8C) = DAT_802f4674;
        *(f32*)((s32)pParty + 0x90) = DAT_802f4678;
        *(f32*)((s32)pParty + 0x7C) = vec3_802f467c;
        *(f32*)((s32)pParty + 0x80) = DAT_802f4680;
        *(f32*)((s32)pParty + 0x84) = DAT_802f4684;
        *(u8*)((s32)pParty + 0x37) = 0;
        *(u32*)pParty &= ~0x1000;
        *(u32*)((s32)pParty + 4) &= ~0x1000;
        *(u32*)pParty &= ~0x200;
        *(u32*)((s32)pParty + 4) &= ~0x100000;
        *(u32*)((s32)pParty + 4) &= ~0x200000;
        *(u8*)((s32)pParty + 0x3A) = 100;
        partyPaperOff(pParty);
        return 1;
    }

    if (*(f32*)((s32)pParty + 0x100) >= float_180_80423108) {
        value = float_0p1_80423114 * (float_270_80423104 - *(f32*)((s32)pParty + 0x10C)) +
                *(f32*)((s32)pParty + 0x10C);
        if ((value - float_270_80423104) < float_1_804230ec &&
            (float_270_80423104 - value) < float_1_804230ec) {
            value = float_270_80423104;
        }
    } else {
        value = float_0p1_80423114 * (float_90_80423118 - *(f32*)((s32)pParty + 0x10C)) +
                *(f32*)((s32)pParty + 0x10C);
        if ((value - float_90_80423118) < float_1_804230ec &&
            (float_90_80423118 - value) < float_1_804230ec) {
            value = float_90_80423118;
        }
    }
    *(f32*)((s32)pParty + 0x10C) = value;
    *(f32*)((s32)pParty + 0x110) = value;
    return 0;
}

s32 party_motion_fall(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 partyGetFootmarkId(void* party);
    extern void partyNextReadId(s32 id);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern s32 marioKeyOffChk(void);
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 toMovedir(f32 dir);
    extern f32 revise360(f32 dir);
    extern void partyGetAppearPos4(void* party, VecLocal* pos);
    extern void partyChgMot(void* party, s32 mot);
    extern void partySearchFrontWall(f32 distance, f32 angle, void* party, VecLocal* pos);
    extern void N_partyPlaySfxPartyLanding1(void* party);
    extern void partyClearFootmark(void);
    extern void partyClearFootmark2(void);
    extern f32 float_neg1p4_8042313c;
    extern f32 float_10_804230a4;
    extern f32 float_neg2000_804230cc;
    extern f32 float_neg16_80423140;
    extern f32 float_neg0p2_80423144;
    extern f32 float_neg2001_80423148;
    extern f32 float_37_8042314c;
    extern f32 float_1p3_80423150;

    void* player;
    void* hit;
    VecLocal pos;
    f32 velAbs;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)pParty & 0x1000) == 0) {
        partyNextReadId(partyGetFootmarkId(pParty));
    }

    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        partyChgPoseId(pParty, 4);
        *(u32*)pParty &= ~0x100000;
    }

    hit = 0;
    if ((*(u32*)pParty & 0x02000000) == 0) {
        velAbs = *(f32*)((s32)pParty + 0x114);
        if (velAbs < float_0_804230bc) {
            velAbs = -velAbs;
        }
        hit = partySearchGround(float_10_804230a4, velAbs, pParty);
    }

    if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0 && *(u8*)((s32)pParty + 0x32) == 0 &&
        *(s8*)((s32)pParty + 0x4C) == 0) {
        hit = 0;
        *(u32*)((s32)pParty + 8) |= 0x01000000;
        *(u32*)pParty |= 0x02000000;
    }
    *(void**)((s32)pParty + 0x138) = hit;

    if (hit == 0) {
        *(u32*)pParty &= ~0x20;
        if ((*(u32*)pParty & 0x40) == 0) {
            *(u32*)pParty |= 0x40;
            if (*(f32*)((s32)pParty + 0x11C) == float_0_804230bc) {
                *(f32*)((s32)pParty + 0x11C) = float_neg1p4_8042313c;
            }
        }
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x11C);
        if (*(f32*)((s32)pParty + 0x114) < float_neg16_80423140) {
            *(f32*)((s32)pParty + 0x114) = float_neg16_80423140;
        }
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);

        if (marioKeyOffChk() == 0) {
            *(f32*)((s32)pParty + 0x104) += float_neg0p2_80423144;
            if (*(f32*)((s32)pParty + 0x104) < float_0_804230bc) {
                *(f32*)((s32)pParty + 0x104) = float_0_804230bc;
            }
        }

        if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
            *(f32*)((s32)pParty + 0x5C) = float_neg2001_80423148;
            if (*(u16*)((s32)player + 0x2E) == 0x1F || *(u16*)((s32)player + 0x2E) == 0x20) {
                return 2;
            }
            *(u32*)pParty &= ~0xE0;
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x11C) = float_0_804230bc;
            *(f32*)((s32)pParty + 0x100) =
                angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                         *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
            *(f32*)((s32)pParty + 0x54) =
                revise360(float_180_80423108 + toMovedir(*(f32*)((s32)player + 0x1A4)));
            partyGetAppearPos4(pParty, (VecLocal*)((s32)pParty + 0x58));
            *(f32*)((s32)pParty + 0x5C) += float_37_8042314c;
            partyChgMot(pParty, 0);
            *(u32*)pParty &= ~0x02000000;
        }

        if (*(f32*)((s32)pParty + 0x104) != float_0_804230bc) {
            pos.x = *(f32*)((s32)pParty + 0x58);
            pos.y = *(f32*)((s32)pParty + 0x5C);
            pos.z = *(f32*)((s32)pParty + 0x60);
            partySearchFrontWall(float_1p3_80423150 * *(f32*)((s32)pParty + 0x104),
                                 *(f32*)((s32)pParty + 0x100), pParty, &pos);
            *(f32*)((s32)pParty + 0x58) = pos.x;
            *(f32*)((s32)pParty + 0x60) = pos.z;
        }
        return 2;
    }

    if ((*(u32*)pParty & 0x40) == 0) {
        *(u32*)pParty &= ~0x2000;
        *(u32*)pParty &= ~0xE0;
        *(u32*)pParty |= 0x20;
        *(u32*)((s32)pParty + 8) &= ~0x200000;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
        if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
            partyChgMot(pParty, 0);
        } else {
            partyChgMot(pParty, 1);
        }
        N_partyPlaySfxPartyLanding1(pParty);
        if ((*(u32*)pParty & 0x1000) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        return 3;
    }

    *(u32*)pParty &= ~0xE0;
    *(u32*)pParty |= 0x20;
    *(u32*)((s32)pParty + 8) &= ~0x200000;
    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
    *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
    if (*(f32*)((s32)pParty + 0x104) == float_0_804230bc) {
        partyChgMot(pParty, 0);
    } else {
        partyChgMot(pParty, 1);
    }
    N_partyPlaySfxPartyLanding1(pParty);
    return 3;
}

void party_motion_jump2(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern f32 angleABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyChgPoseId(void* party, s32 poseId);
    extern u32 psndSFXOn_3D(s32 id, void* pos);
    extern void partySearchFrontWall(f32 distance, f32 angle, void* party, VecLocal* pos);
    extern void* partySearchGround(f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern void N_partyPlaySfxPartyLanding1(void* party);
    extern f32 float_neg1_804230c8;
    extern f32 float_2p25_80423154;
    extern f32 float_50_804230b8;
    extern f32 float_10_804230a4;
    extern f32 float_0p5_804230d0;
    extern f32 float_20_80423090;
    extern f32 float_neg2000_804230cc;
    extern f32 float_5_804230e8;

    void* player;
    void* hit;
    VecLocal pos;
    f32 dist;
    f32 frames;
    f32 yDelta;
    f32 velAbs;
    s32 timer;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(s32*)((s32)pParty + 0x24) = 0;
        *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8 * *(f32*)((s32)player + 0x228);
        *(f32*)((s32)pParty + 0x104) = float_2p25_80423154 * *(f32*)((s32)player + 0x228);
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                     *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                       *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
        if (dist > float_50_804230b8) {
            dist = float_50_804230b8;
        }
        *(f32*)((s32)pParty + 0x128) = dist;
        timer = (s32)((float_10_804230a4 * (dist / *(f32*)((s32)pParty + 0x104)) +
                       float_0p5_804230d0) / float_10_804230a4);
        if (timer < 10) {
            timer = 10;
        }
        *(s32*)((s32)pParty + 0x24) = timer;
        frames = (f32)timer;
        *(f32*)((s32)pParty + 0x104) = *(f32*)((s32)player + 0x228) * (dist / frames);
        yDelta = *(f32*)((s32)pParty + 0x98) - *(f32*)((s32)pParty + 0x5C);
        velAbs = *(f32*)((s32)pParty + 0x11C);
        if (velAbs < float_0_804230bc) {
            velAbs = -velAbs;
        }
        *(f32*)((s32)pParty + 0x114) =
            (float_0p5_804230d0 * frames * frames * velAbs + yDelta) / frames;
        *(f32*)((s32)pParty + 0x114) *= *(f32*)((s32)player + 0x228);
        if (*(f32*)((s32)pParty + 0x114) > float_20_80423090) {
            *(f32*)((s32)pParty + 0x114) = float_20_80423090;
        }
        if (*(f32*)((s32)pParty + 0x114) < float_0_804230bc) {
            *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        }
        partyChgPoseId(pParty, 4);
        *(u32*)pParty |= 0x80;
        *(u32*)pParty &= ~0x20;
        psndSFXOn_3D(0x927, (void*)((s32)pParty + 0x58));
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    if (*(u8*)((s32)pParty + 0x38) == 1) {
        *(f32*)((s32)pParty + 0x114) += *(f32*)((s32)pParty + 0x11C);
        *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
        pos.x = *(f32*)((s32)pParty + 0x58);
        pos.y = *(f32*)((s32)pParty + 0x5C);
        pos.z = *(f32*)((s32)pParty + 0x60);
        partySearchFrontWall(*(f32*)((s32)pParty + 0x104), *(f32*)((s32)pParty + 0x100),
                             pParty, &pos);
        *(f32*)((s32)pParty + 0x58) = pos.x;
        *(f32*)((s32)pParty + 0x60) = pos.z;
        if (*(f32*)((s32)pParty + 0x114) <= float_0_804230bc) {
            velAbs = *(f32*)((s32)pParty + 0x114);
            if (velAbs < float_0_804230bc) {
                velAbs = -velAbs;
            }
            hit = partySearchGround(velAbs, pParty);
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0x118);
            if (hit != 0 && (hitGetAttr(hit) & 0x200) != 0) {
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
                *(void**)((s32)pParty + 0x138) = 0;
                partyChgMot(pParty, 3);
                *(f32*)((s32)pParty + 0x11C) = float_neg1_804230c8;
                return;
            }
            *(void**)((s32)pParty + 0x138) = hit;
            if (hit != 0) {
                N_partyPlaySfxPartyLanding1(pParty);
                *(u32*)pParty &= ~0xE0;
                *(u32*)pParty |= 0x20;
                *(u32*)((s32)pParty + 8) &= ~0x200000;
                *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                dist = distABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                               *(f32*)((s32)pParty + 0x94), *(f32*)((s32)pParty + 0x9C));
                partyChgMot(pParty, dist >= float_5_804230e8);
            }
        }
    } else if (*(u8*)((s32)pParty + 0x38) == 0) {
        *(u8*)((s32)pParty + 0x38) = 1;
    }

    if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
        *(u32*)pParty &= ~0x80;
        *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
        *(f32*)((s32)pParty + 0x100) =
            angleABf(*(f32*)((s32)pParty + 0x58), *(f32*)((s32)pParty + 0x60),
                     *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94));
        *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
        *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
        *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
        partyChgMot(pParty, 0);
    }
}

void party_motion_damage(void* pParty) {
    extern void partyChgPoseId(void* party, s32 poseId);
    extern void marioGetScreenPos(void* pos, f32* x, f32* y, f32* z);
    extern void partyGetReturnPos(void* pos);
    extern void unk_800cbf84(void* party);
    extern void partyMove(void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    extern f32 float_0_804230bc;
    extern f32 float_neg1_804230c8;
    extern f32 float_neg6_804230fc;
    extern f32 float_15_80423130;
    extern f32 float_neg300_80423134;
    extern f32 float_240_80423138;
    extern f32 float_200_804230a8;
    extern f32 float_neg2000_804230cc;
    void* player;
    f32 posA[3];
    f32 posB[3];
    f32 x;
    f32 y;
    f32 z;
    s32 timer;
    u8 state;

    player = *(void**)((s32)pParty + 0x160);
    if ((*(u32*)((s32)pParty + 8) & 2) != 0) {
        *(u32*)((s32)pParty + 8) &= ~2;
        *(u8*)((s32)pParty + 0x38) = 0;
    }

    state = *(u8*)((s32)pParty + 0x38);
    switch (state) {
        case 0:
            partyChgPoseId(pParty, 8);
            *(f32*)((s32)pParty + 0x114) = float_15_80423130;
            *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
            *(s32*)((s32)pParty + 0x24) = 0x3C;
            *(u8*)((s32)pParty + 0x38) = 1;
        case 1:
            *(f32*)((s32)pParty + 0x5C) += *(f32*)((s32)pParty + 0x114);
            posA[0] = *(f32*)((s32)pParty + 0x58);
            posA[1] = *(f32*)((s32)pParty + 0x5C);
            posA[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posA, &x, &y, &z);
            if (y <= float_neg300_80423134) {
                *(s32*)((s32)pParty + 0x24) = 0x14;
                *(u8*)((s32)pParty + 0x38) = 0xA;
            }
            break;
        case 10:
            if (*(u16*)((s32)player + 0x2E) != 0x1F && *(u16*)((s32)player + 0x2E) != 0x20) {
                timer = *(s32*)((s32)pParty + 0x24) - 1;
                *(s32*)((s32)pParty + 0x24) = timer;
                if (timer <= 0) {
                    partyGetReturnPos(posB);
                    *(f32*)((s32)pParty + 0x58) = posB[0];
                    *(f32*)((s32)pParty + 0x5C) = posB[1] + float_200_804230a8;
                    *(f32*)((s32)pParty + 0x60) = posB[2];
                    *(f32*)((s32)pParty + 0x114) = float_0_804230bc;
                    *(f32*)((s32)pParty + 0x118) = float_neg1_804230c8;
                    unk_800cbf84(pParty);
                    *(u32*)pParty |= 0x20;
                    *(u32*)pParty |= 0x02000000;
                    *(u8*)((s32)pParty + 0x38) = 0xB;
                }
            }
            break;
        case 11:
            posB[0] = *(f32*)((s32)pParty + 0x58);
            posB[1] = *(f32*)((s32)pParty + 0x5C);
            posB[2] = *(f32*)((s32)pParty + 0x60);
            marioGetScreenPos(posB, &x, &y, &z);
            if (y > float_240_80423138) {
                *(u32*)pParty &= ~0x02000000;
            }
            partyMove(pParty);
            if (*(f32*)((s32)pParty + 0x114) <= float_neg6_804230fc) {
                *(f32*)((s32)pParty + 0x118) = float_0_804230bc;
                *(f32*)((s32)pParty + 0x114) = float_neg6_804230fc;
            }
            if (*(f32*)((s32)pParty + 0x5C) < float_neg2000_804230cc) {
                *(f32*)((s32)pParty + 0x58) = *(f32*)((s32)player + 0x8C);
                *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)player + 0x90);
                *(f32*)((s32)pParty + 0x60) = *(f32*)((s32)player + 0x94);
                *(f32*)((s32)pParty + 0x5C) += float_200_804230a8;
                *(u32*)pParty &= ~0xE0;
            }
            if (*(void**)((s32)pParty + 0x138) != 0 && (hitGetAttr(*(void**)((s32)pParty + 0x138)) & 0x200) == 0) {
                *(u8*)((s32)pParty + 0x38) = 0xC;
                *(s32*)((s32)pParty + 0x24) = 0x1E;
            }
            break;
        case 12:
            timer = *(s32*)((s32)pParty + 0x24) - 1;
            *(s32*)((s32)pParty + 0x24) = timer;
            if (timer <= 0) {
                partyChgRunMode(pParty, 2);
                *(u8*)((s32)pParty + 0x3B) = 0;
                *(u32*)((s32)pParty + 4) &= ~0x18;
            }
            break;
    }
}

void unk_8014c330(void* party) {
    if (party != 0) {
        if (*(u8*)((s32)party + 0x37) == 1) {
            if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
                PARTY_FORCE_SLIT_OFF_BODY(party);
            }
        }
        *(u8*)((s32)party + 0x37) = 0;
    }
}


void L_partyForceSlitOff(void* party) {
    if (PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}


void U_allPartySlitOffReq(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        *(u32*)((s32)party + 4) &= ~0x3C0;
        partyPaperOff(party);
        *(u32*)party &= ~0x3000;
        *(u8*)((s32)party + 0x37) = 0;
        partyChgPoseId(party, 1);
    }
}


void allPartyForceSlitOff(void) {
    void* party = partyGetPtr(marioGetPartyId());

    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0 && PARTY_CAN_FORCE_SLIT_OFF(party)) {
        PARTY_FORCE_SLIT_OFF_BODY(party);
    }
}


void party_motion_upstairs(void* party) {
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        f32 divisor;
        *(u32*)((s32)party + 8) &= ~2;
        divisor = float_3_804230dc;
        *(u32*)party |= 0x40;
        unk_8041ec0c = 3;
        unk_8041ec08 = (*(f32*)((s32)party + 0x120) - *(f32*)((s32)party + 0x5C)) / divisor;
    }
    *(f32*)((s32)party + 0x5C) += unk_8041ec08;
    unk_8041ec0c--;
    if (unk_8041ec0c <= 0) {
        *(u32*)party &= ~0x40;
        partyChgMot(party, 1);
    }
}


void allPartySlitOn(void) {
    void* party = partyGetPtr(marioGetPartyId());
    void* unk;

    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }

    party = partyGetPtr(marioGetExtraPartyId());
    if (party != 0) {
        unk = *(void**)((s32)party + 0x160);
        if ((*(u32*)((s32)unk + 0xC) & 0x80000000) == 0) {
            *(u32*)((s32)party + 8) |= 1;
            *(u8*)((s32)party + 0x37) = 1;
        }
    }
}


/* CHATGPT FALLBACK MISSING STUBS: main/party/party_motion 20260624_191429 */

/* fallback stub-fill: map=unk_80150fac addr=0x80150fac size=0x00000334 */
#pragma no_register_save_helpers on
#pragma use_lmw_stmw off
void unk_80150fac(void* pParty) {
    typedef struct VecLocal {
        f32 x;
        f32 y;
        f32 z;
    } VecLocal;
    extern s32 strcmp(const char* s1, const char* s2);
    extern void* partySearchHead(f32 dir, void* party, VecLocal* pos, f32* height);
    extern void* partySearchGround(f32 height, f32 vel, void* party);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void* gp;
    extern char str_las_10_802f47c8[];
    extern f32 float_10_804230a4;
    extern f32 float_0_804230bc;
    extern f32 float_0p1_80423114;
    extern f32 float_0p2_80423194;
    extern f32 float_0p05_80423174;
    extern f32 float_5_804230e8;
    extern f32 float_1_804230ec;
    extern f32 float_neg5_80423180;
    extern f32 float_18p5_80423198;

    void* player = *(void**)((s32)pParty + 0x160);
    void* hit;
    u32 attr;
    s32 adjust;
    VecLocal pos;
    f32 height;
    f32 playerY;
    f32 groundY;
    f32 baseY;
    f32 partyY;
    f32 rate;
    f32 offset;
    f32 diff;

    if ((*(u32*)pParty & 0x04000000) != 0) {
        return;
    }

    if (strcmp((const char*)((s32)gp + 0x12C), str_las_10_802f47c8) == 0) {
        pos.x = *(f32*)((s32)pParty + 0x58);
        pos.y = *(f32*)((s32)pParty + 0x5C);
        pos.z = *(f32*)((s32)pParty + 0x60);
        height = *(f32*)((s32)pParty + 0xF0);
        hit = partySearchHead(*(f32*)((s32)pParty + 0x10C), pParty, &pos, &height);
        if (hit != 0) {
            *(void**)((s32)pParty + 0x140) = hit;
            diff = (pos.y - *(f32*)((s32)pParty + 0xF0)) - *(f32*)((s32)pParty + 0x5C);
            if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
                *(f32*)((s32)pParty + 0x5C) += diff * float_0p1_80423114;
            }
        }
    }

    hit = partySearchGround(float_10_804230a4, float_0_804230bc, pParty);
    if (hit != 0) {
        attr = hitGetAttr(hit);
        if ((attr & 0x800) != 0) {
            partyChgRunMode(pParty, 0xD);
            return;
        }
        if ((attr & 0x100) != 0) {
            *(u32*)pParty |= 0x80;
        } else {
            *(u32*)pParty &= ~0x80;
        }
    }

    if ((*(s16*)((s32)player + 0x50) == 0) ||
        (*(f32*)((s32)player + 0x180) != float_0_804230bc)) {
        switch (*(u16*)((s32)player + 0x2E)) {
            case 0x1F:
            case 0x20:
            case 0x10:
            case 0x11:
                adjust = 0;
                break;
            default:
                adjust = 1;
                break;
        }
    } else {
        adjust = 0;
    }

    if (adjust != 0) {
        if (hit == 0 && *(f32*)((s32)pParty + 0xE8) < float_0_804230bc) {
            return;
        }
        playerY = *(f32*)((s32)player + 0x90);
        groundY = *(f32*)((s32)pParty + 0xE4);
        baseY = groundY;
        if (playerY > groundY) {
            baseY = playerY;
        }
        partyY = *(f32*)((s32)pParty + 0x5C);
        rate = float_0p1_80423114;
        if (partyY < groundY) {
            baseY = groundY;
            rate = float_0p2_80423194;
        }
        offset = float_5_804230e8;
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            offset = float_1_804230ec;
        }
        diff = (baseY + offset) - partyY;
        if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
            *(f32*)((s32)pParty + 0x5C) += diff * rate;
        }
    } else if (hit != 0) {
        playerY = *(f32*)((s32)player + 0x90);
        groundY = *(f32*)((s32)pParty + 0xE4);
        baseY = groundY;
        if (playerY > groundY) {
            baseY = playerY;
        }
        partyY = *(f32*)((s32)pParty + 0x5C);
        rate = float_0p05_80423174;
        if (partyY < groundY) {
            baseY = groundY + float_18p5_80423198;
            rate = float_0p2_80423194;
        }
        offset = float_5_804230e8;
        if ((*(u32*)((s32)pParty + 4) & 0x8000) != 0) {
            offset = float_1_804230ec;
        }
        diff = (baseY + offset) - partyY;
        if (diff < float_neg5_80423180 || diff >= float_0_804230bc) {
            *(f32*)((s32)pParty + 0x5C) += diff * rate;
        }
    }
}
#pragma use_lmw_stmw reset
#pragma no_register_save_helpers reset

/* fallback stub-fill: map=unk_801512e0 addr=0x801512e0 size=0x0000018c */
void unk_801512e0(void* pParty) {
    extern void* partySearchHead(void* party, void* pos, f32* height, f32 dir);
    extern void* partySearchGround(void* party, f32 yOffHi, f32 yOffLo);
    extern u32 hitGetAttr(void* hit);
    extern void partyChgMot(void* party, s32 mot);
    extern s32 unk_800cb9bc(void* hit);
    extern void partyGetAppearPos5(void* party, void* hit, void* pos);
    extern f32 float_15_80423130;
    extern f32 float_neg15_8042319c;
    extern f32 float_neg1p5_804231a0;
    f32 pos[3];
    f32 height;
    void* hit;
    u32 attr;

    pos[0] = *(f32*)((s32)pParty + 0x58);
    pos[1] = *(f32*)((s32)pParty + 0x5C);
    pos[2] = *(f32*)((s32)pParty + 0x60);
    height = *(f32*)((s32)pParty + 0xF0);
    hit = partySearchHead(pParty, pos, &height, *(f32*)((s32)pParty + 0x10C));
    if (hit != 0) {
        *(void**)((s32)pParty + 0x140) = hit;
    }
    if ((*(u32*)pParty & 0x04000000) == 0) {
        *(u32*)pParty &= ~0x100;
        hit = partySearchGround(pParty, float_15_80423130, float_neg15_8042319c);
        if (hit != 0) {
            attr = hitGetAttr(hit);
            if ((attr & 0x200) != 0) {
                hit = 0;
                if (*(s8*)((s32)pParty + 0x4C) == 0) {
                    *(u32*)((s32)pParty + 8) |= 0x01000000;
                    *(u32*)pParty |= 0x02000000;
                }
            }
        }
        *(void**)((s32)pParty + 0x138) = hit;
        if (hit == 0) {
            *(u32*)pParty &= ~4;
            partyChgMot(pParty, 3);
            *(f32*)((s32)pParty + 0x11C) = float_neg1p5_804231a0;
        } else {
            *(f32*)((s32)pParty + 0x5C) = *(f32*)((s32)pParty + 0xE4);
            *(u32*)pParty |= 2;
            if ((attr & 0x100) != 0) {
                *(u32*)pParty |= 0x80;
            } else {
                *(u32*)pParty &= ~0x100;
            }
            if ((attr & 0x40000000) != 0) {
                *(void**)((s32)pParty + 0x14C) = hit;
            }
            if (unk_800cb9bc(hit) != 0) {
                partyGetAppearPos5(pParty, hit, (void*)((s32)pParty + 0x94));
                *(u32*)((s32)pParty + 8) |= 0x00400000;
                *(f32*)((s32)pParty + 0x98) = *(f32*)((s32)pParty + 0x5C);
                partyChgMot(pParty, 2);
            }
        }
    }
}

/* fallback stub-fill: map=unk_8015146c addr=0x8015146c size=0x000001e0 */
void unk_8015146c(void* party) {
    extern void partyChgPoseId(void* party, s32 pose);
    extern void partyClearFootmark(void);
    extern void partyClearFootmark2(void);
    extern void party_slit(void* party);
    extern void party_roll(void* party);
    extern void unk_800cbc30(void* party);
    extern void partyChkWall(void* party);
    extern void partyChkGnd(void* party);
    extern f32 distABf(f32 ax, f32 az, f32 bx, f32 bz);
    extern void partyChgRunMode(void* party, s32 mode);
    extern void partyMoveBehindMario(void* party, s32 arg);
    extern void unk_80150fac(void* party);
    extern void unk_801512e0(void* party);
    extern s32 marioKeyOffChk(void);
    extern s32 marioChkDeepSleep(void);
    extern s32 chuchu_searchObject(void);
    extern void partyChgPose(void* party, char* name);
    extern char str_PCH_A2_1_802f46f4[];
    extern f32 float_31_804231a4;
    void* player;
    s32 timer;
    u8 state;

    player = *(void**)((s32)party + 0x160);
    if ((*(u32*)((s32)party + 8) & 2) != 0) {
        *(u32*)((s32)party + 8) &= ~2;
        partyChgPoseId(party, 1);
        if ((*(u32*)party & 8) == 0) {
            partyClearFootmark();
        } else {
            partyClearFootmark2();
        }
        *(s32*)((s32)party + 0x168) = -1;
    }
    party_slit(party);
    party_roll(party);
    unk_800cbc30(party);
    if ((*(u32*)party & 0x01000000) == 0) {
        partyChkWall(party);
    }
    partyChkGnd(party);
    if (distABf(*(f32*)((s32)party + 0x58), *(f32*)((s32)party + 0x60), *(f32*)((s32)player + 0x8C), *(f32*)((s32)player + 0x94)) > float_31_804231a4) {
        partyChgRunMode(party, 2);
    }
    if (*(u16*)((s32)player + 0x2E) == 0xE && (*(u32*)party & 0x80000) == 0) {
        partyMoveBehindMario(party, 0);
    } else if (*(u8*)((s32)party + 0x32) == 1) {
        unk_80150fac(party);
    } else {
        unk_801512e0(party);
        if (*(u8*)((s32)party + 0x31) == 7 && (*(u32*)((s32)party + 4) & 0x8000) == 0 && marioKeyOffChk() == 0 && marioChkDeepSleep() != 1 && chuchu_searchObject() != 0) {
            state = *(u8*)((s32)party + 0x39);
            if (state == 1) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    partyChgPoseId(party, 1);
                    *(s32*)((s32)party + 0x24) = 0x1E;
                    *(u8*)((s32)party + 0x39) = 2;
                }
            } else if (state < 3 && state != 0) {
                timer = *(s32*)((s32)party + 0x24) - 1;
                *(s32*)((s32)party + 0x24) = timer;
                if (timer <= 0) {
                    *(u8*)((s32)party + 0x39) = 0;
                }
            } else {
                partyChgPose(party, str_PCH_A2_1_802f46f4);
                *(u8*)((s32)party + 0x39) = 1;
                *(s32*)((s32)party + 0x24) = 0x1E;
            }
        }
    }
}

