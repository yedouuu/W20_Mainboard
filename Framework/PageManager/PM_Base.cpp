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
#include "PageManager.h"
#include "PM_Log.h"
#include <algorithm>

#define PM_EMPTY_PAGE_NAME "EMPTY_PAGE"

/**
 * @brief Construct a new Page Manager:: Page Manager object
 *
 * @param factory Page factory for creating pages
 */
PageManager::PageManager(PageFactory *factory)
    : _Factory(factory), _PagePrev(nullptr), _PageCurrent(nullptr),
      _root_style_default(nullptr)
{
  // Initialize animation state
  memset(&_AnimState, 0, sizeof(_AnimState));

  SetGlobalLoadAnimType();
}

/**
 * @brief Destroy the Page Manager:: Page Manager object
 *
 */
PageManager::~PageManager() { PageStackClear(); }

/**
 * @brief Find a page in the page pool by its path name
 *
 * @param pathName The path name of the page to find
 * @return PageBase* Pointer to the found page, or nullptr if not found
 */
PageBase *PageManager::FindPageInPool(const char *pathName)
{
  for (auto page : _PagePool) {
    if (strcmp(page->_name, pathName) == 0) { return page; }
  }
  return nullptr;
}

/**
 * @brief Find a page in the page stack by its path name
 *
 * @param pathName  The path name of the page to find
 * @return PageBase* Pointer to the found page, or nullptr if not found
 */
PageBase *PageManager::FindPageInStack(const char *pathName)
{
  decltype(_PageStack) tempStack = _PageStack;
  while (!tempStack.empty()) {
    PageBase *page = tempStack.top();
    tempStack.pop();
    if (strcmp(page->_name, pathName) == 0) { return page; }
  }
  return nullptr;
}

/**
 * @brief Install a page into the page pool
 *
 * @param className The class name of the page to install
 * @param pathName The path name to associate with the page
 * @return true
 * @return false
 */
bool PageManager::Install(const char *className, const char *pathName)
{
  if (nullptr == _Factory) {
    PM_LOG_E("PageFactory is not set.");
    return false;
  }

  if (nullptr == pathName) {
    PM_LOG_W("Path name is null. use class name as path name.");
    pathName = className;
  }

  if (nullptr != FindPageInPool(pathName)) {
    PM_LOG_E("Page '%s' is already installed.", pathName);
    return false;
  }

  PageBase *page = _Factory->CreatePage(className);
  if (nullptr == page) {
    PM_LOG_E("Failed to create page '%s'.", className);
    return false;
  }

  page->_root      = nullptr;
  page->_id        = 0;
  page->_manager   = nullptr;
  page->_user_data = nullptr;
  memset(&page->priv, 0, sizeof(page->priv));

  PM_LOG_I("Install Page[class = %s] '%s'.", className, pathName);
  bool retval = PageManager::Register(page, pathName);

  page->onCustomAttrConfig();

  return retval;
}

/**
 * @brief Uninstall a page from the page pool by its path name
 *
 * @param pathName The path name of the page to uninstall
 * @return true
 * @return false
 */
bool PageManager::Uninstall(const char *pathName)
{
  PM_LOG_I("Page(%s) Uninstalling...", pathName);

  PageBase *page = FindPageInPool(pathName);
  if (nullptr == page) {
    PM_LOG_W("Page '%s' not found.", pathName);
    return false;
  }

  if (!Unregister(pathName)) {
    PM_LOG_E("Failed to unregister page '%s'.", pathName);
    return false;
  }

  if (page->priv.IsCached) {
    PM_LOG_W("Page '%s' is cached, unloading...", pathName);
    page->priv.State = PageBase::PAGE_STATE_UNLOAD;
    StateUpdate(page);
  } else {
    PM_LOG_I("Page(%s) is not cached", pathName);
  }

  delete page;
  PM_LOG_I("Uninstalled Page(%s) success.", pathName);
  return true;
}

/**
 * @brief Register a page into the page pool with a specified path name
 *
 * @param page The page to register
 * @param pathName The path name to associate with the page
 * @return true
 * @return false
 */
bool PageManager::Register(PageBase *page, const char *pathName)
{
  if (nullptr != FindPageInPool(pathName)) {
    PM_LOG_E("Page(%s) was multi registered.", pathName);
    return false;
  }

  page->_name    = pathName;
  page->_manager = this;

  _PagePool.push_back(page);

  return true;
}

/**
 * @brief Unregister a page from the page pool by its path name
 *
 * @param pathName The path name of the page to unregister
 * @return true
 * @return false
 */
bool PageManager::Unregister(const char *pathName)
{
  PM_LOG_I("Unregistering Page(%s)...", pathName);

  PageBase *page = FindPageInStack(pathName);
  if (nullptr != page) {
    PM_LOG_E("Page(%s) is in stack, cannot unregister.", pathName);
    return false;
  }

  page = FindPageInPool(pathName);
  if (nullptr == page) {
    PM_LOG_E("Page(%s) not found in pool.", pathName);
    return false;
  }

  auto iter = std::find(_PagePool.begin(), _PagePool.end(), page);

  if (iter == _PagePool.end()) {
    PM_LOG_E("Page(%s) not found in pool.", pathName);
    return false;
  }

  _PagePool.erase(iter);

  PM_LOG_I("Unregistered Page(%s) successfully.", pathName);
  return true;
}

/**
 * @brief Get the top page from the page stack
 *
 * @return PageBase* Pointer to the top page, or nullptr if the stack is empty
 */
PageBase *PageManager::GetStackTop()
{
  if (_PageStack.empty()) { return nullptr; }
  return _PageStack.top();
}

/**
 * @brief Get the page below the top page from the page stack
 *
 * @return PageBase* Pointer to the page below the top page, or nullptr if there
 * is none
 */
PageBase *PageManager::GetStackTopAfter()
{
  PageBase *top = GetStackTop();
  if (nullptr == top) { return nullptr; }

  _PageStack.pop();
  PageBase *afterTop = GetStackTop();
  _PageStack.push(top);
  return afterTop;
}

/**
 * @brief Clear the page stack, and end the life cycle of each page in the stack
 *
 * @param keepBottom Whether to keep the bottom page in the stack
 */
void PageManager::PageStackClear(bool keepBottom)
{
  while (!_PageStack.empty()) {
    PageBase *top = GetStackTop();
    if (nullptr == top) {
      PM_LOG_I("Page stack is empty.");
      break;
    }

    PageBase *afterTop = GetStackTopAfter();
    if (nullptr == afterTop) {
      if (keepBottom) {
        _PagePrev = top;
        PM_LOG_I("Keep bottom page(%s) in stack.", top->_name);
        break;
      } else {
        _PagePrev = nullptr;
      }
    }

    FourceUnload(top);

    _PageStack.pop();
  }
  PM_LOG_I("Page stack clear done.");
}

/**
 * @brief Get the previous page's path name
 *
 * @return const char* Previous page's path name, or "EMPTY_PAGE" if there is no
 * previous page
 */
const char *PageManager::GetPagePrevPath()
{
  if (nullptr == _PagePrev) { return PM_EMPTY_PAGE_NAME; }
  return _PagePrev->_name;
}
