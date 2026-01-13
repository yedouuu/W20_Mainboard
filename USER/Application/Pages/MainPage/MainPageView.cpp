
#include "MainPageView.h"

// LV_IMG_DECLARE(BankosaMainPage);

/* Private Function ---------------------------------------------------------*/
static void MainPage_SiderCreate(lv_obj_t* parent);
static void MainPage_TopInfoCreate(lv_obj_t* parent);
static void MainPage_ContentCreate(lv_obj_t* parent);
static void MainPage_BottomInfoCreate(lv_obj_t* parent);
static void settings_btn_event_cb(lv_event_t* e);

void MainPage_Create(lv_obj_t* parent)
{
    // // 2. 创建一个图片对象
    // lv_obj_t* img = lv_img_create(parent);

    // // 3. 设置图片源
    // lv_img_set_src(img, &BankosaMainPage);

    // // 4. 将图片居中对齐
    // lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    MainPage_BottomInfoCreate(parent);
    MainPage_ContentCreate(parent);
    MainPage_TopInfoCreate(parent);
    // MainPage_SiderCreate(parent);

}

static void MainPage_SiderCreate(lv_obj_t* parent)
{
    // 创建侧边栏的容器
    lv_obj_t* sider = lv_obj_create(parent);
    lv_obj_set_size(sider, 80, lv_obj_get_height(parent));
    lv_obj_align(sider, LV_ALIGN_LEFT_MID, 0, 0);

    // 设置容器样式（可选）
    lv_obj_set_style_bg_color(sider, lv_color_hex(0xDDDDDD), 0);
    lv_obj_set_style_border_width(sider, 0, 0);

    // 在侧边栏添加按钮或图标
    lv_obj_t* sider_button = lv_btn_create(sider);
    lv_obj_set_size(sider_button, 60, 60);
    lv_obj_align(sider_button, LV_ALIGN_TOP_MID, 0, 20);

    lv_obj_t* btn_label = lv_label_create(sider_button);
    lv_label_set_text(btn_label, "Menu");
    lv_obj_center(btn_label);
}

static void MainPage_TopInfoCreate(lv_obj_t* parent)
{
    // 创建顶部信息区域的容器
    lv_obj_t* top_info = lv_obj_create(parent);
    lv_obj_set_size(top_info, lv_obj_get_width(parent), 50);
    lv_obj_align(top_info, LV_ALIGN_TOP_MID, 0, 0);

    // 设置容器样式（可选）
    lv_obj_set_style_bg_color(top_info, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_style_border_width(top_info, 0, 0);

    // 在顶部信息区域添加标签
    lv_obj_t* info_label = lv_label_create(top_info);
    lv_label_set_text(info_label, "Main Page Header");
    lv_obj_center(info_label);
}

static void MainPage_ContentCreate(lv_obj_t* parent)
{
    // 创建内容区域的容器
    lv_obj_t* content = lv_obj_create(parent);
    lv_obj_set_size(content, lv_obj_get_width(parent), lv_obj_get_height(parent) - 50);
    lv_obj_align(content, LV_ALIGN_TOP_MID, 0, 0);

    // 设置容器样式（可选）
    lv_obj_set_style_bg_color(content, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_border_width(content, 0, 0);

    // 在内容区域添加其他UI元素
    // 例如，添加一个标签
    lv_obj_t* content_label = lv_label_create(content);
    lv_label_set_text(content_label, "Main Content Area");
    lv_obj_center(content_label);
}

static void MainPage_BottomInfoCreate(lv_obj_t* parent)
{
    // 创建底部信息区域的容器
    lv_obj_t* bottom_info = lv_obj_create(parent);
    lv_obj_remove_style_all(bottom_info);


    lv_obj_set_size(bottom_info, lv_obj_get_width(parent), 50);
    lv_obj_align(bottom_info, LV_ALIGN_BOTTOM_MID, 0, 0);

    // 设置容器样式（可选）
    lv_obj_set_style_bg_color(bottom_info, lv_color_hex(0xCCCCCC), 0);
    lv_obj_set_style_border_width(bottom_info, 0, 0);

    // 在底部信息区域添加标签
    // lv_obj_t* info_label = lv_label_create(bottom_info);
    // lv_label_set_text(info_label, "Status: All systems operational");
    // lv_obj_center(info_label);

    

    lv_obj_t* pcs_label = lv_label_create(bottom_info);
    lv_label_set_text(pcs_label, "PCS: 1234");
    lv_obj_center(pcs_label);

    lv_obj_t* SN_label = lv_label_create(bottom_info);
    lv_label_set_text(SN_label, "SN: SA9812432A");
    lv_obj_center(SN_label);

    // 设置按钮
    lv_obj_t* settings_btn = lv_btn_create(bottom_info);
    lv_obj_set_size(settings_btn, 80, 35);
    lv_obj_align(settings_btn, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_add_event_cb(settings_btn, settings_btn_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t* btn_label = lv_label_create(settings_btn);
    lv_label_set_text(btn_label, "Settings");
    lv_obj_center(btn_label);
}


static void settings_btn_event_cb(lv_event_t* e)
{
    // 切换到 UserSettings 页面
    // 这里假设你有一个页面管理器或者直接加载 UserSettings
    lv_obj_t* parent = lv_scr_act();

    // 创建新屏幕
    lv_obj_t* new_scr = lv_obj_create(NULL);

    UserSettings_Init(new_scr);   // 初始化 UserSettings 页面

    lv_scr_load_anim(new_scr, LV_SCR_LOAD_ANIM_OVER_LEFT, 300, 0, true);

    // lv_obj_clean(parent);  // 清除当前屏幕内容
}
