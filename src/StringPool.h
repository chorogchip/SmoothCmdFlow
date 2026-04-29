#pragma once

#include <cstddef>
#include <deque>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace scf {

class StringPool {
public:
    using id_type = std::size_t;

private:
    std::deque<std::string> entries_;
    std::unordered_map<std::string_view, id_type> index_;

public:
    id_type add(std::string_view str);

    std::optional<std::string_view> get(id_type id) const;
    std::optional<id_type> get(std::string_view str) const;

    bool contains(id_type id) const;
    bool contains(std::string_view str) const;

    std::size_t size() const;
    bool empty() const;

    void clear();
};

}