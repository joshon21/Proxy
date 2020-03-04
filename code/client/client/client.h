#ifndef CLIENT_H
#define CLIENT_H
#include <Qtimer>
#include <QString>
#include <QMainWindow>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QSettings>
#include <QVariant>
#include <QTimer>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QString>

class client : public QMainWindow
{
    Q_OBJECT

public:
    client(QWidget *parent = 0);
    void init_data();
    void inittimer();
    void listening();
    int agentstatus();
    void setagent(int ifenable,QString host,QString port);
    QString geta_host();
    QString geta_port();
    QString getusername();
    QString getwinversion();
    ~client();
public:
    QTimer *mytime;
    QTimer *sysnctimer;
    QTimer *contimer;
    QString flag;  //message flag;

    //服务端
    QString server;
    QString s_port;

    //要发送的数据
    QString a_host;
    QString a_port;
    float a_frequency;
    float s_frequency;
    QString username;
    QString localhostname;
    QString winversion;

    QTcpSocket *tcpsocket;
    QString message;
    QString aa_host;
    QString aa_port;
    float aa_frequency;
    float ss_frequency;
private:
    QString GetHtml(QString url);
    QString GetNetIP(QString webCode);
    QString returnOutIP();
private slots:
    void flush_agent();
    void readMessage();
    void sendMessage();
    void connecttoserver();
};

#endif // CLIENT_H
