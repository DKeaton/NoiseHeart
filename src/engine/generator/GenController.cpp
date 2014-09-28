#include "GenController.h"

namespace Noise {

    GenController::GenController(quint8 threadCount)
        : m_corrector(new SettingsCorrector()),
          m_sets(m_corrector->getSettngs()),
          m_tasks(new Command*[threadCount]),
          m_procStates(new ProcessorState[threadCount]),
          m_threadCount(threadCount),
          m_curIndex(32) {
        GenProcessor::registerMetaTypes();
        GenProcessor::calcSteps();
        memset(&m_conStates, 0, sizeof(ControllerState));
        memset(&m_genStates, 0, sizeof(GeneratorState));
        memset(m_octStates, 0, sizeof(OctaveState) * 6);
        for (quint8 i = 0; i < 6; i++) {
            m_octStates[i].curProcessor = 10;
        }
        for (quint8 i = 0; i < m_threadCount; i++) {
            m_tasks[i] = new Command();
            memset(&m_procStates[i], 0, sizeof(ProcessorState));
        }
        m_genStates.finalSummDone = true;
    }

    GenController::~GenController(void) {
        delete[] m_procStates;
        for (quint8 i = 0; i < m_threadCount; i++) {
            delete m_tasks[i];
        }
        delete[] m_tasks;
        delete m_corrector;
    }

    void GenController::newSettings(const Settings &sets) {
        m_corrector->newSettings(sets);
    }

    bool GenController::isGenerating(void) const {
        return m_conStates.isGenerating;
    }

    bool GenController::isInitialized(void) const {
        return m_conStates.isInitialized;
    }

    void GenController::registerMetaTypes(void) {
        qRegisterMetaType<FollowState>("FollowState");
    }

    void GenController::startGenerator(void) {
        initialize();
    }

    void GenController::generate(void) {
        if (!m_conStates.isInitialized) {
            m_conStates.generatingQueued = true;
        } else {
            m_conStates.isGenerating = true;
            buildQueue();
            startGenerating();
        }
    }

    void GenController::nextTask(quint8 id, CommandPtr task) {
        m_mutex.lock();
        if (m_procStates[id].isReleasing) {
            stopReleasing(id, m_procStates[id].acquiringThread);
        } else if (m_procStates[id].isAquiring) {
            stopReleasing(m_procStates[id].releasingThread, id);
        }
        if (m_octStates[task->octave].curProcessor == id) {
            m_octStates[task->octave].isProcessing = false;
            m_octStates[task->octave].curProcessor = 10;
            if (task->action == m_octStates[task->octave].finalTask && m_genStates.toFinish != 0) {
                m_genStates.toFinish--;
            }
        }
        takeTask(id, task);
        if (task->action == Command::toStop) {
            m_conStates.workingThreads--;
        }
        if (m_genStates.toFinish == 0 && m_conStates.workingThreads == 0) {
            m_conStates.isGenerating = false;
            if (m_conStates.isInitialized) {
                m_corrector->correctSettings(1);
                emit generatingCompleted();
            } else {
                m_mutex.unlock();
                m_conStates.isInitialized = true;
                initialize();
                return;
            }
        }
        m_mutex.unlock();
    }

    void GenController::initialize(void) {
        if (!m_conStates.isInitialized) {
            m_genStates.toFinish = 6;
            for (quint8 i = 0; i < 6; i++) {
                m_octStates[i].finalTask = Command::toSmooth;
                m_queue.push_back(Command(Command::toSmooth, 32, i));
            }
            m_conStates.isGenerating = true;
            startGenerating();
        } else {
            if (m_conStates.generatingQueued) {
                m_conStates.generatingQueued = false;
                generate();
            }
        }
    }

