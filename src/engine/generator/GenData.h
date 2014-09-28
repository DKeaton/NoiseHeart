#ifndef GENDATA_H
#define GENDATA_H

#include <QtGlobal>
#include <QDebug>

namespace Noise {

    class GenData {
    public:
        GenData(quint32 size);
        ~GenData(void);

    protected:
        void prepareBuffer(void);
        void swapCells(quint8 oct);

        quint8 **rands;
        quint8 ***smoothed;
        quint8 **octDst;
        quint8 *dst;

        const quint32 m_size;

    private:
        void generateRandLine(quint8 *_dst);
        template<class T> T power(const T &x, qint8 p);
    };

}

#endif // GENDATA_H
