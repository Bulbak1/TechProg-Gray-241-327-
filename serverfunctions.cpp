#include "serverfunctions.h"
#include <QStringList>
QByteArray auth(QString login, QString password){
    return QByteArray("welcome\r\n");
}
QByteArray reg(QString login, QString password){
    return QByteArray("sucess\r\n");
}
QByteArray default_answer(){
    return QByteArray("unknown command\r\n");
}

QByteArray parse(QString input1)
{
    QStringList args = input1.remove("\r\n").split('&');
    if (args[0] == "auth"){
        if (args.size() == 3){ return auth(args[1], args[2]); }
        else{ return default_answer(); }
    } else if (args[0] == "reg"){
        if (args.size() == 3){ return reg(args[1], args[2]); }
        else{ return default_answer(); }
    } else{ return default_answer(); }
}
