#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#define MaxRecBuffer 2000

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "frame.h"
#include <QtSerialPort>
#include <QTimer>
#include <QMessageBox>
#include "database.h"
#include "afn_manage.h"
#include <QDebug>




namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr,int  MW_rec_count=1,int MW_send_count=1);

    ~MainWindow();

    Ui::MainWindow *ui;


    Frame         Frame;
    QSerialPort  *serial;
    QTimer       *Time_delay;



    int MW_rec_count;                                 //接收报文计数器
    int MW_send_count;                                //输出报文计数器


    void        MW_scanport( );
    void        MW_sendframe(QByteArray &);
    QByteArray  MW_Add_Space(QByteArray );


private slots:
    void   on_dakaichuankou_clicked();
    void   MW_TimeUpdate();
    void   MW_recframe();
    void   on_pushButton_2_clicked();





private:


    QByteArray  Master_node ;             //主节点地址



};


#endif // MAINWINDOW_H


extern  Database_meter_id  Meter_id;
extern  int                Router_Type;              //路由的类型
