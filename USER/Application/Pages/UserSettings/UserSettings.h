
#ifndef __USER_SETTINGS_H__
#define __USER_SETTINGS_H__

#include "UserSettingsView.h"
#include "UserSettingsModel.h"

namespace Page
{

class UserSettings : public PageBase
{
public:
  UserSettings();
  virtual ~UserSettings();

  void onCustomAttrConfig() override;
  void onViewLoad() override;
  void onViewDidLoad() override;
  void onViewWillAppear() override;
  void onViewDidAppear() override;
  void onViewWillDisappear() override;
  void onViewDidDisappear() override;
  void onViewUnload() override;
  void onViewDidUnload() override;

private:
  // Private Defines

private:
  static void onTimerUpdate(lv_timer_t *timer);
  static void onEvent(lv_event_t *event);
  void        onBtnClicked(lv_obj_t *btn);

private:
  lv_timer_t       *timer;
  UserSettingsModel _model;
  UserSettingsView  _view;

}; // class UserSettings;

} // namespace Page
#endif // __USER_SETTINGS_H__