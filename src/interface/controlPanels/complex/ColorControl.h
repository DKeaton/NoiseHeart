#ifndef COLORCONTROL_H
#define COLORCONTROL_H

#include <QListWidget>
#include <QPushButton>
#include <qmath.h>

#include <QDebug>

#include "ComplexControlPanel.h"
#include "../basic/SliderSet.h"

#include "../../widgets/ColorBar.h"

namespace Interface {

    class ColorControl : public ComplexControlPanel {
        Q_OBJECT
    public:
        enum Request {
            Highlight,
            SetColor
        };

        ColorControl(quint16 lineCount, QWidget *parent = 0);
        ~ColorControl(void);

        const QColor *colors(void) const;

    private:
        void processRequest(quint8 request, const QVariant &arg0, const QVariant &arg1,
                            const QVariant &, const QVariant &);

        QVBoxLayout *createInterface(void);
        void setInterface(void);

        void setColorSliders(qint32 index, bool set = false);
        void dropToBuffer(bool toMain = true);
        const QPair<qint32, qint32> findBorders(qint32 x) const;
        void cosineInterpolate(quint32 start, quint32 end, quint8 color);
        quint8 accessColor(qint32 index, quint8 color, qint32 set = -1);

        QSignalMapper *m_mapper;
        const quint16 m_lineCount;
        QColor *m_buffers[2];
        qint32 m_curLine;
        bool m_changed;

        QMap<quint8, BasicControlPanel*> m_ssSliderSets;
        ColorBar *m_cbColorBar;
        QListWidget *m_lwKeyPoints;
        QPushButton *m_pbAdd, *m_pbRemove;

    private slots:
        void internalRequestIn(int id);
        void internalRequestIn(quint8 request, const QVariant &arg);
        void internalRequestIn(quint8 request, const QVariant &arg0, const QVariant &arg1);
    };

}

#endif // COLORCONTROL_H
