#include "SmoothCmdFlowSetup.h"

#include <string>

namespace scf {

SmoothCmdFlowSetup::TypeId SmoothCmdFlowSetup::add_type(std::string_view name) {
    return type_ids_.add(name);
}

SmoothCmdFlowSetup::FuncId SmoothCmdFlowSetup::add_func(
    FuncType type,
    TypeId belong_type,
    std::string_view name,
    TypeRef out,
    std::initializer_list<TypeRef> in,
    Callback fp
) {
    FuncId id = funcs_.size();
    funcs_.push_back(FuncDecl{id, type, belong_type, std::string(name), out, in, fp});
    return id;
}

std::optional<SmoothCmdFlowSetup::TypeId> SmoothCmdFlowSetup::find_type(std::string_view name) const {
    return type_ids_.get(name);
}

} // namespace scf
