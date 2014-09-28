#ifndef CHECKBOXSET_H
#define CHECKBOXSET_H

#include <QLayout>
#include <QLabel>
#include <QCheckBox>

#include "BasicControlPanel.h"

namespace Interface {

    class CheckBoxSet : public BasicControlPanel {
        Q_OBJECT
    public:
        enum Mode {
            Horizontal,
            Vertical
        };

        enum Request {
            Value,
            Name
        };

        CheckBoxSet(quint8 count, Mode mode, const QString &title, QWidget *parent = 0);
        ~CheckBoxSet(void);

    private:
        static const quint8 MAX_ITEMS;

        QGridLayout *createInterface(Mode mode);
        void setInterface(void);

        void processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &, const QVariant &);

        const quint8 m_count;

        QLabel **m_lLabels;
        QCheckBox **m_cbCheckBoxes;

    private slots:
        void internalRequestIn(int id);
    };

}

#endif // CHECKBOXSET_H
