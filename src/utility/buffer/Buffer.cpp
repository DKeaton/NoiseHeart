#include "Buffer.h"

Buffer::Buffer(quint32 size, quint8 depth, QObject *parent)
    : QObject(parent), BufData(size, depth),
      m_curDepth(0) {
    memset(&m_states, 0, sizeof(BufferStates));
    m_exMutex.lock();
}

void Buffer::getCell(quint8 *dst) {
    m_exMutex.lock();
    m_inMutex.lock();
    memcpy(dst, m_store[0], m_size);
    m_exMutex.unlock();
    swapCells();
    m_inMutex.unlock();
}

void Buffer::startBuffer(void) {
    m_states.isGenerating = true;
    emit generatingRequested(m_temp);
}

void Buffer::generatingCompleted(void) {
    m_inMutex.lock();
    m_states.isGenerating = false;
    m_states.lineReady = true;
    processBuffer();
    m_inMutex.unlock();
}

void Buffer::swapCells(void) {
    if (--m_curDepth != 0) {
        m_states.swappingNeeded = true;
    } else {
        m_exMutex.lock();
    }
    processBuffer();
}


void Buffer::processBuffer(void) {
    if (m_states.swappingNeeded) {
        swapData();
        m_states.swappingNeeded = false;
    }
    if (m_states.lineReady) {
        insertCell(m_curDepth);
        if (++m_curDepth == 1) {
            m_exMutex.unlock();
        }
        m_states.lineReady = false;
    }
    if (m_curDepth < m_depth && !m_states.isGenerating) {
        emit generatingRequested(m_temp);
        m_states.isGenerating = true;
    }
}
