///  Copyright (C) 2017 BIM Project Future.Inc //NOLINT
///      http://www.futuresoft.com.cn
///
/// \author qiujiajun
/// \version 1.0
/// \date 2017-11-9
#ifndef LOGIC_MANAGER_
#define LOGIC_MANAGER_
#include "define.h"
#include "forward_declare.h"
#include "logic.h"

#include "clip.h"
#include "cube_clip.h"
#include "dragger_manager.h"
#include "manipulator_manager.h"
#include "measure.h"
#include "pick.h"
#include "render_scene.h"
#include "view_cube.h"

#include "extern_model.h"
#include "show_floor.h"
#include "state_machine.h"
#include "thumbnail.h"

#include <wx/dynlib.h>

enum LogicType {
  LOGIC_STATE_MACHINE,
  LOGIC_VIEW_CUBE,
  LOGIC_SCENE,
  LOGIC_DRAGGER,
  LOGIC_PICK,
  LOGIC_CLIP,
  LOGIC_CAMERA,
  LOGIC_MEASURE,
  LOGIC_CUBE_CLIP,
  LOGIC_THUMBNAIL,
  LOGIC_EXTERN_MODEL,
  LOGIC_SHOW_FLOOR,
};
class LogicManager {
 public:
  RenderEngine *engine_;
  int logic_num_;
  std::map<DutyType, CallablePtr> duty_;
  Logic *logics[StaticNum::LOGIC_MAX_NUM];
  std::vector<Logic *> logics_request[StaticNum::LOGIC_MAX_REQUEST];

  StateMachine *state_machine_;
  Pick *pick_;
  Clip *clip_;
  DraggerManager *dragger_;
  ViewCube *view_cube_;
  RenderScene *scene_;
  ManipulatorManager *manipulator_;
  Measure *measure_;
  CubeClip *cube_clip_;
  Thumbnail *thumbnail;
  ExternModel *extern_model;
  ShowFloor *show_floor;
  explicit LogicManager(RenderEngine *engine) {
    engine_ = engine;
    logic_num_ = 0;
    memset(logics, 0, sizeof(logics));
    duty_.clear();
    create_logic();
    register_logic();
    // load_dynamic_logic();
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->set_manager(this, engine);
    }
  }
  ~LogicManager() {}
  void create_logic() {
    state_machine_ = new StateMachine;
    pick_ = new Pick;
    clip_ = new Clip;
    dragger_ = new DraggerManager;
    view_cube_ = new ViewCube;
    scene_ = new RenderScene;
    manipulator_ = new ManipulatorManager;
    measure_ = new Measure;
    cube_clip_ = new CubeClip;
    thumbnail = new Thumbnail;
    extern_model = new ExternModel;
    show_floor = new ShowFloor;
  }
  void register_logic() {
    register_one(LOGIC_STATE_MACHINE, state_machine_);
    register_one(LOGIC_VIEW_CUBE, view_cube_);
    register_one(LOGIC_SCENE, scene_);
    register_one(LOGIC_DRAGGER, dragger_);
    register_one(LOGIC_PICK, pick_);
    register_one(LOGIC_CLIP, clip_);
    register_one(LOGIC_CAMERA, manipulator_);
    register_one(LOGIC_MEASURE, measure_);
    register_one(LOGIC_CUBE_CLIP, cube_clip_);
    register_one(LOGIC_THUMBNAIL, thumbnail);
    register_one(LOGIC_EXTERN_MODEL, extern_model);
    register_one(LOGIC_SHOW_FLOOR, show_floor);
  }

  void register_one(LogicType type, Logic *logic) {
    logics[type] = logic;
    for (auto itr : logic->response_) {
      logics_request[itr.first].push_back(logic);
    }
  }

  // void load_dynamic_logic() {
  //  wxDynamicLibrary lib("test.dll");
  //  auto instance =lib.GetSymbol("instance");
  //  LogicType type;
  //  Logic * logic = instance(logic);
  //  register_one(type, logic);
  //}

  void init() {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->init();
    }
  }

  void post_init() {}

  void update() {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      if (logic->is_active()) logic->update();
    }
  }
  void destory() {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->destory();
    }
  }

  void on_gui() {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      if (logic->is_active()) logic->on_gui();
    }
  }

  void on_size(int x, int y) {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->on_size(x, y);
    }
  }
  void on_click(const osgGA::GUIEventAdapter &ea) {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      if (logic->is_active()) {
        if (!logic->on_click(ea)) break;
      }
    }
  }

  void on_event(const osgGA::GUIEventAdapter &ea) {
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      if (logic->is_active()) logic->on_event(ea);
    }
  }

  void on_request(RequestType type, Params params) {
    for (Logic *logic : logics_request[type]) {
      if (logic == NULL) {
        LOGE(TAG, "LogicManager.on_request:logic is NULL");
        continue;
      }
      if (logic->is_active()) {
        CallablePtr call = logic->response_[type];
        (*call)(params);
      }
    }
  }

  void on_load(std::istream *stream) {
    if (stream == NULL) return;
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->on_load(*stream);
    }
  }

  void on_save(std::ostream *stream) {
    if (stream == NULL) return;
    for (Logic *logic : logics) {
      if (logic == NULL) {
        break;
      }
      logic->on_save(*stream);
    }
  }

  bool register_duty(DutyType type, boost::function<void(void)> any_fun) {
    CallObject<void> *new_call = new CallObject<void>;
    new_call->fun = any_fun;
    duty_[type] = CallablePtr(new_call);
    return true;
  }
  template <typename... Args>
  bool register_duty(DutyType type, boost::function<void(Args...)> any_fun) {
    CallObject<Args...> *new_call = new CallObject<Args...>;
    new_call->fun = any_fun;
    duty_[type] = CallablePtr(new_call);
    return true;
  }

  bool duty(DutyType type, Params params) {
    auto itr = duty_.find(type);
    if (itr == duty_.end()) {
      LOGE(TAG, "LogicManager.request_duty:had no register duty");
      // assert(0);
      return false;
    }
    CallablePtr call = duty_[type];
    (*call)(params);
    return true;
  }
  template <typename... Args>
  bool duty(DutyType type, Args... arg) {
    Params params;
    gen_params(params, arg...);
    auto itr = duty_.find(type);
    if (itr == duty_.end()) {
      LOGE(TAG, "LogicManager.request_duty:had no register duty");
      // assert(0);
      return false;
    }
    duty_[type]->(params);
    return true;
  }

  void gen_params(Params &params) { return; }
  template <class T, class... Args>
  void gen_params(Params &params, T head, Args... rest) {
    params.push_param(head);
    gen_params(params, rest...);
  }
};
#endif
