#include "MainPainter.h"

namespace Interface {

    MainPainter::MainPainter(quint16 w, quint16 h, QWidget *parent)
        : Painter(w, h, parent),
          m_count(0) {
    }

    void MainPainter::setShaderAddress(const QString &address) {
        Painter::setShaderAddress(address);
        m_attribLoc[0] = getProgram()->attributeLocation("vertexParams");
        m_uniformLoc[0] = getProgram()->uniformLocation("offset");
        m_uniformLoc[1] = getProgram()->uniformLocation("texture");
        m_uniformLoc[2] = getProgram()->uniformLocation("palette");
        m_uniformLoc[3] = getProgram()->uniformLocation("layouts");
        getProgram()->setUniformValue(m_uniformLoc[1], 0);
        getProgram()->setUniformValue(m_uniformLoc[2], 1);
        getProgram()->enableAttributeArray(m_attribLoc[0]);
    }

    void MainPainter::setVertices(const float *vertices, const float *params, quint32 count) {
        makeCurrent();
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        getProgram()->setAttributeArray(m_attribLoc[0], GL_FLOAT, params, 4);
        m_count = count;
    }

    void MainPainter::setTexture(const quint8 *data, quint16 w, quint16 h) {
        glActiveTexture(GL_TEXTURE0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    }

    void MainPainter::setPalette(const quint8 *data) {
        glActiveTexture(GL_TEXTURE1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }

    void MainPainter::setOffset(float offset0, float offset1, float offset2, float offset3) {
        QVector4D offsets(offset0, offset1, offset2, offset3);
        getProgram()->setUniformValue(m_uniformLoc[0], offsets);
    }

    void MainPainter::setLayouts(qint32 *layouts) {
        makeCurrent();
        getProgram()->bind();
        getProgram()->setUniformValueArray(m_uniformLoc[3], layouts, 4);
    }

    void MainPainter::initializeGL(void) {
        Painter::initializeGL();
        glEnable(GL_TEXTURE_2D);
        glGenTextures(2, m_textureHandle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureHandle[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_textureHandle[1]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    void MainPainter::paintGL(void) {
        getProgram()->bind();
        glDrawArrays(GL_POINTS, 0, m_count);
        qglColor(QColor(0, 0, 0));
        renderText(200, 200, "Юля", QFont("Times", 40, QFont::Black, true));
        renderText(135, 300, "Ein heller Schein am Firmament", QFont("Times", 12, QFont::Bold, true));
        renderText(190, 320, "Mein Herz brennt", QFont("Times", 12, QFont::Bold, true));
    }

}

