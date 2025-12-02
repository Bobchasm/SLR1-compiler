#include "ConstantFloat.h"
#include "Module.h"
#include <sstream>
#include <iomanip>
#include <cstdint>

ConstantFloat::ConstantFloat(Type *ty, float val)
    : Constant(ty, "", 0), value_(val) {}

ConstantFloat *ConstantFloat::get(float val, Module *m) {
  return new ConstantFloat(Type::get_float_type(m), val);
}

float ConstantFloat::get_value() const {
  return value_;
}

std::string ConstantFloat::print() {
  // 使用十六进制格式表示浮点数，符合LLVM IR标准
  std::ostringstream oss;
  union {
    float f;
    uint32_t i;
  } converter;
  converter.f = value_;
  oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << converter.i;
  return oss.str();
}