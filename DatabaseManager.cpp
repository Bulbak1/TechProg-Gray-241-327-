#include "DatabaseManager.h"
#include <QtSql/QSqlError>
#include <QDebug>
#include <QFile>
#include <QtSql/QSqlQuery>

DatabaseManager* DatabaseManager::p_instance = nullptr;

DatabaseManager::DatabaseManager() {
    //connectToDatabase();
}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) db.close();
}

DatabaseManager* DatabaseManager::instance() {
    //static DatabaseManager instance;
    if(p_instance==nullptr)
        p_instance = new DatabaseManager();
    return  p_instance;
}

bool DatabaseManager::connectToDatabase(const QString& dbFilePath) {

    if (QSqlDatabase::contains("myusersdb")) {
        db = QSqlDatabase::database("myusersdb");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE", "myusersdb");
        db.setDatabaseName(dbFilePath);
    }

    if (!db.open()) {
        qDebug() << "Ошибка подключения к SQLite:" << db.lastError().text();
        return false;
    }

    qDebug() << "Успешное подключение к базе данных SQLite.";

    return true;
}

bool DatabaseManager::reg(QString login, QString pass){
    qDebug() << "Login:" << login << "Pass:" << pass;
    QSqlDatabase db = DatabaseManager::instance()->getDatabase();
    QSqlQuery query(db);
    query.exec("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY AUTOINCREMENT, username TEXT UNIQUE, password TEXT)");
    query.prepare("INSERT INTO users (username, password) VALUES (:u, :p)");
    query.bindValue(":u", login);
    query.bindValue(":p", pass);
    if (query.exec()){
        qDebug() << "Sucess. Login:" << login << "Pass:" << pass;
        return true;
    }
    qDebug() << "Login:" << login << "Pass:" << pass;
    qDebug() << "addPerson error:"
             << query.lastError();
    return false;
}

bool DatabaseManager::auth(QString login, QString pass){
    qDebug() << "Login:" << login << "Pass:" << pass;
    QSqlDatabase db = DatabaseManager::instance()->getDatabase();
    QSqlQuery query(db);
    query.prepare("SELECT * FROM users WHERE username = :u AND password = :p");
    query.bindValue(":u", login);
    query.bindValue(":p", pass);
    if (query.exec() && query.next()){
        qDebug() << "Sucess. Login:" << login << "Pass:" << pass;
        return true;
    }
    qDebug() << "Login:" << login << "Pass:" << pass;
    qDebug() << "addPerson error:"
             << query.lastError();
    return false;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
