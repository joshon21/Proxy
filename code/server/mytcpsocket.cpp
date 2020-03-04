#include "mytcpsocket.h"
#include <QTcpSocket>
#include <QDataStream>
#include <QMetaType>

Mytcpsocket::Mytcpsocket(QObject *parent):
    QTcpSocket(parent)
{
}

void Mytcpsocket::se(QString a_host,QString a_port,QString a_frequency,QString s_frequency)
{
    qDebug()<<"send server setting to client";
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    QString reset="1";
    QString t_host;
    QString t_port;
    QString ta_frequency;
    QString ts_frequency;
    t_host=a_host;
    t_port=a_port;
    ta_frequency=a_frequency;
    ts_frequency=s_frequency;
    QString write_message;
    write_message=reset.append(":").append(t_host).append(":").append(t_port).append(":").append(ta_frequency).append(":").append(ts_frequency);
    std::string str=write_message.toStdString();
    out<<tr(str.c_str());
    this->write(block);
}

void Mytcpsocket::flu()
{
    if(this->UnconnectedState)
        emit this->disconnect();
    qDebug()<<"send flush data to client";
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    QString write_message;
    write_message="2";
    std::string str=write_message.toStdString();
    out<<tr(str.c_str());
    this->write(block);
}

void Mytcpsocket::disableprox(int ifdisable)
{
    if(this->UnconnectedState)
        emit this->disconnect();
    if(ifdisable==0)
    {
        qDebug()<<"send enabale data to client";
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        QString write_message;
        write_message="4";
        std::string str=write_message.toStdString();
        out<<tr(str.c_str());
        this->write(block);
    }
    else if(ifdisable==1)
    {
        qDebug()<<"send disabale data to client";
        QByteArray block;
        QDataStream out(&block,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_5);
        QString write_message;
        write_message="5";
        std::string str=write_message.toStdString();
        out<<tr(str.c_str());
        this->write(block);
    }

}

