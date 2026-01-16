
#ifndef __MAINPAGE_H__
#define __MAINPAGE_H__

#include "MainPageView.h"
#include "MainPageModel.h"

namespace Page
{

class MainPage : public PageBase
{
public:
  MainPage();
  virtual ~MainPage();

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
  lv_timer_t   *timer;
  MainPageModel _model;
  MainPageView  _view;

}; // class MainPage;

} // namespace Page
#endif // __MAINPAGE_H__