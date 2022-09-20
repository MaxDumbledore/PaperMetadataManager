QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    $$files(Tree/*.cpp) \
    config.cpp \
    cstbuttongroup.cpp \
    cstsqlquerymodel.cpp \
    globalsettings.cpp \
    main.cpp \
    mainwindow.cpp \
    metadatamanager.cpp \
    tableeditdialog.cpp \
    tableform.cpp \
    treeform.cpp \
    utils.cpp

HEADERS += \
    $$files(Tree/*.h) \
    config.h \
    cstbuttongroup.h \
    cstsqlquerymodel.h \
    globalsettings.h \
    mainwindow.h \
    metadatamanager.h \
    tableeditdialog.h \
    tableform.h \
    tree.h \
    treeform.h \
    utils.h

FORMS += \
    mainwindow.ui \
    tableeditdialog.ui \
    tableform.ui \
    treeform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    README.md
