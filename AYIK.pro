# -------------------------------------------------
# Project created by QtCreator 2009-09-17T22:56:45
# -------------------------------------------------
QT += network \
    sql \
    xml \
    widgets \
    core5compat
TARGET = AYIK
TEMPLATE = app
SOURCES += main.cpp \
    ayiktray.cpp \
    dictutil.cpp \
    ayikdb.cpp \
    ayikoption.cpp \
    starrating.cpp \
    ayikpopup.cpp \
    ayikword.cpp \
    ayikrating.cpp \
    tableview.cpp \
    tablemodel.cpp \
    itemdelegate.cpp \
    databaseconnection.cpp \
    ayikweb.cpp \
    loadingwidget.cpp
HEADERS += ayiktray.h \
    dictutil.h \
    ayikdb.h \
    ayikoption.h \
    starrating.h \
    ayikpopup.h \
    ayikword.h \
    ayikrating.h \
    tablemodel.h \
    itemdelegate.h \
    tableview.h \
    databaseconnection.h \
    ayikweb.h \
    loadingwidget.h
FORMS += 
RESOURCES += AYIK.qrc
TRANSLATIONS = AYIKtr_la.ts
RC_FILE = AYIK.rc
OTHER_FILES += AYIK.rc \
    todo.txt
