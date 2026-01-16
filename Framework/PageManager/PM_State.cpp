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

void PageManager::StateUpdate(PageBase *page)
{
  switch (page->priv.State)
  {
    case PageBase::PAGE_STATE_IDLE:
      PM_LOG_I("Page( %s ) State IDLE", page->_name);
      break;

    case PageBase::PAGE_STATE_LOAD:
      page->priv.State = StateLoadExecute(page);
      StateUpdate(page);
      break;

    case PageBase::PAGE_STATE_WILL_APPEAR:
      page->priv.State = StateWillAppearExecute(page);
      break;

    case PageBase::PAGE_STATE_DID_APPEAR:
      page->priv.State = StateDidAppearExecute(page);
      PM_LOG_I("Page(%s) state active", page->_name);
      break;

    case PageBase::PAGE_STATE_ACTIVE:
      PM_LOG_I("Page(%s) State ACTIVE Break", page->_name);
      page->priv.State = PageBase::PAGE_STATE_WILL_DISAPPEAR;
      StateUpdate(page);
      break;

    case PageBase::PAGE_STATE_WILL_DISAPPEAR:
      page->priv.State = StateWillDisappearExecute(page);
      break;

    case PageBase::PAGE_STATE_DID_DISAPPEAR:
      page->priv.State = StateDidDisappearExecute(page);
      if (page->priv.State == PageBase::PAGE_STATE_UNLOAD)
      {
        StateUpdate(page);
      }
      break;

    case PageBase::PAGE_STATE_UNLOAD:
      page->priv.State = StateUnloadExecute(page);
      break;

    default:
      PM_LOG_E(
          "Page(%s) state(%d) Unknown State!", page->_name, page->priv.State);
      break;
  }
}

PageBase::State_t PageManager::StateLoadExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State LOAD executing...", page->_name);

  if (page->_root != nullptr)
  {
    PM_LOG_E("Page(%s) root must be null in LOAD state!", page->_name);
  }

  lv_obj_t *root_obj = lv_obj_create(lv_scr_act());
  if (nullptr == root_obj)
  {
    PM_LOG_E("Page(%s) root object create failed!", page->_name);
    return PageBase::PAGE_STATE_IDLE;
  }

  lv_obj_clear_flag(root_obj, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_user_data(root_obj, page);

  if (_root_style_default)
  {
    lv_obj_add_style(root_obj, _root_style_default, LV_PART_MAIN);
  }

  page->_root = root_obj;
  lv_obj_update_layout(root_obj); /* 强制布局更新, 不然会出现获取到的root尺寸为0 */
  page->onViewLoad();

  if (IsOverAnim(GetCurrentLoadAnimType()))
  {
    PageBase *bottomPage = GetStackTopAfter();

    if (bottomPage != nullptr && bottomPage->priv.IsCached)
    {
      LoadAnimAttr_t animAttr;
      if (GetCurrentLoadAnimAttr(&animAttr))
      {
        if (animAttr.drag_dir != ROOT_DRAG_DIR_NONE)
        {
          /* TODO: PM_Drag */
          // RootEnableDrag(page->_root);
        }
      }
    }
  }

  page->onViewDidLoad();

  if (page->priv.IsDisableAutoCache)
  {
    PM_LOG_I("Page(%s) Auto cache is disabled. ReqEnableCache = %d",
             page->_name,
             page->priv.ReqEnableCache);
    page->priv.IsCached = page->priv.ReqEnableCache;
  }
  else
  {
    PM_LOG_I("Page(%s) Auto cache is enabled. Set IsCached = true",
             page->_name);
    page->priv.IsCached = true;
  }

  return PageBase::PAGE_STATE_WILL_APPEAR;
}

PageBase::State_t PageManager::StateWillAppearExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State WILL_APPEAR executing...", page->_name);

  page->onViewWillAppear();

  lv_obj_remove_flag(page->_root, LV_OBJ_FLAG_HIDDEN);
  SwitchAnimCreate(page);
  return PageBase::PAGE_STATE_DID_APPEAR;
}

PageBase::State_t PageManager::StateDidAppearExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State DID_APPEAR executing...", page->_name);

  page->onViewDidAppear();

  return PageBase::PAGE_STATE_ACTIVE;
}

PageBase::State_t PageManager::StateWillDisappearExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State WILL_DISAPPEAR executing...", page->_name);

  page->onViewWillDisappear();

  SwitchAnimCreate(page);
  return PageBase::PAGE_STATE_DID_DISAPPEAR;
}

PageBase::State_t PageManager::StateDidDisappearExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State DID_DISAPPEAR executing...", page->_name);
  lv_obj_add_flag(page->_root, LV_OBJ_FLAG_HIDDEN);

  page->onViewDidDisappear();

  if (page->priv.IsCached)
  {
    PM_LOG_I("Page(%s) is cached. Stay in WILL_APPEAR state.", page->_name);
    return PageBase::PAGE_STATE_WILL_APPEAR;
  }

  return PageBase::PAGE_STATE_UNLOAD;
}

PageBase::State_t PageManager::StateUnloadExecute(PageBase *page)
{
  PM_LOG_I("Page(%s) State UNLOAD executing...", page->_name);

  if (nullptr == page->_root)
  {
    PM_LOG_E("Page(%s) root object is null in UNLOAD state!", page->_name);
    return PageBase::PAGE_STATE_IDLE;
  }

  page->onViewUnload();

  if (page->priv.Stash.ptr != nullptr && page->priv.Stash.size > 0)
  {
    PM_LOG_I("Page(%s) Stash data area release.", page->_name);
    lv_free(page->priv.Stash.ptr);
    page->priv.Stash.ptr  = nullptr;
    page->priv.Stash.size = 0;
  }

  lv_obj_del_async(page->_root);
  page->_root         = nullptr;
  page->priv.IsCached = false;

  page->onViewDidUnload();

  return PageBase::PAGE_STATE_IDLE;
}
