#include "mytcpserver.h"
#include <QDebug>
#include <QDateTime>
#include "serverfunctions.h"

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent) {
    mTcpServer = new QTcpServer(this);
    connect(mTcpServer, &QTcpServer::newConnection, this, &MyTcpServer::slotNewConnection);

    if (!mTcpServer->listen(QHostAddress::Any, 33333)) {
        qDebug() << "Сервер не запущен";
    } else {
        qDebug() << "Сервер запущен на порту 33333";
    }
}

MyTcpServer::~MyTcpServer() {
    mTcpServer->close();
}

void MyTcpServer::slotNewConnection() {
    QTcpSocket* socket = mTcpServer->nextPendingConnection();
    if (!socket) return;

    QString clientId = QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    mTcpSockets.insert(clientId, socket);
    qDebug() << "Новое подключение от" << clientId;

    connect(socket, &QTcpSocket::readyRead, this, &MyTcpServer::slotServerRead);
    connect(socket, &QTcpSocket::disconnected, this, &MyTcpServer::slotClientDisconnected);

    socket->write("Нажмите 1 для регистрации, 2 для атворизации\r\n");
}

void MyTcpServer::slotServerRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray request = socket->readAll();
    QByteArray response = parse(request, socket);
    socket->write(response);
}

void MyTcpServer::slotClientDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;
    QString clientId = QString("%1:%2").arg(socket->peerAddress().toString()).arg(socket->peerPort());
    mTcpSockets.remove(clientId);
    socket->deleteLater();
    qDebug() << "Клиент" << clientId << "отключился";
}
