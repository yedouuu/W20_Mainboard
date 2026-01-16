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
#include "PM_Log.h"
#include "PageManager.h"

/**
 * @brief Replace the current page with a new page
 *
 * @param pathName The path name of the new page
 * @param stash Optional stash data for the new page
 * @return true
 * @return false
 */
bool PageManager::Replace(const char *pathName, const PageBase::Stash_t *stash)
{
  PM_LOG_I("Replacing current page with Page(%s)...", pathName);

  /* Check whether animation of switching page is being performed */
  if (!SwitchAnimStateCheck()) { return false; }

  /* Check if the page is already in the stack */
  if (FindPageInStack(pathName) != nullptr)
  {
    PM_LOG_E("Page(%s) is already in stack, cannot replace.", pathName);
    return false;
  }

  /* Check if the page is registered in the page pool */
  PageBase *newPage = FindPageInPool(pathName);
  if (nullptr == newPage)
  {
    PM_LOG_E("Page(%s) not installed to page pool.", pathName);
    return false;
  }

  /* Replace the current page with the new page */
  PageBase *curPage = GetStackTop();
  if (nullptr == curPage)
  {
    PM_LOG_E("Current page is null, cannot replace.");
    return false;
  }

  /* Force disable cache */
  curPage->priv.IsCached = false;
  /* Synchronize auto cache configuration */
  newPage->priv.IsDisableAutoCache = newPage->priv.ReqDisableAutoCache;

  _PageStack.pop();
  _PageStack.push(newPage);

  PM_LOG_I("Page(%s) replacing Page(%s) (stash = 0x%p)...",
           newPage->_name,
           curPage->_name,
           stash);

  return SwitchTo(newPage, true, stash);
}

/**
 * @brief Push a new page onto the page stack, old page remains in the stack
 *
 * @param pathName
 * @param stash
 * @return true
 * @return false
 */
bool PageManager::Push(const char              *pathName,
                       const PageBase::Stash_t *stash)
{
  PM_LOG_I("Pushing Page(%s) onto stack...", pathName);

  /* Check whether animation of switching page is being performed */
  if (!SwitchAnimStateCheck()) { return false; }

  /* Check if the page is already in the stack */
  if (FindPageInStack(pathName) != nullptr)
  {
    PM_LOG_E("Page(%s) was multi push, already in stack.", pathName);
    return false;
  }

  /* Check if the page is registered in the page pool */
  PageBase *newPage = FindPageInPool(pathName);
  if (nullptr == newPage)
  {
    PM_LOG_E("Page(%s) was not installed to page pool.", pathName);
    return false;
  }

  /* Synchronize auto cache configuration */
  newPage->priv.IsDisableAutoCache = newPage->priv.ReqDisableAutoCache;

  _PageStack.push(newPage);

  PM_LOG_I("Page(%s) pushing onto >> [Screen] (stash = 0x%p)",
           newPage->_name,
           stash);

  return SwitchTo(newPage, true, stash);
}

bool PageManager::Pop()
{
  PM_LOG_I("Popping top page from stack...");

  /* Check whether animation of switching page is being performed */
  if (!SwitchAnimStateCheck()) { return false; }

  PageBase *curPage = GetStackTop();
  if (nullptr == curPage)
  {
    PM_LOG_E("Page stack is empty, cannot pop.");
    return false;
  }

  if (false == curPage->priv.IsDisableAutoCache)
  {
    PM_LOG_I("Page(%s) Auto cache is enabled, disable cache for it.",
             curPage->_name);
    curPage->priv.IsCached = false;
  }

  _PageStack.pop();
  PM_LOG_I("Page(%s) popped from << [Screen].", curPage->_name);

  PageBase *newPage = GetStackTop();
  if (nullptr == newPage)
  {
    PM_LOG_E("No page left in stack after pop, cannot switch.");
    return false;
  }

  return SwitchTo(newPage, false, nullptr);
}

