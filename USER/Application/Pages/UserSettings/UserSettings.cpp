
#include "UserSettings.h"

using namespace Page;

UserSettings::UserSettings()
    : timer(nullptr)
{
}

UserSettings::~UserSettings()
{
}

void UserSettings::onCustomAttrConfig()
{
  SetCustomLoadAnimType(PageManager::LOAD_ANIM_OVER_LEFT, 500, lv_anim_path_ease_in_out);
}

void UserSettings::onViewLoad()
{
  _view.Create(_root);

  timer = lv_timer_create(onTimerUpdate, 1000, this);
  lv_obj_add_event_cb(_view.ui.btn, onEvent, LV_EVENT_ALL, this);
}

void UserSettings::onViewDidLoad()
{
}

void UserSettings::onViewWillAppear()
{
}

void UserSettings::onViewDidAppear()
{
}

void UserSettings::onViewWillDisappear()
{
}

void UserSettings::onViewDidDisappear()
{
}

void UserSettings::onViewUnload()
{
  if (timer)
  {
    lv_timer_del(timer);
    timer = nullptr;
  }
}

void UserSettings::onViewDidUnload()
{
}

void UserSettings::onBtnClicked(lv_obj_t *btn)
{
  LV_LOG("UserSettings: Settings button clicked.\n");
  _manager->Pop();
}

void UserSettings::onTimerUpdate(lv_timer_t *timer)
{
  // lv_timer_get_user_data(timer);
  UserSettings *page =
      static_cast<UserSettings *>(lv_timer_get_user_data(timer));
  LV_ASSERT_NULL(page);
}

void UserSettings::onEvent(lv_event_t *event)
{
  UserSettings *page =
      static_cast<UserSettings *>(lv_event_get_user_data(event));
  LV_ASSERT_NULL(page);

  lv_obj_t       *target = (lv_obj_t *)lv_event_get_current_target(event);
  lv_event_code_t code   = lv_event_get_code(event);

  if (code == LV_EVENT_CLICKED)
  {
    if (target == page->_view.ui.btn) { page->onBtnClicked(target); }
  }
}
