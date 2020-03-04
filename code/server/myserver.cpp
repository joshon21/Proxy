#include "myserver.h"
#include "mythread.h"
#include "widget.h"
#include <QTableWidgetItem>
#include <QAbstractButton>
#include <QHash>
#include <QMetaType>
#include <QSysInfo>
MyServer::MyServer(QObject *parent) :
    QTcpServer(parent)
{
    w=new Widget();
    w->setFixedSize(1300,720);
    qRegisterMetaType< QHash<int,int> >("QHash<int,int>");
    connect(w->ui->tableWidget,SIGNAL(cellChanged(int,int)),this,SLOT(itemchanged(int,int)));
    connect(w->ui->tableWidget->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(checkcolumnsolt(int)));
    connect(w,SIGNAL(confirmclicked(QString,QString,QString,QString)),this,SLOT(confirm(QString,QString,QString,QString)));
    connect(w,SIGNAL(flushclicked()),this,SLOT(flush()));
    w->show();
    init_data();
}

void MyServer::startServer()
{
    int port = 6666;
    if(!this->listen(QHostAddress::AnyIPv4, port))
    {
        qDebug() << "Could not start server";
    }
    else
    {
        qDebug() << "Listening to port " << port << "...";
    }
}

void MyServer::checkcolumnsolt(int col)
{
    if (col==8)
    {
        if(w->ui->tableWidget->horizontalHeaderItem(8)->text()=="全选")
        {

            w->ui->tableWidget->horizontalHeaderItem(8)->setText("取消全选");
            for(int i=0;i<clientlist.size();++i)
            {
                clientlist[i].ifsync=1;
                map[clientlist[i].flag]=1;
            }
            update_ui();
            return;
        }
        if(w->ui->tableWidget->horizontalHeaderItem(8)->text()=="取消全选")
        {
            w->ui->tableWidget->horizontalHeaderItem(8)->setText("全选");
            for(int i=0;i<clientlist.size();++i)
            {
                clientlist[i].ifsync=0;
                map[clientlist[i].flag]=0;
            }
            update_ui();
            return;
        }
    }
}

void MyServer::init_data()
{
    a_host="127.0.0.1";
    a_port="1080";
    a_frequency="1";
    s_frequency="1";
    flag=0;
}

void MyServer::confirm(QString ip,QString port,QString af,QString sf)
{
    qDebug()<<"emit signal_confirm signal";
    emit signal_confirm(map,map1,ip,port,af,sf);
    for(int i=0;i<clientlist.size();++i)
    {
        clientlist[i].ifsync=0;
        map[clientlist[i].flag]=0;
    }
    w->ui->tableWidget->horizontalHeaderItem(8)->setText("全选");
}

void MyServer::flush()
{
    emit signal_flush(map1);
}

void MyServer::myread_info(int flag,QString a_host,QString a_port,QString a_frequency,QString s_frequency,QString clientname,QString clientip,QString cusername,QString winversion)
{
    qDebug()<<"clientlist---="<<clientlist.size();
    if (clientlist.size()==0)
    {
        clienti.flag=flag;
        clienti.a_host=a_host;
        clienti.a_port=a_port;
        clienti.a_frequency=a_frequency;
        clienti.s_frequency=s_frequency;
        clienti.clientname=clientname;
        clienti.clientip=clientip;
        clienti.cusername=cusername;
        clienti.winversion=winversion;
        clienti.ifsync=0;
        clienti.ifdisable=0;
        map.insert(flag,clienti.ifsync);
        map1.insert(flag,clienti.ifdisable);
        clientlist.append(clienti);
        update_ui();
        return ;
    }
    int ifnew=0;
    for(int i=0;i<clientlist.size();++i)
    {
        if(clientlist[i].flag==flag){
            clientlist[i].a_host=a_host;
            clientlist[i].a_port=a_port;
            clientlist[i].a_frequency=a_frequency;
            clientlist[i].s_frequency=s_frequency;
            ifnew=1;
            break;
        }
    }
    if(ifnew==0)
    {
        clienti.flag=flag;
        clienti.a_host=a_host;
        clienti.a_port=a_port;
        clienti.a_frequency=a_frequency;
        clienti.s_frequency=s_frequency;
        clienti.clientname=clientname;
        clienti.clientip=clientip;
        clienti.cusername=cusername;
        clienti.winversion=winversion;
        clienti.ifsync=0;
        clienti.ifdisable=0;
        map.insert(flag,clienti.ifsync);
        map1.insert(flag,clienti.ifdisable);
        clientlist.append(clienti);
    }
    update_ui();
}

