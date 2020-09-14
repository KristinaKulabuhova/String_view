#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <iostream>

template <class T>
class UniquePtr {
    T* pointer_;

public:
    UniquePtr() : pointer_(nullptr) {
    };

    UniquePtr(T* pointer) : pointer_(pointer) {
    };

    ~UniquePtr() {
        delete pointer_;
    }

    UniquePtr(const UniquePtr<T>& point) = delete;
    UniquePtr& operator=(const UniquePtr<T>& point) = delete;

    UniquePtr(UniquePtr&& other) : pointer_(other.pointer_) {
        other.pointer_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr<T>&& other) {
        if (this == &other) {
            return *this;
        }
        delete pointer_;
        pointer_ = other.pointer_;
        other.pointer_ = nullptr;
        return *this;
    }

    T* Release() {
        T* old_pointer = pointer_;
        pointer_ = nullptr;
        return old_pointer;
    }
    void Reset(T* pointer = nullptr) {
        delete pointer_;
        pointer_ = pointer;
    }
    void Swap(UniquePtr<T>& other) {
        std::swap(other.pointer_, this->pointer_);
    }

    T* Get() const {
        return pointer_;
    }

    T& operator*() const {
        return *pointer_;
    }

    T* operator->() const {
        return pointer_;
    }

    explicit operator bool() const {
        return pointer_ != nullptr;
    }
};

#endif //UNIQUE_PTR_H

