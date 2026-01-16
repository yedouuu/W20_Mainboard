/*
 * MIT License
 * Copyright (c) 2026 Yedouuu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef __PAGE_MANAGER_H__
#define __PAGE_MANAGER_H__

#include "PageBase.h"
#include "PageFactory.h"
#include <vector>
#include <stack>

class PageManager
{
public:
  typedef enum
  {
    LOAD_ANIM_GLOBAL = 0,

    /* New page overwrites old page */
    LOAD_ANIM_OVER_LEFT = 1,
    LOAD_ANIM_OVER_RIGHT,
    LOAD_ANIM_OVER_TOP,
    LOAD_ANIM_OVER_BOTTOM,

    /* Old page leaves for new page */
    LOAD_ANIM_MOVE_LEFT,
    LOAD_ANIM_MOVE_RIGHT,
    LOAD_ANIM_MOVE_TOP,
    LOAD_ANIM_MOVE_BOTTOM,

    /* New Page fade in */
    LOAD_ANIM_FADE_IN,

    /* No animation */
    LOAD_ANIM_NONE,

    _LOAD_ANIM_LAST = LOAD_ANIM_NONE
  } LoadAnim_t;

  /* Page dragging direction */
  typedef enum
  {
    ROOT_DRAG_DIR_NONE,
    ROOT_DRAG_DIR_HOR,
    ROOT_DRAG_DIR_VER
  } RootDragDir_t;

  typedef void (*lv_anim_setter_t)(void *, int32_t);
  typedef int32_t (*lv_anim_getter_t)(void *);

  /* Animation switching record  */
  typedef struct
  {
    struct
    {
      int32_t start;
      int32_t end;
    } enter;

    struct
    {
      int32_t start;
      int32_t end;
    } exit;
  } AnimValues_t;

  typedef struct
  {
    lv_anim_setter_t setter;
    lv_anim_getter_t getter;
    RootDragDir_t    drag_dir;
    AnimValues_t     push;
    AnimValues_t     pop;
  } LoadAnimAttr_t;

public:
  PageManager(PageFactory *factory = nullptr);
  ~PageManager();

  /* Loader */
  bool Install(const char *className, const char *pathName);
  bool Uninstall(const char *pathName);
  bool Register(PageBase *base, const char *pathName);
  bool Unregister(const char *pathName);

  /* Router */
  bool Replace(const char *pathName, const PageBase::Stash_t *stash = nullptr);
  bool Push(const char *pathName, const PageBase::Stash_t *stash = nullptr);
  bool Pop();
  bool BackHome();
  const char *GetPagePrevPath();

  /* Global Animation */
  void SetGlobalLoadAnimType(LoadAnim_t        anim = LOAD_ANIM_OVER_LEFT,
                             uint16_t          time = 500,
                             lv_anim_path_cb_t path = lv_anim_path_ease_out);

  void SetRootDefaultStyle(lv_style_t *style) { _root_style_default = style; }

private:
  /* Page Pool */
  PageBase *FindPageInPool(const char *pathName);

  /* Page Stack */
  PageBase *FindPageInStack(const char *pathName);
  PageBase *GetStackTop();
  PageBase *GetStackTopAfter();
  void      PageStackClear(bool keepBottom = false);
  bool      FourceUnload(PageBase *page);

  /* Animation */
  bool GetLoadAnimAttr(uint8_t anim, LoadAnimAttr_t *attr);
  bool IsOverAnim(uint8_t anim)
  {
    return (anim >= LOAD_ANIM_OVER_LEFT && anim <= LOAD_ANIM_OVER_BOTTOM);
  };
  bool IsMoveAnim(uint8_t anim)
  {
    return (anim >= LOAD_ANIM_MOVE_LEFT && anim <= LOAD_ANIM_MOVE_BOTTOM);
  };
  void       AnimDefaultInit(lv_anim_t *a);
  LoadAnim_t GetCurrentLoadAnimType()
  {
    return (LoadAnim_t)_AnimState.current.type;
  }
  bool GetCurrentLoadAnimAttr(LoadAnimAttr_t *attr)
  {
    return GetLoadAnimAttr(GetCurrentLoadAnimType(), attr);
  }

  /* Root */
  // static void onRootDragEvent(lv_event_t *event);
  // static void onRootDragAnimFinish(lv_anim_t *a);
  // static void onRootAsyncLeave(void *base);
  // void        RootEnableDrag(lv_obj_t *root);
  // static void RootGetDragPredict(lv_coord_t *x, lv_coord_t *y);

  /* Switch */
  bool        SwitchTo(PageBase                *newPage,
                       bool                     isEnterAct,
                       const PageBase::Stash_t *stash = nullptr);
  static void onSwitchAnimFinish(lv_anim_t *a);
  void        SwitchAnimCreate(PageBase *base);
  void        SwitchAnimTypeUpdate(PageBase *base);
  bool        SwitchReqCheck();
  bool        SwitchAnimStateCheck();

  /* Life circle state */
  PageBase::State_t StateLoadExecute(PageBase *base);
  PageBase::State_t StateWillAppearExecute(PageBase *base);
  PageBase::State_t StateDidAppearExecute(PageBase *base);
  PageBase::State_t StateWillDisappearExecute(PageBase *base);
  PageBase::State_t StateDidDisappearExecute(PageBase *base);
  PageBase::State_t StateUnloadExecute(PageBase *base);
  void              StateUpdate(PageBase *base);
  PageBase::State_t GetState() { return _PageCurrent->priv.State; }

private:
  PageFactory            *_Factory;
  std::vector<PageBase *> _PagePool;
  std::stack<PageBase *>  _PageStack;

  PageBase *_PagePrev;
  PageBase *_PageCurrent;

  struct
  {
    bool isSwitchReq; // Has a page switch been requested
    bool isBusy;      // Is a page switch in progress
    bool isEntering;  // Is in entering action

    PageBase::AnimAttr_t current;
    PageBase::AnimAttr_t global;
  } _AnimState;

  lv_style_t *_root_style_default;
};

#endif // __PAGE_MANAGER_H__
