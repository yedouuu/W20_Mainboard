
#include "PageBase.h"
#include "PM_Log.h"
#include <string.h>

void PageBase::SetCustomCacheEnable(bool en)
{
  PM_LOG_I("Page(%s) %s = %d", _name, __FUNCTION__, en);
  priv.ReqEnableCache = en;
}

void PageBase::SetCustomAutoCacheEnable(bool en)
{
  PM_LOG_I("Page(%s) %s = %d", _name, __FUNCTION__, en);
  priv.ReqDisableAutoCache = !en;
}

void PageBase::SetCustomLoadAnimType(uint8_t           anim_type,
                                     uint16_t          time,
                                     lv_anim_path_cb_t path_cb)
{
  PM_LOG_I(
      "Page(%s) %s: type=%d, time=%d", _name, __FUNCTION__, anim_type, time);
  priv.Anim.attr.type    = anim_type;
  priv.Anim.attr.time    = time;
  priv.Anim.attr.path_cb = path_cb;
}

bool PageBase::StashPop(void *ptr, uint32_t size)
{
  if (nullptr == priv.Stash.ptr)
  {
    PM_LOG_W("Page(%s) %s: Stash is empty or size is insufficient",
             _name,
             __FUNCTION__);
    return false;
  }

  if (size != priv.Stash.size)
  {
    PM_LOG_W(
        "Page(%s) %s: Requested size (%d) does not match stashed size (%d)",
        _name,
        __FUNCTION__,
        size,
        priv.Stash.size);
    return false;
  }

  memcpy(ptr, priv.Stash.ptr, size);
  PM_LOG_I(
      "Page(%s) %s: Popped %d bytes from stash", _name, __FUNCTION__, size);

  // Clear the stash after popping
  lv_free(priv.Stash.ptr);
  priv.Stash.ptr  = nullptr;
  priv.Stash.size = 0;

  return true;
}
