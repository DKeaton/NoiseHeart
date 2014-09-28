#include "Generator.h"

namespace Noise {

    Generator::Generator(quint32 size, quint8 threadCount, QObject *parent)
        : QThread(parent), GenData(size),
          m_threadCount(threadCount) {
        GenController::registerMetaTypes();
        m_controller = new GenController(threadCount);
        m_semaphores = new QSemaphore[m_threadCount - 1];
        m_threads = new QThread*[m_threadCount];
        m_processors = new GenProcessor*[m_threadCount];
        m_controller->moveToThread(this);
        for (quint8 i = 0; i < m_threadCount; i++) {
            m_threads[i] = new QThread();
            m_processors[i] = new GenProcessor(i, m_size, m_semaphores);
            m_processors[i]->moveToThread(m_threads[i]);
            connect(m_processors[i], SIGNAL(requestNextTask(quint8,CommandPtr)), m_controller, SLOT(nextTask(quint8,CommandPtr)), Qt::DirectConnection);
            m_threads[i]->start();
        }
        connect(m_controller, SIGNAL(requestOut(Request,QVariant)), SLOT(internalRequestIn(Request,QVariant)), Qt::DirectConnection);
        connect(m_controller, SIGNAL(generatingCompleted()), SIGNAL(generatingCompleted()), Qt::DirectConnection);
        connect(this, SIGNAL(started()), m_controller, SLOT(startGenerator()));
    }

    Generator::~Generator(void) {
        quit();
        wait();
        for (quint8 i = 0; i < m_threadCount; i++) {
            m_threads[i]->quit();
        }
        for (quint8 i = 0; i < m_threadCount; i++) {
            m_threads[i]->wait();
            delete m_processors[i];
            delete m_threads[i];
        }
        delete[] m_processors;
        delete[] m_threads;
        delete m_controller;
    }

    void Generator::newSettings(const Settings &sets) {
        m_controller->newSettings(sets);
    }

    void Generator::generate(quint8 *_dst) {
        if (!m_controller->isGenerating() || !m_controller->isInitialized()) {
            dst = _dst;
            QMetaObject::invokeMethod(m_controller, "generate", Qt::QueuedConnection);
        }
    }

    void Generator::internalRequestIn(Request request, const QVariant &arg) {
        switch (request) {
        case swap: {
            qint32 t = arg.toInt();
            if (t != -1) {
                swapCells(t);
            } else {
                prepareBuffer();
            }
            break;
        }
        case startProcessor: {
            InitTask t = arg.value<InitTask>();
            QMetaObject::invokeMethod(m_processors[t.first], "startLoop", Qt::QueuedConnection, Q_ARG(CommandPtr, t.second));
            break;
        }
        case requestPointers: {
            CommandPtr t = arg.value<CommandPtr>();
            switch (t->action) {
            case Command::toSmooth:
                t->dst = smoothed[t->octave][0];
                t->src = rands;
                break;
            case Command::toInterpolate:
                t->dst = octDst[t->octave];
                t->src = smoothed[t->octave];
                break;
            case Command::toFinalSumm:
                t->dst = dst;
                t->src = octDst;
                break;
            default:;
            }
            break;
        }
        case setAcquireState: {
            FollowState t = arg.value<FollowState>();
            m_processors[t.id]->setAcquireState(t.semaphore, t.on);
            break;
        }
        case setReleaseState: {
            FollowState t = arg.value<FollowState>();
            m_processors[t.id]->setReleaseState(t.semaphore, t.on);
            break;
        }
        }
    }

}
