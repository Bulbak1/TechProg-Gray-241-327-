#ifndef SERVERFUNCTIONS_H
#define SERVERFUNCTIONS_H

#include <QTcpSocket>
#include <QByteArray>

QByteArray parse(const QByteArray& request, QTcpSocket* socket);

#endif // SERVERFUNCTIONS_H
