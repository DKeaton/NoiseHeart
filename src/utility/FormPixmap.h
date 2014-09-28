#ifndef FORMPIXMAP_H
#define FORMPIXMAP_H

#include <qmath.h>
#include <QVector>
#include <QDebug>

class FormPixmap {
public:
    FormPixmap(quint16 side, quint16 texWidth, quint16 r);
    const float *getVertices(void) const;
    const float *getParams(void) const;
    quint32 getSize(void) const;

private:
    float sqr(float x) const;

    const quint16 m_side, m_width, m_r;
    const float m_coeff;
    QVector<float> m_vertices, m_params;
};

#endif // FORMPIXMAP_H
