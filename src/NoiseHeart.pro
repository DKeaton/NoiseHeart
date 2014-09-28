#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T20:32:37
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NoiseHeart
TEMPLATE = app

SOURCES += main.cpp \
    interface/controlPanels/basic/BasicControlPanel.cpp \
    interface/controlPanels/basic/CheckBoxSet.cpp \
    utility/exceptions/CommonException.cpp \
    MainApplication.cpp \
    interface/controlPanels/basic/SliderSet.cpp \
    interface/widgets/ColorBar.cpp \
    interface/controlPanels/complex/ColorControl.cpp \
    interface/MainWindow.cpp \
    interface/controlPanels/complex/CommonControl.cpp \
    interface/controlPanels/complex/ComplexControlPanel.cpp \
    utility/Requestable.cpp \
    interface/controlPanels/complex/LayoutControl.cpp \
    interface/widgets/Painter.cpp \
    utility/FormPixmap.cpp \
    engine/ShaderController.cpp \
    interface/widgets/MainPainter.cpp \
    interface/widgets/AnalysisPainter.cpp \
    utility/buffer/Buffer.cpp \
    utility/buffer/BufData.cpp \
    engine/generator/GenController.cpp \
    engine/generator/GenData.cpp \
    engine/generator/Generator.cpp \
    engine/generator/GenProcessor.cpp \
    engine/generator/NoiseSettings.cpp \
    engine/generator/SettingsCorrector.cpp \
    engine/TimingController.cpp \
    DataStore.cpp

HEADERS  += \
    interface/controlPanels/basic/BasicControlPanel.h \
    interface/controlPanels/basic/CheckBoxSet.h \
    utility/exceptions/CommonException.h \
    utility/exceptions/exceptions.h \
    MainApplication.h \
    interface/controlPanels/basic/SliderSet.h \
    interface/widgets/ColorBar.h \
    interface/controlPanels/complex/ColorControl.h \
    interface/MainWindow.h \
    interface/controlPanels/complex/CommonControl.h \
    interface/controlPanels/complex/ComplexControlPanel.h \
    utility/Requestable.h \
    interface/controlPanels/complex/LayoutControl.h \
    interface/widgets/Painter.h \
    utility/FormPixmap.h \
    engine/ShaderController.h \
    interface/widgets/MainPainter.h \
    interface/widgets/AnalysisPainter.h \
    utility/buffer/Buffer.h \
    utility/buffer/BufData.h \
    engine/generator/GenController.h \
    engine/generator/GenData.h \
    engine/generator/Generator.h \
    engine/generator/GenProcessor.h \
    engine/generator/NoiseSettings.h \
    engine/generator/SettingsCorrector.h \
    engine/TimingController.h \
    DataStore.h

OTHER_FILES += \
    GLSL/main.frag \
    GLSL/main.vert \
    GLSL/analysis.frag \
    GLSL/analysis.vert

LIBS += -Lqwindows
