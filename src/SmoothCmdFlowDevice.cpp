#include "SmoothCmdFlowDevice.h"

#include <deque>
#include <sstream>
#include <string_view>
#include <utility>

#include "Lexer.h"
#include "Token.h"

namespace scf {
namespace {

struct Value {
    SmoothCmdFlowSetup::TypeRef type;
    void* data = nullptr;
};

struct Frame {
    const SmoothCmdFlowSetup::FuncDecl* func = nullptr;
    SmoothCmdFlowSetup::TypeRef result_type;
    void* belong_data = nullptr;
    std::vector<void*> input_data;
};

static bool same_type(SmoothCmdFlowSetup::TypeRef lhs, SmoothCmdFlowSetup::TypeRef rhs) {
    return lhs.id == rhs.id && lhs.is_array == rhs.is_array;
}

static bool is_identifier_char(char ch) {
    return ('A' <= ch && ch <= 'Z') ||
           ('a' <= ch && ch <= 'z') ||
           ch == '-' ||
           ch == '_';
}

static std::size_t current_prefix_begin(std::string_view input) {
    std::size_t pos = input.size();

    while (pos > 0 && is_identifier_char(input[pos - 1])) {
        --pos;
    }

    return pos;
}

static bool starts_with(std::string_view value, std::string_view prefix) {
    return value.size() >= prefix.size() && value.substr(0, prefix.size()) == prefix;
}

static bool has_type(SmoothCmdFlowSetup::TypeRef type) {
    return type.id != SmoothCmdFlowSetup::no_type;
}

class Executor {
private:
    const SmoothCmdFlowSetup& setup_;
    const std::vector<SmoothCmdFlowSetup::FuncDecl>& funcs_;
    const std::vector<std::string>& type_names_;
    std::string source_;
    StringPool token_table_;
    std::vector<Lexeme> tokens_;
    std::deque<num_data_t> number_storage_;
    std::vector<Frame> stack_;
    Value current_;
    bool has_current_ = false;
    bool after_dot_ = false;
    std::string error_;
    SmoothCmdFlowSetup::TypeId i64_type_ = SmoothCmdFlowSetup::no_type;

public:
    Executor(
        const SmoothCmdFlowSetup& setup,
        const std::vector<SmoothCmdFlowSetup::FuncDecl>& funcs,
        const std::vector<std::string>& type_names,
        std::string source
    ) : setup_(setup), funcs_(funcs), type_names_(type_names), source_(std::move(source)) {}

    bool run(std::string& error) {
        auto i64 = setup_.find_type("I64");
        if (i64.has_value()) {
            i64_type_ = *i64;
        }
        tokens_ = Lexer::lex(source_, token_table_);

        for (std::size_t i = 0; i < tokens_.size(); ++i) {
            const Lexeme& lexeme = tokens_[i];

            if (lexeme.tok == Token::EndOfFile) {
                break;
            }
            if (!consume(lexeme)) {
                error = error_;
                return false;
            }
        }

        if (!close_tail_bars()) {
            error = error_;
            return false;
        }
        if (after_dot_) {
            error = "expected method name after '.'";
            return false;
        }
        if (!stack_.empty()) {
            error = "input ended before all function arguments were supplied";
            return false;
        }

        return true;
    }

private:
    bool consume(const Lexeme& lexeme) {
        switch (lexeme.tok) {
            case Token::Identifier:
                return consume_identifier(lexeme.id);
            case Token::Number:
                return consume_number(static_cast<num_data_t>(lexeme.id));
            case Token::Dot:
                return consume_dot();
            case Token::Bar:
                return consume_bar();
            case Token::Invalid:
                error_ = "invalid token at offset " + std::to_string(lexeme.id);
                return false;
            case Token::EndOfFile:
                return true;
        }

        error_ = "unknown token";
        return false;
    }

    bool consume_identifier(std::uint64_t token_id) {
        auto name = token_table_.get(token_id);
        if (!name.has_value()) {
            error_ = "internal lexer identifier lookup failed";
            return false;
        }

        const SmoothCmdFlowSetup::FuncDecl* func = resolve_func(*name);
        if (func == nullptr) {
            if (error_.empty()) {
                error_ = "unknown function '" + std::string(*name) + "'";
            }
            return false;
        }

        return start_func(*func);
    }

