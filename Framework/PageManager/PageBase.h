
#ifndef __PAGE_BASE_H__
#define __PAGE_BASE_H__

#include "LVGL/lvgl.h"

#define PAGE_ANIM_TIME_DEFAULT 500 //[ms]
#define PAGE_ANIM_PATH_DEFAULT lv_anim_path_ease_out

class PageManager;

class PageBase
{
public:
  /* Page Life cycle */
  typedef enum
  {
    PAGE_STATE_IDLE,
    PAGE_STATE_LOAD,
    PAGE_STATE_WILL_APPEAR,
    PAGE_STATE_DID_APPEAR,
    PAGE_STATE_ACTIVE,
    PAGE_STATE_WILL_DISAPPEAR,
    PAGE_STATE_DID_DISAPPEAR,
    PAGE_STATE_UNLOAD,
    _PAGE_STATE_MAX,
  } State_t;

  /* Stash data area */
  typedef struct
  {
    void    *ptr;
    uint32_t size;
  } Stash_t;

  /* Page switching animation attribute */
  typedef struct
  {
    uint8_t           type;
    uint16_t          time; /* in ms */
    lv_anim_path_cb_t path_cb;
  } AnimAttr_t;

public:
  lv_obj_t    *_root;
  PageManager *_manager;
  const char  *_name;
  uint16_t     _id;
  void        *_user_data;

  /* Private data */
  struct
  {
    bool ReqEnableCache;      // Cache enable request
    bool ReqDisableAutoCache; // Automatic cache management enable request

    bool IsDisableAutoCache; // Whether it is automatic cache management
    bool IsCached;           // Cache enable

    Stash_t Stash; // Stash area
    State_t State; // Page state

    struct
    {
      bool       isEnter; // Whether it is an entering part of the animation
      bool       isBusy;  // Whether the animation is in progress
      AnimAttr_t attr;    // Animation attributes
    } Anim;

  } priv;

public:
  virtual ~PageBase()
  {
  }

  /* Syncronize user-defined animation attribute configuration */
  virtual void onCustomAttrConfig()
  {
  }

  virtual void onViewLoad()
  {
  }
  virtual void onViewDidLoad()
  {
  }
  virtual void onViewWillAppear()
  {
  }
  virtual void onViewDidAppear()
  {
  }
  virtual void onViewWillDisappear()
  {
  }
  virtual void onViewDidDisappear()
  {
  }
  virtual void onViewUnload()
  {
  }
  virtual void onViewDidUnload()
  {
  }

  /* Set whether to manually manage the cache */
  void SetCustomCacheEnable(bool en);

  /* Set whether to automatically manage the cache */
  void SetCustomAutoCacheEnable(bool en);

  /* Set custom animation type for page switching */
  void
  SetCustomLoadAnimType(uint8_t           anim_type,
                        uint16_t          time    = PAGE_ANIM_TIME_DEFAULT,
                        lv_anim_path_cb_t path_cb = PAGE_ANIM_PATH_DEFAULT);

  /* Pop data from stash */
  bool StashPop(void *ptr, uint32_t size);
};

#endif // __PAGE_BASE_H__
