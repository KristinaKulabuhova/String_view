#ifndef SHARED_PTR_H
#define SHARED_PTR_H

#include <stdexcept>
#include <string>

class BadWeakPtr : public std::exception {
    std::string error_;

public:
    BadWeakPtr(std::string error) : error_(std::move(error)) {
    }

    const char* what() const noexcept override {
        return error_.c_str();
    }
};

template <class T>
class WeakPtr;

struct Counters {
    size_t strong_cnt_ = 0;
    size_t weak_cnt_ = 0;

    Counters(size_t strong, size_t weak) : strong_cnt_(strong), weak_cnt_(weak) {
    }
};

template <class T>
class SharedPtr {
    T* ptr_ = nullptr;
    Counters* counters = nullptr;

    void DeleteStrong() {
        if (counters != nullptr) {
            --counters->strong_cnt_;
            if (counters->strong_cnt_ == 0) {
                delete ptr_;
                if (counters->weak_cnt_ == 0) {
                    delete counters;
                }
            }
        }
    }

public:
    SharedPtr() = default;

    SharedPtr(T* ptr) : ptr_(ptr), counters(nullptr) {
        if (ptr != nullptr) {
            counters = new Counters{1, 0};
        }
    }

    SharedPtr(const WeakPtr<T>& other) : ptr_(other.ptr_), counters(other.counters) {
        if (other.Expired()) {
            throw BadWeakPtr("Bad weak ptr");
        }
        if (ptr_ != nullptr) {
            ++counters->strong_cnt_;
        }
    }

    SharedPtr(const SharedPtr<T>& other) : ptr_(other.ptr_), counters(other.counters) {
        if (ptr_ != nullptr) {
            ++counters->strong_cnt_;
        }
    }


    SharedPtr(SharedPtr<T>&& other) : ptr_(other.ptr_), counters(other.counters) {
        other.ptr_ = nullptr;
        other.counters = nullptr;
    }

    void Reset(T* ptr = nullptr) {
        SharedPtr object(std::move(ptr));
        this->Swap(object);
    }

    void Swap(SharedPtr<T>& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counters, other.counters);
    }

    T* Get() {
        return this->ptr_;
    }

    size_t UseCount() {
        return counters == nullptr ? 0 : counters->strong_cnt_;
    }

    SharedPtr& operator=(const SharedPtr<T>& other) {
        if (this == &other) {
            return *this;
        }
        SharedPtr object(other);
        this->Swap(object);
        return *this;
    }

    SharedPtr<T>& operator=(SharedPtr<T>&& other) {
        SharedPtr object(std::move(other));
        this->Swap(object);
        return *this;
    }

    T& operator*() const {
        return *(this->ptr_);
    }

    T* operator->() const {
        return ptr_;
    }

    explicit operator bool() const {
        return ptr_ != nullptr;
    }

    ~SharedPtr() {
        if (counters == nullptr) {
            return;
        }
        DeleteStrong();
    }

    friend class WeakPtr<T>;
};

template <class T>
class WeakPtr {
    Counters* counters = nullptr;
    T* ptr_ = nullptr;

    void DeleteWeak() {
        if (counters != nullptr) {
            --counters->weak_cnt_;
            if (counters->strong_cnt_ + counters->weak_cnt_ == 0) {
                delete counters;
            }
        }
    }

public:
    WeakPtr() = default;

    WeakPtr(const SharedPtr<T>& pointer) : counters(pointer.counters), ptr_(pointer.ptr_) {
        if (counters != nullptr) {
            ++counters->weak_cnt_;
        }
    }

    WeakPtr(SharedPtr<T>&& other) : counters(std::move(other.counters)), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
        other.counters = nullptr;
    }

    WeakPtr(const WeakPtr<T>& other) : counters(other.counters), ptr_(other.ptr_) {
        if (counters != nullptr) {
            ++counters->weak_cnt_;
        }
    }

    WeakPtr& operator=(const WeakPtr<T>& other) {
        if (this == &other) {
            return *this;
        }
        WeakPtr<T> object(other);
        this->Swap(object);
        return *this;
    }

    WeakPtr& operator=(WeakPtr<T>&& other) {
        WeakPtr<T> object(other);
        this->Swap(object);
        return *this;
    }

    void Swap(WeakPtr<T>& other) {
        std::swap(ptr_, other.ptr_);
        std::swap(counters, other.counters);
    }

    void Reset() {
        WeakPtr<T> object;
        this->Swap(object);
    }

    size_t UseCount() const {
        return counters == nullptr? 0 : counters->strong_cnt_;
    }

    bool Expired() const {
        return !this->UseCount();
    }

    SharedPtr<T> Lock() const {
        return this->Expired() ? nullptr : SharedPtr<T>(*this);
    }

    ~WeakPtr() {
        if (counters == nullptr) {
            return;
        }
        DeleteWeak();
    }

    friend class SharedPtr<T>;
};

#endif //SHARED_PTR_H