void MyServer::update_ui()
{
    qDebug()<<"clientlist="<<clientlist.size();
    w->ui->tableWidget->clearContents();
    for(int i=0;i<clientlist.size();++i)
    {
        clientinfo clientin=clientlist.at(i);
        QTableWidgetItem *temp0=new QTableWidgetItem(clientin.clientname);
        temp0->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,0,temp0);

        QTableWidgetItem *temp1=new QTableWidgetItem(clientin.cusername);
        temp1->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,1,temp1);

        QTableWidgetItem *temp2=new QTableWidgetItem(clientin.winversion);
        temp2->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,2,temp2);

        QTableWidgetItem *temp3=new QTableWidgetItem(clientin.clientip);
        temp3->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,3,temp3);

        QTableWidgetItem *temp4=new QTableWidgetItem(clientin.a_host);
        temp4->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,4,temp4);

        QTableWidgetItem *temp5=new QTableWidgetItem(clientin.a_port);
        temp5->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,5,temp5);

        QTableWidgetItem *temp6=new QTableWidgetItem(clientin.a_frequency);
        temp6->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,6,temp6);

        QTableWidgetItem *temp7=new QTableWidgetItem(clientin.s_frequency);
        temp7->setTextAlignment(Qt::AlignCenter);
        w->ui->tableWidget->setItem(i,7,temp7);

        if(clientin.ifsync==0)
        {
            check=new QTableWidgetItem;
            check->setCheckState(Qt::Unchecked);
            check->setTextAlignment(Qt::AlignVCenter);
            w->ui->tableWidget->setItem(i,8,check);
        }
        else if(clientin.ifsync==1){
            check=new QTableWidgetItem;
            check->setCheckState(Qt::Checked);
            check->setTextAlignment(Qt::AlignVCenter);
            w->ui->tableWidget->setItem(i,8,check);
        }
        if(clientin.ifdisable==0)
        {
            btn=new QPushButton("已启用");
            connect(btn,SIGNAL(clicked(bool)),this,SLOT(btnclicked(bool)));
            w->ui->tableWidget->setCellWidget(i,9,btn);
        }
        else if(clientin.ifdisable==1)
        {
            btn=new QPushButton("已禁用");
            connect(btn,SIGNAL(clicked(bool)),this,SLOT(btnclicked(bool)));
            w->ui->tableWidget->setCellWidget(i,9,btn);
        }
    }
    w->repaint();
}


void MyServer::incomingConnection(qintptr socketDescriptor)
{   
    flag+=1;
    MyThread *thread = new MyThread(socketDescriptor);
    thread->flag=flag;
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(this,SIGNAL(signal_confirm(QHash<int,int>,QHash<int,int>,QString,QString,QString,QString)),thread,SLOT(send_r(QHash<int,int>,QHash<int,int>,QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(this,SIGNAL(disableagent(QHash<int,int>)),thread,SLOT(disableagent(QHash<int,int>)),Qt::QueuedConnection);
    connect(this,SIGNAL(signal_flush(QHash<int,int>)),thread,SLOT(send_flush_Message(QHash<int,int>)),Qt::QueuedConnection);
    connect(thread,SIGNAL(read_info(int,QString,QString,QString,QString,QString,QString,QString,QString)),this,SLOT(myread_info(int,QString,QString,QString,QString,QString,QString,QString,QString)),Qt::QueuedConnection);
    connect(thread,SIGNAL(disconnectalready(int)),this,SLOT(discon(int)),Qt::QueuedConnection);
    thread->start();
}

void MyServer::discon(int flag)
{
    qDebug()<<"entering discon now";
    for(int i=0;i<clientlist.size();++i)
    {
        if(clientlist[i].flag==flag){
            qDebug()<<"flag ="<<flag<<" will be remove";
            clientlist.removeAt(i);
            map.remove(flag);
            map1.remove(flag);
        }
    }
    update_ui();
}

void MyServer::itemchanged(int row,int col)
{
    if(col==8)
    {
        if(w->ui->tableWidget->item(row,col)->checkState()==Qt::Checked)
        {
            clientlist[row].ifsync=1;
            map[clientlist[row].flag]=clientlist[row].ifsync;
            return;
        }
        if (w->ui->tableWidget->item(row,col)->checkState()==Qt::Unchecked)
        {
            clientlist[row].ifsync=0;
            map[clientlist[row].flag]=clientlist[row].ifsync;
            return;
        }
    }

}


void MyServer::btnclicked(bool)
{
    QPushButton *button = dynamic_cast<QPushButton *>(QObject::sender());
    QModelIndex index = w->ui->tableWidget->indexAt(button->pos());
    int row=index.row();
    int col=index.column();
    qDebug()<<"button clicked";
    if(clientlist[row].ifdisable==0)
    {
        qDebug()<<"ifdisable==0";
        clientlist[row].ifdisable=1;
        map1[clientlist[row].flag]=clientlist[row].ifdisable=1;

    }
    else if(clientlist[row].ifdisable==1)
    {
        qDebug()<<"ifdisable==1";
        clientlist[row].ifdisable=0;
        map1[clientlist[row].flag]=clientlist[row].ifdisable=0;
    }
    update_ui();
    emit disableagent(map1);

}
