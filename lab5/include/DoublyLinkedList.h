#pragma once
#include <memory_resource>
#include "ListIterator.h"

template <typename T>
class DoublyLinkedList {
public:
    struct Node {
        T data;
        Node* next;
        Node* prev;
        
        Node(const T& val, Node* p = nullptr, Node* n = nullptr)
            : data(val), prev(p), next(n) {}
    };

    using iterator = ListIterator<Node, T>;

private:
    std::pmr::polymorphic_allocator<Node> allocator_;
    Node* head_;
    Node* tail_;

public:
    DoublyLinkedList(std::pmr::memory_resource* mr)
        : allocator_(mr), head_(nullptr), tail_(nullptr) {}

    ~DoublyLinkedList() {
        clear();
    }

    void push_back(const T& value) {
        Node* newNode = allocator_.allocate(1);
        
        allocator_.construct(newNode, value, tail_, nullptr);

        if (tail_) {
            tail_->next = newNode;
        } else {
            head_ = newNode;
        }
        tail_ = newNode;
    }

    void pop_back() {
        if (!tail_) return;

        Node* toDelete = tail_;
        tail_ = tail_->prev;
        
        if (tail_) tail_->next = nullptr;
        else head_ = nullptr;

        allocator_.destroy(toDelete);
        allocator_.deallocate(toDelete, 1);
    }

    void clear() {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            allocator_.destroy(temp);
            allocator_.deallocate(temp, 1);
        }
        tail_ = nullptr;
    }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }
};