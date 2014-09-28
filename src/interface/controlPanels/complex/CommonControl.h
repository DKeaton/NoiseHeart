#ifndef COMMONCONTROL_H
#define COMMONCONTROL_H

#include "ComplexControlPanel.h"
#include "../basic/CheckBoxSet.h"
#include "../basic/SliderSet.h"

namespace Interface {

    class CommonControl : public ComplexControlPanel {
        Q_OBJECT
    public:
        enum Request {
            Fps,
            Layout,
            FpsRange
        };

        CommonControl(quint8 layoutCount, QWidget *parent = 0);

    private:
        enum BasicPanels {
            InterFps = 0,
            InterLayout = 1
        };

        void processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &, const QVariant &);

        QHBoxLayout *createInterface(quint8 layoutCount);
        void setInterface(void);

        QMap<quint8, BasicControlPanel*> m_bcpPanels;

    private slots:
        void internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // COMMONCONTROL_H
