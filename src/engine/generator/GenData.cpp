#include "GenData.h"

namespace Noise {

    GenData::GenData(quint32 size) : m_size(size) {
        quint8 i;
        quint32 j;
        rands = new quint8*[45];
        for (i = 0; i < 45; i++) {
            rands[i] = new quint8[m_size];
            generateRandLine(rands[i]);
        }
        smoothed = new quint8**[6];
        octDst = new quint8 *[6];
        for (i = 0; i < 6; i++) {
            smoothed[i] = new quint8*[2];
            octDst[i] = new quint8[m_size];
            for (j = 0; j < 2; j++) {
                smoothed[i][j] = new quint8[m_size];
            }
        }
    }

    GenData::~GenData(void) {
        quint8 i, j;
        for (i = 0; i < 45; i++) {
            delete[] rands[i];
        }
        delete[] rands;
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 2; j++) {
                delete[] smoothed[i][j];
            }
            delete[] smoothed[i];
            delete[] octDst[i];
        }
        delete[] smoothed;
        delete[] octDst;
    }

    void GenData::prepareBuffer(void) {
        quint8 i, j;
        for (i = 0, j = 6; i <= 8; i++, j += power(quint8(2), 5 - i)) {
            qSwap(rands[i], rands[j]);
        }
        for (i = 41, j = 10; i <= 44; i++, j += power(quint8(2), i - 40)) {
            qSwap(rands[j], rands[i]);
        }
        generateRandLine(rands[9]);
        for (i = 11; i <= 13; i++) {
            generateRandLine(rands[i]);
        }
        for (i = 15; i <= 21; i++) {
            generateRandLine(rands[i]);
        }
        for (i = 23; i <= 37; i++) {
            generateRandLine(rands[i]);
        }
        for (i = 39; i <= 44; i++) {
            generateRandLine(rands[i]);
        }
    }

    void GenData::swapCells(quint8 oct) {
        qSwap(smoothed[oct][0], smoothed[oct][1]);
    }

    void GenData::generateRandLine(quint8 *_dst) {
        for (quint32 i = 0; i < m_size; i++) {
            _dst[i] = rand() % 255;
        }
    }

    template<class T> T GenData::power(const T &x, qint8 p) {
        T result = static_cast<T>(1.0f);
        for (quint8 i = 1; i <= p; i++) {
            result *= x;
        }
        return result;
    }

}
