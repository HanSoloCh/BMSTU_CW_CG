QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    abstractmodel.cpp \
    canvas.cpp \
    carcasmodel.cpp \
    curve.cpp \
    curvecanvas.cpp \
    drawvisitor.cpp \
    facade.cpp \
    light.cpp \
    main.cpp \
    point.cpp \
    solutionviewer.cpp \
    strategy.cpp \
    triangle.cpp \
    mainwindow.cpp

HEADERS += \
    abstractmodel.h \
    canvas.h \
    carcasmodel.h \
    curve.h \
    curvecanvas.h \
    drawvisitor.h \
    facade.h \
    light.h \
    point.h \
    solutionviewer.h \
    strategy.h \
    triangle.h \
    mainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
