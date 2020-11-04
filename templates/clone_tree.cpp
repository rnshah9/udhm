/*
 Do not modify, auto-generated by model_gen.tcl

 Copyright 2019-2020 Alain Dargelas

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

/*
 * File:   clone_tree.cpp
 * Author:
 *
 * Created on December 14, 2019, 10:03 PM
 */

#include "uhdm.h"
#include "clone_tree.h"
#include "ElaboratorListener.h"

using namespace UHDM;

namespace UHDM {

BaseClass* clone_tree (const BaseClass* root, Serializer& s, ElaboratorListener* elaborator) {
  return root ? root->DeepClone(&s, elaborator, nullptr) : nullptr;
}

// Hardcoded implementations
bool ElaboratorListener::isFunctionCall(const std::string& name, const expr* prefix) {
  if (instStack_.size()) {
    for (InstStack::reverse_iterator i = instStack_.rbegin();
         i != instStack_.rend(); ++i) {
      ComponentMap& funcMap = std::get<2>((*i).second);
      ComponentMap::iterator funcItr = funcMap.find(name);
      if (funcItr != funcMap.end()) {
        return ((*funcItr).second->UhdmType() == uhdmfunction);
      }
    }
  }
  if (prefix) {
    const ref_obj* ref = dynamic_cast<const ref_obj*> (prefix);
    const class_var* vprefix = nullptr;
    if (ref) vprefix = dynamic_cast<const class_var*> (ref->Actual_group());
    any* func = bindTaskFunc(name, vprefix);
    if (func) {
      if (func->UhdmType() == uhdmfunction) {
        return true;
      } else {
        return false;
      }
    }
  }
  return true;
}

bool ElaboratorListener::isTaskCall(const std::string& name, const expr* prefix) {
  if (instStack_.size()) {
    for (InstStack::reverse_iterator i = instStack_.rbegin();
         i != instStack_.rend(); ++i) {
      ComponentMap& funcMap = std::get<2>((*i).second);
      ComponentMap::iterator funcItr = funcMap.find(name);
      if (funcItr != funcMap.end()) {
        return ((*funcItr).second->UhdmType() == uhdmtask);
      }
    }
  }
  if (prefix) {
    const ref_obj* ref = dynamic_cast<const ref_obj*> (prefix);
    const class_var* vprefix = nullptr;
    if (ref) vprefix = dynamic_cast<const class_var*> (ref->Actual_group());
    any* func = bindTaskFunc(name, vprefix);
    if (func) {
      if (func->UhdmType() == uhdmtask) {
        return true;
      } else {
        return false;
      }
    }
  }
  return true;
}


tf_call* sys_func_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  sys_func_call* const clone = serializer->MakeSys_func_call();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto obj = User_systf()) clone->User_systf(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Scope()) clone->Scope(obj->DeepClone(serializer, elaborator, clone));
  if (auto vec = Tf_call_args()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Tf_call_args(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto obj = Typespec()) clone->Typespec(obj->DeepClone(serializer, elaborator, clone));

  return clone;
}

tf_call* sys_task_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  sys_task_call* const clone = serializer->MakeSys_task_call();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto obj = User_systf()) clone->User_systf(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Scope()) clone->Scope(obj->DeepClone(serializer, elaborator, clone));
  if (auto vec = Tf_call_args()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Tf_call_args(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto obj = Typespec()) clone->Typespec(obj->DeepClone(serializer, elaborator, clone));

  return clone;
}

