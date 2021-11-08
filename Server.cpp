#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Server)
{
    
    dbConnection(); // to connect to data base . 
    readSettings(); //Read setting from fconfig file in ./
    timer =new QTimer;
    timerSetting(); // this function is responsible for timer event . 
}

void Server::readSettings()
{
    QSettings setting("fconfig.ini",QSettings::IniFormat);
    int myPort = setting.value("address/port","9000").toInt();
    p1 = setting.value("prefix/p1","").toInt();
    p2 = setting.value("prefix/p2","").toString();
    startUpSetting(myPort);
}

void Server::slot_response()
{
    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(slot_onReadyRead()), Qt::QueuedConnection);
}

void Server::slot_onReadyRead()
{
    QByteArray data = socket->readAll();

    qDebug() << "  New Data from Conncetion is comming ...";


   QString SUserID = "" ;
   QString Transaction = "" ;
   QString dataRec = QString(data); //to cast byte array to String

   qDebug()<<data;

   bool prefix_1 = false;
   bool prefix_2 = false;
   bool isNumber = false;
   
   unsigned int UserID = 0;
   unsigned int Device_ID = 0;
   static bool valid;

   qDebug() << "text " << data ;

   QString find="p1";
   
  
   if ( dataRec[0] == "p" && dataRec[1] == "1" ) {
    prefix_1=true;
     }
   else if (dataRec[0] == "p" && dataRec[1] == "2"){
       prefix_2=true;
   }
   else {
       qDebug() << "Wrong Prefix " ; 
   }

   

   if (prefix_1 == true) {
       
       for (int k=2; k <= dataRec.length() ; k++){
           
           qDebug() << " lets get loud " ;
           if (dataRec[k].isDigit()){
               
               SUserID  += dataRec[k];
           }
           else { 
                qDebug () << "Wrong ID number " ;
                return;
           }
           
           insertData ( SUserID ) ;
           prefix_1 =false;
                 
       }
   }
   
   if (prefix_2 == true) {
          
           for (int k=2; k <= dataRec.length() ; k++){
               
               qDebug() << " lets get loud " ;
               Transaction  += dataRec[k];
               prefix_1 =false;
                  
               }
        insertData ( Transaction ) ;
        prefix_2 = false ;
        
       }
   qDebug() << "The user ID  : " << SUserID ;
   qDebug() << "The Transaction : " << Transaction ;
   
}

void Server::insertData(QString input)
{
    QSqlQuery q (*mydb) ;
    QString str ="INSERT INTO Pasargad (prefix) " "VALUES"  "("+input+")";

    if (q.exec(str)){
        
        qDebug()<<"inserted !";
        

    }
    else{
        qDebug() << q.lastError().text();
    }
}
    
void Server::dbConnection()
{
    mydb = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","myconnection"));
    mydb->setHostName("localhost");
    mydb->setPort(3306);
    mydb->setUserName("root");
    mydb->setPassword("1234");
    mydb->setDatabaseName("world");
    qDebug()<< QSqlDatabase::isDriverAvailable("QMYSQL");
    
    if (!mydb->open()){
    qDebug()<<mydb->lastError().text();
    
    }
    else{
    qDebug() << "open success";
    
    }
}

void Server::startUpSetting(int port)
{
    server =new QTcpServer(this) ;
    socket = new QTcpSocket(this);

    connect( server,SIGNAL(newConnection()),this,SLOT(response()),Qt::QueuedConnection);
    server->listen (QHostAddress::Any , port);
    
}

void Server::slot_queryTimer()

{
    //Ascending
    QSqlQuery timerQuery (*mydb) ;
    QString str ="SELECT * FROM pasargad ORDER BY p1 ASC";

    if (timerQuery.exec(str)){
        
        qDebug()<<"Done !";
        

    }
    else{
        qDebug() << timerQuery.lastError().text();
    }
   
}

void Server::timerSetting()
{
    connect(timer,SIGNAL(timeout()),this,SLOT( slot_queryTimer()),Qt::QueuedConnection);
    timer->start(120000);
}



Server::~Server()
{
    delete ui;
}
