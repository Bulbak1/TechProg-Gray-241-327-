#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include <QMap>

class MyTcpServer : public QObject {
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    ~MyTcpServer();

private slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();

private:
    QTcpServer *mTcpServer;
    QMap<QString, QTcpSocket*> mTcpSockets;
};

#endif // MYTCPSERVER_H
