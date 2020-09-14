#ifndef ANY_H
#define ANY_H

#include <iostream>
#include <memory>
#include <string>
#include <utility>

class BadAnyCast: public std::exception {
    std::string error_;

public:
    BadAnyCast(std::string error): error_(std::move(error)) {
    }

    const char* what() const noexcept {
        return error_.c_str();
    }
};

class Base {
public:
    virtual ~Base() = default;
    virtual std::unique_ptr<Base> Clone() const = 0;
};

template <class T>
class Derived: public Base {
    T value_;

public:
    Derived(T value): value_(std::move(value)) {
    }

    std::unique_ptr<Base> Clone () const override {
        std::unique_ptr<Derived<T>> object = std::make_unique<Derived<T>>(value_);
        return object;
    }

    T GetValue() const {
        return value_;
    }
};

class Any {
    std::unique_ptr<Base> ptr_;

public:
    Any() =default;

    ~Any() = default;

    template <class T>
    Any(T value) : ptr_(std::make_unique<Derived<T>>(std::move(value))) {
    }

    Any(const Any& other): ptr_(other.ptr_->Clone()) {
    }

    Any(Any&& other) = default;

    template <class U>
    Any& operator=(const U& value) noexcept {
        this->Reset();
        ptr_ = std::make_unique<Derived<U>>(value);
        return *this;
    }

    Any& operator=(const Any& other) noexcept {
        if (this == &other) {
            return *this;
        }
        ptr_ = other.ptr_->Clone();
        return *this;
    }

    Any& operator=(Any &&other) noexcept = default;

    bool HasValue() const noexcept {
        return this->ptr_ != nullptr;
    }

    void Reset() noexcept {
        ptr_.reset();
    }

    void Swap(Any& other) noexcept {
        std::swap(this->ptr_, other.ptr_);
    }

    template <class T>
    friend T any_cast(const Any& value);
};

template <class T>
T any_cast(const Any& value) {
    const auto* tmp = dynamic_cast<const Derived<T>*>(value.ptr_.get());
    if (!tmp) {
        throw BadAnyCast("Bad any cast");
    } else {
        return tmp->GetValue();
    }
}

#endif // ANY_H

