QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += thirdparty/nlohmann_json/single_include/nlohmann/

SOURCES += \
    book.cpp \
    bookdialog.cpp \
    bookmodel.cpp \
    costitemsmodel.cpp \
    main.cpp \
    mainwindow.cpp \
    settingsdialog.cpp

HEADERS += \
    book.h \
    bookdialog.h \
    bookmodel.h \
    costitemsmodel.h \
    mainwindow.h \
    settingsdialog.h

FORMS += \
    bookdialog.ui \
    mainwindow.ui \
    settingsdialog.ui

RESOURCES = resource.qrc

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
