#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager* instance();
    ~DatabaseManager();

    bool connectToDatabase(const QString& dbFilePath);
    bool auth(QString login, QString pass);
    bool reg(QString login, QString pass);
    QSqlDatabase getDatabase();

private:
    DatabaseManager();
    QSqlDatabase db;
    static DatabaseManager* p_instance;
};

#endif // DATABASEMANAGER_H
