#pragma once

#include "SmoothCmdFlowDevice.h"
#include "SmoothCmdFlowSetup.h"
#include "Token.h"

namespace scf::testexamples {

inline int entity_data = 1;
inline int bool_data = 1;
inline int pos_data = 2;
inline int block_data = 3;
inline int damage_called = 0;
inline int pos_called = 0;
inline int setblock_called = 0;
inline int kill_called = 0;
inline num_data_t last_damage = 0;
inline num_data_t last_pos_x = 0;
inline num_data_t last_pos_y = 0;
inline num_data_t last_pos_z = 0;

inline void* noop_callback(void*, void**) {
    return nullptr;
}

inline void reset_test_state() {
    damage_called = 0;
    pos_called = 0;
    setblock_called = 0;
    kill_called = 0;
    last_damage = 0;
    last_pos_x = 0;
    last_pos_y = 0;
    last_pos_z = 0;
}

inline void* entity_callback(void*, void**) {
    return &entity_data;
}

inline void* stone_callback(void*, void**) {
    return &block_data;
}

inline void* damage_callback(void* bel, void** in) {
    if (bel == &entity_data && in != nullptr && in[0] != nullptr) {
        ++damage_called;
        last_damage = *static_cast<num_data_t*>(in[0]);
    }
    return &bool_data;
}

inline void* kill_callback(void* bel, void**) {
    if (bel == &entity_data) {
        ++kill_called;
    }
    return &bool_data;
}

inline void* pos_callback(void*, void** in) {
    if (in != nullptr && in[0] != nullptr && in[1] != nullptr && in[2] != nullptr) {
        ++pos_called;
        last_pos_x = *static_cast<num_data_t*>(in[0]);
        last_pos_y = *static_cast<num_data_t*>(in[1]);
        last_pos_z = *static_cast<num_data_t*>(in[2]);
    }
    return &pos_data;
}

inline void* setblock_callback(void*, void** in) {
    if (in != nullptr && in[0] == &pos_data && in[1] == &block_data) {
        ++setblock_called;
    }
    return &bool_data;
}

inline SmoothCmdFlowSetup make_minecraft_like_setup() {
    SmoothCmdFlowSetup setup;

    const auto bool_type = setup.add_type("Bool");
    const auto i64_type = setup.add_type("I64");
    const auto entity_type = setup.add_type("Entity");
    const auto block_type = setup.add_type("Block");
    const auto effect_type = setup.add_type("Effect");
    const auto pos_type = setup.add_type("Pos");

    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "entity", SmoothCmdFlowSetup::type(entity_type), {}, entity_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "entities", SmoothCmdFlowSetup::array(entity_type), {}, noop_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "stone", SmoothCmdFlowSetup::type(block_type), {}, stone_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "spark", SmoothCmdFlowSetup::type(effect_type), {}, noop_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, entity_type, "kill", SmoothCmdFlowSetup::type(bool_type), {}, kill_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, entity_type, "damage", SmoothCmdFlowSetup::type(bool_type), {SmoothCmdFlowSetup::type(i64_type)}, damage_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, entity_type, "emit", SmoothCmdFlowSetup::type(bool_type), {SmoothCmdFlowSetup::type(effect_type)}, noop_callback);

    // N-ary example: pos x | y | z |
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "pos", SmoothCmdFlowSetup::type(pos_type), {
        SmoothCmdFlowSetup::type(i64_type),
        SmoothCmdFlowSetup::type(i64_type),
        SmoothCmdFlowSetup::type(i64_type),
    }, pos_callback);

    // 2-ary example: setblock pos | block |
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "setblock", SmoothCmdFlowSetup::type(bool_type), {
        SmoothCmdFlowSetup::type(pos_type),
        SmoothCmdFlowSetup::type(block_type),
    }, setblock_callback);

    return setup;
}

inline bool test_device_execute() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("entity.damage 10");
    return device.execute() && damage_called == 1 && last_damage == 10;
}

inline bool test_device_execute_explicit_bar() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("entity.damage 7 |");
    return device.execute() && damage_called == 1 && last_damage == 7;
}

inline bool test_device_execute_zero_arg_method() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("entity.kill");
    return device.execute() && kill_called == 1;
}

inline bool test_device_execute_nary_global() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("pos 1 | 2 | 3");
    return device.execute() &&
           pos_called == 1 &&
           last_pos_x == 1 &&
           last_pos_y == 2 &&
           last_pos_z == 3;
}

inline bool test_device_execute_nested_global_args() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("setblock pos 1 | 2 | 3 | stone");
    return device.execute() && pos_called == 1 && setblock_called == 1;
}

inline bool test_device_reject_type_mismatch() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    reset_test_state();

    device.set_string("entity.damage stone");
    return !device.execute() && damage_called == 0;
}

inline bool test_device_reject_unknown_function() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);

    device.set_string("entity.fly");
    return !device.execute();
}

inline bool run_device_tests() {
    return test_device_execute() &&
           test_device_execute_explicit_bar() &&
           test_device_execute_zero_arg_method() &&
           test_device_execute_nary_global() &&
           test_device_execute_nested_global_args() &&
           test_device_reject_type_mismatch() &&
           test_device_reject_unknown_function();
}

inline void test1() {
    SmoothCmdFlowSetup setup = make_minecraft_like_setup();

    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);
    device.set_string("entity.damage 10 |");
}

} // namespace scf::testexamples
