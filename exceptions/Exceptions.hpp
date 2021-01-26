#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <string>

class Exception {
public:
    Exception() noexcept = default;
    Exception(const Exception& other) noexcept = default;
    Exception& operator=(const Exception& other) noexcept = default;
    virtual ~Exception() noexcept = default;
    virtual const char* What() const noexcept;
};

class BadAlloc : public Exception {
public:
    const char* What() const noexcept override;
};

class LogicError : public Exception {
public:
    LogicError() = default;
    LogicError(const LogicError& other) = default;
    LogicError& operator=(const LogicError& other) = default;
    explicit LogicError(const char* what_args);
    explicit LogicError(const std::string& what_args);
    ~LogicError() override = default;

    const char* What() const noexcept override;

protected:
    std::string message_;
};

class InvalidArgument : public LogicError {
public:
    InvalidArgument() = default;
    InvalidArgument(const InvalidArgument& other) = default;
    InvalidArgument& operator=(const InvalidArgument& other) = default;
    explicit InvalidArgument(const char* what_args);
    explicit InvalidArgument(const std::string& what_args);
    ~InvalidArgument() override = default;
};

class DomainError : public LogicError {
public:
    DomainError() = default;
    DomainError(const DomainError& other) = default;
    DomainError& operator=(const DomainError& other) = default;
    explicit DomainError(const char* what_args);
    explicit DomainError(const std::string& what_args);
    ~DomainError() override = default;
};

class OutOfRange : public LogicError {
public:
    OutOfRange() = default;
    OutOfRange(const OutOfRange& other) = default;
    OutOfRange& operator=(const OutOfRange& other) = default;
    explicit OutOfRange(const char* what_args);
    explicit OutOfRange(const std::string& what_args);
    ~OutOfRange() override = default;
};

class LengthError : public LogicError {
public:
    LengthError() = default;
    LengthError(const LengthError& other) = default;
    LengthError& operator=(const LengthError& other) = default;
    explicit LengthError(const char* what_args);
    explicit LengthError(const std::string& what_args);
    ~LengthError() override = default;
};

class RuntimeError : public Exception {
public:
    RuntimeError() = default;
    RuntimeError(const RuntimeError& other) = default;
    RuntimeError& operator=(const RuntimeError& other) = default;
    explicit RuntimeError(const char* what_args);
    explicit RuntimeError(const std::string& what_args);
    ~RuntimeError() override = default;

    const char* What() const noexcept override;

protected:
    std::string message_;
};

class RangeError : public RuntimeError {
public:
    RangeError() = default;
    RangeError(const RangeError& other) = default;
    RangeError& operator=(const RangeError& other) = default;
    explicit RangeError(const char* what_args);
    explicit RangeError(const std::string& what_args);
    ~RangeError() override = default;
};

class OverflowError : public RuntimeError {
public:
    OverflowError() = default;
    OverflowError(const OverflowError& other) = default;
    OverflowError& operator=(const OverflowError& other) = default;
    explicit OverflowError(const char* what_args);
    explicit OverflowError(const std::string& what_args);
    ~OverflowError() override = default;
};

class UnderflowError : public RuntimeError {
public:
    UnderflowError() = default;
    UnderflowError(const UnderflowError& other) = default;
    UnderflowError& operator=(const UnderflowError& other) = default;
    explicit UnderflowError(const char* what_args);
    explicit UnderflowError(const std::string& what_args);
    ~UnderflowError() override = default;
};



#endif //EXCEPTIONS_HPP
