///  Copyright (C) 2017 BIM Project Future.Inc //NOLINT
///      http://www.futuresoft.com.cn
///
/// \author qiujiajun
/// \version 1.0
/// \date 2017-11-9
#ifndef LOGIC_BASE_
#define LOGIC_BASE_
#include "define.h"
#include "event_manager.h"
#include "forward_declare.h"
#include "imgui.h"
#include "osg.h"
#include "render_interface.h"
class Logic {
 public:
  LogicManager *manager_;
  RenderEngine *engine_;
  EventManager *event_manager_;
  std::map<RequestType, CallablePtr> response_;
  bool focus_;
  bool enable_;
  bool active_;
  Logic()
      : manager_(NULL),
        engine_(NULL),
        event_manager_(NULL),
        focus_(false),
        enable_(false),
        active_(false) {}
  virtual ~Logic() {}

  virtual void init() {}
  virtual void update() {}
  virtual void destory() {}
  ///
  /// \brief 启用/关闭时被调
  ///
  virtual void on_enable() {}
  virtual void on_disable() {}
  ///
  /// \brief 每次gui阶段被调用
  ///
  virtual void on_gui() {}
  ///
  /// \brief 每次窗口大小改变时被调用
  /// \param [I] 窗口当前横向大小
  /// \param [I] 窗口当前纵向大小
  ///
  virtual void on_size(int x, int y) {}
  virtual bool on_click(const osgGA::GUIEventAdapter &ea) { return true; }
  virtual void on_event(const osgGA::GUIEventAdapter &ea) {}
  virtual void on_focus(bool focus) {}

  virtual void on_load(std::istream &stream) {}
  virtual void on_save(std::ostream &stream) {}
  ///////////////////////////////////////////////////////////////////////////////////////
  ///工具函数
  void set_manager(LogicManager *manager, RenderEngine *engine);

  bool is_active() { return active_; }
  void set_active(bool active) {
    active_ = active;
    active ? on_enable() : on_disable();
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  ///事件
  template <typename... Args>
  bool bind_event(Event::EventType type,
                  boost::function<void(Args...)> any_fun) {
    event_manager_->bind(type, any_fun);
  }
  bool bind_event(Event::EventType type, boost::function<void(void)> any_fun) {
    event_manager_->bind(type, any_fun);
  }
  template <typename... Args>
  void fire(Event::EventType type, Args... args) {
    event_manager_->fire(type, args...);
  }
  void fire(Event::EventType type) {
    Params params;
    event_manager_->fire(type, params);
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  ///职责
  ///
  /// \brief 注册职责
  /// \param [I] 职责类型
  /// \param [I] 函数对象
  /// \return 是否成功注册
  ///
  bool register_duty(DutyType type, boost::function<void(void)> any_fun);
  template <typename... Args>
  bool register_duty(DutyType type, boost::function<void(Args...)> any_fun) {
    return manager_->register_duty(type, any_fun);
  }

  bool duty(DutyType type, Params params);
  template <typename... Args>
  bool duty(DutyType type, Args... args) {
    Params params;
    gen_params(params, args...);
    return duty(type, params);
  }
  ///////////////////////////////////////////////////////////////////////////////////////
  ///请求
  ///
  /// \brief 注册请求处理函数
  /// \param [I] 请求类型
  /// \param [I] 函数对象
  /// \return 是否成功注册
  ///
  bool register_response(RequestType type, boost::function<void(void)> any_fun);
  template <typename... Args>
  bool register_response(RequestType type,
                         boost::function<void(Args...)> any_fun) {
    CallObject<Args...> *new_call = new CallObject<Args...>;
    new_call->fun = any_fun;
    response_[type] = CallablePtr(new_call);
    return true;
  }
  ///
  /// \brief 分发请求
  /// \param [I] 请求类型
  /// \return 是否成功注册
  ///
  void dispatch_request(RequestType type, Params &params);
  void request(RequestType type);
  template <typename... Args>
  void request(RequestType type, Args... args) {
    Params params;
    gen_params(params, args...);
    dispatch_request(type, params);
  }
  void gen_params(Params &params) { return; }
  template <class T, class... Args>
  void gen_params(Params &params, T head, Args... rest) {
    params.push_param(head);
    gen_params(params, rest...);
  }
};

#endif
