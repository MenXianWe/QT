#ifndef TRANSITION_H
#define TRANSITION_H

#include <QString>
#include <QStringList>


/***转换函数***/

 QString byteArrayToHexStr(const QByteArray &data);//QByteArray类型转换为QString
 QByteArray hexStrToByteArray(const QString &data);//将16进制的字符串转换成QByteArray
 char hexStrToChar(char data);                     //将16进制字符串转换成每一个字符
 QString stringToCharToHexStr(QString str);        //将字符串转换成字符，再将字符转换成16进制字符串


#endif // TRANSITION_H
