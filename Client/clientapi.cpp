#include "clientapi.h"

// ClientAPI::ClientAPI() {}

ClientAPI* ClientAPI::p_instance;
SingletonDestroyer ClientAPI::destroyer;


SingletonDestroyer::~SingletonDestroyer()
{
    delete p_instance;
}
void SingletonDestroyer::initialize(ClientAPI* p)
    {p_instance = p;};


ClientAPI::ClientAPI(QObject *parent)
{
    mTcpSocket = new QTcpSocket(this);

    connect(this->mTcpSocket, &QTcpSocket::disconnected,
            this, &ClientAPI::slotServerDisconnection);
    mTcpSocket->connectToHost("127.0.0.1",33333);
    mTcpSocket->waitForReadyRead();
    QByteArray array;
    while(mTcpSocket->bytesAvailable()>0)
    {
        array.append(mTcpSocket->readAll());

    }
    qDebug()<<array;
    // if(!mTcpServer->listen(QHostAddress::Any, 33333)){
    //     qDebug() << "server is not started";
    // } else {
    //     //server_status=1;
    //     qDebug() << "server is started";
    // }
}

ClientAPI::~ClientAPI()
{
    this->mTcpSocket->close();
}
ClientAPI* ClientAPI::getInstance(){
    if (!p_instance)
    {
        p_instance = new ClientAPI();
        destroyer.initialize(p_instance);
    }
    return p_instance;
}
void ClientAPI::slotServerDisconnection()
{
    mTcpSocket->close();
    // emit disconnected();
}

QByteArray ClientAPI::query_to_server(QString msg)
{
    mTcpSocket->write(msg.toUtf8());
    mTcpSocket->waitForReadyRead();
    QByteArray array;
    while(mTcpSocket->bytesAvailable()>0)
    {
        array.append(mTcpSocket->readAll());

    }
    return array;
}

QByteArray ClientAPI::interactiveLagrangeTest() {
    // Отправляем команду Sh
    mTcpSocket->write("Sh");
    mTcpSocket->waitForReadyRead();

    // Читаем сгенерированные точки от сервера
    QByteArray pointsData;
    while (mTcpSocket->bytesAvailable() > 0) {
        pointsData.append(mTcpSocket->readAll());
    }
    qDebug() << "Сгенерированные точки:\n" << pointsData;

    // Запрашиваем ввод точки у пользователя через консоль
    qDebug() << "Введите точку в формате x&y:";
    QTextStream stream(stdin);
    QString input = stream.readLine();

    // Отправляем точку на сервер
    mTcpSocket->write(input.toUtf8());
    mTcpSocket->waitForReadyRead();

    // Читаем и возвращаем результат
    QByteArray result;
    while (mTcpSocket->bytesAvailable() > 0) {
        result.append(mTcpSocket->readAll());
    }
    return result;
}
