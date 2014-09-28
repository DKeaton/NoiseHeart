#include "DataStore.h"

const quint8 DataStore::layoutCount = 4;
const quint8 DataStore::octaveCount = 6;

DataStore::DataStore(quint8 threadCount) {
    m_mainWindow = new Interface::MainWindow(layoutCount, octaveCount);
    m_sController = new ShaderController(2048, 320, 512);
    m_tController = new TimingController();
    for (quint8 i = 0; i < 4; i++) {
        m_generator[i] = new Noise::Generator(2048, threadCount);
        m_buffer[i] = new Buffer(2048, 4);
        m_colors[i] = m_mainWindow->colorBuffer(i);
    }
    m_speed[0] = 100;
    m_speed[1] = 80;
    m_speed[2] = 60;
    m_speed[3] = 40;
}

DataStore::~DataStore(void) {
    for (quint8 i = 0; i < 4; i++) {
        delete m_buffer[i];
    }
    delete m_mainWindow;
    delete m_sController;
}
