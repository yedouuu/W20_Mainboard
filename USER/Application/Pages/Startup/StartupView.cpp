
#include "StartupView.h"

using namespace Page;

#define COLOR_ORANGE lv_color_hex(0xff931e)

// LV_IMG_DECLARE(BankosaStartup);

void StartupView::Create(lv_obj_t *parent)
{
  // // 2. 创建一个图片对象
  // lv_obj_t* img = lv_img_create(parent);

  // // 3. 设置图片源
  // lv_img_set_src(img, &BankosaStartup);

  // // 4. 将图片居中对齐
  // lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);
  lv_obj_t *cont = lv_obj_create(parent);
  lv_obj_remove_style_all(cont);
  lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_size(cont, 110, 50);
  lv_obj_set_style_border_color(cont, COLOR_ORANGE, 0);
  lv_obj_set_style_border_side(cont, LV_BORDER_SIDE_BOTTOM, 0);
  lv_obj_set_style_border_width(cont, 3, 0);
  lv_obj_set_style_border_post(cont, true, 0);
  lv_obj_center(cont);
  ui.cont = cont;

  lv_obj_t *label = lv_label_create(cont);
  // lv_obj_set_style_text_font(label, ResourcePool::GetFont("agencyb_36"), 0);
  lv_obj_set_style_text_color(label, lv_color_white(), 0);
  lv_label_set_text(label, "UNION");
  lv_obj_center(label);
  ui.labelLogo = label;

  ui.anim_timeline = lv_anim_timeline_create();

#define ANIM_DEF(start_time, obj, attr, start, end)                            \
  {start_time,                                                                 \
   obj,                                                                        \
   LV_ANIM_EXEC(attr),                                                         \
   start,                                                                      \
   end,                                                                        \
   500,                                                                        \
   lv_anim_path_ease_out,                                                      \
   true}

  
  lv_anim_timeline_wrapper_t wrapper[] = {
      ANIM_DEF(0, ui.cont, width, 0, lv_obj_get_style_width(ui.cont, LV_PART_MAIN)),
      ANIM_DEF(500,
               ui.labelLogo,
               y,
               lv_obj_get_style_height(ui.cont, LV_PART_MAIN),
               lv_obj_get_y(ui.labelLogo)),
      LV_ANIM_TIMELINE_WRAPPER_END};

  lv_anim_timeline_add_wrapper(ui.anim_timeline, wrapper);
}

void StartupView::Destroy()
{
  if (ui.anim_timeline)
  {
    lv_anim_timeline_delete(ui.anim_timeline);
    ui.anim_timeline = nullptr;
  }
}