bool PageManager::SwitchTo(PageBase                *newPage,
                           bool                     isEnterAct,
                           const PageBase::Stash_t *stash)
{
  PM_LOG_I("Switching to Page(%s)...", newPage->_name);

  if (nullptr == newPage)
  {
    PM_LOG_E("New page is null, cannot switch.");
    return false;
  }

  if (_AnimState.isSwitchReq)
  {
    PM_LOG_W(
        "Another switch request is in progress, cannot switch to Page(%s).",
        newPage->_name);
    return false;
  }

  _AnimState.isSwitchReq = true;

  if (stash != nullptr)
  {
    PM_LOG_I("Page(%s) Stash data provided: ptr=0x%p, size=%u",
             newPage->_name,
             stash->ptr,
             stash->size);
    void *buffer = nullptr;

    if (newPage->priv.Stash.ptr == nullptr)
    {
      buffer = lv_malloc(stash->size);
      if (buffer == nullptr)
      {
        PM_LOG_E("Page(%s) Stash data allocation failed.", newPage->_name);
      }
      else
      {
        PM_LOG_I("Page(%s) Stash data area allocated: ptr=0x%p, size=%u",
                 newPage->_name,
                 buffer,
                 stash->size);
      }
    }
    else if (newPage->priv.Stash.size == stash->size)
    {
      buffer = newPage->priv.Stash.ptr;
      PM_LOG_I("Page(%s) Reusing existing Stash data area: ptr=0x%p, size=%u",
               newPage->_name,
               buffer,
               stash->size);
    }

    if (buffer != nullptr)
    {
      memcpy(buffer, stash->ptr, stash->size);
      newPage->priv.Stash.ptr  = buffer;
      newPage->priv.Stash.size = stash->size;
    }
  }

  // _PagePrev    = _PageCurrent;
  _PageCurrent = newPage;

  /* If page has cached, directly display no need to load */
  if (_PageCurrent->priv.IsCached)
  {
    PM_LOG_I("Page(%s) is cached, skip LOAD state.", newPage->_name);
    newPage->priv.State = PageBase::PAGE_STATE_WILL_APPEAR;
  }
  else
  {
    newPage->priv.State = PageBase::PAGE_STATE_LOAD;
  }

  if (_PagePrev != nullptr) { _PagePrev->priv.Anim.isEnter = false; }

  _PageCurrent->priv.Anim.isEnter = true;
  _AnimState.isEntering           = isEnterAct;

  if (_AnimState.isEntering) { SwitchAnimTypeUpdate(_PageCurrent); }

  if (_PagePrev) StateUpdate(_PagePrev);
  StateUpdate(_PageCurrent);

  if (_AnimState.isEntering)
  {
    PM_LOG_I("Switch to Page(%s) with ENTER animation.", newPage->_name);
    if (_PagePrev) { lv_obj_move_foreground(_PagePrev->_root); }
    lv_obj_move_foreground(_PageCurrent->_root);
  }
  else
  {
    PM_LOG_I("Switch to Page(%s) with EXIT animation.", newPage->_name);
    lv_obj_move_foreground(_PageCurrent->_root);
    if (_PagePrev) { lv_obj_move_foreground(_PagePrev->_root); }
  }

  return true;
}

/**
 * @brief Fource the end of the page life cycle, and unload the page with out
 * animation
 *
 * @param page The page to be unloaded
 * @return true
 * @return false
 */
bool PageManager::FourceUnload(PageBase *page)
{
  if (nullptr == page) { return false; }

  PM_LOG_I("Page(%s) Fource unloading...", page->_name);

  if (page->priv.State == PageBase::PAGE_STATE_ACTIVE)
  {
    PM_LOG_I("Page state is ACTIVITY, Disappearing...", page->_name);
    page->onViewWillDisappear();
    page->onViewDidDisappear();
  }

  page->priv.State = StateUnloadExecute(page);

  return true;
}

/**
 * @brief Navigate back to the home page (bottom page in the stack)
 *
 * @return true
 * @return false
 */
bool PageManager::BackHome()
{
  if (!SwitchAnimStateCheck()) { return false; }

  PageStackClear(true);

  _PagePrev = nullptr;

  PageBase *homePage = GetStackTop();

  SwitchTo(homePage, false);

  return true;
}

/**
 * @brief Check if the page switch animation is being executed
 *
 * @return true
 * @return false
 */
bool PageManager::SwitchAnimStateCheck()
{
  if (_AnimState.isSwitchReq || _AnimState.isBusy)
  {
    PM_LOG_W("Page switch busy[AnimState.isSwitchReq = %d,"
             "AnimState.isBusy = %d],"
             "request ignored",
             _AnimState.isSwitchReq,
             _AnimState.isBusy);
    return false;
  }

  return true;
}

/**
 * @brief Check if the switch request animation is being executed for all pages
 *
 * @return true
 * @return false
 */
