#include <QCoreApplication>
#include <QSqlDatabase>
#include "mytcpserver.h"
#include "DatabaseManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << QSqlDatabase::drivers();
    DatabaseManager::instance()->connectToDatabase("myusersdb.sqlite");

    MyTcpServer server;
    return a.exec();
}
