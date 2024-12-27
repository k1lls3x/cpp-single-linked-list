#pragma once

#include <cassert>
#include <cstddef>
#include <string>
#include <utility>
#include <stdexcept>
#include <initializer_list>

using namespace std;

template <typename Type>
class SingleLinkedList {
    struct Node {
        Node() = default;
        Node(const Type& val, Node* next)
            : value(val)
            , next_node(next) {
        }
        Type value;
        Node* next_node = nullptr;
    };

    template <typename ValueType>
    class BasicIterator {
        friend class SingleLinkedList;

        explicit BasicIterator(Node* node) :node_(node) {}
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;

        BasicIterator() = default;
        BasicIterator(const BasicIterator<Type>& other) noexcept : node_(other.node_) { }
        BasicIterator& operator=(const BasicIterator& rhs) = default;

        [[nodiscard]] bool operator==(const BasicIterator<const Type>& rhs) const noexcept {
           return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<const Type>& rhs) const noexcept {
               return this->node_ != rhs.node_;
        }

        [[nodiscard]] bool operator==(const BasicIterator<Type>& rhs) const noexcept {
            return this->node_ == rhs.node_;
        }

        [[nodiscard]] bool operator!=(const BasicIterator<Type>& rhs) const noexcept {
             return this->node_ != rhs.node_;
        }

        BasicIterator& operator++() noexcept {
              assert(node_ != nullptr);
            node_ = node_->next_node;
            return *this;
        }

        BasicIterator operator++(int) noexcept {
                 assert(node_ != nullptr);
       BasicIterator temp = *this;
            node_ = node_->next_node;
            return temp;
        }

        [[nodiscard]] reference operator*() const noexcept {
            assert(node_ != nullptr);
                return node_->value;
        }

        [[nodiscard]] pointer operator->() const noexcept {
             assert(node_ != nullptr);
                return &(node_->value);
        }

    private:
        Node* node_ = nullptr;
    };

public:
    using value_type = Type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using Iterator = BasicIterator<Type>;
    using ConstIterator = BasicIterator<const Type>;


 SingleLinkedList(std::initializer_list<Type> values) {
     for (const auto& el : values){
        PushFront(el);
     }
    Reverse();
    }

     SingleLinkedList(const SingleLinkedList& other) {
      Node* current = other.head_.next_node;
      Node**last_ptr = &head_.next_node;

        while(current){
            *last_ptr = new Node(current->value,nullptr);
            last_ptr = &((*last_ptr)->next_node);
            current = current->next_node;

        }
        size_ = other.size_;

    }
 SingleLinkedList& operator=(const SingleLinkedList& rhs) {
        if (this != &rhs){
            SingleLinkedList temp(rhs);
            swap(temp);
        }
        return *this;
    }
    SingleLinkedList() : head_(), size_(0) {}

    [[nodiscard]] size_t GetSize() const noexcept {
        return size_;
    } 
    ~SingleLinkedList(){
        Clear();
    }
    [[nodiscard]] bool IsEmpty() const noexcept {
       return size_ == 0;
    }
    void PushFront(const Type& value) {
       Node* next = new Node(value,head_.next_node);
       head_.next_node = next;
       ++size_;
    } 
     void swap(SingleLinkedList& other) noexcept {
      std::swap(head_.next_node, other.head_.next_node); 
    std::swap(size_, other.size_);         
    }
    void Reverse(){
        Node* prev = nullptr;
        Node* current = head_.next_node;
        while(current){
            Node* next = current->next_node;
            current->next_node  = prev;
            prev = current;
            current = next;
        }
        head_.next_node = prev;
    }
    void Clear() noexcept {
      Node* current = head_.next_node;
      while(current !=nullptr){
          Node* next = current->next_node;
          delete current;
          current = next;  
      }
      head_.next_node = nullptr;
      size_ = 0;
    }
    [[nodiscard]] Iterator begin() noexcept {
      return Iterator(head_.next_node);
    }

    [[nodiscard]] Iterator end() noexcept {
       return Iterator(nullptr);
    }

    [[nodiscard]] ConstIterator begin() const noexcept {
      return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator end() const noexcept {
         return ConstIterator(nullptr);
    }

    [[nodiscard]] ConstIterator cbegin() const noexcept {
      return ConstIterator(head_.next_node);
    }

    [[nodiscard]] ConstIterator cend() const noexcept {
       return ConstIterator(nullptr);
    }

    [[nodiscard]] Iterator before_begin() noexcept {
    
      return Iterator(&head_);
    }

    [[nodiscard]] ConstIterator cbefore_begin() const noexcept {
   return ConstIterator(const_cast<Node*>(&head_));
    }


    [[nodiscard]] ConstIterator before_begin() const noexcept {
        return ConstIterator(&head_);
    }

    Iterator InsertAfter(ConstIterator pos, const Type& value) {
    assert(pos.node_ != nullptr);
    Node* new_node = new Node(value,pos.node_ ->next_node);
    pos.node_->next_node = new_node;

    ++size_;
    return Iterator(new_node);
    }

    void PopFront() noexcept {
     if (head_.next_node !=nullptr){
        Node* first_node = head_.next_node;
        head_.next_node = first_node->next_node;
        delete first_node;
        --size_;
     }
    }

    Iterator EraseAfter(ConstIterator pos) noexcept {
    assert(pos.node_ != nullptr);

    Node* delete_mode = pos.node_->next_node;

    if (delete_mode != nullptr) {
        pos.node_->next_node = delete_mode->next_node; // Пропускаем удаляемый узел
        delete delete_mode; // Удаляем узел
        --size_; // Уменьшаем размер списка
    }

    return Iterator(pos.node_->next_node);
    }
private:
    Node head_;
    size_t size_ =0;
};
template <typename Type>
void swap(SingleLinkedList<Type>& lhs, SingleLinkedList<Type>& rhs) noexcept {
 lhs.swap(rhs);
}

template <typename Type>
bool operator==(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    if (lhs.GetSize() != rhs.GetSize()) {
        return false;
    }


    auto it1 = lhs.cbegin();
    auto it2 = rhs.cbegin();
    while (it1 != lhs.cend() && it2 != rhs.cend()) {
        if (*it1 != *it2) {
            return false; 
        }
        ++it1;
        ++it2;
    }

    return true;


}

template <typename Type>
bool operator!=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
     return !(lhs == rhs);
}

template <typename Type>
bool operator<(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return lexicographical_compare(lhs.cbegin(),lhs.cend(),
                                    rhs.cbegin(),rhs.cend()
   );
    return true;
}
template <typename Type>
bool operator<=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
    return !(rhs < lhs);
}
template <typename Type>
bool operator>(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
  return rhs < lhs;
}

template <typename Type>
bool operator>=(const SingleLinkedList<Type>& lhs, const SingleLinkedList<Type>& rhs) {
       return !(lhs < rhs);
} 
