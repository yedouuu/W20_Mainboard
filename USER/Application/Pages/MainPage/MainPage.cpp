
#include "MainPage.h"

using namespace Page;

MainPage::MainPage()
    : timer(nullptr)
{
}

MainPage::~MainPage()
{
}

void MainPage::onCustomAttrConfig()
{
  SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void MainPage::onViewLoad()
{
  _view.Create(_root);

  timer = lv_timer_create(onTimerUpdate, 1000, this);
  lv_obj_add_event_cb(_view.ui.btn, onEvent, LV_EVENT_ALL, this);
}

void MainPage::onViewDidLoad()
{
}

void MainPage::onViewWillAppear()
{
}

void MainPage::onViewDidAppear()
{
}

void MainPage::onViewWillDisappear()
{
}

void MainPage::onViewDidDisappear()
{
}

void MainPage::onViewUnload()
{
  if (timer)
  {
    lv_timer_del(timer);
    timer = nullptr;
  }
}

void MainPage::onViewDidUnload()
{
}

void MainPage::onBtnClicked(lv_obj_t *btn)
{
  _manager->Push("Pages/UserSettings");
}

void MainPage::onTimerUpdate(lv_timer_t *timer)
{
  // lv_timer_get_user_data(timer);
  MainPage *page = static_cast<MainPage *>(lv_timer_get_user_data(timer));
  LV_ASSERT_NULL(page);
}

void MainPage::onEvent(lv_event_t *event)
{
  MainPage *page = static_cast<MainPage *>(lv_event_get_user_data(event));
  LV_ASSERT_NULL(page);

  lv_obj_t       *target = (lv_obj_t *)lv_event_get_current_target(event);
  lv_event_code_t code   = lv_event_get_code(event);

  if (code == LV_EVENT_CLICKED)
  {
    if (target == page->_view.ui.btn) { page->onBtnClicked(target); }
  }
}
