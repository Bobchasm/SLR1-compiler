#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <unordered_map>
#include "../ir_lib/include/Module.h"
#include "../ir_lib/include/Function.h"
#include "../ir_lib/include/BasicBlock.h"
#include "../ir_lib/include/IRbuilder.h"
#include "../ir_lib/include/IRprinter.h"
#include "../ir_lib/include/Constant.h"
#include "../ir_lib/include/Value.h"
#include "../ir_lib/include/GlobalVariable.h"
#include "../ir_lib/include/Instruction.h"
#include "../ir_lib/include/Type.h"
#include "../ir_lib/include/User.h"

class SymbolTable {
private:
    std::unordered_map<std::string, Value*> table_;
    SymbolTable* parent_;

public:
    SymbolTable(SymbolTable* parent = nullptr) : parent_(parent) {}
    
    void put(const std::string& name, Value* value) {
        table_[name] = value;
    }
    
    Value* get(const std::string& name) {
        auto it = table_.find(name);
        if (it != table_.end()) {
            return it->second;
        }
        if (parent_) {
            return parent_->get(name);
        }
        return nullptr;
    }
    
    bool contains(const std::string& name) {
        return get(name) != nullptr;
    }
};

#endif