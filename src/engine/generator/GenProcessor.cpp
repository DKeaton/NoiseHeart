#include "GenProcessor.h"

#define Pi 3.141592

namespace Noise {

    float GenProcessor::m_steps[32];

    GenProcessor::GenProcessor(quint8 id, quint32 size, QSemaphore *semaphores)
        : m_semaphores(semaphores),
          m_iter(0),
          m_size(size), m_id(id),
          m_acquiring(-1), m_releasing(-1) {
    }

    void GenProcessor::startLoop(CommandPtr task) {
        while (task->action != Command::toStop) {
            switch (task->action) {
            case Command::toSmooth:
                procSmooth(static_cast<quint8*>(task->dst), static_cast<const quint8**>(task->src), task->index, task->octave);
                break;
            case Command::toInterpolate:
                procInterpolate(static_cast<quint8*>(task->dst), static_cast<const quint8**>(task->src), task->index, task->octave);
                break;
            case Command::toFinalSumm:
                procFinalSumm(static_cast<quint8*>(task->dst), static_cast<const quint8**>(task->src), static_cast<const Settings*>(task->optional));
                break;
            default:;
            }
            emit requestNextTask(m_id, task);
            m_iter = 0;
        }
    }

    void GenProcessor::setAcquireState(quint8 sem, bool on) {
        m_mutex.lock();
        m_acquiring = (on) ? sem : -1;
        m_mutex.unlock();
    }

    void GenProcessor::setReleaseState(quint8 sem, bool on) {
        m_mutex.lock();
        if (on) {
            m_semaphores[m_releasing = sem].release(m_iter);
        } else {
            m_semaphores[sem].acquire(m_semaphores[sem].available());
            m_releasing = -1;
        }
        m_mutex.unlock();
    }

    void GenProcessor::registerMetaTypes(void) {
        qRegisterMetaType<CommandPtr>("CommandPtr");
        qRegisterMetaType<InitTask>("InitTask");
    }

    void GenProcessor::calcSteps(void) {
        for (quint8 i = 0; i < 32; i++) {
            m_steps[i] = i / 32.0f;
        }
    }

    void GenProcessor::procSmooth(quint8 *dst, const quint8 **src, quint8 index, quint8 octave) {
        quint8 t = 1 << octave;
        const quint8 y[3] = {ny(index - t), ny(index), ny(index + t)};
        quint32 count = m_size >> octave;
        quint32 x[3] = {m_size - t, 0, t};
        dst[0] = smooth(src, x, y);
        x[0] = x[1]; x[1] = x[2]; x[2] += t;
        for (quint32 i = 0; i < count; i++) {
            m_mutex.lock();
            dst[x[1]] = smooth(src, x, y);
            for (quint8 k = 0; k < t; k++) {
                if (octave != 0 && k != 0) {
                    dst[x[0] + k] = interpolate(dst[x[0]], dst[x[1]], m_steps[k << (5 - octave)]);
                }
                if (m_releasing != -1) {
                    m_semaphores[m_releasing].release();
                }
                m_iter++;
            }
            x[0] = x[1]; x[1] = x[2];
            if (i != count - 3) {
                x[2] += t;
            } else {
                x[2] = 0;
            }
            m_mutex.unlock();
        }
    }

    void GenProcessor::procInterpolate(quint8 *dst, const quint8 **src, quint8 index, quint8 octave) {
        const float &val = m_steps[(index % (1 << octave)) << (5 - octave)];
        for (quint32 i = 0; i < m_size; i++) {
            m_mutex.lock();
            if (m_acquiring != -1) {
                m_semaphores[m_acquiring].acquire();
            }
            dst[i] = (octave != 0 && val != 0.0f) ? interpolate(src[1][i], src[0][i], val) : src[1][i];
            if (m_releasing != -1) {
                m_semaphores[m_releasing].release();
            }
            m_iter++;
            m_mutex.unlock();
        }
    }

    void GenProcessor::procFinalSumm(quint8 *dst, const quint8 **src, const Settings *sets) {
        float dAmp = 0.0f;
        quint8 maxAmp = 0;
        for (quint8 i = 0; i < 6; i++) {
            if (sets->octaves[i]) {
                dAmp += sets->amplitudes[i];
                if (sets->amplitudes[i] > maxAmp) {
                    maxAmp = sets->amplitudes[i];
                }
            }
        }
        if (dAmp) {
            dAmp = maxAmp / dAmp;
        }
        for (; m_iter < m_size; m_iter++) {
            m_mutex.lock();
            if (m_acquiring != -1) {
                m_semaphores[m_acquiring].acquire();
            }
            quint16 temp = 0;
            for (quint8 i = 0; i < 6; i++) {
                if (sets->octaves[i]) {
                    temp += src[i][m_iter] * sets->amplitudes[i] / 100.0f;
                }
            }
            dst[m_iter] = temp * dAmp;
            m_mutex.unlock();
        }
    }

    quint8 GenProcessor::smooth(const quint8 **src, const quint32 *x, const quint8 *y) const {
        quint8 center = src[y[1]][x[1]] / 4;
        quint8 sides = (src[y[0]][x[1]] + src[y[1]][x[2]] + src[y[2]][x[1]] + src[y[1]][x[0]]) / 8;
        quint8 corners = (src[y[0]][x[2]] + src[y[2]][x[2]] + src[y[2]][x[0]] + src[y[0]][x[0]]) / 16;
        return center + sides + corners;
    }

    quint8 GenProcessor::interpolate(quint8 v0, quint8 v1, float step) {
        float f = (1.0f - qCos(Pi * step)) / 2.0f;
        return v0 * (1.0f - f) + v1 * f;
    }

    quint8 GenProcessor::ny(quint8 y) const {
        if (y >= 32 && y <= 64) {
            return y - 26;
        } else if (y < 32) {
            return osy(y);
        }
        return 44 - osy(96 - y);
    }

    quint8 GenProcessor::osy(quint8 y) const {
        switch (y) {
        case 31: return 5;
        case 30: return 4;
        case 28: return 3;
        case 24: return 2;
        case 16: return 1;
        case 0: return 0;
        }
        return 255;
    }

}

#undef Pi