    bool consume_number(num_data_t value) {
        if (after_dot_) {
            error_ = "expected method name after '.', got number literal " + std::to_string(value);
            return false;
        }
        if (stack_.empty()) {
            error_ = "number literal " + std::to_string(value) + " cannot start a sentence";
            return false;
        }
        if (has_current_) {
            error_ = "expected '|' or '.', got number literal " + std::to_string(value);
            return false;
        }
        if (i64_type_ == SmoothCmdFlowSetup::no_type) {
            error_ = "number literal requires setup type 'I64'";
            return false;
        }

        number_storage_.push_back(value);
        current_ = Value{SmoothCmdFlowSetup::type(i64_type_), &number_storage_.back()};
        has_current_ = true;
        return true;
    }

    bool consume_dot() {
        if (!has_current_) {
            error_ = "'.' requires a receiver value before it";
            return false;
        }
        if (after_dot_) {
            error_ = "unexpected repeated '.'";
            return false;
        }

        after_dot_ = true;
        return true;
    }

    bool consume_bar() {
        if (after_dot_) {
            error_ = "expected method name after '.', got '|'";
            return false;
        }
        if (stack_.empty()) {
            error_ = "unexpected '|'";
            return false;
        }
        if (!has_current_) {
            error_ = "expected argument before '|' for function '" + stack_.back().func->name + "'";
            return false;
        }

        return close_for_bar();
    }

    const SmoothCmdFlowSetup::FuncDecl* resolve_func(std::string_view name) {
        if (after_dot_) {
            return resolve_method(name);
        }
        if (!has_current_) {
            return resolve_starter(name);
        }

        error_ = "expected '|' or '.' before function '" + std::string(name) + "'";
        return nullptr;
    }

    const SmoothCmdFlowSetup::FuncDecl* resolve_starter(std::string_view name) {
        const SmoothCmdFlowSetup::TypeRef* expected = expected_input_type();
        const SmoothCmdFlowSetup::FuncDecl* selected = nullptr;
        const SmoothCmdFlowSetup::FuncDecl* wrong_type = nullptr;

        for (const SmoothCmdFlowSetup::FuncDecl& func : funcs_) {
            if (func.name != name) {
                continue;
            }
            if (stack_.empty()) {
                if (func.type != SmoothCmdFlowSetup::FuncType::GLOBAL) {
                    continue;
                }
            } else {
                if (func.type != SmoothCmdFlowSetup::FuncType::GLOBAL &&
                    func.type != SmoothCmdFlowSetup::FuncType::ARGUMENT) {
                    continue;
                }
                if (expected != nullptr && !same_type(func.out, *expected)) {
                    if (wrong_type == nullptr) {
                        wrong_type = &func;
                    }
                    continue;
                }
            }

            if (selected != nullptr) {
                error_ = "ambiguous function '" + std::string(name) + "'";
                return nullptr;
            }
            selected = &func;
        }

        if (selected == nullptr && expected != nullptr && wrong_type != nullptr) {
            error_ = "function '" + std::string(name) + "' returns " + type_name(wrong_type->out) +
                     ", expected " + type_name(*expected) + " for argument " +
                     std::to_string(stack_.back().input_data.size() + 1) +
                     " of function '" + stack_.back().func->name + "'";
        }

        return selected;
    }

    const SmoothCmdFlowSetup::FuncDecl* resolve_method(std::string_view name) {
        if (!has_current_) {
            error_ = "method call requires a receiver";
            return nullptr;
        }

        const SmoothCmdFlowSetup::FuncDecl* selected = nullptr;

        for (const SmoothCmdFlowSetup::FuncDecl& func : funcs_) {
            if (func.type != SmoothCmdFlowSetup::FuncType::METHOD) {
                continue;
            }
            if (func.name != name || func.belong_type != current_.type.id) {
                continue;
            }

            if (selected != nullptr) {
                error_ = "ambiguous method '" + std::string(name) + "'";
                return nullptr;
            }
            selected = &func;
        }

        if (selected == nullptr) {
            error_ = "unknown method '" + std::string(name) + "' for type " + type_name(current_.type);
        }

        return selected;
    }

    const SmoothCmdFlowSetup::TypeRef* expected_input_type() const {
        if (stack_.empty()) {
            return nullptr;
        }

        const Frame& frame = stack_.back();
        if (frame.input_data.size() >= frame.func->in.size()) {
            return nullptr;
        }
        return &frame.func->in[frame.input_data.size()];
    }

    bool start_func(const SmoothCmdFlowSetup::FuncDecl& func) {
        void* belong = nullptr;
        SmoothCmdFlowSetup::TypeRef result_type = func.out;

        if (after_dot_) {
            belong = current_.data;
            if (current_.type.is_array && has_type(result_type)) {
                result_type.is_array = true;
            }
            has_current_ = false;
            after_dot_ = false;
        }

        if (func.in.empty()) {
            return execute_func(func, result_type, belong, {});
        }

        stack_.push_back(Frame{&func, result_type, belong, {}});
        return true;
    }

