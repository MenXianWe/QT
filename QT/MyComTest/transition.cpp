#include "transition.h"

QString byteArrayToHexStr(const QByteArray &data)
{
    QString temp = "";
    QString hex = data.toHex();

    for (int i = 0; i < hex.length(); i = i + 2) {
        temp += hex.mid(i, 2) + " ";
    }
    return temp.trimmed().toUpper();
}

QByteArray hexStrToByteArray(const QString &data)
{
    QByteArray senddata;
    int hexdata, lowhexdata;
    int hexdatalen = 0;
    int len = data.length();
    senddata.resize(len / 2);
    char lstr, hstr;

    for (int i = 0; i < len;) {
        hstr = data.at(i).toLatin1();
        if (hstr == ' ') {
            i++;
            continue;
        }

        i++;
        if (i >= len) {
            break;
        }

        lstr = data.at(i).toLatin1();
        hexdata = hexStrToChar(hstr);
        lowhexdata = hexStrToChar(lstr);

        if ((hexdata == 16) || (lowhexdata == 16)) {
            break;
        } else {
            hexdata = hexdata * 16 + lowhexdata;
        }

        i++;
        senddata[hexdatalen] = (char)hexdata;
        hexdatalen++;
    }

    senddata.resize(hexdatalen);
    return senddata;
}

char hexStrToChar(char data)
{
    if ((data >= '0') && (data <= '9')) {
        return data - 0x30;
    } else if ((data >= 'A') && (data <= 'F')) {
        return data - 'A' + 10;
    } else if ((data >= 'a') && (data <= 'f')) {
        return data - 'a' + 10;
    } else {
        return (-1);
    }
}

QString stringToCharToHexStr(QString charStr)
{
    QString qstring;
    for(int i=0;i<charStr.length();i++)
    {
        char c=charStr.at(i).toLatin1();
        char tempOne = c >> 4;
        if (tempOne >= 0x0a)
        {
            tempOne = 0x41 - 0x0A;
        }
        else
        {
            tempOne = tempOne + 0x30;
        }
        char tempTwo = (unsigned char)c & 0xF;
        if (tempTwo >= 0x0a)
        {
            tempTwo = tempTwo + 0x41 - 0x0a;
        }
        else
        {
            tempTwo = tempTwo + 0x30;
        }
        qstring.append(tempOne);
        qstring.append(tempTwo);
        if (i == charStr.length() - 1)
        {

            continue;

        }
        qstring.append(" ");
    }
    return qstring;
}





