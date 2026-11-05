










#include "musyx/musyx.h"

#include "musyx/assert.h"
#include "musyx/sal.h"

void sndAuxCallbackDelay(u8 reason, SND_AUX_INFO* info, void* user) {
  s32 l;
  s32 r;
  s32 s;
  s32* lBuf;
  s32* rBuf;
  s32* sBuf;
  u32 i;
  SND_AUX_DELAY* c;
  s32* left;
  s32* right;
  s32* sur;

  switch (reason) {
  case SND_AUX_REASON_BUFFERUPDATE:
    c = (SND_AUX_DELAY*)user;
    left = info->data.bufferUpdate.left;
    right = info->data.bufferUpdate.right;
    sur = info->data.bufferUpdate.surround;
    lBuf = c->left + (c->currentPos[0] * SND_AUX_BLOCKSIZE);
    rBuf = c->right + (c->currentPos[1] * SND_AUX_BLOCKSIZE);
    sBuf = c->sur + (c->currentPos[2] * SND_AUX_BLOCKSIZE);
    for (i = 0; i < SND_AUX_BLOCKSIZE; ++i) {

      l = *lBuf;
      r = *rBuf;
      s = *sBuf;

      *lBuf++ = *left + ((s32)(l * c->currentFeedback[0]) >> 7);
      *rBuf++ = *right + ((s32)(r * c->currentFeedback[1]) >> 7);
      *sBuf++ = *sur + ((s32)(s * c->currentFeedback[2]) >> 7);

      *left++ = (s32)(l * c->currentOutput[0]) >> 7;
      *right++ = (s32)(r * c->currentOutput[1]) >> 7;
      *sur++ = (s32)(s * c->currentOutput[2]) >> 7;
    }

    c->currentPos[0] = (c->currentPos[0] + 1) % c->currentSize[0];
    c->currentPos[1] = (c->currentPos[1] + 1) % c->currentSize[1];
    c->currentPos[2] = (c->currentPos[2] + 1) % c->currentSize[2];
    break;
  case SND_AUX_REASON_PARAMETERUPDATE:
    break;
  default:
    MUSY_ASSERT(FALSE);
    break;
  }
}

bool sndAuxCallbackUpdateSettingsDelay(SND_AUX_DELAY* delay) {
  u32 i;
  s32* left;
  s32* right;
  s32* sur;
  sndAuxCallbackShutdownDelay(delay);

  for (i = 0; i < 3; ++i) {
    delay->currentSize[i] = ((delay->delay[i] - 5) * 32 + (SND_AUX_BLOCKSIZE - 1)) / SND_AUX_BLOCKSIZE;
    delay->currentPos[i] = 0;
    delay->currentFeedback[i] = (delay->feedback[i] << 7) / 100;
    delay->currentOutput[i] = (delay->output[i] << 7) / 100;
  }

  delay->left = (s32*)salMalloc(delay->currentSize[0] * SND_AUX_BLOCKSIZE * sizeof(s32));
  delay->right = (s32*)salMalloc(delay->currentSize[1] * SND_AUX_BLOCKSIZE * sizeof(s32));
  delay->sur = (s32*)salMalloc(delay->currentSize[2] * SND_AUX_BLOCKSIZE * sizeof(s32));

  /* clang-format off */
  MUSY_ASSERT(delay->left!=NULL);
  MUSY_ASSERT(delay->right!=NULL);
  MUSY_ASSERT(delay->sur!=NULL);
  /* clang-format on */
  left = delay->left;
  right = delay->right;
  sur = delay->sur;

  for (i = 0; i < delay->currentSize[0] * SND_AUX_BLOCKSIZE; ++i) {
    *left++ = 0;
  }

  for (i = 0; i < delay->currentSize[1] * SND_AUX_BLOCKSIZE; ++i) {
    *right++ = 0;
  }
  for (i = 0; i < delay->currentSize[2] * SND_AUX_BLOCKSIZE; ++i) {
    *sur++ = 0;
  }
  return TRUE;
}

bool sndAuxCallbackPrepareDelay(SND_AUX_DELAY* delay) {
  delay->left = NULL;
  return sndAuxCallbackUpdateSettingsDelay(delay);
}

// The pointers keep their values after salFree; sndAuxCallbackUpdateSettingsDelay allocates new ones
// right after its call here.
bool sndAuxCallbackShutdownDelay(SND_AUX_DELAY* delay) {
  if (delay->left != NULL) {
    salFree(delay->left);
    salFree(delay->right);
    salFree(delay->sur);
  }
  return TRUE;
}
