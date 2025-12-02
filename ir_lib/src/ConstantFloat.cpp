#include "ConstantFloat.h"
#include "Module.h"
#include <sstream>

ConstantFloat::ConstantFloat(Type *ty, float val)
    : Constant(ty, "", 0), value_(val) {}

ConstantFloat *ConstantFloat::get(float val, Module *m) {
  return new ConstantFloat(Type::get_float_type(m), val);
}

float ConstantFloat::get_value() const {
  return value_;
}

std::string ConstantFloat::print() {
  std::ostringstream oss;
  oss << value_;
  return oss.str();
}