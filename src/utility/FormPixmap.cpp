#include "FormPixmap.h"

#define Pi 3.141592

FormPixmap::FormPixmap(quint16 side, quint16 texWidth, quint16 r)
    : m_side(side), m_width(texWidth), m_r(r),
      m_coeff(m_width / (m_r * 2.0f * Pi)) {
    for (qint16 h = -m_side / 2; h < m_side / 2; h++) {
        for (qint16 w = -m_side / 2; w < m_side / 2; w++) {
            if (h / (m_side / 4.0f) > acos(1 - qAbs(w / (m_side / 4.0f))) - Pi + 1 &&
                    h / (m_side / 4.0) < sqrt(1 - sqr(qAbs(w / (m_side / 4.0f)) - 1)) + 1) {
                r = sqrt(w * w + h * h) + 1;
                m_vertices.push_back(static_cast<float>(w) / (m_side / 2));
                m_vertices.push_back(static_cast<float>(h) / (m_side / 2));
                float at;
                if (w != 0) {
                    at = atan(static_cast<float>(h) / w) + Pi / 2.0f;
                    if (w < 0) {
                        at += Pi;
                    }
                } else {
                    at = (h < 0) ? 0.0f : Pi;
                }
                float y = static_cast<float>(r - 1) / m_r;
                quint16 pointCount = (y == 0.0) ? texWidth : ceil(m_coeff / y);
                float sPoint = m_width * (at / (Pi * 2.0f)) - pointCount / 2.0f;
                m_params.push_back(sPoint / m_width);
                m_params.push_back(static_cast<float>(pointCount));
                m_params.push_back(1.0f / (2.0f * Pi * r * pointCount));
                m_params.push_back(y * ((m_r - 1.0) / m_r));
            }
        }
    }
}

const float *FormPixmap::getVertices(void) const {
    return m_vertices.data();
}

const float *FormPixmap::getParams(void) const {
    return m_params.data();
}

quint32 FormPixmap::getSize(void) const {
    return m_vertices.size() / 2;
}

float FormPixmap::sqr(float x) const {
    return x * x;
}

#undef Pi
