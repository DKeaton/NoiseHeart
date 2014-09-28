#ifndef GENCONTROLLER_H
#define GENCONTROLLER_H

#include <QLinkedList>
#include <QVariant>

#include "GenProcessor.h"
#include "SettingsCorrector.h"

namespace Noise {

    enum Request {
        swap,
        startProcessor,
        requestPointers,
        setAcquireState,
        setReleaseState
    };

    struct ProcessorState {
        bool isAquiring : 1;
        bool isReleasing : 1;
        quint8 releasingThread : 4;
        quint8 acquiringThread : 4;
        quint8 semaphore : 4;
    };

    struct GeneratorState {
        quint16 semaphores : 9;
        quint8 toFinish : 4;
        bool finalSummDone : 1;
    };

    struct OctaveState {
        bool isProcessing : 1;
        quint8 curProcessor : 4;
        Command::Action finalTask;
    };

    struct ControllerState {
        bool isInitialized : 1;
        bool isGenerating : 1;
        bool generatingQueued : 1;
        quint8 workingThreads : 4;
    };

    struct FollowState {
        FollowState(void) {
        }

        FollowState(quint8 _id, quint8 _semaphore, bool _on) {
            id = _id;
            semaphore = _semaphore;
            on = _on;
        }

        quint8 id : 4;
        quint8 semaphore : 4;
        bool on : 1;
    };

    class GenController : public QObject {
        Q_OBJECT
    public:
        GenController(quint8 threadCount);
        ~GenController(void);
        void newSettings(const Settings &sets);
        bool isGenerating(void) const;
        bool isInitialized(void) const;

        static void registerMetaTypes(void);

    public slots:
        void startGenerator(void);
        void generate(void);
        void nextTask(quint8 id, CommandPtr task);

    private:
        void initialize(void);
        void buildQueue(void);
        void startGenerating(void);
        void takeTask(quint8 id, CommandPtr task);
        void startReleasing(quint8 id, quint8 acquiringId);
        void stopReleasing(quint8 id, quint8 acquiringId);

        SettingsCorrector *m_corrector;
        QLinkedList<Command> m_queue;
        QMutex m_mutex;
        const Settings *m_sets;
        Command **m_tasks;
        ProcessorState *m_procStates;
        GeneratorState m_genStates;
        OctaveState m_octStates[6];
        ControllerState m_conStates;
        const quint8 m_threadCount;
        quint8 m_curIndex;

    signals:
        void generatingCompleted(void);
        void requestOut(Request request, const QVariant &arg);
    };

}

Q_DECLARE_METATYPE(Noise::FollowState)

#endif // GENCONTROLLER_H
