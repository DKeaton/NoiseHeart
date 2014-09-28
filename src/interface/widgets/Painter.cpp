#include "Painter.h"

namespace Interface {

    Painter::Painter(quint16 w, quint16 h, QWidget *parent) :
        QGLWidget(parent), m_w(w), m_h(h),
        m_program(new QGLShaderProgram(this)),
        m_inited(false) {
        setFixedSize(m_w, m_h);
    }

    Painter::~Painter(void) {
        delete m_program;
    }

    void Painter::setShaderAddress(const QString &address) {
        makeCurrent();
        m_program->addShaderFromSourceFile(QGLShader::Vertex, address + ".vert");
        m_program->addShaderFromSourceFile(QGLShader::Fragment, address + ".frag");
        m_program->bind();
    }

    QGLShaderProgram *Painter::getProgram(void) {
        return m_program;
    }

    void Painter::initializeGL(void) {
        //glewInit();
        initializeGLFunctions();
        glEnableClientState(GL_VERTEX_ARRAY);
    }

    void Painter::resizeGL(int w, int h) {
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluOrtho2D(-w / 2, w / 2 - 1, -h / 2, h / 2 - 1);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
    }

}
