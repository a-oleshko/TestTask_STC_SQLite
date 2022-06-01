QT       += core gui xml widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    broker_sqlite.cpp \
    broker_xml.cpp \
    delegateeditablerow.cpp \
    itemclass.cpp \
    main.cpp \
    OnlyWindow.cpp \
    model_editortable.cpp \
    windowimportxml.cpp

HEADERS += \
    OnlyWindow.h \
    broker_sqlite.h \
    broker_xml.h \
    delegateeditablerow.h \
    itemclass.h \
    model_editortable.h \
    windowimportxml.h

FORMS += \
    OnlyWindow.ui \
    windowimportxml.ui

CONFIG += lrelease

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
