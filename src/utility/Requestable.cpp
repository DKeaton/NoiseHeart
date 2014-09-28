#include "Requestable.h"

void Requestable::requestIn(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &arg2, const QVariant &arg3) {
    processRequest(request, arg0, arg1, arg2, arg3);
}

void Requestable::processRequest(quint8, const QVariant &, const QVariant &, const QVariant &, const QVariant &) {
    throw new NotSupportedException("Processing incoming requests is not supported for class.");
}
