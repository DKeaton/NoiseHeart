#ifndef GENPROCESSOR_H
#define GENPROCESSOR_H

#include <QObject>
#include <QMetaType>
#include <QSemaphore>
#include <QMutex>
#include <QPair>
#include <qmath.h>

#include <QDebug>

#include "NoiseSettings.h"

namespace Noise {

    struct Command {
        enum Action {
            toStop = -1,
            toSmooth = 0,
            toInterpolate = 1,
            toFinalSumm = 2
        };

        Command(void) {
        }

        Command(Action _action, quint8 _index, quint8 _octave) {
            action = _action;
            index = _index;
            octave = _octave;
        }

        Action action;
        void *src;
        void *dst;
        quint8 index;
        quint8 octave;
        const Settings *optional;
    };

    typedef Command *CommandPtr;
    typedef QPair<quint8, CommandPtr> InitTask;

    class GenProcessor : public QObject {
        Q_OBJECT
    public:
        GenProcessor(quint8 id, quint32 size, QSemaphore *semaphores);
        void setAcquireState(quint8 sem, bool on);
        void setReleaseState(quint8 sem, bool on);

        static void registerMetaTypes(void);
        static void calcSteps(void);

    public slots:
        void startLoop(CommandPtr task);

    private:
        void procSmooth(quint8 *dst, const quint8 **src, quint8 index, quint8 octave);
        void procInterpolate(quint8 *dst, const quint8 **src, quint8 index, quint8 octave);
        void procFinalSumm(quint8 *dst, const quint8 **src, const Settings *sets);
        quint8 smooth(const quint8 **src, const quint32 *x, const quint8 *y) const;
        quint8 interpolate(quint8 v0, quint8 v1, float step);
        quint8 ny(quint8 y) const;
        quint8 osy(quint8 y) const;

        static float m_steps[32];

        QSemaphore *m_semaphores;
        QMutex m_mutex;
        quint32 m_iter;
        const quint32 m_size;
        const quint8 m_id;
        qint8 m_acquiring;
        qint8 m_releasing;

    signals:
        void requestNextTask(quint8 id, CommandPtr task);
    };

}

Q_DECLARE_METATYPE(Noise::CommandPtr)
Q_DECLARE_METATYPE(Noise::InitTask)

#endif // GENPROCESSOR_H
