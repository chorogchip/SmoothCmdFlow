#include "StringPool.h"

namespace scf {

StringPool::id_type StringPool::add(std::string_view str) {
    auto it = index_.find(str);

    if (it != index_.end()) {
        return it->second;
    }

    id_type id = entries_.size();
    entries_.emplace_back(str);

    index_.emplace(std::string_view(entries_.back()), id);

    return id;
}

std::optional<std::string_view> StringPool::get(id_type id) const {
    if (id >= entries_.size()) {
        return std::nullopt;
    }

    return std::string_view(entries_[id]);
}

std::optional<StringPool::id_type> StringPool::get(std::string_view str) const {
    auto it = index_.find(str);

    if (it == index_.end()) {
        return std::nullopt;
    }

    return it->second;
}

bool StringPool::contains(id_type id) const {
    return id < entries_.size();
}

bool StringPool::contains(std::string_view str) const {
    return index_.find(str) != index_.end();
}

std::size_t StringPool::size() const {
    return entries_.size();
}

bool StringPool::empty() const {
    return entries_.empty();
}

void StringPool::clear() {
    entries_.clear();
    index_.clear();
}

}