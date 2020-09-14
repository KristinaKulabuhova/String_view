#ifndef STRING_VIEW_H
#define STRING_VIEW_H

#include <cmath>
#include <cstring>
#include <stdexcept>

class StringView {
    const char* buffer_;
    size_t size_;

public:
    typedef const char* const_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
    const_iterator begin() const {
        return buffer_;
    }

    const_iterator cbegin() const {
        return buffer_;
    }

    const_iterator end() const {
        return buffer_ + size_;
    }

    const_iterator cend() const {
        return buffer_ + size_;
    }

    const_reverse_iterator rend() {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator crend() {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator crbegin() {
        return const_reverse_iterator(end());
    }

    StringView() : buffer_(nullptr), size_(0) {
    }

    StringView(const char* str) : buffer_(str), size_(strlen(str)) {
    }

    StringView(const char* str, size_t str_size) : buffer_(str), size_(str_size) {
    }

    ~StringView() = default;

    size_t Size() const {
        return size_;
    }

    size_t Length() const {
        return size_;
    }

    bool Empty() const {
        return !size_;
    }

    char operator[](size_t idx) const {
        return buffer_[idx];
    }

    char At(size_t idx) const {
        if (idx >= this->size_) {
            throw std::out_of_range("Index out of range");
        } else {
            return buffer_[idx];
        }
    }

    const char* Data() const {
        return buffer_;
    }

    char Front() const {
        return *buffer_;
    }

    char Back() const {
        return *(buffer_ + size_ - 1);
    }

    void Swap(StringView& other) {
        std::swap(size_, other.size_);
        std::swap(buffer_, other.buffer_);
    }

    void RemovePrefix(size_t prefix_size) {
        buffer_ += prefix_size;
        size_ -= prefix_size;
    }

    void RemoveSuffix(size_t suffix_size) {
        size_ -= suffix_size;
    }

    StringView Substr(size_t pos, size_t count = -1) {
        return StringView(buffer_ + pos, std::min(count, size_ - pos));
    }
};

#endif //STRING_VIEW_H

