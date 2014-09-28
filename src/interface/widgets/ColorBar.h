#ifndef COLORBAR_H
#define COLORBAR_H

#include <QMetaType>
#include <QWidget>
#include <QPainter>
#include <QList>
#include <QPair>
#include <QVariant>
#include <QMouseEvent>

namespace Interface {

    class ColorBar : public QWidget {
        Q_OBJECT
    public:
        static void registerMetaTypes(void);

        enum Request {
            Highlight,
            SetLine
        };

        ColorBar(quint16 lineCount, QWidget *parent = 0);
        ~ColorBar(void);
        void requestIn(quint8 request, const QVariant &arg0 = QVariant(), const QVariant &arg1 = QVariant());

    private:
        static const quint16 MAX_LINES;

        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void paintEvent(QPaintEvent *);

        void draw(QPainter *painter);
        bool checkCursor(qint32 x, qint32 y);
        quint16 toIndex(qint32 y);

        const quint16 m_lineCount;
        QColor *m_lines;
        quint16 m_curLine;
        bool m_scrolling;

    signals:
        void requestOut(quint8 request, const QVariant &arg);
    };

}

#endif // COLORBAR_H
