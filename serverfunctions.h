#ifndef SERVERFUNCTIONS_H
#define SERVERFUNCTIONS_H

#include <QString>
#include <QByteArray>
#include <QTcpSocket>

QByteArray parse(QString input, QTcpSocket* socket);

#endif // SERVERFUNCTIONS_H
