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
#include "clone_tree.h"

#include "ElaboratorListener.h"
#include "uhdm.h"

using namespace UHDM;

namespace UHDM {

BaseClass* clone_tree (const BaseClass* root, Serializer& s, ElaboratorListener* elaborator) {
  return root ? root->DeepClone(&s, elaborator, nullptr) : nullptr;
}

// Hardcoded implementations

any* ElaboratorListener::bindNet(const std::string& name) {
  for (InstStack::reverse_iterator i = instStack_.rbegin();
       i != instStack_.rend(); ++i ) {
    ComponentMap& netMap = std::get<0>((*i).second);
    ComponentMap::iterator netItr = netMap.find(name);
    if (netItr != netMap.end()) {
      return (any*) (*netItr).second;
    }
  }
  return nullptr;
}

// Bind to a net or parameter in the current instance
any* ElaboratorListener::bindAny(const std::string& name) {
  for (InstStack::reverse_iterator i = instStack_.rbegin();
       i != instStack_.rend(); ++i ) {
    ComponentMap& netMap = std::get<0>((*i).second);
    ComponentMap::iterator netItr = netMap.find(name);
    if (netItr != netMap.end()) {
      return (any*) (*netItr).second;
    }

    ComponentMap& paramMap = std::get<1>((*i).second);
    ComponentMap::iterator paramItr = paramMap.find(name);
    if (paramItr != paramMap.end()) {
      return (any*) (*paramItr).second;
    }
  }
  return nullptr;
}

// Bind to a param in the current instance
any* ElaboratorListener::bindParam(const std::string& name) {
  for (InstStack::reverse_iterator i = instStack_.rbegin();
       i != instStack_.rend(); ++i ) {
    ComponentMap& paramMap = std::get<1>((*i).second);
    ComponentMap::iterator paramItr = paramMap.find(name);
    if (paramItr != paramMap.end()) {
      return (any*) (*paramItr).second;
    }
  }
  return nullptr;
}

// Bind to a function or task in the current scope
any* ElaboratorListener::bindTaskFunc(const std::string& name, const class_var* prefix) {
  for (InstStack::reverse_iterator i = instStack_.rbegin();
       i != instStack_.rend(); ++i ) {
    ComponentMap& funcMap = std::get<2>((*i).second);
    ComponentMap::iterator funcItr = funcMap.find(name);
    if (funcItr != funcMap.end()) {
      return (any*) (*funcItr).second;
    }
  }
  if (prefix) {
    const typespec* tps = prefix->Typespec();
    if (tps && tps->UhdmType() == uhdmclass_typespec) {
      const class_defn* def = ((class_typespec*) tps)->Class_defn();
      while (def) {
	if (def->Task_funcs()) {
	  for (task_func* tf : *def->Task_funcs()) {
	    if (tf->VpiName() == name)
	      return tf;
	  }
	}
	const UHDM::extends* ext = def->Extends();
	if (ext) {
	  const class_typespec* tps = ext->Class_typespec();
	  def = tps->Class_defn();
	} else {
	  break;
	}
      }
    }
  }
  return nullptr;
}

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

constant* constant::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  if (elaborator->uniquifyTypespec()) {
    constant* const clone = serializer->MakeConstant();
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Typespec()) clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
    return clone;
  } else {
    return (constant*) this;
  }
}

tagged_pattern* tagged_pattern::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  if (elaborator->uniquifyTypespec()) {
    tagged_pattern* const clone = serializer->MakeTagged_pattern();
    const unsigned long id = clone->UhdmId();
    *clone = *this;
    clone->UhdmId(id);
    clone->VpiParent(parent);
    if (auto obj = Typespec()) clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
    if (auto obj = Pattern()) clone->Pattern(obj->DeepClone(serializer, elaborator, clone));
    return clone;
  } else {
    return (tagged_pattern*) this;
  }
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
    } else {
      elaborator->scheduleTaskFuncBinding(clone);
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

gen_scope_array* gen_scope_array::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  gen_scope_array* const clone = serializer->MakeGen_scope_array();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto obj = Gen_var()) clone->Gen_var(obj->DeepClone(serializer, elaborator, clone));
  if (auto vec = Gen_scopes()) {
    auto clone_vec = serializer->MakeGen_scopeVec();
    clone->Gen_scopes(clone_vec);
    for (auto obj : *vec) {
      elaborator->enterGen_scope(obj, parent, nullptr, nullptr);
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      elaborator->leaveGen_scope(obj, parent, nullptr, nullptr);
    }
  }
  if (auto obj = VpiInstance()) clone->VpiInstance(obj->DeepClone(serializer, elaborator, clone));

  return clone;
}

