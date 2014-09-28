#ifndef DATASTORE_H
#define DATASTORE_H

#include <QDebug>

#include "interface/MainWindow.h"
#include "engine/ShaderController.h"
#include "engine/TimingController.h"
#include "engine/generator/Generator.h"
#include "utility/buffer/Buffer.h"

class DataStore {
    static const quint8 layoutCount;
    static const quint8 octaveCount;

public:
    DataStore(quint8 threadCount);
    ~DataStore(void);

protected:
    ShaderController *m_sController;
    Interface::MainWindow *m_mainWindow;
    Noise::Generator *m_generator[4];
    Buffer *m_buffer[4];
    TimingController *m_tController;
    Settings m_sets[4];
    quint8 m_speed[4];
    const QColor *m_colors[4];
};

#endif // DATASTORE_H
