#include "CommonControl.h"

namespace Interface {

    CommonControl::CommonControl(quint8 layoutCount, QWidget *parent)
        : ComplexControlPanel(parent) {
        setLayout(createInterface(layoutCount));
        setInterface();
    }

    QHBoxLayout *CommonControl::createInterface(quint8 layoutCount) {
        QHBoxLayout *mainLayout = new QHBoxLayout;
        mainLayout->addWidget(m_bcpPanels[InterFps] = new SliderSet(1, SliderSet::HorizontalUpName, "Fps", this));
        m_bcpPanels[InterFps]->requestIn(SliderSet::Range, 0, 15, 40);

        mainLayout->addWidget(m_bcpPanels[InterLayout] = new CheckBoxSet(layoutCount, CheckBoxSet::Horizontal, "Layouts", this));
        for (quint8 i = 0; i < layoutCount; i++) {
            m_bcpPanels[InterLayout]->requestIn(CheckBoxSet::Name, i, QString::number(i));
        }
        return mainLayout;
    }

    void CommonControl::setInterface(void) {
        registerSignal(m_bcpPanels[InterFps], InterFps);
        registerSignal(m_bcpPanels[InterLayout], InterLayout);
    }

    void CommonControl::processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                                       const QVariant &, const QVariant &) {
        switch (request) {
        case FpsRange:
            m_bcpPanels[InterFps]->requestIn(SliderSet::Range, 0, arg0, arg1);
            break;
        case Fps:
            m_bcpPanels[InterFps]->requestIn(SliderSet::Value, 0, arg0.toInt());
            break;
        case Layout:
            m_bcpPanels[InterLayout]->requestIn(CheckBoxSet::Value, arg0, arg1);
            break;
        }
    }

    void CommonControl::internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1) {
        switch (getSenderId(qobject_cast<BasicControlPanel*>(QObject::sender()))) {
        case InterFps:
            switch (request) {
            case SliderSet::Value:
                emit requestOut(Fps, request, arg0, arg1);
                break;
            }
            break;
        case InterLayout:
            switch (request) {
            case CheckBoxSet::Value:
                emit requestOut(Layout, request, arg0, arg1);
                break;
            }
            break;
        }
    }

}
