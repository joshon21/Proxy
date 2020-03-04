#ifndef MYSERVER_H
#define MYSERVER_H
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QThread>
#include "widget.h"
#include <QList>
#include <QRadioButton>
#include <QCheckBox>
#include <QTableWidgetItem>
#include <QHash>
#include <QMetaType>
#include <QPushButton>

struct clientinfo
{
    int flag;
    QString a_host;
    QString a_port;
    QString a_frequency;
    QString s_frequency;
    QString clientname;
    QString clientip;
    QString cusername;
    QString winversion;
    int ifsync;
    int ifdisable;
};
Q_DECLARE_METATYPE(clientinfo)

class MyServer : public QTcpServer
{

     Q_OBJECT

public:
    explicit MyServer(QObject *parent = 0);
    void init_data();
    void update_ui();
    void startServer();
public:
    QPushButton * btn;
    QTableWidgetItem *check;
    Widget *w;
    int flag;
    int ifsync;
    int ifdisable;
    QHash<int,int> map;
    QHash<int,int> map1;
    QString a_host;
    QString a_port;
    QString a_frequency;
    QString s_frequency;
    QString cusername;
    QString winversion;
    clientinfo clienti;  //单个信息
    QList<clientinfo> clientlist;
private slots:
    void confirm(QString ip,QString port,QString af,QString sf);
    void itemchanged(int row,int col);
    void flush();
    void discon(int flag);
    void myread_info(int client,QString a_host,QString a_port,QString a_frequency,QString s_frequency,QString clientname,QString clientip,QString cusername,QString winversion);
    void checkcolumnsolt(int col);
    void btnclicked(bool);
signals:
    void signal_confirm(QHash<int,int> map,QHash<int,int> map1,QString host,QString port,QString af,QString sf);
    void signal_flush(QHash<int,int> map);
    void disableagent(QHash<int,int> map);

protected:
    void incomingConnection(qintptr socketDescriptor);
};


#endif // MYSERVER_H
