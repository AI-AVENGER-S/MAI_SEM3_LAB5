#pragma once
#include <iterator>

template <typename NodeType, typename ValueType>
class ListIterator {
public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = ValueType;
    using difference_type = std::ptrdiff_t;
    using pointer = ValueType*;
    using reference = ValueType&;

    ListIterator(NodeType* node) : current_(node) {}

    reference operator*() const { return current_->data; }
    pointer operator->() const { return &current_->data; }

    ListIterator& operator++() {
        if (current_) current_ = current_->next;
        return *this;
    }

    ListIterator& operator--() {
        if (current_) current_ = current_->prev;
        return *this;
    }

    bool operator!=(const ListIterator& other) const { 
        return current_ != other.current_; 
    }
    bool operator==(const ListIterator& other) const { 
        return current_ == other.current_; 
    }

private:
    NodeType* current_;
};