tf_call* method_func_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  const expr* prefix = Prefix();
  if (prefix) {
    prefix = prefix->DeepClone(serializer, elaborator, (BaseClass*) this);
  }
  bool is_function = elaborator->isFunctionCall(VpiName(), prefix);
  tf_call* the_clone = nullptr;
  if (is_function) {
    method_func_call* const clone = serializer->MakeMethod_func_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Prefix())
      clone->Prefix(obj->DeepClone(serializer, elaborator, clone));
    const ref_obj* ref = dynamic_cast<const ref_obj*>(clone->Prefix());
    const class_var* prefix = nullptr;
    if (ref) prefix = dynamic_cast<const class_var*>(ref->Actual_group());
    if (function* f = dynamic_cast<function*>(
            elaborator->bindTaskFunc(VpiName(), prefix))) {
      clone->Function(f);
    }
    if (auto obj = With())
      clone->With(obj->DeepClone(serializer, elaborator, clone));
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  } else {
    method_task_call* const clone = serializer->MakeMethod_task_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    //*clone = *this;
    clone->VpiName(VpiName());
    clone->Tf_call_args(Tf_call_args());
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Prefix())
      clone->Prefix(obj->DeepClone(serializer, elaborator, clone));
    const ref_obj* ref = dynamic_cast<const ref_obj*>(clone->Prefix());
    const class_var* prefix = nullptr;
    if (ref) prefix = dynamic_cast<const class_var*>(ref->Actual_group());
    if (task* f = dynamic_cast<task*>(
            elaborator->bindTaskFunc(VpiName(), prefix))) {
      clone->Task(f);
    }
    if (auto obj = With())
      clone->With(obj->DeepClone(serializer, elaborator, clone));
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  }
  return the_clone;
}

tf_call* method_task_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  const expr* prefix = Prefix();
  if (prefix) {
    prefix = prefix->DeepClone(serializer, elaborator, (BaseClass*) this);
  }
  bool is_task = elaborator->isTaskCall(VpiName(), prefix);
  tf_call* the_clone = nullptr;
  if (is_task) {
    method_task_call* const clone = serializer->MakeMethod_task_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Prefix())
      clone->Prefix(obj->DeepClone(serializer, elaborator, clone));
    const ref_obj* ref = dynamic_cast<const ref_obj*>(clone->Prefix());
    const class_var* prefix = nullptr;
    if (ref) prefix = dynamic_cast<const class_var*>(ref->Actual_group());
    if (task* t =
            dynamic_cast<task*>(elaborator->bindTaskFunc(VpiName(), prefix))) {
      clone->Task(t);
    }
    if (auto obj = With())
      clone->With(obj->DeepClone(serializer, elaborator, clone));
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  } else {
    method_func_call* const clone = serializer->MakeMethod_func_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    //*clone = *this;
    clone->VpiName(VpiName());
    clone->Tf_call_args(Tf_call_args());
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Prefix())
      clone->Prefix(obj->DeepClone(serializer, elaborator, clone));
    const ref_obj* ref = dynamic_cast<const ref_obj*>(clone->Prefix());
    const class_var* prefix = nullptr;
    if (ref) prefix = dynamic_cast<const class_var*>(ref->Actual_group());
    if (function* t =
            dynamic_cast<function*>(elaborator->bindTaskFunc(VpiName(), prefix))) {
      clone->Function(t);
    }
    if (auto obj = With())
      clone->With(obj->DeepClone(serializer, elaborator, clone));
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  }
  return the_clone;
}

tf_call* func_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  bool is_function = elaborator->isFunctionCall(VpiName(), nullptr);
  tf_call* the_clone = nullptr;
  if (is_function) {
    func_call* const clone = serializer->MakeFunc_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (function* f =
            dynamic_cast<function*>(elaborator->bindTaskFunc(VpiName()))) {
      clone->Function(f);
    }
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  } else {
    task_call* const clone = serializer->MakeTask_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    //*clone = *this;
    clone->VpiName(VpiName());
    clone->Tf_call_args(Tf_call_args());
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (task* f =
            dynamic_cast<task*>(elaborator->bindTaskFunc(VpiName()))) {
      clone->Task(f);
    }
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  }

  return the_clone;
}

