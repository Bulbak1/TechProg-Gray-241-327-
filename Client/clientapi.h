#ifndef CLIENTAPI_H
#define CLIENTAPI_H
#include <QObject>
#include <QTcpSocket>

#include <QtNetwork>
#include <QByteArray>
#include <QDebug>

class ClientAPI;

class SingletonDestroyer {
    private:
        ClientAPI* p_instance;
    public:
        ~SingletonDestroyer();
        void initialize(ClientAPI* p);
};

class ClientAPI: public QObject
{
    Q_OBJECT
    private:
        static ClientAPI* p_instance;
        static SingletonDestroyer destroyer;
        QTcpSocket * mTcpSocket;
    protected:
        ClientAPI(QObject *parent = nullptr);
        ~ClientAPI();
        ClientAPI(const ClientAPI& ) = delete;
        ClientAPI& operator = (ClientAPI&) = delete;
        friend class SingletonDestroyer;
    public:
        static ClientAPI* getInstance();
    public:
        QByteArray query_to_server(QString);
        QByteArray interactiveLagrangeTest();
    signals:
        void disconnected();
public slots:
    void slotServerDisconnection();
        // {
        //     if (!p_instance)
        //     {
        //         p_instance = new ClientAPI();
        //         destroyer.initialize(p_instance);
        //     }
        //     return p_instance;
        // }


};



#endif // CLIENTAPI_H
