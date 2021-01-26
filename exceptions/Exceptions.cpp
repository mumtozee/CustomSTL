#include "Exceptions.hpp"

const char* Exception::What() const noexcept {
    return "some error occurred";
}

const char* BadAlloc::What() const noexcept {
    return "bad allocation";
}

LogicError::LogicError(const char* what_args) : message_(what_args) {
}

LogicError::LogicError(const std::string& what_args) : message_(what_args) {
}

const char* LogicError::What() const noexcept {
    return message_.c_str();
}

RuntimeError::RuntimeError(const char* what_args) : message_(what_args) {
}

RuntimeError::RuntimeError(const std::string& what_args) : message_(what_args) {
}

const char* RuntimeError::What() const noexcept {
    return message_.c_str();
}

InvalidArgument::InvalidArgument(const char* what_args) : LogicError(what_args) {
}

InvalidArgument::InvalidArgument(const std::string& what_args) : LogicError(what_args) {
}

DomainError::DomainError(const char* what_args) : LogicError(what_args) {
}

DomainError::DomainError(const std::string& what_args) : LogicError(what_args) {
}

LengthError::LengthError(const char* what_args) : LogicError(what_args) {
}

LengthError::LengthError(const std::string& what_args) : LogicError(what_args) {
}

OutOfRange::OutOfRange(const char* what_args) : LogicError(what_args) {
}

OutOfRange::OutOfRange(const std::string& what_args) : LogicError(what_args) {
}

RangeError::RangeError(const char* what_args) : RuntimeError(what_args) {
}

RangeError::RangeError(const std::string& what_args) : RuntimeError(what_args) {
}

OverflowError::OverflowError(const char* what_args) : RuntimeError(what_args) {
}

OverflowError::OverflowError(const std::string& what_args) : RuntimeError(what_args) {
}

UnderflowError::UnderflowError(const char* what_args) : RuntimeError(what_args) {
}

UnderflowError::UnderflowError(const std::string& what_args) : RuntimeError(what_args) {
}