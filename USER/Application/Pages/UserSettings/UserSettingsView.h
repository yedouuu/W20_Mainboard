
#ifndef __USER_SETTINGS_VIEW_H__
#define __USER_SETTINGS_VIEW_H__

#include "Pages/Page.h"

namespace Page
{

class UserSettingsView
{
public:
  struct
  {
    lv_obj_t *btn;
  } ui;

public:
  void Create(lv_obj_t *parent);

private:
  void SiderCreate(lv_obj_t *parent);
  void TopInfoCreate(lv_obj_t *parent);
  void ContentCreate(lv_obj_t *parent);
  void BottomInfoCreate(lv_obj_t *parent);
  void settings_btn_event_cb(lv_event_t *e);

}; // class UserSettingsView;

}

#endif // __USER_SETTINGS_VIEW_H__
