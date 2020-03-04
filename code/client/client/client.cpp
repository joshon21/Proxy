#include "client.h"
#include <Qtimer>
#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QSettings>
#include <QVariant>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QDesktopServices>
#include <qdesktopservices.h>
#include <QProcess>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#define REGEDIT_NAME "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings\\"
client::client(QWidget *parent)
    : QMainWindow(parent)
{
    this->init_data();
    tcpsocket=new QTcpSocket(this);
    contimer=new QTimer(this);
    contimer->setInterval(30000);
    connect(tcpsocket,SIGNAL(connected()),contimer,SLOT(stop()));
    connect(contimer,SIGNAL(timeout()),this,SLOT(connecttoserver()));
    connect(tcpsocket,SIGNAL(disconnected()),contimer,SLOT(start()));
    connect(tcpsocket,SIGNAL(error(QAbstractSocket::SocketError)),contimer,SLOT(start()));
    contimer->start();
    connect(tcpsocket,SIGNAL(connected()),this,SLOT(sendMessage()));
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMessage()));
    this->inittimer();
    this->connecttoserver();
}

int client::agentstatus()
{
    QSettings *reg = new QSettings(QString(REGEDIT_NAME),QSettings::NativeFormat);
    if(reg->value("ProxyEnable")==1)
    {
        return 1;
    }
    else{
        return -1;
    }
}

QString client::geta_host()
{
    if(agentstatus()==1)
    {
        QSettings *reg = new QSettings(QString(REGEDIT_NAME),QSettings::NativeFormat);
        QString porxyserver=reg->value("ProxyServer").toString();
        if(porxyserver.isEmpty())
        {
            qDebug()<<"server not setting";
            return "0";
        }
        QStringList pro=porxyserver.split(":");
        qDebug()<<"a_host1="<<pro[0];
        return pro[0];
    }
    else{
        qDebug()<<"proxy disable";
        return "-1";
    }
}

QString client::geta_port()
{
    if(agentstatus()==1)
    {
        QSettings *reg = new QSettings(QString(REGEDIT_NAME),QSettings::NativeFormat);
        QString porxyserver=reg->value("ProxyServer").toString();
        if(porxyserver.isEmpty())
        {
            qDebug()<<"server not setting";
            return "0";
        }
        if(!porxyserver.contains(":"))
        {
            qDebug()<<"port not setting";
            return "0";
        }
        QStringList pro=porxyserver.split(":");
        QString port=pro[1];
        return pro[1];
    }
    else{
        qDebug()<<"proxy disable";
        return "-1";
    }
}

void client::setagent(int ifenable,QString host,QString port)
{
    QSettings *reg = new QSettings(QString(REGEDIT_NAME),QSettings::NativeFormat);
    if(ifenable==1)
    {
        reg->setValue("ProxyEnable",1);
        QString host_port=host.append(":").append(port);
        reg->setValue("ProxyServer",host_port);
        reg->setValue("ProxyOverride","windows10.microdone.cn");
        return ;
    }
    else if(ifenable==0){
        reg->setValue("ProxyEnable",0);
        return ;
    }
}


void client::init_data()
{
    server="192.168.8.150";
    s_port="6666";
    a_host=geta_host();
    a_port=geta_port();
    if(a_host=="-1"|a_host=="0"|a_port=="0")
    {
        a_host="127.0.0.1";
        a_port="1080";
        flush_agent();
    }
    a_frequency=1.0;
    s_frequency=1.0;
    username=this->getusername();
    localhostname=QHostInfo::localHostName();
    winversion=getwinversion();
}

void client::inittimer()
{
    mytime = new QTimer(this);
    sysnctimer=new QTimer(this);
    connect(mytime, SIGNAL(timeout()), this, SLOT(flush_agent()));
    connect(sysnctimer,SIGNAL(timeout()),this,SLOT(sendMessage()));
    mytime->start(a_frequency*60000);
    sysnctimer->start(s_frequency*60000);
}

void client::connecttoserver()
{
    tcpsocket->connectToHost(server,6666);
}

void client::readMessage()
{

    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_5_5);
    message="";
    in>>message;
    flag=message.mid(0,1);
    qDebug()<<"read data from server:";
    qDebug()<<"message:"<<message;
    if(flag=="2")//将本机代理设置传送到服务端
    {
        sendMessage();
    }
    if(flag=="1")//同步服务器端代理设置
    {

        QStringList list=message.split(":");
        a_host=list[1];
        a_port=list[2];
        a_frequency=list[3].toFloat();
        s_frequency=list[4].toFloat();
        message="";
        flush_agent();
        sendMessage();
    }
    else if(flag=="4")//enable
    {
        mytime->start();
        setagent(1,a_host,a_port);
    }
    else if(flag=="5")//disable
    {
        mytime->stop();
        setagent(0,a_host,a_port);
    }

}

void client::sendMessage()
{
    qDebug()<<"send setting to server";
    QByteArray block;
    QDataStream out(&block,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_5);
    aa_host=a_host;
    aa_port=a_port;
    aa_frequency=a_frequency;
    ss_frequency=s_frequency; 
    message=aa_host.append(":").append(aa_port).append(":").append(QString::number(aa_frequency)).append(":").append(QString::number(ss_frequency)).append(":").append(localhostname).append(":").append(username).append(":").append(winversion);
    std::string str=message.toStdString();
    out<<tr(str.c_str());
    tcpsocket->write(block);
}

void client::flush_agent()
{
    qDebug()<<"flush ie proxy setting";
    setagent(1,a_host,a_port);
}

QString client::GetHtml(QString url)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(url)));
    QByteArray responseData;
    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    eventLoop.exec();
    responseData = reply->readAll();
    return QString(responseData);
}
QString client::GetNetIP(QString webCode)
{
    QString web = webCode.replace(" ", "");
    web = web.replace("\r", "");
    web = web.replace("\n", "");
    QStringList list = web.split("<br/>");
    if(list.size() < 4)
        return "0.0.0.0";
    QString tar = list[3];
    QStringList ip = tar.split("=");
    return ip[1];
}

QString client::returnOutIP()
{
    return GetNetIP(GetHtml("http://whois.pconline.com.cn/"));
}

QString client::getusername()
{

#if 1
    QStringList envVariables;
    envVariables << "USERNAME.*" << "USER.*" << "USERDOMAIN.*"
                 << "HOSTNAME.*" << "DOMAINNAME.*";
    QStringList environment = QProcess::systemEnvironment();
    foreach (QString string, envVariables) {
        int index = environment.indexOf(QRegExp(string));
        if (index != -1) {
            QStringList stringList = environment.at(index).split('=');
            if (stringList.size() == 2) {
                return stringList.at(1);
                break;
            }
        }
    }
    return "unknown";
#else
    QString userName = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    userName = userName.section("/", -1, -1);
    return userName;
#endif
}

QString client::getwinversion()
{
    QString winVersion = "";
    QSysInfo::WinVersion wv = QSysInfo::windowsVersion();
    switch (wv) {
    case QSysInfo::WV_XP:
        winVersion="windows xp";
        break;
    case QSysInfo::WV_WINDOWS7:
        winVersion="windows 7";
        break;
    default:
        break;
    }
    return winVersion;
}

client::~client()
{

}
