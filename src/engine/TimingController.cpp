#include "TimingController.h"

TimingController::TimingController(QObject *parent)
    : QObject(parent) {
    m_delaySet = false;
    m_count = 0;
    memset(m_late, 0, 32 * sizeof(quint16));
}

void TimingController::setDelay(quint16 delay) {
    m_delay = m_factDelay = delay;
    m_count = 0;
    if (!m_delaySet) {
        m_delaySet = true;
        m_time.start();
        m_timer.singleShot(m_delay, this, SIGNAL(updateFrame()));
    }
}

void TimingController::frameUpdated(void) {
    quint16 late = 0;
    if (m_count != 32) {
        m_count++;
    }
    for (quint8 i = m_count - 1; i > 0; i--) {
        m_late[i] = m_late[i - 1];
        late += m_late[i];
    }
    m_late[0] = (m_delay < m_time.elapsed()) ? m_time.elapsed() - m_delay : 0;
    late = (late + m_late[0]) / m_count;
    if (late > m_delay / 4) {
        if (m_factDelay != 1) {
            m_factDelay--;
        }
    } else if (m_time.elapsed() < m_delay * 0.75f && m_factDelay < m_delay) {
        m_factDelay++;
    }
    m_time.start();
    m_timer.singleShot(m_factDelay, this, SIGNAL(updateFrame()));
}
