
#ifndef __Startup_H__
#define __Startup_H__

#include "StartupView.h"
#include "StartupModel.h"

namespace Page
{

class Startup : public PageBase
{
public:
  Startup();
  virtual ~Startup();

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
  static void onTimer(lv_timer_t *timer);
  static void onEvent(lv_event_t *event);
  void        onBtnClicked(lv_obj_t *btn);

private:
  lv_timer_t  *timer;
  StartupModel _model;
  StartupView  _view;

}; // class Startup;

} // namespace Page
#endif // __Startup_H__