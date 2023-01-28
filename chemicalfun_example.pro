TEMPLATE = app
CONFIG += thread console c++17
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += sanitaze sanitaze_thread

!win32 {
  DEFINES += __unix

QMAKE_CFLAGS += pedantic -Wall -Wextra -Wwrite-strings -Werror
#QMAKE_CXXFLAGS += -ansi -pedantic -Wall -Wextra -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith \
 -Wmissing-declarations -Winline -Wundef \ #-Weffc++ \
 -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter \
 -Wfloat-equal -pedantic -ansi

#QMAKE_CXXFLAGS += -pg  # gprof information
}

macx-g++ {
  DEFINES += __APPLE__
}

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

# Define the directory where source code is located
CHEMICALFUN_DIR =  $$PWD/src
CHEMICALFUN_HEADERS_DIR =  $$PWD/include

DEPENDPATH   += $$CHEMICALFUN_DIR
DEPENDPATH   += $$CHEMICALFUN_HEADERS_DIR

INCLUDEPATH   += $$CHEMICALFUN_DIR
INCLUDEPATH   += $$CHEMICALFUN_HEADERS_DIR

#LIBS += -pg  # gprof information
#unix:!macx-clang:LIBS += -pg  -lstdc++fs

OBJECTS_DIR   = obj

include($$CHEMICALFUN_DIR/chemicalfun.pri)

SOURCES += \
#   examples/elements_example.cpp \
#      examples/dbelements_example.cpp \
#      examples/formula_parser_example.cpp
    examples/thread_example.cpp

