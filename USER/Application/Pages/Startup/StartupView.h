
#ifndef __Startup_VIEW_H__
#define __Startup_VIEW_H__

#include "Pages/Page.h"

namespace Page
{

class StartupView
{

public:
  struct
  {
    lv_obj_t *cont;
    lv_obj_t *labelLogo;

    lv_anim_timeline_t *anim_timeline;
  } ui;

  void Create(lv_obj_t *parent);
  void Destroy();

private:
}; // class StartupView;

} // namespace Page

#endif // __StartupVIEW_H__
