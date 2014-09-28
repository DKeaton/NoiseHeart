#ifndef COMMONEXCEPTION_H
#define COMMONEXCEPTION_H

#include <QString>

class CommonException {
public:
    CommonException(const QString& message);
    const QString getMessage(void) const;

private:
    const QString m_message;
};

#endif // COMMONEXCEPTION_H
