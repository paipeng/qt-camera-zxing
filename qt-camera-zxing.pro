QT       += core gui

QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    qt-camera-zxing_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations


RESOURCES += \
    qt-camera-zxing.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




INCLUDEPATH += $$PWD/../cp_qt_camera/



INCLUDEPATH += $$PWD/../cp-qt-barcode/
INCLUDEPATH += $$PWD/../cp-qt-barcode/zxing

DEPENDPATH += $$PWD/libs/x64




win32:CONFIG (release, debug|release): LIBS += -L$$PWD/libs/x64/ -lcp_qt_camera -lcp-qt-barcode.1.0.0
else:win32:CONFIG (debug, debug|release): LIBS += -L$$PWD/libs/x64/ -lcp_qt_camera -lcp-qt-barcode.1.0.0
else:unix: LIBS += -L$$PWD/libs/x64/ -lcp_qt_camera.1 -lcp-qt-barcode.1 -L$$PWD/../cp-qt-barcode/libs/x64/



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libs/x64/release/ -lZXing
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libs/x64/debug/ -lZXing
else:unix: LIBS += -L$$PWD/libs/x64/ -lZXing.1




DYLD_LIBRARY_PATH += -L$$PWD/libs/x64/
DYLD_LIBRARY_PATH += -L$$PWD/../cp-qt-barcode/libs/x64/
