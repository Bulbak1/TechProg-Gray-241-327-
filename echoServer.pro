QT -= qui
QT += network
QT += core network
QT += core network sql
QT += sql

CONFIG += c++17 cmdline
CONFIG += console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        DatabaseManager.cpp \
        main.cpp \
        mytcpserver.cpp \
        serverfunctions.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    DatabaseManager.h \
    mytcpserver.h \
    serverfunctions.h

DISTFILES += \
    icudt67.dll \
    icuin67.dll \
    icuio67.dll \
    icutu67.dll \
    icuuc67.dll \
    libcrypto-3-x64.dll \
    libcurl.dll \
    libecpg.dll \
    libecpg_compat.dll \
    libiconv-2.dll \
    libintl-9.dll \
    liblz4.dll \
    libpgtypes.dll \
    libpq.dll \
    libssl-3-x64.dll \
    libwinpthread-1.dll \
    libxml2.dll \
    libxslt.dll \
    libzstd.dll \
    myusersdb.sqlite \
    sqldrivers/qsqlite.dll \
    sqldrivers/qsqlmimer.dll \
    sqldrivers/qsqlodbc.dll \
    sqldrivers/qsqlpsql.dll
