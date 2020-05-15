QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

RC_ICONS = icon.ico

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    artisttemplate.cpp \
    currency.cpp \
    logging.cpp \
    main.cpp \
    mainwindow.cpp \
    settings.cpp

HEADERS += \
    artisttemplate.h \
    currency.h \
    logging.h \
    mainwindow.h \
    settings.h

FORMS += \
    artisttemplate.ui \
    mainwindow.ui \
    settings.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/../../../../../CppLibs/qtcsv/build/release/ -lqtcsv1

INCLUDEPATH += $$PWD/../../../../../CppLibs/qtcsv/include
DEPENDPATH += $$PWD/../../../../../CppLibs/qtcsv/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../../../../../CppLibs/qtcsv/build/release/qtcsv1.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../../../../../CppLibs/qtcsv/build/release/libqtcsv1.a

RESOURCES += \
    resources.qrc
