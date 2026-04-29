#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "SmoothCmdFlowSetup.h"

namespace scf {


    class SmoothCmdFlowDevice {
    private:
        const SmoothCmdFlowSetup* setting_;
        std::string input_;
        std::string err_msg_;
    public:
        SmoothCmdFlowDevice(): setting_{nullptr}, input_(), err_msg_() {}
        void bind_setup(const SmoothCmdFlowSetup* ref) { setting_ = ref; }

        void clear_string();
        void set_string(std::string input);
        void add_char(char input);
        void sub_char();
        void apply_autocomplete();
        bool execute();

        std::string_view get_string() const { return input_; }
        std::string_view get_last_error_msg() const { return err_msg_; }
        std::vector<std::string> get_command_candidate() const;
    };
};
