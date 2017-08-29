#pragma once
#include "qstring.h"
class CTextHexDecode
{
public:
    CTextHexDecode(void);
    ~CTextHexDecode(void);
public:
    QString textToAnsiHex(QString sText);
    QString textToUtf8Hex(QString sText);
    QString textToUnicodeHex(QString sText);

    QString ansiHexToText(QString sHexText);
    QString utf8HexToText(QString sHexText);
    QString unioceHexToText(QString sHexText);
public:
    // char/w_char to hex
    static QString convertWCharToHex(wchar_t ch);
    static QString convertCharToHex(char ch);
    static QString byteArrayToHex(QByteArray& bytArray);
    static QString wstringToHex(std::wstring& str);

    // hex to char/w_char
    static wchar_t convertHexToWChar(char ch1, char ch2, char ch3, char ch4);
    static unsigned char convertHexToChar(char ch1, char ch2);
};

