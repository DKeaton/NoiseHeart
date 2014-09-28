#ifndef BUFDATA_H
#define BUFDATA_H

#include <QtGlobal>

class BufData {
public:
    BufData(quint32 size, quint8 depth);
    ~BufData(void);

protected:
    void insertCell(quint8 dst);
    void swapData(void);

    const quint32 m_size;
    const quint8 m_depth;
    quint8 **m_store;
    quint8 *m_temp;
};

#endif // BUFDATA_H
