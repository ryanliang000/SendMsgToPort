# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

QT       += core gui network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

TARGET = SendMsgToPort

HEADERS += \
   SendMsgToPort/MyTcpClient.h \
   SendMsgToPort/sendmsgtoport.h \
   SendMsgToPort/TextHexDecode.h \
    SendMsgToPort/MyUdpClient.h

SOURCES += \
   SendMsgToPort/main.cpp \
   SendMsgToPort/MyTcpClient.cpp \
   SendMsgToPort/sendmsgtoport.cpp \
   SendMsgToPort/TextHexDecode.cpp \
    SendMsgToPort/MyUdpClient.cpp

FORMS += SendMsgToPort/sendmsgtoport.ui
#DEFINES = 

