#include "MainWindow.h"

namespace Interface {

    const quint16 MainWindow::m_mw = 512;
    const quint16 MainWindow::m_aw = 128;
    const quint16 MainWindow::m_h = 512;

    MainWindow::MainWindow(quint8 layoutCount, quint8 octaveCount, QWidget *parent) :
        QWidget(parent),
        m_mode(false),
        m_sizeCaught(false) {
        setLayout(createInterface(layoutCount, octaveCount));
        setInterface();
    }

    void MainWindow::requestIn(quint8 type, quint8 request, const QVariant &arg0, const QVariant &arg1, const QVariant &arg2) {
        m_ccpPanels[type]->requestIn(request, arg0, arg1, arg2);
    }

    MainPainter *MainWindow::getMainPainter(void) {
        return m_pMain;
    }

    AnalysisPainter *MainWindow::getAnalysisPainter(void) {
        return m_pAnalysis;
    }

    const QColor *MainWindow::colorBuffer(quint8 layout) const {
        return dynamic_cast<LayoutControl*>(m_ccpPanels[getLayoutRequest(layout)])->colorBuffer();
    }

    void MainWindow::show(void) {
        QWidget::show();
        m_modeSizes[0] = size();
        changeMode();
        m_modeSizes[1] = size();
        m_sizeCaught = true;
        changeMode();
    }

    QHBoxLayout *MainWindow::createInterface(quint8 layoutCount, quint8 octaveCount) {
        QHBoxLayout *mainLayout = new QHBoxLayout;
        QVBoxLayout *settingsLayout = new QVBoxLayout;
        QVBoxLayout *analysisLayout = new QVBoxLayout;
        m_pbMode = new QPushButton(this);
        m_cbAnalysisLayout = new QComboBox(this);
        m_twExtended = new QTabWidget(this);
        m_ccpPanels[Common] = new CommonControl(layoutCount, this);
        m_cbAnalysisLayout->addItem("Main");
        for (quint8 i = 0; i < layoutCount; i++) {
            m_cbAnalysisLayout->addItem("Layout " + QString::number(i));
            m_ccpPanels[getLayoutRequest(i)] = new LayoutControl(octaveCount, m_twExtended);
            m_twExtended->addTab(m_ccpPanels[getLayoutRequest(i)], "Layout " + QString::number(i));
        }
        settingsLayout->addWidget(m_ccpPanels[Common]);
        settingsLayout->addWidget(m_pbMode);
        settingsLayout->addStretch();
        settingsLayout->addWidget(m_twExtended);
        analysisLayout->addWidget(m_cbAnalysisLayout);
        analysisLayout->addWidget(m_pAnalysis = new AnalysisPainter(m_aw, m_h, this));
        mainLayout->addWidget(m_pMain = new MainPainter(m_mw, m_h, this));
        mainLayout->addLayout(settingsLayout);
        mainLayout->addLayout(analysisLayout);
        return mainLayout;
    }

    void MainWindow::setInterface(void) {
        m_twExtended->setVisible(m_mode);
        m_pAnalysis->setVisible(m_mode);
        m_cbAnalysisLayout->setVisible(m_mode);
        connect(m_pbMode, SIGNAL(clicked()), SLOT(changeMode()));
        for (QMap<quint8, ComplexControlPanel *>::ConstIterator iter = m_ccpPanels.constBegin();
             iter != m_ccpPanels.constEnd(); iter++) {
            connect(iter.value(), SIGNAL(requestOut(quint8,quint8,QVariant,QVariant)),
                    SLOT(internalRequestIn(quint8,quint8,QVariant,QVariant)));
        }
        connect(m_cbAnalysisLayout, SIGNAL(currentIndexChanged(int)), SLOT(internalRequestIn(int)));
    }

    quint8 MainWindow::getLayoutRequest(quint8 index) const {
        switch (index) {
        case 0: return Layout0;
        case 1: return Layout1;
        case 2: return Layout2;
        case 3: return Layout3;
        default: return 0;
        }
    }

    void MainWindow::changeMode(void) {
        if ((m_mode = !m_mode) & m_sizeCaught) {
            setFixedSize(m_modeSizes[1]);
        }
        m_twExtended->setVisible(m_mode);
        m_pAnalysis->setVisible(m_mode);
        m_cbAnalysisLayout->setVisible(m_mode);
        if (!m_mode) {
            setFixedSize(m_modeSizes[0]);
        }
        m_pbMode->setText(QChar((m_mode) ? 0x2191 : 0x2193) + QString(" Advanced ") + QChar((m_mode) ? 0x2190 : 0x2192));
    }

    void MainWindow::internalRequestIn(quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1) {
        QObject *sender = QObject::sender();
        for (QMap<quint8, ComplexControlPanel *>::ConstIterator iter = m_ccpPanels.constBegin();
             iter != m_ccpPanels.constEnd(); iter++) {
            if (sender == iter.value()) {
                emit requestOut(iter.key(), id, request, arg0, arg1);
                break;
            }
        }
    }

    void MainWindow::internalRequestIn(int item) {
        if (item == 0) {
            item = 4;
        } else {
            item -= 1;
        }
        requestOut(Analysis, 0, 0, item, 0);
    }

}