bool PageManager::SwitchReqCheck()
{
  bool ret          = false;
  bool lastPageBusy = _PagePrev && _PagePrev->priv.Anim.isBusy;

  if (!_PageCurrent->priv.Anim.isBusy && !lastPageBusy)
  {
    PM_LOG_I("Page switch animation finished.");
    _AnimState.isSwitchReq = false;
    ret                    = true;
    _PagePrev              = _PageCurrent;
  }
  else
  {
    PM_LOG_I("Page switch animation in progress..."
             "Current page busy=%d, Last page busy=%d",
             _PageCurrent->priv.Anim.isBusy,
             lastPageBusy);
  }

  return ret;
}

/**
 * @brief Callback when the page switch animation is finished
 *
 * @param a The animation object
 */
void PageManager::onSwitchAnimFinish(lv_anim_t *a)
{
  PageBase *page = (PageBase *)lv_anim_get_user_data(a);

  PM_LOG_I("Page(%s) switch animation finished.", page->_name);
  page->priv.Anim.isBusy = false;

  PageManager *manager = page->_manager;
  manager->StateUpdate(page);
  bool isFinished = manager->SwitchReqCheck();

  if (!manager->_AnimState.isEntering && isFinished)
  {
    manager->SwitchAnimTypeUpdate(manager->_PageCurrent);
  }
}

/**
 * @brief Create the page switch animation
 *
 * @param page The page to create the animation for
 */
void PageManager::SwitchAnimCreate(PageBase *page)
{
  LoadAnimAttr_t attr;
  if (!GetCurrentLoadAnimAttr(&attr))
  {
    PM_LOG_E("Failed to get current load animation attributes.");
    return;
  }

  lv_anim_t a;
  AnimDefaultInit(&a);
  lv_anim_set_user_data(&a, page);
  lv_anim_set_var(&a, page->_root);
  lv_anim_set_ready_cb(&a, onSwitchAnimFinish);
  lv_anim_set_exec_cb(&a, attr.setter);

  int32_t start = 0;

  if (attr.getter) { start = attr.getter(page->_root); }

  if (_AnimState.isEntering)
  {
    if (page->priv.Anim.isEnter)
    {
      lv_anim_set_values(&a, attr.push.enter.start, attr.push.enter.end);
    }
    else
    {
      lv_anim_set_values(&a, start, attr.push.exit.end);
    }
  }
  else
  {
    if (page->priv.Anim.isEnter)
    {
      lv_anim_set_values(&a, attr.pop.enter.start, attr.pop.enter.end);
    }
    else
    {
      lv_anim_set_values(&a, start, attr.pop.exit.end);
    }
  }

  page->priv.Anim.isBusy = true;

  PM_LOG_I("Page(%s) starting switch animation...", page->_name);
  lv_anim_start(&a);
}

/**
 * @brief Set the global page load animation type
 *
 * @param anim The animation type
 * @param time The duration of the animation
 * @param path The animation path callback(Animation curve)
 */
void PageManager::SetGlobalLoadAnimType(LoadAnim_t        anim,
                                        uint16_t          time,
                                        lv_anim_path_cb_t path)
{

  _AnimState.global.type    = anim;
  _AnimState.global.time    = time;
  _AnimState.global.path_cb = path;

  PM_LOG_I("Global load animation set: type=%d, time=%u, path_cb=0x%p",
           anim,
           time,
           path);
}

/**
 * @brief Update the animation attributes, base on page's anim type
 *
 * @param page The page to update animation attributes
 */
void PageManager::SwitchAnimTypeUpdate(PageBase *page)
{
  if (page->priv.Anim.attr.type == LOAD_ANIM_GLOBAL)
  {
    PM_LOG_I("Page(%s) anim type unset, using global animation type.",
             page->_name);
    _AnimState.current = _AnimState.global;
  }
  else
  {
    if (page->priv.Anim.attr.type > _LOAD_ANIM_LAST)
    {
      PM_LOG_W("Page(%s) anim type invalid (%d), using global animation type.",
               page->_name,
               page->priv.Anim.attr.type);
      _AnimState.current = _AnimState.global;
    }
    else
    {
      PM_LOG_I("Page(%s) using custom animation type (%d).",
               page->_name,
               page->priv.Anim.attr.type);
      _AnimState.current = page->priv.Anim.attr;
    }
  }
}

/**
 * @brief Initialize the default animation parameters
 *
 * @param a The animation object to initialize
 */
void PageManager::AnimDefaultInit(lv_anim_t *a)
{
  lv_anim_init(a);

  uint32_t time = (GetCurrentLoadAnimType() == LOAD_ANIM_NONE)
                      ? 0
                      : _AnimState.current.time;
  lv_anim_set_time(a, time);
  lv_anim_set_path_cb(a, _AnimState.current.path_cb);
}
