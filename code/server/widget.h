#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include <QString>
#include <QRadioButton>
#include <QCheckBox>
#include <QTableWidget>
#include "ui_widget.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    void init_table();
    ~Widget();
public:
    Ui::Widget *ui;
    QTableWidgetItem *checkBox_1;
private slots:
    void on_flushbutton_clicked();
    void on_confirm_clicked();
signals:
    void flushclicked();
    void confirmclicked(QString ip,QString port,QString af,QString sf);
};


#endif // WIDGET_H
