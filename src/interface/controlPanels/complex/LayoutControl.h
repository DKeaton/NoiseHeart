#ifndef LAYOUTCONTROL_H
#define LAYOUTCONTROL_H

#include "ComplexControlPanel.h"
#include "ColorControl.h"

#include "../basic/CheckBoxSet.h"
#include "../basic/SliderSet.h"

namespace Interface {

    class LayoutControl : public ComplexControlPanel {
        Q_OBJECT
    public:
        enum Request {
            Persistance,
            PersistanceRange,
            Octave,
            Amplitude,
            AmplitudeRange,
            Color
        };

        LayoutControl(quint8 octaveCount, QWidget *parent = 0);
        const QColor *colorBuffer(void);

    private:
        void processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &arg2, const QVariant &);

        QVBoxLayout *createInterface(quint8 octaveCount);
        void setInterface(void);

        const quint8 m_octaveCount;
        QMap<quint8, BasicControlPanel*> m_bcpPanels;
        ColorControl *m_ccColor;

    private slots:
        void internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1);
        void internalRequestIn(quint8, quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // LAYOUTCONTROL_H
