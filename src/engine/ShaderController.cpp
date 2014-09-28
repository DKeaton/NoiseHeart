#include "ShaderController.h"

const float ShaderController::m_coeff = 0.9f / 40.0f;

ShaderController::ShaderController(quint16 mtw, quint16 mth, quint16 mSide, QObject *parent)
    : QObject(parent),
      m_mtw(mtw), m_mth(mth), m_mSide(mSide),
      m_singleStep(1.0f / m_mth),
      m_mTextureData(new quint8[m_mtw * (m_mth + 1) * 4]),
      m_aTextureData(new quint8[m_mSide * 4]),
      m_paletteTextureData(new quint8[256 * 4 * 4]),
      m_aVertices(new float[m_mSide * 2]),
      m_aColors(new float[m_mSide * 3]),
      m_form(new FormPixmap(m_mSide, m_mtw, m_mth)) {
    memset(m_mTextureData, 0, m_mtw * (m_mth + 1) * 4);
    memset(m_aTextureData, 0, m_mSide * 4);
    memset(m_paletteTextureData, 0, 256 * 4 * 4);
    for (quint16 i = 0; i < m_mSide; i++) {
        m_aVertices[i * 2 + 1] = i / (m_mSide / 2.0f) - 1;
    }
    memset(m_curOffset, 0, sizeof(float) * 4);
    memset(m_layouts, 0, 4 * sizeof(qint32));
    m_offset[0] = calcOffset(25, 100);
    m_offset[1] = calcOffset(25, 75);
    m_offset[2] = calcOffset(25, 50);
    m_offset[3] = calcOffset(25, 40);
    for (quint8 i = 0; i < 4; i++) {
        emit requestLine(&m_mTextureData[i * m_mtw * (m_mth + 1)], m_mtw);
    }
}

ShaderController::~ShaderController(void) {
    delete[] m_mTextureData;
    delete[] m_aTextureData;
    delete[] m_paletteTextureData;
    delete[] m_aVertices;
    delete m_form;
}

void ShaderController::setPainters(Interface::MainPainter *pMain, Interface::AnalysisPainter *pAnalysis) {
    m_main = pMain;
    m_analysis = pAnalysis;
}

void ShaderController::setShaderFolder(const QString &address) {
    m_main->setShaderAddress(address + "/main");
    m_analysis->setShaderAddress(address + "/analysis");
    m_main->setVertices(m_form->getVertices(), m_form->getParams(), m_form->getSize());
    m_main->setPalette(m_paletteTextureData);
    m_main->setTexture(m_mTextureData, m_mtw, (m_mth + 1) * 4);
}

void ShaderController::setSpeed(quint8 layout, quint8 fps, quint8 speed) {
    m_offset[layout] = calcOffset(fps, speed);
}

void ShaderController::setCurAnalysisLayout(quint8 layout) {
    m_curLayout = layout;
}

void ShaderController::setLayoutOn(quint8 layout, bool on) {
    m_layouts[layout] = on ? 1 : 0;
    m_main->setLayouts(m_layouts);
}

void ShaderController::setPalette(quint8 layout, const quint8 *palette) {
    memcpy(&m_paletteTextureData[layout * 256 * 4], palette, 256 * 4);
    m_main->makeCurrent();
    m_main->setPalette(m_paletteTextureData);
}

void ShaderController::updateFrame(void) {
    bool newTex = false;
    for (quint8 i = 0; i < 4; i++) {
        if (m_layouts[i] == 1) {
            m_curOffset[i] -= m_offset[i];
            while (m_curOffset[i] < 0) {
                newTex = true;
                m_curOffset[i] += m_singleStep;
                quint16 sIndex = i * (m_mth + 1);
                for (quint16 j = m_mth; j > 0; j--) {
                    memcpy(&m_mTextureData[(j + sIndex) * m_mtw],
                            &m_mTextureData[(j - 1 + sIndex) * m_mtw],
                            m_mtw);
                }
                emit requestLine(&m_mTextureData[sIndex * m_mtw], i);
                memcpy(&m_aTextureData[i * m_mSide],
                        &m_mTextureData[sIndex * m_mtw],
                        m_mSide);
            }
        }
    }
    m_main->setOffset(m_curOffset[0], m_curOffset[1], m_curOffset[2], m_curOffset[3]);
    m_main->makeCurrent();
    if (newTex) {
        m_main->setTexture(m_mTextureData, m_mtw, (m_mth + 1) * 4);
    }
    m_main->updateGL();
    m_analysis->makeCurrent();
    calcAnalysis();
    if (newTex) {
        m_analysis->setVertices(m_aVertices, m_mSide);
    }
    m_analysis->setColors(m_aColors);
    m_analysis->updateGL();
    emit frameUpdated();
}

float ShaderController::calcOffset(quint8 fps, quint8 speed) {
    return m_singleStep / fps * speed / 100.0f / m_coeff;
}

void ShaderController::calcAnalysis(void) {
    for (quint16 i = 0; i < m_mSide; i++) {
        memset(&m_aColors[i * 3], 0, sizeof(float) * 3);
        m_aVertices[i * 2] = 0.0f;
        quint8 s, e;
        if (m_curLayout == 4) {
            s = 0; e = 3;
        } else if (m_curLayout != -1) {
            s = e = m_curLayout;
        }
        for (quint8 j = s; j <= e; j++) {
            if (m_layouts[j] == 1.0f) {
                for (quint8 k = 0; k < 3; k++) {
                    m_aColors[i * 3 + k] = qMax((quint8)m_aColors[i * 3 + k], m_paletteTextureData[(j * 256 + m_aTextureData[j * m_mSide + i]) * 4 + k]);
                }
            }
        }
        for (quint8 j = 0; j < 3; j++) {
            m_aVertices[i * 2] = qMax(m_aVertices[i * 2], m_aColors[i * 3 + j]);
            m_aColors[i * 3 + j] /= 255.0f;
        }
        m_aVertices[i * 2] = (m_aVertices[i * 2] - 128) / 128.0f;
    }
}
