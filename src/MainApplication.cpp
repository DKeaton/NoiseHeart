#include "MainApplication.h"

using Interface::MainWindow;
using Interface::LayoutControl;
using Interface::CommonControl;
using Interface::ColorControl;

MainApplication::MainApplication(QObject *parent)
    : QThread(parent), DataStore(QThread::idealThreadCount()) {
    connect(m_mainWindow, SIGNAL(requestOut(quint8,quint8,quint8,QVariant,QVariant)),
            SLOT(internalRequestIn(quint8,quint8,quint8,QVariant,QVariant)));
    m_mainWindow->show();
    m_sController->setPainters(m_mainWindow->getMainPainter(), m_mainWindow->getAnalysisPainter());
    m_sController->setShaderFolder("../src/GLSL");
    start();
    for (quint8 i = 0; i < 4; i++) {
        m_generator[i]->moveToThread(this);
        connect(m_generator[i], SIGNAL(generatingCompleted()), m_buffer[i], SLOT(generatingCompleted()), Qt::DirectConnection);
        connect(m_buffer[i], SIGNAL(generatingRequested(quint8*)), m_generator[i], SLOT(generate(quint8*)), Qt::QueuedConnection);
        m_generator[i]->start();
        m_buffer[i]->startBuffer();
    }
    m_tController->moveToThread(this);
    connect(m_sController, SIGNAL(requestLine(quint8*,quint8)), SLOT(lineRequested(quint8*,quint8)), Qt::DirectConnection);
    connect(m_tController, SIGNAL(updateFrame()), m_sController, SLOT(updateFrame()));
    connect(m_sController, SIGNAL(frameUpdated()), m_tController, SLOT(frameUpdated()), Qt::DirectConnection);
    memset(m_sets, 0, sizeof(Settings) * 4);
    setUp();
    for (quint8 i = 0; i < 4; i++) {
        m_generator[i]->newSettings(m_sets[i]);
    }
}

MainApplication::~MainApplication(void) {
    quit();
    wait();
    for (quint8 i = 0; i < 4; i++) {
        delete m_generator[i];
    }
    delete m_tController;
}

void MainApplication::setUp(void) {
    for (quint8 i = 0; i <= 3; i += 3) {
        quint8 layout;
        quint8 per;
        switch (i) {
        case 0:
            layout = MainWindow::Layout0; per = 70;
            m_mainWindow->requestIn(layout, LayoutControl::Color, 25, QColor(0, 0, 0));
            m_mainWindow->requestIn(layout, LayoutControl::Color, 170, QColor(230, 0, 0));
            m_mainWindow->requestIn(layout, LayoutControl::Color, 255, QColor(255, 255, 0));
            break;
        case 3:
            layout = MainWindow::Layout3; per = 55;
            m_mainWindow->requestIn(layout, LayoutControl::Color, 120, QColor(0, 0, 0));
            m_mainWindow->requestIn(layout, LayoutControl::Color, 190, QColor(255, 90, 0));
            m_mainWindow->requestIn(layout, LayoutControl::Color, 255, QColor(255, 255, 0));
            break;
        default:;
        }
        m_mainWindow->requestIn(layout, LayoutControl::Color, 0, QColor(0, 0, 0));
        for (qint8 j = 5; j >= 0; j--) {
            m_mainWindow->requestIn(layout, LayoutControl::Octave, j, true);
        }
        m_mainWindow->requestIn(layout, LayoutControl::Persistance, 0, per);
        m_mainWindow->requestIn(MainWindow::Common, CommonControl::Layout, i, true);
    }
    m_mainWindow->requestIn(MainWindow::Layout0, LayoutControl::Octave, 0, false);
    m_mainWindow->requestIn(MainWindow::Common, CommonControl::Fps, 30, 0);
    m_sController->setCurAnalysisLayout(4);
}

void MainApplication::lineRequested(quint8 *dst, quint8 layout) {
    m_buffer[layout]->getCell(dst);
}

void MainApplication::internalRequestIn(quint8 type, quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1) {
    switch (type) {
    case MainWindow::Common:
        switch (id) {
        case CommonControl::Fps: {
            quint8 fps = arg1.toInt();
            m_tController->setDelay(1000 / fps);
            for (quint8 i = 0; i < 4; i++) {
                m_sController->setSpeed(i, fps, m_speed[i]);
            }
            break;
        }
        case CommonControl::Layout: {
            m_sController->setLayoutOn(arg0.toInt(), arg1.toBool());
            break;
        }
        }
        break;
    case MainWindow::Analysis:
        m_sController->setCurAnalysisLayout(arg0.toInt());
        break;
    default: {
        quint8 layout = getLayoutId(type);
        switch (id) {
        case LayoutControl::Octave: {
            quint8 octave = arg0.toInt();
            m_sets[layout].octaves[octave] = arg1.toBool();
            m_generator[layout]->newSettings(m_sets[layout]);
            break;
        }
        case LayoutControl::Amplitude: {
            quint8 octave = arg0.toInt();
            m_sets[layout].amplitudes[octave] = arg1.toInt();
            m_generator[layout]->newSettings(m_sets[layout]);
            break;
        }
        case LayoutControl::Color: {
            quint8 colors[256 * 4];
            for (quint16 i = 0; i < 256; i++) {
                const QColor &cur = m_colors[layout][i];
                colors[i * 4 + 0] = cur.red();
                colors[i * 4 + 1] = cur.green();
                colors[i * 4 + 2] = cur.blue();
                colors[i * 4 + 3] = 255;
            }
            m_sController->setPalette(layout, colors);
            break;
        }
        }
        break;
    }
    }
}

quint8 MainApplication::getLayoutId(quint8 layout) const {
    switch (layout) {
    case MainWindow::Layout0: return 0;
    case MainWindow::Layout1: return 1;
    case MainWindow::Layout2: return 2;
    case MainWindow::Layout3: return 3;
    }
    return 0;
}
