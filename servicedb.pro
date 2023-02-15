TEMPLATE = app
CONFIG += console
CONFIG -= qt

HEADERS += \
    src/serverdb.h \
    src/request_parser.h \
    src/request_handler.h \
    src/request.h \
    src/reply.h \
    src/logger.h \
    src/http_status.h \
    src/http_header.h \
    src/clientsession.h

SOURCES += main.cpp \
    src/serverdb.cpp \
    src/request_parser.cpp \
    src/request_handler.cpp \
    src/reply.cpp \
    src/logger.cpp \
    src/http_status.cpp \
    src/clientsession.cpp

INCLUDEPATH +=src

win32 {
    BOOSTHOME = c:/Boost/include/boost-1_81

    INCLUDEPATH += $$BOOSTHOME \
      $$BOOSTHOME/boost \
      c:/openssl-1.1.1t/include

    LIBS += -lws2_32
    LIBS += -L"c:/Boost/lib/" -llibboost_thread-mgw7-mt-x64-1_81
    LIBS += -L"c:/openssl-1.1.1t/" -lssl -lcrypto
}



