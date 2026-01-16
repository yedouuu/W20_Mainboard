
#include "App.h"
#include "Pages/AppPageFactory.h"
#include "PageManager/PageManager.h"

void App_Init(void)
{
  // 应用程序初始化代码
  static AppPageFactory appPageFactory;
  static PageManager    pageManager(&appPageFactory);

  if (!lv_group_get_default())
  {
    lv_group_t *g = lv_group_create();
    lv_group_set_default(g);
  }

  lv_obj_t *scr = lv_scr_act();
  lv_obj_remove_style_all(scr);
  lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_COVER, 0);
  lv_obj_set_style_bg_color(lv_scr_act(), lv_color_black(), 0);
  lv_obj_clear_flag(scr, LV_OBJ_FLAG_SCROLLABLE);
  // lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

  static lv_style_t root_style;
  lv_style_init(&root_style);
  lv_style_set_width(&root_style, LV_HOR_RES);
  lv_style_set_height(&root_style, LV_VER_RES);
  lv_style_set_bg_opa(&root_style, LV_OPA_COVER);
  lv_style_set_bg_color(&root_style, lv_color_black());
  lv_style_set_border_width(&root_style, 0);
  // lv_style_set_border_color(&root_style, lv_color_make(0, 255, 0));
  // lv_style_set_radius(&root_style, 5);
  lv_style_set_pad_all(&root_style, 0);
  pageManager.SetRootDefaultStyle(&root_style);

  pageManager.Install("MainPage",     "Pages/MainPage");
  pageManager.Install("Startup",      "Pages/Startup");
  pageManager.Install("UserSettings", "Pages/UserSettings");

  pageManager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_LEFT);

  pageManager.Push("Pages/Startup");
  // pageManager.Push("Pages/UserSettings");
}
