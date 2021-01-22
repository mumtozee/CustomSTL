#ifndef STRING_STRING_HPP
#define STRING_STRING_HPP

#include <cctype>

#include <iostream>

class String {
public:
    String();
    String(const char* str);
    String(const char* str, size_t size);
    String(size_t size, char symbol);

    String(const String& other);
    String& operator=(String other);
    ~String();

    size_t Size() const;
    size_t Length() const;
    // without null terminator
    size_t Capacity() const;
    void Resize(size_t new_size, char fill = char());
    bool Empty() const;
    void Clear();
    void Reserve(size_t new_capacity = 0);
    void ShrinkToFit();
    void Swap(String& other);

    char& Back();
    char& Front();

    char& operator[](size_t idx);
    char operator[](size_t idx) const;

    const char& Back() const;
    const char& Front() const;

    const char* CStr() const;
    const char* Data() const;

    String& operator+=(const String& other);
    String& operator+=(const char* str);
    String& operator+=(char symbol);

    void PushBack(char symbol);
    void PopBack();

private:
    char* buffer_;
    size_t size_;
    size_t capacity_;

    const static size_t kIncreaseFactor = 2;

    void Reallocate(size_t new_cap);
};

size_t CStrLen(const char* str);

String operator+(String lhs, const String& rhs);
String operator+(String lhs, const char* rhs);
String operator+(char lhs, const String& rhs);
String operator+(String lhs, const char& rhs);

bool operator==(const String& lhs, const String& rhs);
bool operator>=(const String& lhs, const String& rhs);
bool operator<=(const String& lhs, const String& rhs);
bool operator!=(const String& lhs, const String& rhs);
bool operator>(const String& lhs, const String& rhs);
bool operator<(const String& lhs, const String& rhs);

std::ostream& operator<<(std::ostream& os, const String& out_str);
std::istream& operator>>(std::istream& is, String& input_str);

#endif //STRING_STRING_HPP
