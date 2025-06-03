#include "serverfunctions.h"
#include "DatabaseManager.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>
#include <QMap>

double f(double x) {
    return x * x; // Пример: f(x) = x^2
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
static QMap<QTcpSocket*, bool> authenticatedMap;
static QMap<QTcpSocket*, QString> userMap;

QByteArray parse(const QByteArray& request, QTcpSocket* socket) {
    QString msg = QString::fromUtf8(request).trimmed();
    QSqlDatabase db = DatabaseManager::instance().getDatabase();
    QSqlQuery query(db);

    if (!authenticatedMap.value(socket, false)) {
        if (msg == "1") {
            socket->write("Введите логин:\n");
            socket->waitForReadyRead(30000);
            QString login = socket->readAll().trimmed();

            socket->write("Введите пароль:\n");
            socket->waitForReadyRead(30000);
            QString pass = socket->readAll().trimmed();

            query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT)");

            query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
            query.bindValue(":u", login);
            query.bindValue(":p", pass);

            if (query.exec()) {
                authenticatedMap[socket] = true;
                userMap[socket] = login;
                return "Регистрация успешна. Введите f(x)&a&b&n:\n";
            } else {
                qDebug() << "Login:" << login << "Pass:" << pass;
                qDebug() << "addPerson error:"
                         << query.lastError();
                return "Ошибка: возможно, пользователь уже существует.\n";
            }
        } else if (msg == "2") {
            socket->write("Введите логин:\n");
            socket->waitForReadyRead(30000);
            QString login = socket->readAll().trimmed();

            socket->write("Введите пароль:\n");
            socket->waitForReadyRead(30000);
            QString pass = socket->readAll().trimmed();

            query.prepare("SELECT * FROM users WHERE username = :u AND password = :p");
            query.bindValue(":u", login);
            query.bindValue(":p", pass);

            if (query.exec() && query.next()) {
                authenticatedMap[socket] = true;
                userMap[socket] = login;
                return "Авторизация успешна. Введите f(x)&a&b&n:\n";
            } else {
                return "Неверные данные.\n";
            }
        } else {
            return "Введите 1 — регистрация, 2 — авторизация:\n";
        }
    } else {
        QStringList parts = msg.split('&');
        if (parts.size() != 4) return "Формат: f(x)&a&b&n\n";

        bool ok1, ok2, ok3;
        double a = parts[1].toDouble(&ok1);
        double b = parts[2].toDouble(&ok2);
        int n = parts[3].toInt(&ok3);
        if (!ok1 || !ok2 || !ok3) return "Ошибка в числах.\n";

        double result = parabolaMethod(a, b, n);
        return QString("Результат интегрирования: %1\n").arg(result).toUtf8();
    }
}
