#include "DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QFile>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) db.close();
}

DatabaseManager& DatabaseManager::instance() {
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::connectToDatabase(const QString& dbFilePath) {

    db = QSqlDatabase::addDatabase("QSQLITE", "myusersdb");
    db.setDatabaseName(dbFilePath);

    if (!db.open()) {
        qDebug() << "Ошибка подключения к SQLite:" << db.lastError().text();
        return false;
    }

    qDebug() << "Успешное подключение к базе данных SQLite.";

    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
