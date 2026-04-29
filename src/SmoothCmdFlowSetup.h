#pragma once

#include <cstddef>
#include <cstdint>
#include <initializer_list>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "StringPool.h"

namespace scf {

class SmoothCmdFlowDevice;

class SmoothCmdFlowSetup {
public:
    using TypeId = StringPool::id_type;
    using FuncId = std::size_t;
    using Callback = void* (*)(void* bel, void** in);

    static constexpr TypeId no_type = static_cast<TypeId>(-1);
    static constexpr FuncId no_func = static_cast<FuncId>(-1);

    enum class FuncType : std::uint8_t {
        GLOBAL,
        METHOD,
        ARGUMENT,
    };

    struct TypeRef {
        TypeId id = no_type;
        bool is_array = false;
    };

    struct FuncDecl {
        FuncId id = no_func;
        FuncType type = FuncType::GLOBAL;
        TypeId belong_type = no_type;
        std::string name;
        TypeRef out;
        std::vector<TypeRef> in;
        Callback fp = nullptr;
    };

private:
    StringPool type_ids_;
    std::vector<FuncDecl> funcs_;

    friend class SmoothCmdFlowDevice;

public:
    SmoothCmdFlowSetup() = default;

    static TypeRef type(TypeId id) { return TypeRef{id, false}; }
    static TypeRef array(TypeId id) { return TypeRef{id, true}; }

    TypeId add_type(std::string_view name);

    FuncId add_func(
        FuncType type,
        TypeId belong_type,
        std::string_view name,
        TypeRef out,
        std::initializer_list<TypeRef> in,
        Callback fp
    );

    std::optional<TypeId> find_type(std::string_view name) const;
};

}
