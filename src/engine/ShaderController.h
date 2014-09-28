#ifndef SHADERCONTROLLER_H
#define SHADERCONTROLLER_H

#include <QVector4D>

#include <QDebug>

#include "interface/widgets/MainPainter.h"
#include "interface/widgets/AnalysisPainter.h"
#include "utility/FormPixmap.h"

class ShaderController : public QObject {
    Q_OBJECT
    static const float m_coeff;

public:
    ShaderController(quint16 mtw, quint16 mth, quint16 mSide, QObject *parent = 0);
    ~ShaderController(void);
    void setPainters(Interface::MainPainter *pMain, Interface::AnalysisPainter *pAnalysis);
    void setShaderFolder(const QString &address);
    void setSpeed(quint8 layout, quint8 fps, quint8 speed);
    void setCurAnalysisLayout(quint8 layout);
    void setLayoutOn(quint8 layout, bool on);
    void setPalette(quint8 layout, const quint8 *palette);

public slots:
    void updateFrame(void);

private:
    float calcOffset(quint8 fps, quint8 speed);
    void calcAnalysis(void);

    const quint16 m_mtw, m_mth, m_mSide;
    const float m_singleStep;
    quint8 *m_mTextureData;
    quint8 *m_aTextureData;
    quint8 *m_paletteTextureData;
    float *m_aVertices;
    float *m_aColors;
    Interface::MainPainter *m_main;
    Interface::AnalysisPainter *m_analysis;
    qint8 m_curLayout;
    float m_offset[4];
    float m_curOffset[4];
    FormPixmap *m_form;
    qint32 m_layouts[4];

signals:
    void requestLine(quint8 *dst, quint8 layout);
    void frameUpdated(void);
};

#endif // SHADERCONTROLLER_H
