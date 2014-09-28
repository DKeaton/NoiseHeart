#ifndef PAINTER_H
#define PAINTER_H

#include <QGLWidget>
#include <QGLFunctions>
#include <GL/glu.h>
#include <QGLShaderProgram>

#include <QDebug>

namespace Interface {

    class Painter : public QGLWidget, protected QGLFunctions {
    public:
        Painter(quint16 w, quint16 h, QWidget *parent = 0);
        ~Painter(void);
        virtual void setShaderAddress(const QString &address);

    protected:
        QGLShaderProgram *getProgram(void);
        void initializeGL(void);

    private:
        void resizeGL(int w, int h);

        const quint16 m_w, m_h;
        GLuint m_type;
        quint32 m_count;
        QGLShaderProgram *m_program;
        quint32 m_shaders[2];

        bool m_inited;
    };

}

#endif // PAINTER_H
