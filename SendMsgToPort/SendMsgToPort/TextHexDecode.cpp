#include "TextHexDecode.h"
#include "qregularexpression.h"

CTextHexDecode::CTextHexDecode(void)
{
}


CTextHexDecode::~CTextHexDecode(void)
{
}

QString CTextHexDecode::textToAnsiHex(QString sText)
{
    QString sReturnText = CTextHexDecode::byteArrayToHex(sText.toLocal8Bit());
    return  sReturnText;
}

QString CTextHexDecode::textToUtf8Hex(QString sText)
{
    QString sReturnText = CTextHexDecode::byteArrayToHex(sText.toUtf8());
    return  sReturnText;
}

QString CTextHexDecode::textToUnicodeHex(QString sText)
{
    QString sReturnText = CTextHexDecode::wstringToHex(sText.toStdWString());
    return  sReturnText;
}

QString CTextHexDecode::ansiHexToText(QString sHexText)
{
    // 清理Hex中的异常字符
    sHexText = sHexText.remove(QRegularExpression(QString("[^0-9a-fA-F]"), QRegularExpression::NoPatternOption));
    QByteArray srcByteArray = sHexText.toLocal8Bit();
    QByteArray destByteArray;
    for (int i=1; i<=srcByteArray.length(); i=i+2)
    {
        destByteArray.append(convertHexToChar(srcByteArray[i-1], srcByteArray[i]));
    }
    return  QString::fromLocal8Bit(destByteArray);
}

QString CTextHexDecode::utf8HexToText(QString sHexText)
{
    sHexText = sHexText.remove(QRegularExpression(QString("[^0-9a-fA-F]"), QRegularExpression::NoPatternOption));
    QByteArray srcByteArray = sHexText.toLocal8Bit();
    QByteArray destByteArray;
    for (int i=1; i<=srcByteArray.length(); i=i+2)
    {
        destByteArray.append(convertHexToChar(srcByteArray[i-1], srcByteArray[i]));
    }
    return  QString::fromUtf8(destByteArray);
}

QString CTextHexDecode::unioceHexToText(QString sHexText)
{
    sHexText = sHexText.remove(QRegularExpression(QString("[^0-9a-fA-F]"), QRegularExpression::NoPatternOption));
    QByteArray srcByteArray = sHexText.toLocal8Bit();
    std::wstring dest;
    for (int i=3; i<=srcByteArray.length(); i=i+4)
    {
        wchar_t ch = convertHexToWChar(srcByteArray[i-3], srcByteArray[i-2], srcByteArray[i-1], srcByteArray[i]);
        dest.push_back(ch);
    }
    return  QString::fromStdWString(dest);
}

inline char convertHexToAsc(unsigned char ch)
{
    if((ch >= 0) && (ch <= 9))
        return '0' + ch;
    else
        return 'A' + ch - 10;
}
QString CTextHexDecode::convertCharToHex(char ch)
{
    // low
    unsigned char ch1 = (ch & 0xF0) >> 4;
    unsigned char ch2 = (ch & 0x0F);

    return QString("%1%2 ")
        .arg(convertHexToAsc(ch1))
        .arg(convertHexToAsc(ch2));
}
QString CTextHexDecode::convertWCharToHex(wchar_t ch)
{
    // 输出格式little Endian（低位在前，高位在后）
    char chHigh = (ch & 0xFF00) >> 8;
    char chLow = (ch & 0x00FF);
    return convertCharToHex(chLow) + convertCharToHex(chHigh);
}
QString CTextHexDecode::byteArrayToHex(const QByteArray& bytArray)
{
    QString sReturnText;
    for (int i=0; i<bytArray.length(); i++)
    {
        sReturnText.append(convertCharToHex(bytArray[i]));
    }
    return sReturnText;
}

QString CTextHexDecode::wstringToHex(const std::wstring& str)
{
    QString sReturnText;
    for (size_t i=0; i<str.length(); i++)
    {
        sReturnText.append(convertWCharToHex(str.at(i)));
    }
    return sReturnText;
}

inline unsigned char convertAscToHex(unsigned char ch)
{
    if((ch >= '0') && (ch <= '9'))
        return ch - '0';
    else if((ch >= 'a') && (ch <= 'f'))
        return ch - 'a' + 10;
    else
        return ch - 'A' + 10;
}

wchar_t CTextHexDecode::convertHexToWChar(char ch1, char ch2, char ch3, char ch4)
{
    // Hex输出格式little Endian（低位在前，高位在后）
    unsigned short charLow = convertHexToChar(ch1, ch2);
    unsigned short charHigh =  convertHexToChar(ch3, ch4);
    return ( charHigh << 8) | charLow;
}

unsigned char CTextHexDecode::convertHexToChar(char ch1, char ch2)
{
    return (convertAscToHex(ch1) << 4) | convertAscToHex(ch2);
}