function* function::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  if (function* f = dynamic_cast<function*>( elaborator->bindTaskFunc(VpiName(), nullptr))) {
    return f;
  }
  function* const clone = serializer->MakeFunction();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto obj = Left_range()) clone->Left_range(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Right_range()) clone->Right_range(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Return()) clone->Return((variables*)obj);
  if (auto obj = Instance()) clone->Instance((instance*)obj);
  if (instance* inst = dynamic_cast<instance*>(parent))
    clone->Instance(inst);
  if (auto obj = Class_defn()) clone->Class_defn(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Ref_obj()) clone->Ref_obj(obj->DeepClone(serializer, elaborator, clone));
  if (auto vec = Io_decls()) {
    auto clone_vec = serializer->MakeIo_declVec();
    clone->Io_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Variables()) {
    auto clone_vec = serializer->MakeVariablesVec();
    clone->Variables(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Parameters()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Parameters(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Scopes()) {
    auto clone_vec = serializer->MakeScopeVec();
    clone->Scopes(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Typespecs()) {
    auto clone_vec = serializer->MakeTypespecVec();
    clone->Typespecs(clone_vec);
    for (auto obj : *vec) {
      if (elaborator->uniquifyTypespec()) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      } else {
        clone_vec->push_back(obj);
      }
    }
  }
  elaborator->enterTask_func(clone, parent, nullptr, nullptr);
  if (auto vec = Concurrent_assertions()) {
    auto clone_vec = serializer->MakeConcurrent_assertionsVec();
    clone->Concurrent_assertions(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Property_decls()) {
    auto clone_vec = serializer->MakeProperty_declVec();
    clone->Property_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Sequence_decls()) {
    auto clone_vec = serializer->MakeSequence_declVec();
    clone->Sequence_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Named_events()) {
    auto clone_vec = serializer->MakeNamed_eventVec();
    clone->Named_events(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Named_event_arrays()) {
    auto clone_vec = serializer->MakeNamed_event_arrayVec();
    clone->Named_event_arrays(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Virtual_interface_vars()) {
    auto clone_vec = serializer->MakeVirtual_interface_varVec();
    clone->Virtual_interface_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Logic_vars()) {
    auto clone_vec = serializer->MakeLogic_varVec();
    clone->Logic_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Array_vars()) {
    auto clone_vec = serializer->MakeArray_varVec();
    clone->Array_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Array_var_mems()) {
    auto clone_vec = serializer->MakeArray_varVec();
    clone->Array_var_mems(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Param_assigns()) {
    auto clone_vec = serializer->MakeParam_assignVec();
    clone->Param_assigns(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Let_decls()) {
    auto clone_vec = serializer->MakeLet_declVec();
    clone->Let_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Attributes()) {
    auto clone_vec = serializer->MakeAttributeVec();
    clone->Attributes(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Instance_items()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Instance_items(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto obj = Stmt()) clone->Stmt(obj->DeepClone(serializer, elaborator, clone));
  elaborator->leaveTask_func(clone, parent, nullptr, nullptr);
  return clone;
}

task* task::DeepClone(Serializer* serializer, ElaboratorListener* elaborator, BaseClass* parent) const {
  task* const clone = serializer->MakeTask();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto obj = Left_range()) clone->Left_range(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Right_range()) clone->Right_range(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Return()) clone->Return(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Instance()) clone->Instance((instance*)obj);
  if (instance* inst = dynamic_cast<instance*>(parent))
    clone->Instance(inst);
  if (auto obj = Class_defn()) clone->Class_defn(obj->DeepClone(serializer, elaborator, clone));
  if (auto obj = Ref_obj()) clone->Ref_obj(obj->DeepClone(serializer, elaborator, clone));
  if (auto vec = Io_decls()) {
    auto clone_vec = serializer->MakeIo_declVec();
    clone->Io_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Variables()) {
    auto clone_vec = serializer->MakeVariablesVec();
    clone->Variables(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Scopes()) {
    auto clone_vec = serializer->MakeScopeVec();
    clone->Scopes(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Typespecs()) {
    auto clone_vec = serializer->MakeTypespecVec();
    clone->Typespecs(clone_vec);
    for (auto obj : *vec) {
      if (elaborator->uniquifyTypespec()) {
        clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
      } else {
        clone_vec->push_back(obj);
      }
    }
  }
  elaborator->enterTask_func(clone, parent, nullptr, nullptr);
  if (auto vec = Concurrent_assertions()) {
    auto clone_vec = serializer->MakeConcurrent_assertionsVec();
    clone->Concurrent_assertions(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Property_decls()) {
    auto clone_vec = serializer->MakeProperty_declVec();
    clone->Property_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Sequence_decls()) {
    auto clone_vec = serializer->MakeSequence_declVec();
    clone->Sequence_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Named_events()) {
    auto clone_vec = serializer->MakeNamed_eventVec();
    clone->Named_events(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Named_event_arrays()) {
    auto clone_vec = serializer->MakeNamed_event_arrayVec();
    clone->Named_event_arrays(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Virtual_interface_vars()) {
    auto clone_vec = serializer->MakeVirtual_interface_varVec();
    clone->Virtual_interface_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Logic_vars()) {
    auto clone_vec = serializer->MakeLogic_varVec();
    clone->Logic_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Array_vars()) {
    auto clone_vec = serializer->MakeArray_varVec();
    clone->Array_vars(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Array_var_mems()) {
    auto clone_vec = serializer->MakeArray_varVec();
    clone->Array_var_mems(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Param_assigns()) {
    auto clone_vec = serializer->MakeParam_assignVec();
    clone->Param_assigns(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Let_decls()) {
    auto clone_vec = serializer->MakeLet_declVec();
    clone->Let_decls(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Attributes()) {
    auto clone_vec = serializer->MakeAttributeVec();
    clone->Attributes(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Parameters()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Parameters(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto vec = Instance_items()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Instance_items(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (auto obj = Stmt()) clone->Stmt(obj->DeepClone(serializer, elaborator, clone));
  elaborator->leaveTask_func(clone, parent, nullptr, nullptr);
  return clone;
}

static void propagateParamAssign(param_assign* pass, const any* target) {
  UHDM_OBJECT_TYPE targetType = target->UhdmType();
  Serializer& s = *pass->GetSerializer();
  switch (targetType) {
    case uhdmclass_defn: {
      class_defn* defn = (class_defn*)target;
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
            pa->Rhs((any*)pass->Rhs());
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
      class_var* var = (class_var*)target;
      propagateParamAssign(pass, var->Typespec());
      break;
    }
    case uhdmclass_typespec: {
      class_typespec* defn = (class_typespec*)target;
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
            pa->Rhs((any*)pass->Rhs());
            passigns->push_back(pa);
          }
        }
      }
      const class_defn* def = defn->Class_defn();
      if (def) {
        propagateParamAssign(pass, (class_defn*)def);
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
  Serializer* s = ((variables*)object)->GetSerializer();
  if (object->UhdmType() == uhdmclass_var) {
    if (!inHierarchy_)
      return; // Only do class var propagation while in elaboration
    const class_var* cv = (class_var*)object;
    class_var* const rw_cv = (class_var*) cv;
    class_typespec* ctps = (class_typespec*)cv->Typespec();
    if (ctps) {
      ctps = ctps->DeepClone(s, this, rw_cv);
      rw_cv->Typespec(ctps);
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

void ElaboratorListener::enterTask_func(const task_func* object, const BaseClass* parent,
				       vpiHandle handle, vpiHandle parentHandle) {

  // Collect instance elaborated nets
  ComponentMap varMap;
  if (object->Variables()) {
    for (variables* var : *object->Variables()) {
      varMap.insert(std::make_pair(var->VpiName(), var));
    }
  }
  if (object->Io_decls()) {
    for (io_decl* decl : *object->Io_decls()) {
      varMap.insert(std::make_pair(decl->VpiName(), decl));
    }
  }
  varMap.insert(std::make_pair(object->VpiName(), object->Return()));
  
  ComponentMap paramMap;
  
  ComponentMap funcMap;
  
  instStack_.push_back(std::make_pair(object, std::make_tuple(varMap, paramMap, funcMap)));

}

void ElaboratorListener::leaveTask_func(const task_func* object, const BaseClass* parent,
				       vpiHandle handle, vpiHandle parentHandle) {
  instStack_.pop_back();
}

void ElaboratorListener::enterGen_scope(const gen_scope* object, const BaseClass* parent, vpiHandle handle, vpiHandle parentHandle) {
  // Collect instance elaborated nets

  ComponentMap netMap;
  if (object->Nets()) {
    for (net* net : *object->Nets()) {
      netMap.insert(std::make_pair(net->VpiName(), net));
    }
  }
  if (object->Array_nets()) {
    for (array_net* net : *object->Array_nets()) {
      netMap.insert(std::make_pair(net->VpiName(), net));
    }
  }
  // Collect instance parameters, defparams
  ComponentMap paramMap;
  if (object->Parameters()) {
    for (any* param : *object->Parameters()) {
      paramMap.insert(std::make_pair(param->VpiName(), param));
    }
  }
  if (object->Def_params()) {
    for (def_param* param : *object->Def_params()) {
      paramMap.insert(std::make_pair(param->VpiName(), param));
    }
  }
  if (object->Variables()) {
    for (variables* var : *object->Variables()) {
      paramMap.insert(std::make_pair(var->VpiName(), var));
    }
  }
  
  ComponentMap funcMap;
  
  instStack_.push_back(std::make_pair(object, std::make_tuple(netMap, paramMap, funcMap)));
}

void ElaboratorListener::leaveGen_scope(const gen_scope* object, const BaseClass* parent, vpiHandle handle, vpiHandle parentHandle) {
  instStack_.pop_back();
}

hier_path* hier_path::DeepClone(Serializer* serializer,
                                ElaboratorListener* elaborator,
                                BaseClass* parent) const {
  hier_path* const clone = serializer->MakeHier_path();
  const unsigned long id = clone->UhdmId();
  *clone = *this;
  clone->UhdmId(id);
  clone->VpiParent(parent);
  if (auto vec = Path_elems()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->Path_elems(clone_vec);
    any* previous = nullptr;
    for (auto obj : *vec) {
      any* current = nullptr;
      current = obj->DeepClone(serializer, elaborator, clone);
      clone_vec->push_back(current);
      bool found = false;
      if (previous) {
        const std::string& name = obj->VpiName();
        if (previous->UhdmType() == uhdmref_obj) {
          ref_obj* ref = (ref_obj*)previous;
          const any* actual = ref->Actual_group();
          if (actual && (actual->UhdmType() == uhdmstruct_net)) {
            struct_typespec* stpt =
                (struct_typespec*)((struct_net*)actual)->Typespec();
            for (typespec_member* member : *stpt->Members()) {
              if (member->VpiName() == name) {
                if (current->UhdmType() == uhdmref_obj) {
                  ((ref_obj*)current)->Actual_group(member);
                  previous = member;
                  found = true;
                  break;
                }
              }
            }
          }
        } else if (previous->UhdmType() == uhdmtypespec_member) {
          typespec_member* member = (typespec_member*)previous;
          const typespec* tps = member->Typespec();
          if (tps && (tps->UhdmType() == uhdmstruct_typespec)) {
            struct_typespec* stpt = (struct_typespec*)tps;
            for (typespec_member* member : *stpt->Members()) {
              if (member->VpiName() == name) {
                if (current->UhdmType() == uhdmref_obj) {
                  ((ref_obj*)current)->Actual_group(member);
                  previous = member;
                  found = true;
                  break;
                }
              }
            }
          }
        }
      }
      if (!found) previous = current;
    }
  }
  if (auto vec = VpiUses()) {
    auto clone_vec = serializer->MakeAnyVec();
    clone->VpiUses(clone_vec);
    for (auto obj : *vec) {
      clone_vec->push_back(obj->DeepClone(serializer, elaborator, clone));
    }
  }
  if (elaborator->uniquifyTypespec()) {
    if (auto obj = Typespec())
      clone->Typespec(obj->DeepClone(serializer, elaborator, clone));
  } else {
    if (auto obj = Typespec()) clone->Typespec((typespec*)obj);
  }

  return clone;
}

void ElaboratorListener::leaveRef_obj(const ref_obj* object, const BaseClass* parent,
				      vpiHandle handle, vpiHandle parentHandle) {
  if (!object->Actual_group())
    ((ref_obj*)object)->Actual_group(bindAny(object->VpiName()));
}

// Auto generated implementations

<CLONE_IMPLEMENTATIONS>

}  // UHDM namespace

