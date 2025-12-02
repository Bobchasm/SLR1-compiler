#ifndef SYSYC_CONSTANTFLOAT_H
#define SYSYC_CONSTANTFLOAT_H

#include "Constant.h"
#include "Type.h"

class Module;

class ConstantFloat : public Constant {
private:
  float value_;

public:
  explicit ConstantFloat(Type *ty, float val);
  static ConstantFloat *get(float val, Module *m);
  float get_value() const;
  std::string print() override;
};

#endif 