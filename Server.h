#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QtDebug>
#include <QSettings>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QTimer>


QT_BEGIN_NAMESPACE

namespace Ui { class Server; }

QT_END_NAMESPACE

class Server : public QMainWindow
{
    Q_OBJECT

public:
    Server(QWidget *parent = nullptr);
    ~Server();
    int p1;
    QString p2;
    
    QTimer *timer ;
    QSqlDatabase* mydb;
    QTcpServer*  server;  
    QTcpSocket*  socket;

//functions : 
    void dbConnection ();
    void startUpSetting (int);
    void readSettings ();
    void insertData (QString input);
    void timerSetting();
    
    
public slots :
    void slot_response();
    void slot_onReadyRead();
    void slot_queryTimer ();
    
private:
    Ui::Server *ui;
};
#endif // SERVER_H
