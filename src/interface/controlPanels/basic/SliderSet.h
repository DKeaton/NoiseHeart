#ifndef SLIDERSET_H
#define SLIDERSET_H

#include <QLayout>
#include <QLabel>
#include <QSlider>
#include <QSpinBox>

#include "BasicControlPanel.h"

namespace Interface {

    class SliderSet : public BasicControlPanel {
        Q_OBJECT
    public:
        enum Mode {
            HorizontalLeftName,
            HorizontalUpName,
            Vertical
        };

        enum Request {
            Value,
            Name,
            Range
        };

        SliderSet(quint8 count, Mode mode, const QString &title, QWidget *parent = 0);
        ~SliderSet(void);

    private:
        static const quint8 MAX_ITEMS;

        QLayout *createInterface(Mode mode);
        void setInterface(void);

        void processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &arg2, const QVariant &);

        const quint8 m_count;

        QLabel **m_lLabels;
        QSlider **m_sSliders;
        QSpinBox **m_sbSpinBoxes;

    private slots:
        void internalRequestIn(int id);
    };

}

#endif // SLIDERSET_H
