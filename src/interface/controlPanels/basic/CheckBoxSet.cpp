#include "CheckBoxSet.h"

namespace Interface {

    const quint8 CheckBoxSet::MAX_ITEMS = 6;

    CheckBoxSet::CheckBoxSet(quint8 count, Mode mode, const QString &title, QWidget *parent)
        : BasicControlPanel(title, parent), m_count(count) {
        if (m_count > MAX_ITEMS) {
            throw new OutOfRangeException("Check box count out of range.");
        }
        setLayout(createInterface(mode));
        setInterface();
    }

    CheckBoxSet::~CheckBoxSet(void) {
        for (quint8 i = 0; i < m_count; i++) {
            delete m_lLabels[i];
            delete m_cbCheckBoxes[i];
        }
        delete[] m_lLabels;
        delete[] m_cbCheckBoxes;
    }

    QGridLayout *CheckBoxSet::createInterface(Mode mode) {
        QGridLayout *mainLayout = new QGridLayout();
        m_lLabels = new QLabel*[m_count];
        m_cbCheckBoxes = new QCheckBox*[m_count];
        for (quint8 i = 0; i < m_count; i++) {
            quint8 index = (mode == Vertical) ? 0 : 1;
            quint8 pos[2][2];
            memset(pos[index], i, 2);
            for (quint8 j = 0; j < 2; j++) {
                pos[1 - index][j] = j;
            }
            mainLayout->addWidget(m_lLabels[i] = new QLabel(this), pos[0][0], pos[1][0]);
            mainLayout->addWidget(m_cbCheckBoxes[i] = new QCheckBox(this), pos[0][1], pos[1][1]);
        }
        return mainLayout;
    }

    void CheckBoxSet::setInterface(void) {
        for (quint8 i = 0; i < m_count; i++) {
            registerSignal(m_cbCheckBoxes[i], SIGNAL(stateChanged(int)), i);
        }
    }

    void CheckBoxSet::processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1, const QVariant &, const QVariant &) {
        quint8 index = arg0.toInt();
        if (index >= MAX_ITEMS) {
            throw new OutOfRangeException("Check box index out of range.");
        }
        switch (request) {
        case Value:
            m_cbCheckBoxes[index]->setChecked(arg1.toBool());
            break;
        case Name:
            m_lLabels[index]->setText(arg1.toString());
            break;
        }
    }

    void CheckBoxSet::internalRequestIn(int id) {
        emit requestOut(Value, QVariant::fromValue<int>(id), QVariant::fromValue<bool>(m_cbCheckBoxes[id]->isChecked()));
    }

}
