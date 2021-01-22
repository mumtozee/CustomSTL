#include "String.hpp"

#include <iostream>

size_t CStrLen(const char* input_str) {
    size_t length = 0;
    while (input_str[length] != '\0') {
        ++length;
    }

    return length;
}

static void BufferCopy(const char* from, char* to, size_t amount) {
    for (size_t i = 0; i < amount; ++i) {
        to[i] = from[i];
    }

    to[amount] = '\0';
}

void FillWith(char symbol, const size_t size, char* c_string) {
    for (size_t i = 0; i < size; ++i) {
        c_string[i] = symbol;
    }
    c_string[size] = '\0';
}

void String::Reallocate(size_t new_cap) {
    char* newBuffer = new char[new_cap + 1];

    BufferCopy(buffer_, newBuffer, size_);

    delete[] buffer_;
    buffer_ = newBuffer;
    capacity_ = new_cap;
}

void String::PushBack(char symbol) {
    if (size_ >= capacity_ && capacity_ != 0) {
        Reallocate(kIncreaseFactor * capacity_);
    } else if (capacity_ == 0) {
        Reallocate(1);
    }

    buffer_[size_] = symbol;
    buffer_[size_ + 1] = '\0';
    ++size_;
}

void String::PopBack() {
    if (!Empty()) {
        --size_;
        buffer_[size_] = '\0';
    }
}

size_t String::Size() const {
    return size_;
}

size_t String::Length() const {
    return size_;
}

size_t String::Capacity() const {
    return capacity_;
}

char& String::Back() {
    return buffer_[size_ - 1];
}

const char& String::Back() const {
    return buffer_[size_ - 1];
}

char& String::Front() {
    return buffer_[0];
}

const char& String::Front() const {
    return buffer_[0];
}

void String::Resize(size_t new_size, char fill) {
    if (new_size > size_ && new_size > capacity_) {
        Reallocate(new_size);
        FillWith(fill, new_size - size_, buffer_ + size_);
    } else if(new_size > size_ && new_size <= capacity_) {
        FillWith(fill, new_size - size_, buffer_ + size_);
    }

    size_ = new_size;
    buffer_[size_] = '\0';
}

void String::ShrinkToFit() {
    if (capacity_ > size_) {
        Reallocate(size_);
    }
}

void String::Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        Reallocate(new_capacity);
    }
}

bool String::Empty() const {
    return size_ == 0;
}

const char* String::CStr() const {
    return buffer_;
}

const char* String::Data() const {
    return buffer_;
}

void String::Clear() {
    buffer_[0] = '\0';
    size_ = 0;
}

static int StringCompare(const String& first, const String& second) {
    for (size_t i = 0; i < first.Size() && i < second.Size(); ++i) {
        if (first[i] != second[i]) {
            return (static_cast<int>(first[i]) - static_cast<int>(second[i]));
        }
    }

    return static_cast<int>(first.Length()) - static_cast<int>(second.Length());
}

void String::Swap(String &other) {
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(buffer_, other.buffer_);
}

String::String() : size_(0), capacity_(0) {
    buffer_ = new char[capacity_ + 1];
    buffer_[capacity_] = '\0';
}

String::String(const char* str) : String(str, CStrLen(str)) {
}

String::String(const char* str, size_t size) : size_(size), capacity_(CStrLen(str)) {
    buffer_ = new char[capacity_ + 1];
    BufferCopy(str, buffer_, size);
}

String::String(size_t size, char symbol) : size_(size), capacity_(size) {
    buffer_ = new char[capacity_ + 1];
    FillWith(symbol, size_, buffer_);
}

String::String(const String& other) : size_(other.size_), capacity_(other.capacity_) {
    buffer_ = new char[capacity_ + 1];
    BufferCopy(other.buffer_, buffer_, size_);
}

String& String::operator=(String other) {
    Swap(other);
    return *this;
}

String& String::operator+=(const String& other) {
    if (capacity_ <= size_ + other.size_) {
        Reallocate(capacity_ + other.size_);
    }

    BufferCopy(other.buffer_, buffer_ + size_, other.size_);
    size_ += other.size_;

    return *this;
}

String& String::operator+=(char symbol) {
    PushBack(symbol);
    return *this;
}

String& String::operator+=(const char* str) {
    const size_t size = CStrLen(str);

    if (capacity_ <= size_ + size) {
        Reallocate(capacity_ + size);
    }

    BufferCopy(str, buffer_ + size_, size);
    size_ += size;

    return *this;
}

char& String::operator[](size_t idx) {
    return buffer_[idx];
}

char String::operator[](size_t idx) const {
    return buffer_[idx];
}

String::~String() {
    delete[] buffer_;
}

String operator+(String lhs, const String& rhs) {
    return lhs += rhs;
}

String operator+(String lhs, const char& rhs) {
    return lhs += rhs;
}

String operator+(String lhs, const char* rhs) {
    return lhs += rhs;
}

String operator+(char lhs, const String& rhs) {
    String tmp_str(1, lhs);
    return tmp_str += rhs;
}

bool operator==(const String& lhs, const String& rhs) {
    return StringCompare(lhs, rhs) == 0;
}

bool operator<(const String& lhs, const String& rhs) {
    return StringCompare(lhs, rhs) < 0;
}

bool operator>(const String& lhs, const String& rhs) {
    return StringCompare(lhs, rhs) > 0;
}

bool operator>=(const String& lhs, const String& rhs) {
    return !(lhs < rhs);
}

bool operator<=(const String& lhs, const String& rhs) {
    return !(lhs > rhs);
}

bool operator!=(const String& lhs, const String& rhs) {
    return !(lhs == rhs);
}

std::ostream& operator<<(std::ostream& os, const String& out_str) {
    os << out_str.CStr();
    return os;
}

std::istream& operator>>(std::istream& is, String& input_str) {
    char symb = getchar();

    while (!isspace(symb) && symb != '\n') {
        input_str.PushBack(symb);
        symb = getchar();
    }

    return is;
}