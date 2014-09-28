#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTabWidget>
#include <QComboBox>
#include <conio.h>

#include "controlPanels/complex/CommonControl.h"
#include "controlPanels/complex/LayoutControl.h"
#include "widgets/MainPainter.h"
#include "widgets/AnalysisPainter.h"

namespace Interface {

    class MainWindow : public QWidget {
        Q_OBJECT

        static const quint16 m_mw;
        static const quint16 m_aw;
        static const quint16 m_h;
    public:
        enum Request {
            Common,
            Layout0,
            Layout1,
            Layout2,
            Layout3,
            Analysis
        };

        MainWindow(quint8 layoutCount, quint8 octaveCount, QWidget *parent = 0);
        void requestIn(quint8 type, quint8 request, const QVariant &arg0, const QVariant &arg1, const QVariant &arg2 = QVariant());
        MainPainter *getMainPainter(void);
        AnalysisPainter *getAnalysisPainter(void);
        const QColor *colorBuffer(quint8 layout) const;

    public slots:
        void show(void);

    private:
        QHBoxLayout *createInterface(quint8 layoutCount, quint8 octaveCount);
        void setInterface(void);
        quint8 getLayoutRequest(quint8 index) const;

        QMap<quint8, ComplexControlPanel*> m_ccpPanels;
        QTabWidget *m_twExtended;
        MainPainter *m_pMain;
        AnalysisPainter *m_pAnalysis;
        QPushButton *m_pbMode;
        QComboBox *m_cbAnalysisLayout;

        bool m_mode, m_sizeCaught;
        QSize m_modeSizes[2];

    private slots:
        void changeMode(void);
        void internalRequestIn(quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1);
        void internalRequestIn(int item);

    signals:
        void requestOut(quint8 type, quint8 id, quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // MAINWINDOW_H
