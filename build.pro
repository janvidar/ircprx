QT -= core
QT -= gui

CONFIG += debug

LIBS += -lsamurai
LIBS += -lbz2
LIBS += -lssl

HEADERS = irclink.h   channel.h   identity.h   ircmsg.h   ircserver.h   ircconfig.h   user.h
SOURCES = irclink.cpp channel.cpp identity.cpp ircmsg.cpp ircserver.cpp ircconfig.cpp user.cpp main.cpp

