
#ifndef ____TEMPLATE_VIEW_H__
#define ____TEMPLATE_VIEW_H__

#include "Pages/Page.h"

namespace Page
{

class __TemplateView
{

public:
  struct
  {
    lv_obj_t *btn;
  } ui;

  void Create(lv_obj_t *parent);

private:
  void SiderCreate(lv_obj_t *parent);
  void TopInfoCreate(lv_obj_t *parent);
  void ContentCreate(lv_obj_t *parent);
  void BottomInfoCreate(lv_obj_t *parent);
  void settings_btn_event_cb(lv_event_t *e);

}; // class __TemplateView;

} // namespace Page

#endif // ____TemplateVIEW_H__
