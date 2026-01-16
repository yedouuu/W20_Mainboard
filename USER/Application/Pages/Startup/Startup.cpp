
#include "Startup.h"

using namespace Page;

Startup::Startup()
    : timer(nullptr)
{
}

Startup::~Startup()
{
}

void Startup::onCustomAttrConfig()
{
  SetCustomCacheEnable(false);
  SetCustomAutoCacheEnable(false);
  SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void Startup::onViewLoad()
{
  _view.Create(_root);

  timer = lv_timer_create(onTimer, 2000, this);
  // lv_timer_set_repeat_count(timer, 1);
}

void Startup::onViewDidLoad()
{
}

void Startup::onViewWillAppear()
{
  lv_anim_timeline_start(_view.ui.anim_timeline);
}

void Startup::onViewDidAppear()
{
  lv_obj_fade_out(_root, 500, 1500);
}

void Startup::onViewWillDisappear()
{
}

void Startup::onViewDidDisappear()
{
}

void Startup::onViewUnload()
{
  if (timer)
  {
    lv_timer_del(timer);
    timer = nullptr;
  }
  _view.Destroy();
  
}

void Startup::onViewDidUnload()
{
}

void Startup::onTimer(lv_timer_t *timer)
{
  Startup *page = static_cast<Startup *>(lv_timer_get_user_data(timer));
  LV_ASSERT_NULL(page);

  page->_manager->Replace("Pages/MainPage");
}

void Startup::onBtnClicked(lv_obj_t *btn)
{
  LV_LOG("Button Clicked!");
}

void Startup::onTimerUpdate(lv_timer_t *timer)
{
  // lv_timer_get_user_data(timer);
  Startup *page = static_cast<Startup *>(lv_timer_get_user_data(timer));
  LV_ASSERT_NULL(page);
}

void Startup::onEvent(lv_event_t *event)
{
  Startup *page = static_cast<Startup *>(lv_event_get_user_data(event));
  LV_ASSERT_NULL(page);

  lv_obj_t       *target = (lv_obj_t *)lv_event_get_current_target(event);
  lv_event_code_t code   = lv_event_get_code(event);

  // if (code == LV_EVENT_CLICKED)
  // {
  //   if (target == page->_view.ui.btn) { page->onBtnClicked(target); }
  // }
}
