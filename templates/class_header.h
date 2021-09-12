/*
 Do not modify, auto-generated by model_gen.tcl

 Copyright 2019 Alain Dargelas

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
 * File:   <CLASSNAME>.h
 * Author:
 *
 * Created on December 14, 2019, 10:03 PM
 */

#ifndef UHDM_<UPPER_CLASSNAME>_H
#define UHDM_<UPPER_CLASSNAME>_H

#include <uhdm/sv_vpi_user.h>
#include <uhdm/uhdm_vpi_user.h>

#include <uhdm/SymbolFactory.h>
#include <uhdm/BaseClass.h>
#include <uhdm/containers.h>

#include <uhdm/<EXTENDS>.h>

<GROUP_HEADER_DEPENDENCY>

namespace UHDM {
<TYPE_FORWARD_DECLARE>

class <CLASSNAME> <FINAL_CLASS> : public <EXTENDS> {
public:
  // Implicit constructor used to initialize all members,
  // comment: <CLASSNAME>();
  <VIRTUAL>~<CLASSNAME>() <FINAL_DESTRUCTOR> {}
  <METHODS>
  <VIRTUAL> UHDM_OBJECT_TYPE UhdmType() const <OVERRIDE_OR_FINAL> { return uhdm<CLASSNAME>; }

private:
  <MEMBERS>
};

<DISABLE_OBJECT_FACTORY>
class <CLASSNAME>Factory {
  friend Serializer;
public:
  <CLASSNAME>* Make() {
    <CLASSNAME>* obj = new <CLASSNAME>();
    objects_.push_back(obj);
    return obj;
  }
  void Erase(<CLASSNAME>* tps) {
    for (std::vector<<CLASSNAME>*>::iterator itr = objects_.begin(); itr != objects_.end(); itr++) {
      if ((*itr) == tps) {
        objects_.erase(itr);
        break;
      }
    }
  }

private:
  std::vector<<CLASSNAME>*> objects_;
};
<END_DISABLE_OBJECT_FACTORY>

class VectorOf<CLASSNAME>Factory {
  friend Serializer;
public:
 std::vector<<CLASSNAME>*>* Make() {
   std::vector<<CLASSNAME>*>* obj = new std::vector<<CLASSNAME>*>();
   objects_.push_back(obj);
   return obj;
 }
private:
 std::vector<std::vector<<CLASSNAME>*>*> objects_;
};

}  // namespace UHDM

#endif
