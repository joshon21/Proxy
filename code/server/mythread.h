#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QString>
#include <QWidget>
#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include "mytcpsocket.h"
#include <QTimer>
#include <QDateTime>
#include <QCheckBox>
#include <QHash>

class MyThread : public QThread
{

    Q_OBJECT
public:
    explicit MyThread(int socketDescriptor, QObject *parent = 0);
    void run();

private slots:
    void readMassage();
    void send_flush_Message(QHash<int,int> map1);
    void alreadydisconnect();
    void checkifalive();
    void heartbeat();
    void disableagent(QHash<int,int> map1);
    void send_r(QHash<int,int> map,QHash<int,int> map1,QString a_host,QString a_port,QString a_frequency,QString s_frequency);
public:
    int socketDescriptor;
    int ifsync;
    Mytcpsocket *tcpsocket;
    QString text;

    QString username;
    QString clienthostname;
    QString ip;
    QString a_host;
    QString a_port;
    QString a_frequency;
    QString s_frequency;
    QString winversion;
    int flag;
    int controlflag=0;


    QString reset;
    QString t_host;
    QString t_port;
    QString ta_frequency;
    QString ts_frequency;
    QString write_message;
    QString read_message;

    QTimer *m_heartbeat;
    QDateTime m_lasttime;
signals:
   void read_info(int flag,QString a_host,QString a_port,QString a_frequency,QString s_frequency,QString clientname,QString clientip,QString cusername,QString winversion);
   void send_reset(QString a_host,QString a_port,QString a_frequency,QString s_frequency);
   void disconnectalready(int flag);
   void fl();
   void disableproxy(int ifdisable);
};

#endif // MYTHREAD_H
