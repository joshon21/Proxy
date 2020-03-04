#include "widget.h"
#include <QWidget>
#include <QRadioButton>
#include <QCheckBox>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);   
    init_table();
}

void Widget::init_table()
{
    this->setWindowTitle("代理管理工具");
    ui->lineEdit1->setText("127.0.0.1");
    ui->lineEdit2->setText("1080");
    ui->lineEdit3->setText("1");//服务器端设置默认IE刷新频率
    ui->lineEdit4->setText("1");//服务器端设置默认连接频率
    ui->tableWidget->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);
    ui->tableWidget->setRowCount(50);
    ui->tableWidget->setColumnCount(10);
    ui->tableWidget->setColumnWidth(0,130);
    ui->tableWidget->setColumnWidth(1,130);
    ui->tableWidget->setColumnWidth(2,130);
    ui->tableWidget->setColumnWidth(3,130);
    ui->tableWidget->setColumnWidth(4,120);
    ui->tableWidget->setColumnWidth(5,120);
    ui->tableWidget->setColumnWidth(6,120);
    ui->tableWidget->setColumnWidth(7,120);
    ui->tableWidget->setColumnWidth(8,120);
    ui->tableWidget->setColumnWidth(9,120);
    checkBox_1 = new QTableWidgetItem();
    checkBox_1->setText("全选");
    checkBox_1->setCheckState(Qt::Unchecked);
    QStringList headersting;
    headersting<<"HOSTNAME"<<"UserName"<<"系统版本"<<"IP"<<"AGENT"<<"PORT"<<"刷新频率(分)"<<"连接频率(分)";
    ui->tableWidget->setHorizontalHeaderLabels(headersting);
    ui->tableWidget->setHorizontalHeaderItem(8,checkBox_1);
    ui->tableWidget->setHorizontalHeaderItem(9,new QTableWidgetItem("代理状态"));
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//不能改变表格
}



void Widget::on_flushbutton_clicked()
{
    emit flushclicked();
}


void Widget::on_confirm_clicked()
{
    emit confirmclicked(ui->lineEdit1->text(),ui->lineEdit2->text(),ui->lineEdit3->text(),ui->lineEdit4->text());
}

Widget::~Widget()
{
    delete ui;
}
