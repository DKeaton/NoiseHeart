#ifndef REQUESTABLE_H
#define REQUESTABLE_H

#include <QVariant>

#include "exceptions/exceptions.h"

class Requestable {
public:
    void requestIn(quint8 request, const QVariant &arg0 = QVariant(), const QVariant &arg1 = QVariant(),
                   const QVariant &arg2 = QVariant(), const QVariant &arg3 = QVariant());

private:
    virtual void processRequest(quint8, const QVariant&, const QVariant&,
                                const QVariant&, const QVariant&);
};

#endif // REQUESTABLE_H
