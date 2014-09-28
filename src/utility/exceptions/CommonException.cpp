#include "CommonException.h"

CommonException::CommonException(const QString& message) : m_message(message) {
}

const QString CommonException::getMessage(void) const {
    return m_message;
}