tf_call* task_call::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  bool is_task = elaborator->isTaskCall(VpiName(), nullptr);
  tf_call* the_clone = nullptr;
  if (is_task) {
    task_call* const clone = serializer->MakeTask_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (task* t = dynamic_cast<task*>(elaborator->bindTaskFunc(VpiName()))) {
      clone->Task(t);
    }
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  } else {
    func_call* const clone = serializer->MakeFunc_call();
    the_clone = clone;
    const unsigned long id = clone->UhdmId();
    //*clone = *this;
    clone->VpiName(VpiName());
    clone->Tf_call_args(Tf_call_args());
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (function* t = dynamic_cast<function*>(elaborator->bindTaskFunc(VpiName()))) {
      clone->Function(t);
    }
    if (auto obj = Scope())
      clone->Scope(obj->DeepClone(serializer, elaborator, clone));
    if (auto vec = Tf_call_args()) {
      auto clone_vec = serializer->MakeAnyVec();
      clone->Tf_call_args(clone_vec);
      for (auto obj : *vec) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      }
    }
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  }
  return the_clone;
}



static void propagateParamAssign(param_assign* pass, const any* target) {
  UHDM_OBJECT_TYPE targetType = target->UhdmType();
  Serializer& s = *pass->GetSerializer();
  switch (targetType) {
    case uhdmclass_defn: {
      class_defn* defn = (class_defn*) target;
      const any* lhs = pass->Lhs();
      const std::string& name = lhs->VpiName();
      VectorOfany* params = defn->Parameters();
      if (params) {
          for (any* param : *params) {
            if (param->VpiName() == name) {
              VectorOfparam_assign* passigns = defn->Param_assigns();
              if (passigns == nullptr) {
                defn->Param_assigns(s.MakeParam_assignVec());
                passigns = defn->Param_assigns();
              }
              param_assign* pa = s.MakeParam_assign();
              pa->Lhs(param);
              pa->Rhs((any*) pass->Rhs());
              passigns->push_back(pa);
            }
         }
      }
      const UHDM::extends* extends = defn->Extends();
      if (extends) {
        propagateParamAssign(pass, extends->Class_typespec());
      }
      const auto vars = defn->Variables();
      if (vars) {
        for (auto var : *vars) {
           propagateParamAssign(pass, var);
        }
      }
      break;
    }
    case uhdmclass_var: {
      class_var* var = (class_var*) target;
      propagateParamAssign(pass, var->Typespec());
      break;
    }
    case uhdmclass_typespec: {
      class_typespec* defn = (class_typespec*) target;
      const any* lhs = pass->Lhs();
      const std::string& name = lhs->VpiName();
      VectorOfany* params = defn->Parameters();
      if (params) {
          for (any* param : *params) {
            if (param->VpiName() == name) {
              VectorOfparam_assign* passigns = defn->Param_assigns();
              if (passigns == nullptr) {
                defn->Param_assigns(s.MakeParam_assignVec());
                passigns = defn->Param_assigns();
              }
              param_assign* pa = s.MakeParam_assign();
              pa->Lhs(param);
              pa->Rhs((any*) pass->Rhs());
              passigns->push_back(pa);
            }
         }
      }
      const class_defn* def = defn->Class_defn();
      if (def) {
         propagateParamAssign(pass, (class_defn*) def);
      }
      break;
    }
    default:
      break;
  }

}

void ElaboratorListener::enterVariables(const variables* object,
                                        const BaseClass* parent,
                                        vpiHandle handle,
                                        vpiHandle parentHandle) {
  if (object->UhdmType() == uhdmclass_var) {
    const class_var* cv = (class_var*)object;
    class_typespec* ctps = (class_typespec*)cv->Typespec();
    if (ctps) {
      VectorOfparam_assign* params = ctps->Param_assigns();
      if (params) {
        for (param_assign* pass : *params) {
          propagateParamAssign(pass, ctps->Class_defn());
        }
      }
    }
  }
}

void ElaboratorListener::leaveVariables(const variables* object, const BaseClass* parent,
                   vpiHandle handle, vpiHandle parentHandle) {

}

// Auto generated implementations

<CLONE_IMPLEMENTATIONS>

}  // UHDM namespace
