#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    static DatabaseManager& instance();
    ~DatabaseManager();

    bool connectToDatabase(const QString& dbFilePath); // путь до .sqlite/.db
    QSqlDatabase getDatabase();

private:
    DatabaseManager();
    QSqlDatabase db;
};

#endif // DATABASEMANAGER_H
