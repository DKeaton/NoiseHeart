#ifndef MAINPAINTER_H
#define MAINPAINTER_H

#include <QVector4D>

#include "Painter.h"

namespace Interface {

    class MainPainter : public Painter {
    public:
        MainPainter(quint16 w, quint16 h, QWidget *parent = 0);
        virtual void setShaderAddress(const QString &address);
        void setVertices(const float *vertices, const float *params, quint32 count);
        void setTexture(const quint8 *data, quint16 w, quint16 h);
        void setPalette(const quint8 *data);
        void setOffset(float offset0, float offset1, float offset2, float offset3);
        void setLayouts(qint32 *layouts);

    private:
        void initializeGL(void);
        void paintGL(void);

        quint32 m_textureHandle[2];
        qint32 m_attribLoc[1];
        qint32 m_uniformLoc[4];
        quint32 m_count;
        bool set;
    };

}

#endif // MAINPAINTER_H
