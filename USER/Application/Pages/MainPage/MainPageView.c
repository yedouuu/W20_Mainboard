
#include "MainPageView.h"

void MainPage_Create(lv_obj_t* parent)
{
    // 创建一个标签
    lv_obj_t* label = lv_label_create(parent);
    lv_label_set_text(label, "Welcome to the Main Page!");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    // 创建一个按钮
    lv_obj_t* button = lv_btn_create(parent);
    lv_obj_set_size(button, 100, 50);
    lv_obj_align(button, LV_ALIGN_CENTER, 0, 50);

    // 给按钮添加标签
    lv_obj_t* btn_label = lv_label_create(button);
    lv_label_set_text(btn_label, "Click Me");
    lv_obj_center(btn_label);
}

