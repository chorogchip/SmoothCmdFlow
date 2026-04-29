#pragma once

#include "SmoothCmdFlowDevice.h"
#include "SmoothCmdFlowSetup.h"

namespace scf::domaintests {

inline int bool_data = 1;
inline int table_data = 2;
inline int column_data = 3;
inline int predicate_data = 4;
inline int rows_data = 5;
inline int scalar_data = 6;
inline int row_data = 7;
inline int path_data = 8;
inline int entries_data = 9;
inline int pattern_data = 10;
inline int vm_data = 11;
inline int register_data = 12;
inline int address_data = 13;
inline int i64_data = 14;

inline void* bool_callback(void*, void**) { return &bool_data; }
inline void* table_callback(void*, void**) { return &table_data; }
inline void* column_callback(void*, void**) { return &column_data; }
inline void* predicate_callback(void*, void**) { return &predicate_data; }
inline void* rows_callback(void*, void**) { return &rows_data; }
inline void* scalar_callback(void*, void**) { return &scalar_data; }
inline void* row_callback(void*, void**) { return &row_data; }
inline void* path_callback(void*, void**) { return &path_data; }
inline void* entries_callback(void*, void**) { return &entries_data; }
inline void* pattern_callback(void*, void**) { return &pattern_data; }
inline void* vm_callback(void*, void**) { return &vm_data; }
inline void* register_callback(void*, void**) { return &register_data; }
inline void* address_callback(void*, void**) { return &address_data; }
inline void* i64_callback(void*, void**) { return &i64_data; }

inline bool executes(SmoothCmdFlowSetup& setup, const char* command) {
    SmoothCmdFlowDevice device;
    device.bind_setup(&setup);
    device.set_string(command);
    return device.execute();
}

inline SmoothCmdFlowSetup make_sql_setup() {
    SmoothCmdFlowSetup setup;

    const auto i64_type = setup.add_type("I64");
    const auto table_type = setup.add_type("Table");
    const auto column_type = setup.add_type("Column");
    const auto predicate_type = setup.add_type("Predicate");
    const auto rows_type = setup.add_type("Rows");
    const auto scalar_type = setup.add_type("Scalar");
    const auto row_type = setup.add_type("Row");

    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "users", SmoothCmdFlowSetup::type(table_type), {}, table_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "orders", SmoothCmdFlowSetup::type(table_type), {}, table_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "alice", SmoothCmdFlowSetup::type(row_type), {}, row_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "age", SmoothCmdFlowSetup::type(column_type), {}, column_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "name", SmoothCmdFlowSetup::type(column_type), {}, column_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "age-gt", SmoothCmdFlowSetup::type(predicate_type), {SmoothCmdFlowSetup::type(i64_type)}, predicate_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "name-is", SmoothCmdFlowSetup::type(predicate_type), {SmoothCmdFlowSetup::type(scalar_type)}, predicate_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, table_type, "where", SmoothCmdFlowSetup::type(table_type), {SmoothCmdFlowSetup::type(predicate_type)}, table_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, table_type, "select", SmoothCmdFlowSetup::type(rows_type), {SmoothCmdFlowSetup::type(column_type)}, rows_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, row_type, "name", SmoothCmdFlowSetup::type(scalar_type), {}, scalar_callback);

    return setup;
}

inline SmoothCmdFlowSetup make_filesystem_setup() {
    SmoothCmdFlowSetup setup;

    const auto bool_type = setup.add_type("Bool");
    const auto path_type = setup.add_type("Path");
    const auto entries_type = setup.add_type("Entries");
    const auto pattern_type = setup.add_type("Pattern");

    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "home", SmoothCmdFlowSetup::type(path_type), {}, path_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "readme", SmoothCmdFlowSetup::type(path_type), {}, path_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "todo", SmoothCmdFlowSetup::type(path_type), {}, path_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "cpp", SmoothCmdFlowSetup::type(pattern_type), {}, pattern_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "user", SmoothCmdFlowSetup::type(path_type), {}, path_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "project", SmoothCmdFlowSetup::type(path_type), {}, path_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "md", SmoothCmdFlowSetup::type(path_type), {}, path_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "ls", SmoothCmdFlowSetup::type(entries_type), {}, entries_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "touch", SmoothCmdFlowSetup::type(bool_type), {SmoothCmdFlowSetup::type(path_type)}, bool_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, path_type, "rm", SmoothCmdFlowSetup::type(bool_type), {SmoothCmdFlowSetup::type(path_type)}, bool_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, entries_type, "filter", SmoothCmdFlowSetup::type(entries_type), {SmoothCmdFlowSetup::type(pattern_type)}, entries_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, pattern_type, "files", SmoothCmdFlowSetup::type(pattern_type), {}, pattern_callback);

    return setup;
}

inline SmoothCmdFlowSetup make_small_vm_setup() {
    SmoothCmdFlowSetup setup;

    const auto bool_type = setup.add_type("Bool");
    const auto vm_type = setup.add_type("Vm");
    const auto register_type = setup.add_type("Register");
    const auto address_type = setup.add_type("Address");
    const auto i64_type = setup.add_type("I64");

    setup.add_func(SmoothCmdFlowSetup::FuncType::GLOBAL, {}, "vm", SmoothCmdFlowSetup::type(vm_type), {}, vm_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "zero-reg", SmoothCmdFlowSetup::type(register_type), {}, register_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "one-reg", SmoothCmdFlowSetup::type(register_type), {}, register_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "pc", SmoothCmdFlowSetup::type(register_type), {}, register_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::ARGUMENT, {}, "addr", SmoothCmdFlowSetup::type(address_type), {}, address_callback);

    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, address_type, "stack", SmoothCmdFlowSetup::type(address_type), {}, address_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, vm_type, "step", SmoothCmdFlowSetup::type(vm_type), {}, vm_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, vm_type, "run", SmoothCmdFlowSetup::type(vm_type), {SmoothCmdFlowSetup::type(i64_type)}, vm_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, vm_type, "set", SmoothCmdFlowSetup::type(bool_type), {SmoothCmdFlowSetup::type(register_type), SmoothCmdFlowSetup::type(i64_type)}, bool_callback);
    setup.add_func(SmoothCmdFlowSetup::FuncType::METHOD, vm_type, "peek", SmoothCmdFlowSetup::type(i64_type), {SmoothCmdFlowSetup::type(address_type)}, i64_callback);

    return setup;
}

inline bool test_sql_domain_examples() {
    SmoothCmdFlowSetup setup = make_sql_setup();

    return executes(setup, "users.where age-gt 18 | .select name") &&
           executes(setup, "users.where name-is alice.name | .select age") &&
           executes(setup, "orders.select name");
}

inline bool test_filesystem_domain_examples() {
    SmoothCmdFlowSetup setup = make_filesystem_setup();

    return executes(setup, "home.user.project.ls") &&
           executes(setup, "home.user.project.ls.filter cpp.files") &&
           executes(setup, "home.user.project.touch readme.md") &&
           executes(setup, "home.user.project.rm todo.md");
}

inline bool test_small_vm_domain_examples() {
    SmoothCmdFlowSetup setup = make_small_vm_setup();

    return executes(setup, "vm.step") &&
           executes(setup, "vm.run 1") &&
           executes(setup, "vm.set zero-reg | 0") &&
           executes(setup, "vm.peek addr.stack");
}

inline bool run_domain_tests() {
    return test_sql_domain_examples() &&
           test_filesystem_domain_examples() &&
           test_small_vm_domain_examples();
}

} // namespace scf::domaintests