    bool execute_func(
        const SmoothCmdFlowSetup::FuncDecl& func,
        SmoothCmdFlowSetup::TypeRef result_type,
        void* belong,
        const std::vector<void*>& input_data
    ) {
        if (func.fp == nullptr) {
            error_ = "function '" + func.name + "' has no callback";
            return false;
        }

        void** input_ptr = input_data.empty() ? nullptr : const_cast<void**>(input_data.data());
        current_ = Value{result_type, func.fp(belong, input_ptr)};
        has_current_ = true;
        return true;
    }

    bool close_one_argument() {
        Frame& frame = stack_.back();
        const std::size_t index = frame.input_data.size();

        if (index >= frame.func->in.size()) {
            error_ = "too many arguments for function '" + frame.func->name + "'";
            return false;
        }
        if (!same_type(current_.type, frame.func->in[index])) {
            error_ = "argument " + std::to_string(index + 1) + " for function '" +
                     frame.func->name + "' expected " + type_name(frame.func->in[index]) +
                     ", got " + type_name(current_.type);
            return false;
        }

        frame.input_data.push_back(current_.data);
        has_current_ = false;

        if (frame.input_data.size() != frame.func->in.size()) {
            return true;
        }

        Frame complete = std::move(frame);
        stack_.pop_back();
        return execute_func(*complete.func, complete.result_type, complete.belong_data, complete.input_data);
    }

    bool close_for_bar() {
        if (!close_one_argument()) {
            return false;
        }

        while (has_current_ && !stack_.empty()) {
            if (!close_one_argument()) {
                return false;
            }
        }

        return true;
    }

    bool close_tail_bars() {
        while (!stack_.empty()) {
            if (!has_current_) {
                const Frame& frame = stack_.back();
                error_ = "missing argument " + std::to_string(frame.input_data.size() + 1) +
                         " for function '" + frame.func->name + "'";
                return false;
            }
            if (!close_one_argument()) {
                return false;
            }
        }

        return true;
    }

    std::string type_name(const SmoothCmdFlowSetup::TypeRef& type) const {
        if (type.id == SmoothCmdFlowSetup::no_type) {
            return "<none>";
        }

        std::string base;
        if (type.id < type_names_.size()) {
            base = type_names_[type.id];
        } else {
            base = "#" + std::to_string(type.id);
        }

        if (type.is_array) {
            return "Array<" + base + ">";
        }
        return base;
    }
};

} // namespace

    void SmoothCmdFlowDevice::clear_string() {
        input_.clear();
    }

    void SmoothCmdFlowDevice::set_string(std::string input) {
        input_ = std::move(input);
    }

    void SmoothCmdFlowDevice::add_char(char input) {
        input_.push_back(input);
    }

    void SmoothCmdFlowDevice::sub_char() {
        if (!input_.empty()) {
            input_.pop_back();
        }
    }

    void SmoothCmdFlowDevice::apply_autocomplete() {
        if (setting_ == nullptr) return;
        auto candidates = get_command_candidate();

        if (candidates.size() != 1) return;

        const std::size_t prefix_begin = current_prefix_begin(input_);
        input_.replace(prefix_begin, input_.size() - prefix_begin, candidates.front());
    }

    bool SmoothCmdFlowDevice::execute() {
        err_msg_.clear();

        if (setting_ == nullptr) {
            err_msg_ = "SmoothCmdFlowDevice has no setup bound";
            return false;
        }
        if (input_.empty()) return true;

        std::vector<std::string> type_names;
        for (std::size_t i = 0; ; ++i) {
            auto name = setting_->type_ids_.get(i);
            if (!name.has_value()) {
                break;
            }
            type_names.push_back(std::string(*name));
        }

        Executor executor(*setting_, setting_->funcs_, type_names, input_);
        return executor.run(err_msg_);
    }

    std::vector<std::string> SmoothCmdFlowDevice::get_command_candidate() const {
        std::vector<std::string> result;

        if (setting_ == nullptr) return result;

        const std::size_t prefix_begin = current_prefix_begin(input_);
        const std::string_view prefix(input_.data() + prefix_begin, input_.size() - prefix_begin);

        for (const SmoothCmdFlowSetup::FuncDecl& func : setting_->funcs_) {
            if (!starts_with(func.name, prefix)) {
                continue;
            }

            bool duplicated = false;
            for (const std::string& existing : result) {
                if (existing == func.name) {
                    duplicated = true;
                    break;
                }
            }

            if (!duplicated) {
                result.push_back(func.name);
            }
        }

        return result;
    }

} // namespace scf
