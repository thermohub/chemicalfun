TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG += thread
CONFIG -= qt

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15

!win32 {
  DEFINES += __unix
}

macx-g++ {
  DEFINES += __APPLE__
}

# Define the directory where source code is located
CHEMICALFUN_DIR =  $$PWD/src
CHEMICALFUN_HEADERS_DIR =  $$PWD/include
TESTS_DIR =  $$PWD/tests

DEPENDPATH   += $$CHEMICALFUN_DIR
DEPENDPATH   += $$CHEMICALFUN_HEADERS_DIR
DEPENDPATH   += $$TESTS_DIR

INCLUDEPATH   += $$CHEMICALFUN_DIR
INCLUDEPATH   += $$CHEMICALFUN_HEADERS_DIR
INCLUDEPATH   += $$TESTS_DIR


macx-clang {
  DEFINES += __APPLE__
  INCLUDEPATH   += "/usr/local/include"
  DEPENDPATH   += "/usr/local/include"
  LIBPATH += "/usr/local/lib/"
}

win32 {
  INCLUDEPATH   += "C:\usr\local\include"
  DEPENDPATH   += "C:\usr\local\include"
  LIBPATH += "C:\usr\local\lib"
}


OBJECTS_DIR   = obj

include($$TESTS_DIR/gtest_dependency.pri)
include($$CHEMICALFUN_DIR/chemicalfun.pri)


HEADERS += \
        $$TESTS_DIR/tst_parser.h \

SOURCES += \
        $$TESTS_DIR/main.cpp

