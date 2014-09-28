#include "LayoutControl.h"

namespace Interface {

    LayoutControl::LayoutControl(quint8 octaveCount, QWidget *parent)
        : ComplexControlPanel(parent),
          m_octaveCount(octaveCount) {
        setLayout(createInterface(m_octaveCount));
        setInterface();
    }

    QVBoxLayout *LayoutControl::createInterface(quint8 octaveCount) {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->addWidget(m_bcpPanels[Persistance] = new SliderSet(1, SliderSet::Vertical, "Coeff", this));
        m_bcpPanels[Persistance]->requestIn(SliderSet::Range, 0, 0, 100);
        hLayout->addWidget(m_bcpPanels[Amplitude] = new SliderSet(octaveCount, SliderSet::HorizontalLeftName, "Amplitudes", this));
        hLayout->addWidget(m_bcpPanels[Octave] = new CheckBoxSet(octaveCount, CheckBoxSet::Vertical, "Octaves", this));
        for (quint8 i = 0; i < octaveCount; i++) {
            m_bcpPanels[Octave]->requestIn(CheckBoxSet::Name, i, i);
            m_bcpPanels[Amplitude]->requestIn(SliderSet::Name, i, i);
            m_bcpPanels[Amplitude]->requestIn(SliderSet::Range, i, 0, 100);
        }
        mainLayout->addLayout(hLayout);
        mainLayout->addWidget(m_ccColor = new ColorControl(256, this));
        return mainLayout;
    }

    void LayoutControl::setInterface(void) {
        registerSignal(m_bcpPanels[Persistance], Persistance);
        registerSignal(m_bcpPanels[Octave], Octave);
        registerSignal(m_bcpPanels[Amplitude], Amplitude);
        connect(m_ccColor, SIGNAL(requestOut(quint8,quint8,QVariant,QVariant)),
                SLOT(internalRequestIn(quint8,quint8,QVariant,QVariant)));
    }

    const QColor *LayoutControl::colorBuffer(void) {
        return m_ccColor->colors();
    }

    void LayoutControl::processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                                       const QVariant &arg2, const QVariant &) {
        switch (request) {
        case PersistanceRange:
        case AmplitudeRange:
            m_bcpPanels[(request == PersistanceRange) ? Persistance : Amplitude]->requestIn(SliderSet::Range, arg0, arg1, arg2);
            break;
        case Persistance:
        case Amplitude:
            m_bcpPanels[request]->requestIn(SliderSet::Value, arg0, arg1);
            break;
        case Octave:
            m_bcpPanels[Octave]->requestIn(CheckBoxSet::Value, arg0, arg1);
            break;
        case Color:
            m_ccColor->requestIn(ColorControl::SetColor, arg0, arg1);
            break;
        default:;
        }
    }

    void LayoutControl::internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1) {
        quint8 sender = getSenderId(qobject_cast<BasicControlPanel*>(QObject::sender()));
        if (sender != Persistance) {
            emit requestOut(sender, request, arg0, arg1);
        } else {
            quint8 cur = 100;
            float per = arg1.toInt() / 100.0f;
            for (qint8 i = m_octaveCount - 1; i >= 0; i--) {
                m_bcpPanels[Amplitude]->requestIn(SliderSet::Value, QVariant::fromValue<int>(i), QVariant::fromValue<int>(cur));
                cur *= per;
            }
        }
    }

    void LayoutControl::internalRequestIn(quint8, quint8 request, const QVariant &arg0, const QVariant &arg1) {
        emit requestOut(Color, request, arg0, arg1);
    }

}
