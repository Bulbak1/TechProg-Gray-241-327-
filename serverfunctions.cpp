#include "serverfunctions.h"
#include "DatabaseManager.h"
#include <QDebug>
#include <QMap>

double f(double x) {
    return x * 2 * (x * x + 1);
}

double parabolaMethod(double a, double b, int n) {
    if (n % 2 != 0) ++n;
    double h = (b - a) / n;
    double sum = f(a) + f(b);

    for (int i = 1; i < n; ++i) {
        double x = a + i * h;
        sum += (i % 2 == 0) ? 2 * f(x) : 4 * f(x);
    }

    return sum * h / 3.0;
}

// Состояния клиентов
static QMap<QString, bool> authenticatedMap;
static QMap<QString, QString> userMap;

QByteArray parse(const QByteArray& request, const QString userID) {
    QString msg = QString::fromUtf8(request).trimmed();
    qDebug() << "Raw request:" << request;

    if (!authenticatedMap.value(userID, false)) {
        QStringList parts = msg.split('&');
        if (parts[0] == "reg") {
            if (parts.size() != 3) return QByteArray("error");
            QString login = parts[1];
            QString pass = parts[2];

            bool flag = DatabaseManager::instance()->reg(login, pass);

            if (flag) {
                authenticatedMap[userID] = true;
                userMap[userID] = login;
                return QByteArray("sucess");
            }

            return QByteArray("error");
        } else if (parts[0] == "auth") {
            if (parts.size() != 3) return QByteArray("error");
            QString login = parts[1];
            QString pass = parts[2];

            bool flag = DatabaseManager::instance()->auth(login, pass);

            if (flag) {
                authenticatedMap[userID] = true;
                userMap[userID] = login;
                return QByteArray("sucess");
            }
            return QByteArray("error");
        }
    }else if (msg.split('&')[0] == "f"){
        QStringList parts = msg.split('&');
        if (parts.size() != 4) return QByteArray("error");

        bool ok1, ok2, ok3;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        int n = parts[3].toInt(&ok3);
        if (!ok1 || !ok2 || !ok3) return QByteArray("error");

        double result = parabolaMethod(a, b, n);
        return QByteArray(QString("res %1").arg(result).toUtf8());
    }
    return QByteArray("");
}