    void GenController::buildQueue(void) {
        quint8 t = 1;
        if (m_curIndex == 64) {
            m_curIndex = 32;
            emit requestOut(swap, QVariant::fromValue<int>(-1));
        }
        for (quint8 i = 0; i < 6; i++) {
            Command::Action action = Command::toStop;
            if (!(m_curIndex % t)) {
                emit requestOut(swap, QVariant::fromValue<int>(i));
                action = Command::toSmooth;
                m_octStates[i].finalTask = (m_sets->octaves[i]) ? Command::toInterpolate : Command::toSmooth;
            } else if (m_sets->octaves[i]) {
                action = m_octStates[i].finalTask = Command::toInterpolate;
            }
            if (action != Command::toStop) {
                quint8 index = m_curIndex;
                if (action == Command::toSmooth) {
                    index += t;
                }
                m_queue.push_back(Command(action, index, i));
                m_genStates.toFinish++;
            }
            t *= 2;
        }
        m_genStates.finalSummDone = false;
        m_curIndex++;
    }

    void GenController::startGenerating(void) {
        m_mutex.lock();
        for (quint8 i = 0; i < m_threadCount; i++) {
            takeTask(i, m_tasks[i]);
            if (m_tasks[i]->action != Command::toStop) {
                m_conStates.workingThreads++;
                emit requestOut(startProcessor, QVariant::fromValue<InitTask>(qMakePair<quint8, CommandPtr>(i, m_tasks[i])));
            }
        }
        m_mutex.unlock();
    }

    void GenController::takeTask(quint8 id, CommandPtr task) {
        if (m_queue.isEmpty()) {
            if (m_genStates.toFinish == 0 && !m_genStates.finalSummDone) {
                m_genStates.finalSummDone = true;
                m_queue.push_back(Command(Command::toFinalSumm, 0, 0));
                m_queue.last().optional = m_sets;
            } else {
                task->action = Command::toStop;
                return;
            }
        }
        QLinkedList<Command>::Iterator iter;
        bool parallel = true;
        for (iter = m_queue.begin(); iter != m_queue.end(); iter++) {
            if (!m_octStates[iter->octave].isProcessing) {
                parallel = false;
                break;
            }
        }
        if (parallel) {
            iter = m_queue.begin();
            startReleasing(m_octStates[iter->octave].curProcessor, id);
        } else {
            m_octStates[iter->octave].isProcessing = true;
        }
        m_octStates[iter->octave].curProcessor = id;
        *task = *iter;
        emit requestOut(requestPointers, QVariant::fromValue<CommandPtr>(task));
        if (m_octStates[task->octave].finalTask > task->action) {
            iter->action = Command::toInterpolate;
            iter->index -= 1 << iter->octave;
        } else {
            m_queue.erase(iter);
        }
    }

    void GenController::startReleasing(quint8 id, quint8 acquiringId) {
        quint8 sem = 0;
        while (m_genStates.semaphores & (0x1 << sem)) {
            sem++;
        }
        emit requestOut(setReleaseState, QVariant::fromValue<FollowState>(FollowState(id, sem, true)));
        emit requestOut(setAcquireState, QVariant::fromValue<FollowState>(FollowState(acquiringId, sem, true)));
        m_genStates.semaphores |= 0x1 << sem;
        m_procStates[id].semaphore = sem;
        m_procStates[id].acquiringThread = acquiringId;
        m_procStates[id].isReleasing = true;
        m_procStates[acquiringId].releasingThread = id;
        m_procStates[acquiringId].isAquiring = true;
    }

    void GenController::stopReleasing(quint8 id, quint8 acquiringId) {
        if (m_procStates[id].isAquiring) {
            stopReleasing(m_procStates[id].releasingThread, id);
        }
        emit requestOut(setAcquireState, QVariant::fromValue<FollowState>(FollowState(acquiringId, 0, false)));
        emit requestOut(setReleaseState, QVariant::fromValue<FollowState>(FollowState(id, m_procStates[id].semaphore, false)));
        m_genStates.semaphores ^= 0x1 << m_procStates[id].semaphore;
        m_procStates[id].semaphore = 0;
        m_procStates[id].acquiringThread = 0;
        m_procStates[id].isReleasing = false;
        m_procStates[acquiringId].releasingThread = 0;
        m_procStates[acquiringId].isAquiring = false;
    }

}
