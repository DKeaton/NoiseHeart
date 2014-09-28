#include "ColorControl.h"

#define Pi 3.141592

namespace Interface {

    ColorControl::ColorControl(quint16 lineCount, QWidget *parent)
        : ComplexControlPanel(parent), m_lineCount(lineCount) {
        ColorBar::registerMetaTypes();
        m_mapper = new QSignalMapper(this);
        for (quint8 i = 0; i < 2; i++) {
            m_buffers[i] = new QColor[m_lineCount];
        }
        m_changed = false;
        m_curLine = 0;
        setLayout(createInterface());
        setInterface();
    }

    ColorControl::~ColorControl(void) {
        for (quint8 i = 0; i < 2; i++) {
            delete[] m_buffers[i];
        }
    }

    const QColor *ColorControl::colors(void) const {
        return m_buffers[0];
    }

    void ColorControl::processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                                      const QVariant &, const QVariant &) {
        switch (request) {
        case SetColor: {
            quint8 index = arg0.toInt();
            QColor color = arg1.value<QColor>();
            m_ssSliderSets[Highlight]->requestIn(SliderSet::Value, 0, index);
            m_buffers[1][index] = color;
            setColorSliders(index, true);
            internalRequestIn(0);
            break;
        }
        default:;
        }
    }

    QVBoxLayout *ColorControl::createInterface(void) {
        QVBoxLayout *mainLayout = new QVBoxLayout();
        QVBoxLayout *rLayout = new QVBoxLayout();
        QHBoxLayout *lLayout = new QHBoxLayout();
        QHBoxLayout *buttonLayout = new QHBoxLayout();

        buttonLayout->addWidget(m_pbAdd = new QPushButton("Add", this));
        m_pbAdd->setFixedWidth(64);
        buttonLayout->addWidget(m_pbRemove = new QPushButton("Remove", this));
        m_pbRemove->setFixedWidth(64);
        m_pbRemove->setEnabled(false);

        rLayout->addWidget(m_ssSliderSets[SetColor] = new SliderSet(3, SliderSet::Vertical, "Color", this));
        for (quint8 i = 0; i < 3; i++) {
            QString name;
            switch (i) {
            case 0: name = "R"; break;
            case 1: name = "G"; break;
            case 2: name = "B"; break;
            default:;
            }
            m_ssSliderSets[SetColor]->requestIn(SliderSet::Name, i, name);
            m_ssSliderSets[SetColor]->requestIn(SliderSet::Range, i, 0, 255);
        }
        rLayout->addLayout(buttonLayout);
        lLayout->addWidget(m_lwKeyPoints = new QListWidget(this));
        m_lwKeyPoints->setFixedWidth(64);

        lLayout->addWidget(m_cbColorBar = new ColorBar(m_lineCount, this));
        m_cbColorBar->setFixedWidth(64);
        lLayout->addLayout(rLayout);

        mainLayout->addWidget(m_ssSliderSets[Highlight] = new SliderSet(1, SliderSet::HorizontalLeftName, "", this));
        m_ssSliderSets[Highlight]->requestIn(SliderSet::Name, 0, "Line");
        m_ssSliderSets[Highlight]->requestIn(SliderSet::Range, 0, 0, 255);
        mainLayout->addLayout(lLayout);
        return mainLayout;
    }

    void ColorControl::setInterface(void) {
        registerSignal(m_ssSliderSets[SetColor], SetColor);
        registerSignal(m_ssSliderSets[Highlight], Highlight);
        connect(m_mapper, SIGNAL(mapped(int)), SLOT(internalRequestIn(int)));
        for (quint8 i = 0; i < 2; i++) {
            QPushButton *cur;
            switch (i) {
            case 0: cur = m_pbAdd; break;
            case 1: cur = m_pbRemove; break;
            }
            connect(cur, SIGNAL(clicked()), m_mapper, SLOT(map()));
            m_mapper->setMapping(cur, i);
        }
        connect(m_lwKeyPoints, SIGNAL(currentRowChanged(int)), m_mapper, SLOT(map()));
        m_mapper->setMapping(m_lwKeyPoints, 2);
        connect(m_cbColorBar, SIGNAL(requestOut(quint8,QVariant)), SLOT(internalRequestIn(quint8,QVariant)));
    }

    void ColorControl::setColorSliders(qint32 index, bool set) {
        if (!set) {
            disconnect(m_ssSliderSets[SetColor], SIGNAL(requestOut(quint8,QVariant,QVariant)), this, SLOT(internalRequestIn(quint8,QVariant,QVariant)));
        }
        for (quint8 i = 0; i < 3; i++) {
            m_ssSliderSets[SetColor]->requestIn(SliderSet::Value, i, accessColor(index, i));
        }
        if (!set) {
            connect(m_ssSliderSets[SetColor], SIGNAL(requestOut(quint8,QVariant,QVariant)), SLOT(internalRequestIn(quint8,QVariant,QVariant)));
        }
    }

    void ColorControl::dropToBuffer(bool toMain) {
        quint8 index = (toMain) ? 0 : 1;
        memcpy(m_buffers[index], m_buffers[1 - index], sizeof(QColor) * m_lineCount);
        for (quint16 i = 0; i < m_lineCount; i++) {
            m_cbColorBar->requestIn(ColorBar::SetLine, i, m_buffers[1][i]);
        }
        if (toMain) {
            emit requestOut(0, SetColor, QVariant(), QVariant());
        }
    }

    const QPair<qint32, qint32> ColorControl::findBorders(qint32 x) const {
        QPair<qint32, qint32> result(0, m_lineCount - 1);
        for (qint32 i = 0; i < m_lwKeyPoints->count(); i++) {
            qint32 cur = m_lwKeyPoints->item(i)->data(0x0100).toInt();
            if (cur < x) {
                result.first = cur;
            } else if (cur > x) {
                result.second = cur;
                break;
            }
        }
        return result;
    }

    void ColorControl::cosineInterpolate(quint32 start, quint32 end, quint8 color) {
        quint8 borderColors[2] = {accessColor(start, color), accessColor(end, color)};
        for (quint32 i = start + 1; i < end; i++) {
            float f = (1 - qCos(Pi * (static_cast<float>(i) - start) / (end - start))) / 2;
            quint8 val = borderColors[0] * (1.0f - f) + borderColors[1] * f;
            accessColor(i, color, val);
        }
    }

    quint8 ColorControl::accessColor(qint32 index, quint8 color, qint32 set) {
        quint8 result = 0;
        switch (color) {
        case 0:
            result = m_buffers[1][index].red();
            if (set != -1) {
                m_buffers[1][index].setRed(set);
            }
            break;
        case 1:
            result = m_buffers[1][index].green();
            if (set != -1) {
                m_buffers[1][index].setGreen(set);
            }
            break;
        case 2:
            result = m_buffers[1][index].blue();
            if (set != -1) {
                m_buffers[1][index].setBlue(set);
            }
            break;
        default:;
        }
        return result;
    }

    void ColorControl::internalRequestIn(int id) {
        switch (id) {
        case 0: {
            qint32 index = 0;
            dropToBuffer();
            for (; index < m_lwKeyPoints->count(); index++) {
                if (m_lwKeyPoints->item(index)->data(0x0100).toInt() >= m_curLine) {
                    break;
                }
            }
            if (index == m_lwKeyPoints->count() ? true : (m_lwKeyPoints->item(index)->data(0x0100).toInt() != m_curLine)) {
                m_lwKeyPoints->insertItem(index, QString::number(m_curLine));
            }
            m_lwKeyPoints->item(index)->setData(0x0100, QVariant::fromValue<int>(m_curLine));
            m_changed = false;
            break;
        }
        case 1: {
            qint32 cur = m_lwKeyPoints->currentRow();
            qint32 line = m_lwKeyPoints->takeItem(cur)->data(0x0100).toInt();
            QPair<qint32, qint32> borders = findBorders(line);
            if (cur == m_lwKeyPoints->count() && borders.second == m_lineCount - 1) {
                m_buffers[1][m_lineCount - 1] = QColor(0, 0, 0);
            }
            if (cur == 0 && borders.first == 0) {
                m_buffers[1][0] = QColor(0, 0, 0);
            }
            for (quint8 i = 0; i < 3; i++) {
                cosineInterpolate(borders.first, borders.second, i);
            }
            dropToBuffer();
            setColorSliders((m_lwKeyPoints->currentRow() == -1) ? m_curLine : m_lwKeyPoints->currentItem()->data(0x0100).toInt());
            break;
        }
        case 2:
            m_changed = true;
            if (m_lwKeyPoints->currentRow() != -1) {
                m_ssSliderSets[Highlight]->requestIn(SliderSet::Value, 0, m_lwKeyPoints->currentItem()->data(0x0100).toInt());
            }
            m_pbRemove->setEnabled(m_lwKeyPoints->currentRow() != -1);
            break;
        }
    }

    void ColorControl::internalRequestIn(quint8 request, const QVariant &arg) {
        switch (request) {
        case ColorBar::Highlight:
            m_ssSliderSets[Highlight]->requestIn(SliderSet::Value, 0, m_curLine = arg.toInt());
            break;
        }
    }

    void ColorControl::internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1) {
        switch (getSenderId(qobject_cast<BasicControlPanel*>(QObject::sender()))) {
        case SetColor: {
            quint8 color = arg0.toInt();
            switch (request) {
            case SliderSet::Value:
                accessColor(m_curLine, color, arg1.toInt());
                QPair<qint32, qint32> borders = findBorders(m_curLine);
                cosineInterpolate(borders.first, m_curLine, color);
                cosineInterpolate(m_curLine, borders.second, color);
                for (qint32 i = borders.first; i <= borders.second; i++) {
                    m_cbColorBar->requestIn(ColorBar::SetLine, i, m_buffers[1][i]);
                }
                m_changed = true;
                break;
            }
            break;
        }
        case Highlight: {
            switch (request) {
            case SliderSet::Value:
                if (arg0.toInt() == 0) {
                    m_cbColorBar->requestIn(ColorBar::Highlight, m_curLine = arg1.toInt());
                    if (m_changed) {
                        dropToBuffer(false);
                        m_changed = false;
                    }
                    setColorSliders(m_curLine);
                }
                break;
            }
            break;
        }
        }
    }

}

#undef Pi
