#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QTcpSocket>
#include <QHash>

class Mytcpsocket : public QTcpSocket
{
    Q_OBJECT

public:
    Mytcpsocket(QObject *parent=0);

private slots:
    void se(QString a_host,QString a_port,QString a_frequency,QString s_frequency);
    void flu();
    void disableprox(int ifdisable);

};

#endif // MYTCPSOCKET_H
