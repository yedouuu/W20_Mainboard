
#include "__Template.h"

using namespace Page;

__Template::__Template()
    : timer(nullptr)
{
}

__Template::~__Template()
{
}

void __Template::onCustomAttrConfig()
{
  // SetCustomLoadAnimType(PageManager::LOAD_ANIM_NONE);
}

void __Template::onViewLoad()
{
  _view.Create(_root);

  timer = lv_timer_create(onTimerUpdate, 1000, this);
  lv_obj_add_event_cb(_view.ui.btn, onEvent, LV_EVENT_ALL, this);
}

void __Template::onViewDidLoad()
{
}

void __Template::onViewWillAppear()
{
}

void __Template::onViewDidAppear()
{
}

void __Template::onViewWillDisappear()
{
}

void __Template::onViewDidDisappear()
{
}

void __Template::onViewUnload()
{
  if (timer)
  {
    lv_timer_del(timer);
    timer = nullptr;
  }
}

void __Template::onViewDidUnload()
{
}

void __Template::onBtnClicked(lv_obj_t *btn)
{
  LV_LOG("Button Clicked!");
}

void __Template::onTimerUpdate(lv_timer_t *timer)
{
  // lv_timer_get_user_data(timer);
  __Template *page = static_cast<__Template *>(lv_timer_get_user_data(timer));
  LV_ASSERT_NULL(page);
}

void __Template::onEvent(lv_event_t *event)
{
  __Template *page = static_cast<__Template *>(lv_event_get_user_data(event));
  LV_ASSERT_NULL(page);

  lv_obj_t       *target = (lv_obj_t *)lv_event_get_current_target(event);
  lv_event_code_t code   = lv_event_get_code(event);

  if (code == LV_EVENT_CLICKED)
  {
    if (target == page->_view.ui.btn) { page->onBtnClicked(target); }
  }
}
