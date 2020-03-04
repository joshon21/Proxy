#include "mythread.h"
#include <QTcpSocket>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QTime>
#include <QTimer>
#include <QWidget>
#include <QObject>
#include "mytcpsocket.h"
#include <QTimer>
#include <QTime>
#include <QProcess>

MyThread::MyThread(int socketDescriptor, QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
}

void MyThread::run()
{
    m_heartbeat=new QTimer;
    m_heartbeat->setInterval(300000);  //设置超时时间
    m_lasttime=QDateTime::currentDateTime();
    tcpsocket=new Mytcpsocket();
    if(!tcpsocket->setSocketDescriptor(socketDescriptor))
    {
        return;
    }
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMassage()),Qt::DirectConnection);
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(heartbeat()),Qt::DirectConnection);
    connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(alreadydisconnect()),Qt::DirectConnection);
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(alreadydisconnect()),Qt::DirectConnection);
    connect(tcpsocket, SIGNAL(disconnected()),tcpsocket, SLOT(deleteLater()));   
    connect(this,SIGNAL(send_reset(QString,QString,QString,QString)),tcpsocket,SLOT(se(QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(fl()),tcpsocket,SLOT(flu()),Qt::QueuedConnection);
    connect(this,SIGNAL(disableproxy(int)),tcpsocket,SLOT(disableprox(int)));
    connect(m_heartbeat,SIGNAL(timeout()),this,SLOT(checkifalive()),Qt::DirectConnection);
    m_heartbeat->start();
    this->exec();
}

void MyThread::send_r(QHash<int,int> map,QHash<int,int> map1,QString a_host,QString a_port,QString a_frequency,QString s_frequency)
{
    qDebug()<<"emit send_reset signal";
    qDebug()<<"flag="<<flag<<"  "<<map[flag];
    if (map[flag]==0){
        return ;
    }
    if (map1[flag]==1)//已经是disable状态的
    {
        return ;
    }
    emit send_reset(a_host,a_port,a_frequency,s_frequency);
}

void  MyThread::send_flush_Message(QHash<int,int> map1)
{
    qDebug()<<"map1[flag]=="<<map1[flag];
    if(map1[flag]==0)
    {
        emit fl();
    }
}

void MyThread::readMassage()
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_5_5);
    read_message="";
    in>>read_message;
    qDebug()<<"read client data:"<<read_message;
    QStringList list=read_message.split(":");
    a_host=list[0];
    a_port=list[1];
    a_frequency=list[2];
    s_frequency=list[3];
    clienthostname=list[4];
    username=list[5];
    winversion=list[6];
    ip=tcpsocket->peerAddress().toString();
    qDebug()<<"client_flag="<<flag;
    emit read_info(flag,a_host,a_port,a_frequency,s_frequency,clienthostname,ip,username,winversion);
}

void MyThread::alreadydisconnect()
{
    qDebug()<<"send disconnect signal now";
    m_heartbeat->stop();
    emit disconnectalready(flag);
}

void MyThread::checkifalive()
{
    if(m_lasttime.secsTo(QDateTime::currentDateTime())>=120)
    {
        qDebug()<<"heartbet 超时，即将断开连接";
        m_heartbeat->stop();
        tcpsocket->disconnectFromHost();
    }
}
void MyThread::heartbeat()
{
    if(!m_heartbeat->isActive())
    {
        m_heartbeat->start();
    }
    m_lasttime=QDateTime::currentDateTime();
}

void MyThread::disableagent(QHash<int,int> map1)
{

     emit disableproxy(map1[flag]);

}



