#ifndef GENERATOR_H
#define GENERATOR_H

#include <QThread>

#include "GenData.h"
#include "GenController.h"

namespace Noise {

    class Generator : public QThread, private GenData {
        Q_OBJECT
    public:
        Generator(quint32 size, quint8 threadCount, QObject *parent = 0);
        ~Generator(void);
        void newSettings(const Settings &sets);

    public slots:
        void generate(quint8 *dst);

    private:
        GenController *m_controller;
        GenProcessor **m_processors;
        QThread **m_threads;
        QSemaphore *m_semaphores;
        const quint8 m_threadCount;
        bool m_newSettings;

    private slots:
        void internalRequestIn(Request request, const QVariant &arg);

    signals:
        void generatingCompleted();
    };

}

#endif // GENERATOR_H
