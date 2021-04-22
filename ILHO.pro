QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    dialog.cpp \
    qcustomplot.cpp \
    scatterdialog.cpp

HEADERS += \
    dialog.h \
    qcustomplot.h \
    scatterdialog.h

FORMS += \
    dialog.ui \
    scatterdialog.ui

#INCLUDEPATH += "/Applications/CPLEX_Studio1210/cplex/include/"
#INCLUDEPATH += "/Applications/CPLEX_Studio1210/concert/include"

#LIBS += "/Applications/CPLEX_Studio1210/cplex/lib/x86-64_osx/static_pic/lib/libcplex.a"


RC_ICONS = ilho.ico
macx-clang {
    ICON = ilho.icns
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#QMAKE_CXXFLAGS += -stdlib=libc++
#QMAKE_CXXFLAGS += -stdlib=libstdc++


INCLUDEPATH += "/usr/local/Cellar/glpk/5.0/include/"
LIBS +=  "/usr/local/Cellar/glpk/5.0/lib/libglpk.dylib"

DISTFILES += \
    ilho.icns
