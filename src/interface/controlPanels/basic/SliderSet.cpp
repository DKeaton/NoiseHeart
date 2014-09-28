#include "SliderSet.h"

namespace Interface {

    const quint8 SliderSet::MAX_ITEMS = 6;

    SliderSet::SliderSet(quint8 count, Mode mode, const QString &title, QWidget *parent)
        : BasicControlPanel(title, parent), m_count(count) {
        if (m_count > MAX_ITEMS) {
            throw new OutOfRangeException("Slider set count out of range.");
        }
        setLayout(createInterface(mode));
        setInterface();
    }

    SliderSet::~SliderSet(void) {
        for (quint8 i = 0; i < m_count; i++) {
            delete m_lLabels[i];
            delete m_sSliders[i];
            delete m_sbSpinBoxes[i];
        }
        delete[] m_lLabels;
        delete[] m_sSliders;
        delete[] m_sbSpinBoxes;
    }

    QLayout *SliderSet::createInterface(Mode mode) {
        QLayout *mainLayout;
        if (mode == HorizontalUpName) {
            mainLayout = new QVBoxLayout();
        } else {
            mainLayout = new QGridLayout();
        }
        m_lLabels = new QLabel*[m_count];
        m_sSliders = new QSlider*[m_count];
        m_sbSpinBoxes = new QSpinBox*[m_count];
        for (quint8 i = 0; i < m_count; i++) {
            m_lLabels[i] = new QLabel(this);
            m_sSliders[i] = new QSlider(mode == Vertical ? Qt::Vertical : Qt::Horizontal, this);
            m_sbSpinBoxes[i] = new QSpinBox(this);
            if (mode == Vertical) {
                m_sSliders[i]->setMinimumHeight(100);
            } else {
                m_sSliders[i]->setMinimumWidth(100);
            }
            if (mode == HorizontalUpName) {
                QHBoxLayout *tempLayout = new QHBoxLayout();
                QVBoxLayout *tempMainLayout = reinterpret_cast<QVBoxLayout*>(mainLayout);
                tempLayout->addWidget(m_sSliders[i]);
                tempLayout->addWidget(m_sbSpinBoxes[i]);
                tempMainLayout->addWidget(m_lLabels[i]);
                tempMainLayout->addLayout(tempLayout);
            } else {
                QGridLayout *tempMainLayout = reinterpret_cast<QGridLayout*>(mainLayout);
                quint8 index = (mode == Vertical) ? 1 : 0;
                quint8 pos[2][3];
                memset(pos[index], i, 3);
                for (quint8 j = 0; j < 3; j++) {
                    pos[1 - index][j] = j;
                }
                tempMainLayout->addWidget(m_lLabels[i], pos[0][0], pos[1][0]);
                tempMainLayout->addWidget(m_sSliders[i], pos[0][1], pos[1][1]);
                tempMainLayout->addWidget(m_sbSpinBoxes[i], pos[0][2], pos[1][2]);
            }
        }
        return mainLayout;
    }

    void SliderSet::setInterface(void) {
        for (quint8 i = 0; i < m_count; i++) {
            registerSignal(m_sSliders[i], SIGNAL(valueChanged(int)), i);
            registerSignal(m_sbSpinBoxes[i], SIGNAL(valueChanged(int)), i + m_count);
        }
    }

    void SliderSet::processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                                   const QVariant &arg2, const QVariant &) {
        quint8 index = arg0.toInt();
        if (index >= MAX_ITEMS) {
            throw new OutOfRangeException("Slider index out of range.");
        }
        switch (request) {
        case Value:
            m_sSliders[index]->setValue(arg1.toInt());
            break;
        case Name:
            m_lLabels[index]->setText(arg1.toString());
            break;
        case Range: {
            quint8 min = arg1.toInt();
            quint8 max = arg2.toInt();
            m_sSliders[index]->setRange(min, max);
            m_sbSpinBoxes[index]->setRange(min, max);
            break;
        }
        }
    }

    void SliderSet::internalRequestIn(int id) {
        quint8 normIndex = id % m_count;
        if (m_sSliders[normIndex]->value() != m_sbSpinBoxes[normIndex]->value()) {
            if (id < m_count) {
                m_sbSpinBoxes[normIndex]->setValue(m_sSliders[normIndex]->value());
            } else {
                m_sSliders[normIndex]->setValue(m_sbSpinBoxes[normIndex]->value());
            }
            emit requestOut(Value, QVariant::fromValue<int>(normIndex), QVariant::fromValue<int>(m_sSliders[normIndex]->value()));
        }
    }

}
