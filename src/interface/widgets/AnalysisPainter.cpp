#include "AnalysisPainter.h"

namespace Interface {

    AnalysisPainter::AnalysisPainter(quint16 w, quint16 h, QWidget *parent)
        : Painter(w, h, parent),
          m_count(0) {
    }

    void AnalysisPainter::setShaderAddress(const QString &address) {
        Painter::setShaderAddress(address);
    }

    void AnalysisPainter::setVertices(const float *vertices, quint32 count) {
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        m_count = count;
    }

    void AnalysisPainter::setColors(const float *colors) {
        glColorPointer(3, GL_FLOAT, 0, colors);
    }

    void AnalysisPainter::initializeGL(void) {
        Painter::initializeGL();
        glEnableClientState(GL_COLOR_ARRAY);
    }

    void AnalysisPainter::paintGL(void) {
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_LINE_STRIP, 0, m_count);
    }

}
