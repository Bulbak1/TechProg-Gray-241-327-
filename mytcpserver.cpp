#include "mytcpserver.h"
#include <QDebug>
#include <QCoreApplication>
#include<QString>
#include <QDateTime>
#include "serverfunctions.h"

MyTcpServer::~MyTcpServer()
{

    mTcpServer->close();
    //server_status=0;
}

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent){
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection,
            this, &MyTcpServer::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 33333)){
        qDebug() << "server is not started";
    } else {
        //server_status=1;
        qDebug() << "server is started";
    }
}

void MyTcpServer::slotNewConnection(){
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    //   if(server_status==1){
    if (!socket) return;

    // Генерируем уникальный ключ (IP:порт)
    QString clientId = QString("%1:%2")
                           .arg(socket->peerAddress().toString())
                           .arg(socket->peerPort());

    mTcpSockets.insert(clientId, socket); // Добавляем в QMap

    //лог о новом соединении
    qDebug() << "Новое подключение: клиент" << clientId
             << "установлен в" << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    //сообщение клиенту
    QString welcomeMsg = QString("Добро пожаловать на сервер! Ваш ID: %1\n").arg(clientId);
    socket->write(welcomeMsg.toUtf8());
    //подключение сигналов
    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);
    //}
}

void MyTcpServer::slotServerRead(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) {
        qDebug() << "Error: socket is null!";
        return;
    }
    QString res = "";
    QString clientId = QString("%1:%2")
                           .arg(socket->peerAddress().toString())
                           .arg(socket->peerPort());
    while(socket->bytesAvailable()>0)
    {
        QByteArray array =socket->readAll();
        qDebug() << "Client:" << clientId << "sent: " << array << "\n";
        if(array=="\x01")
        {
            socket->write(res.toUtf8());
            res = "";
        }
        else
            res.append(array);
    }
    socket->write(parse(res.toUtf8()));

}

void MyTcpServer::slotClientDisconnected(){
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QString clientId = QString("%1:%2")
                           .arg(socket->peerAddress().toString())
                           .arg(socket->peerPort());
    // Удаляем сокет из QMap по значению
    mTcpSockets.remove(mTcpSockets.key(socket));
    socket->deleteLater();
    qDebug() << "Клиент" << clientId << "отключился";
}
