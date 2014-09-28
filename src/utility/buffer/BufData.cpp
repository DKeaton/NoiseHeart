#include "BufData.h"

BufData::BufData(quint32 size, quint8 depth)
    : m_size(size), m_depth(depth),
      m_store(new quint8*[m_depth]),
      m_temp(new quint8[m_size]) {
    for (quint8 i = 0; i < m_depth; i++) {
        m_store[i] = new quint8[m_size];
    }
}

BufData::~BufData(void) {
    for (quint8 i = 0; i < m_depth; i++) {
        delete[] m_store[i];
    }
    delete[] m_store;
    delete[] m_temp;
}

void BufData::swapData(void) {
    quint8 *t = m_store[0];
    for (quint8 i = 1; i < m_depth; i++) {
        m_store[i - 1] = m_store[i];
    }
    m_store[m_depth - 1] = t;
}

void BufData::insertCell(quint8 dst) {
    qSwap(m_store[dst], m_temp);
}
