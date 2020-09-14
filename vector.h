#ifndef VECTOR_H
#define VECTOR_H

#include <cstddef>
#include <iostream>

template <class T>
class Vector {
    T* buffer_;
    size_t size_;
    size_t capacity_;
    const static size_t kIncreaseFactor = 2;

    void Fill(const T& value, size_t begin, size_t end) {
        for (size_t i = begin; i < end; ++i) {
            buffer_[i] = value;
        }
    }

    void Copy(const Vector<T>& other) {
        for (size_t i = 0; i < size_; ++i) {
            buffer_[i] = other.buffer_[i];
        }
    }

    void Reallocate(size_t new_capacity) {
        capacity_ = new_capacity;
        T* new_buffer = new T[capacity_]();
        for (size_t i = 0; i < size_; ++i) {
            new_buffer[i] = buffer_[i];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
    }

    void Estimation() {
        if (capacity_ == 0) {
            capacity_ = 1;
        }
        while (capacity_ <= size_) {
            capacity_ *= kIncreaseFactor;
        }
    }

public:
    Vector():size_(0), capacity_(0), buffer_(nullptr) {
    }

    explicit Vector(size_t number):size_(number), capacity_(number) {
        buffer_ = !number ? nullptr : new T[capacity_]();
    }

    Vector(size_t size, const T& value):Vector(size) {
        Fill(value, 0, size_);
    }

    Vector(const Vector<T>& other):size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ == 0) {
            buffer_ = nullptr;
        } else {
            buffer_ = new T[capacity_]();
            Copy(other);
        }
    }

    Vector& operator=(const Vector<T>& other) {
        if (this == &other) {
            return *this;
        }
        Clear();
        if (other.capacity_ > capacity_ ) {
            Reallocate(other.capacity_);
        }
        size_ = other.size_;
        Copy(other);
        return *this;
    }

    ~Vector() {
        delete[] buffer_;
    }

    void Clear() {
        size_ = 0;
    }

    void PushBack(const T& value) {
        if (capacity_ <= size_) {
            Estimation();
            Reallocate(capacity_);
        }
        buffer_[size_] = value;
        ++size_;
    }

    void PopBack() {
        if (size_ > 0) {
            --size_;
        }
    }

    void Resize(size_t new_size) {
        if (capacity_ < new_size) {
            Reallocate(new_size);
        }
        size_ = new_size;
    }

    const T* Data() const {
        return buffer_;
    }

    T* Data() {
        return buffer_;
    }

    void Resize(size_t new_size, const T& value)  {
        if (capacity_ < new_size) {
            Reallocate(new_size);
            Fill(value, size_, new_size);
        }
        size_ = new_size;
    }

    void Reserve(size_t new_cap) {
        if (new_cap > capacity_) {
            Reallocate(new_cap);
	}
    }

    void ShrinkToFit() {
        if (capacity_ > size_) {
            Reallocate(size_);
	}
    }

    void Swap(Vector &other) {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
    }

    T operator[](size_t idx) const {
        return buffer_[idx];
    }

    T& operator[](size_t idx) {
        return buffer_[idx];
    }

    T& Back() {
        return buffer_[size_ - 1];
    }

    T& Front() {
        return buffer_[0];
    }

    T Back() const {
        return buffer_[size_ - 1];
    }

    T Front() const {
        return buffer_[0];
    }

    bool Empty() const {
        return !size_;
    }

    size_t Size() const {
        return size_;
    }

    size_t Capacity() const {
        return capacity_;
    }

    friend std::istream& operator>>(std::istream& is, Vector<T> &vector) {
        vector.Clear();
        T cur = 0;
        is.get(cur);
        while (!isspace(cur) && cur != '\n' && cur != '\0') {
            vector.PushBack(cur);
            is.get(cur);
        }
        return is;
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector<T> &vector) {
        for (size_t i = 0; i < vector.Size(); ++i) {
            os << vector[i] << " ";
        }
        return os;
    }
};

template <class T>
bool operator<(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    for (size_t i = 0; i < first_vector.Size() && i < second_vector.Size(); ++i) {
        if (first_vector[i] != second_vector[i]) {
            return first_vector[i] < second_vector[i];
        }
    }
    return first_vector.Size() < second_vector.Size();
}

template <class T>
bool operator>(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    return second_vector < first_vector;
}

template <class T>
bool operator==(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    return !(second_vector < first_vector) && !(first_vector < second_vector);
}

template <class T>
bool operator!=(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    return !(first_vector == second_vector);
}

template <class T>
bool operator<=(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    return !(first_vector > second_vector);
}

template <class T>
bool operator>=(const Vector<T> &first_vector, const Vector<T> &second_vector) {
    return !(first_vector < second_vector);
}

#endif// VECTOR_H

