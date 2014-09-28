#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include "CommonException.h"

class OutOfRangeException : public CommonException {
public:
    OutOfRangeException(const QString &message) : CommonException(message) {
    }
};

class NotSupportedException : public CommonException {
public:
    NotSupportedException(const QString &message) : CommonException(message) {
    }
};

#endif // EXCEPTIONS_H
