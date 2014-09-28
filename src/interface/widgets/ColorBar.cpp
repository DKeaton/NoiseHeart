#include "ColorBar.h"

namespace Interface {

    const quint16 ColorBar::MAX_LINES = 256;

    void ColorBar::registerMetaTypes(void) {
        qRegisterMetaType<QColor>("QColor");
    }

    ColorBar::ColorBar(quint16 lineCount, QWidget *parent)
        : QWidget(parent), m_lineCount(lineCount) {
        m_lines = new QColor[m_lineCount];
        m_curLine = m_lineCount - 1;
    }

    ColorBar::~ColorBar(void) {
        delete[] m_lines;
    }

    void ColorBar::requestIn(quint8 request, const QVariant &arg0, const QVariant &arg1) {
        quint16 index = m_lineCount - arg0.toInt() - 1;
        switch (request) {
        case Highlight:
            m_curLine = index;
            break;
        case SetLine:
            m_lines[index] = arg1.value<QColor>();
            break;
        }
        update();
    }

    void ColorBar::mousePressEvent(QMouseEvent *event) {
        switch (event->button()) {
        case Qt::LeftButton:
            if (checkCursor(event->x(), event->y())) {
                m_scrolling = true;
                emit requestOut(Highlight, QVariant::fromValue<int>(toIndex(event->y())));
                break;
            }
        default:;
        }
    }

    void ColorBar::mouseReleaseEvent(QMouseEvent *event) {
        switch (event->button()) {
        case Qt::LeftButton:
            m_scrolling = false;
            break;
        default:;
        }
    }

    void ColorBar::mouseMoveEvent(QMouseEvent *event) {
        if (m_scrolling & checkCursor(event->x(), event->y())) {
            emit requestOut(Highlight, QVariant::fromValue<int>(toIndex(event->y())));
        }
    }

    void ColorBar::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        painter.setViewport(0, 0, width(), height());
        painter.setWindow(0, 0, 30, m_lineCount);
        draw(&painter);
    }

    void ColorBar::draw(QPainter *painter) {
        QPen pen;
        pen.setWidth(1);
        painter->setRenderHint(QPainter::NonCosmeticDefaultPen);
        for (quint16 i = 0; i < m_lineCount; i++) {
            pen.setColor(m_lines[i]);
            painter->setPen(pen);
            painter->drawLine(10, i, 29, i);
        }
        painter->setRenderHint(QPainter::Antialiasing);
        pen.setWidth(2);
        pen.setColor(QColor(255, 0, 0));
        painter->setPen(pen);
        painter->drawLine(0, m_curLine, 9, m_curLine);
        painter->drawPoint(8, m_curLine - 1);
        painter->drawPoint(8, m_curLine + 1);
    }

    bool ColorBar::checkCursor(qint32 x, qint32 y) {
        return (x >= width() / 3 && x < width()) && (y >= 0 && y < height());
    }

    quint16 ColorBar::toIndex(qint32 y) {
        return (m_lineCount - 1) * (1.0f - static_cast<float>(y) / (height() - 1));
    }

}
