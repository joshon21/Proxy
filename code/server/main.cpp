//#include "widget.h"
#include "myserver.h"
#include <QApplication>
#include <QHash>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);  
    MyServer s;
    s.startServer();
    return a.exec();
}
