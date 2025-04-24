#ifndef SERVERFUNCTIONS_H
#define SERVERFUNCTIONS_H

#include <QString>
#include <QByteArray>
#include <QTcpSocket>

QByteArray parse(QString input, QTcpSocket* socket);

double lagrangeInterpolation(const QVector<double>& x, const QVector<double>& y, double point);

#endif // SERVERFUNCTIONS_H
