#ifndef ANALYSISPAINTER_H
#define ANALYSISPAINTER_H

#include <QVector4D>

#include "Painter.h"

namespace Interface {

    class AnalysisPainter : public Painter {
    public:
        AnalysisPainter(quint16 w, quint16 h, QWidget *parent = 0);
        virtual void setShaderAddress(const QString &address);
        void setVertices(const float *vertices, quint32 count);
        void setColors(const float *colors);

    private:
        void initializeGL(void);
        void paintGL(void);

        quint32 m_count;
    };

}

#endif // ANALYSISPAINTER_H
