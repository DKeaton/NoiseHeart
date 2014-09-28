#ifndef BUFFER_H
#define BUFFER_H

#include <QObject>
#include <QMutex>

#include <QDebug>

#include "BufData.h"

struct BufferStates {
    bool isGenerating : 1;
    bool lineReady : 1;
    bool swappingNeeded : 1;
};

class Buffer : public QObject, private BufData {
    Q_OBJECT
public:
    Buffer(quint32 size, quint8 depth, QObject *parent = 0);
    void startBuffer(void);

public slots:
    void getCell(quint8 *dst);
    void generatingCompleted(void);

private:
    void swapCells(void);
    void processBuffer(void);

    BufferStates m_states;
    QMutex m_exMutex, m_inMutex;
    quint8 m_curDepth;

signals:
    void generatingRequested(quint8 *dst);
};

#endif // BUFFER_H
