QT += testlib
QT += xml
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_calculatecurrentandvoltage_tests.cpp \
            ../circuitMaster_main/circuitConnection.cpp \
            ../circuitMaster_main/circuitElement.cpp \
            ../circuitMaster_main/ioFunctions.cpp \
            ../circuitMaster_main/testFunctions.cpp

HEADERS += ../circuitMaster_main/circuitConnection.h \
            ../circuitMaster_main/circuitElement.h \
            ../circuitMaster_main/ioFunctions.h \
            ../circuitMaster_main/testFunctions.h
