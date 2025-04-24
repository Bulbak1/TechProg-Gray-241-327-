#include "serverfunctions.h"
#include <QStringList>
#include <QTcpSocket>



QByteArray auth(QString login, QString password){
    return QByteArray("welcome\r\n");
}
QByteArray reg(QString login, QString password){
    return QByteArray("sucess\r\n");
}
double lagrangeInterpolation(const QVector<double>& x, const QVector<double>& y, double point) {
    double result = 0.0;
    for (size_t i = 0; i < x.size(); ++i) {
        double term = y[i];
        for (size_t j = 0; j < x.size(); ++j) {
            if (j != i) {
                term *= (point - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}
QByteArray lagrangetest(QTcpSocket* socket){

    int numPoints = 3 + rand() % 4;

    QVector<double> x_values(numPoints);
    QVector<double> y_values(numPoints);
    bool is_unique = false;
    double new_x;

    qDebug() << "Сгенерированный набор точек:\n";
    socket->write("Сгенерированный набор точек:\r\n");
    for (int i = 0; i < numPoints; i++) {
        while (is_unique == false) {
            new_x = -5 + rand() % 11; // x от -5 до 5
            if (std::find(x_values.begin(), x_values.end(), new_x) == x_values.end()) {
                is_unique = true;
            }
        }
        is_unique = false;
        x_values[i] = new_x;
        y_values[i] = -10 + rand() % 21; // y от -10 до 10
        qDebug() << x_values[i] << ", " << y_values[i] << "\n";
        QString mess = QString("%1, %2\r\n").arg(x_values[i]).arg(y_values[i]);
        socket->write(mess.toUtf8());
    }

    // Ввод точки
    qDebug() << "\nВведите точку (x y), которая должна лежать на полиноме в формате x&y: ";
    socket->write("\r\nВведите точку (x y), которая должна лежать на полиноме в формате x&y: ");


    if (!socket->waitForReadyRead(180000)) { // Таймаут 3 минуты
        return QByteArray("ERROR: Timeout\r\n");
    }
    QByteArray array =socket->readAll();
    QString res = "";
    if(array=="\x01")
    {
        socket->write(res.toUtf8());
        res = "";
    }
    else{
        res.append(array);
    }
    QStringList answer = res.remove("\r\n").split('&');
    if (answer.size() != 2) {
        socket->write("ERROR: Invalid format. Use x&y\r\n");
        return QByteArray("ERROR\r\n");
    }
    bool ok1, ok2;
    double x = answer[0].toDouble(&ok1);
    double y = answer[1].toDouble(&ok2);

    if (!ok1 || !ok2) {
        socket->write("ERROR: Invalid numbers\r\n");
        return QByteArray("ERROR\r\n");
    }
        if (x_values.contains(x)) {
            qDebug() << "Ошибка: x уже есть в исходных данных!\n";
            socket->write("Ошибка: x уже есть в исходных данных!\r\n");
            return "провал";
        }

    // Проверка, лежит ли точка на полиноме
    double computed_y = lagrangeInterpolation(x_values, y_values, x);
    const double pogresh = 0.2;

    if (abs(computed_y - y) < pogresh) {
        QString trueanswer = QString("Верно! Точка (%1, %2) лежит на полиноме.\r\n").arg(x).arg(y);
        qDebug() << trueanswer;
        socket->write(trueanswer.toUtf8());
        return QByteArray("sucess\r\n");
        //cout << "Верно! Точка (" << x << ", " << y << ") лежит на полиноме.\n";
    }
    else {
        QString falseanswer = QString("Неверно! Для x=%1 правильный y=%2\r\n").arg(x).arg(computed_y);
        qDebug() << falseanswer;
        socket->write(falseanswer.toUtf8());
        return QByteArray("non sucess\r\n");
        //cout << "Неверно! Для x=" << x << " правильный y=" << computed_y << "\n";
    }
}
QByteArray default_answer(){
    return QByteArray("unknown command\r\n");
}

QByteArray parse(QString input1,QTcpSocket* socket)
{
    QStringList args = input1.remove("\r\n").split('&');
    if (args[0] == "auth"){
        if (args.size() == 3){
            return auth(args[1], args[2]);
        }
        else{
            return default_answer();
        }
    }
    if (args[0] == "reg"){
        if (args.size() == 3){
            return reg(args[1], args[2]);
        }
        else{
            return default_answer();
        }
    }
    else if(args[0] == "Sh"){
        if (args.size() == 1){
            return lagrangetest(socket);
        }
        else{
            return default_answer();
        }
    }
    else{ return default_answer(); }
}
