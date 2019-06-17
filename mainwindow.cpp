#include "mainwindow.h"



MainWindow::MainWindow(QWidget *parent,int MW_rec_count,int MW_send_count) :
    QMainWindow(parent),MW_rec_count(1),MW_send_count(1),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    Meter_id.DB_BuildDatabase();
    MW_scanport();
}

/*--全局变量定义*/

Database_meter_id  Meter_id;
int                Router_Type;



MainWindow::~MainWindow()
{
    delete ui;
}


/*--打开串口*/
/*-串口初始化：
 * 数据位：8
 * 停止位：1
 * 偶校验
 * 无流控制
 * 速率可选
 * 最大字节200-*/
void MainWindow:: on_dakaichuankou_clicked()
{
   if(ui->dakaichuankou->text()=="打开串口")
     {
       serial      = new QSerialPort();
       Time_delay  = new QTimer();
       serial->setPortName(ui->chuankouhao->currentText());
       serial->setBaudRate(ui->chuankousulv ->currentData().toInt());
       serial->setDataBits(QSerialPort::Data8);
       serial->setParity(QSerialPort::EvenParity);
       serial->setStopBits(QSerialPort::OneStop);
       serial->setFlowControl(QSerialPort::NoFlowControl);
       serial->setReadBufferSize(MaxRecBuffer);
       Router_Type=ui->fangan->currentData().toInt() ;
       Meter_id.DB_Query_Rter_Typ(Router_Type);

       if (serial->open(QIODevice::ReadWrite))
            {
                QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow:: MW_recframe);
                QObject::connect(Time_delay, &QTimer::timeout, this, &MainWindow:: MW_TimeUpdate);
                ui->chuankouhao->setEnabled(false);
                ui->chuankousulv->setEnabled(false);
                ui->chuankoujiaoyan->setEnabled(false);
                ui->fangan->setEnabled(false);
                ui->dakaichuankou->setText("关闭串口");
             }
        else
             {
                 QMessageBox::warning(this,tr("提示"),tr("串口打开失败"),QMessageBox::Ok);
             }
   }

   else
   {
       ui->dakaichuankou->setText("打开串口");
       ui->chuankouhao->setEnabled(true);
       ui->chuankousulv->setEnabled(true);
       ui->chuankoujiaoyan->setEnabled(true);
       ui->fangan->setEnabled(true);

       serial->clear();
       serial->close();
       serial->deleteLater();
       delete serial;
       delete Time_delay;
   }
}


/*--清除报文*/
void MainWindow:: on_pushButton_2_clicked()
{
    ui->baowenjiaohu->clear();
    MW_rec_count=1;
    MW_send_count=1;
}

/*--扫描当前可用串口*/
void MainWindow:: MW_scanport()
{
    QString PotName;
    QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
    int PotNum=serialPortInfo.count();
    ui->chuankouhao->clear();
    for(int i=0;i<PotNum;i++)
        {
           PotName= serialPortInfo.at(i).portName();
           ui->chuankouhao->addItem(PotName);

        }
    ui->chuankoujiaoyan->addItem(tr("偶校验"),QSerialPort::EvenParity);
    ui->chuankoujiaoyan->addItem(tr("奇校验"),QSerialPort::OddParity);
    ui->chuankousulv->addItem(tr("9600bps"),QSerialPort::Baud9600);
    ui->chuankousulv->addItem(tr("115200bps"),QSerialPort::Baud115200);
    ui->fangan->addItem(tr("窄带路由"),1);
    ui->fangan->addItem(tr("宽带路由"),2);

}

/*--接收报文*/
void MainWindow::  MW_recframe()
{
     if(serial->bytesAvailable()>MaxRecBuffer)
         {
            QMessageBox::warning(this,tr("提示"),tr("数据长度超限"),QMessageBox::Ok);
         }
      else
         {
           Time_delay->start(100);
           Frame.FRM_Rec_Frame.append(serial->readAll());
         }
}

/*--定时器*/
/*--增加延时读取串口的完整报文，否则一帧报文会分开多次接收*/
void MainWindow::  MW_TimeUpdate()
{
      Time_delay->stop();
      if( Frame.FRM_Cs_test(Frame.FRM_Rec_Frame))
        {
           Frame.FRM_Analysis_Frame(Frame.FRM_Rec_Frame,Frame.FRM_Send_Frame);   // 报文分析函数
           ui->baowenjiaohu->appendPlainText ( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz")+" [接收报文  "+QString::number( MW_rec_count++ )+"]: "+ MW_Add_Space(Frame.FRM_Rec_Frame));
           MW_sendframe(Frame.FRM_Send_Frame);

         }
       Frame.FRM_Rec_Frame.clear();
}

/*--报文发送*/
void MainWindow::  MW_sendframe(QByteArray & send_frame)
{

   send_frame.push_back(static_cast<char>( Frame.FRM_Cs_count(send_frame)));
   send_frame.push_back(0x16);
   serial->write(send_frame);
   ui->baowenjiaohu->appendPlainText ( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz")+" [发送报文  "+QString::number( MW_send_count++)+"]: "+ MW_Add_Space(Frame.FRM_Send_Frame));
   ui->baowenjiaohu->appendPlainText ("=======================================================================================================");
   send_frame.clear();
}



/*--格式处理*/
/*--每字节加入空格符*/
QByteArray MainWindow::  MW_Add_Space(QByteArray rec_frame )
{
    QString frame_space=rec_frame.toHex();
    int leng=frame_space.size();
    if(leng%2==0)
    {
        leng=leng+leng/2;
        for (int i=0;i<=leng;i++)
        {
           while(i%3==0)
             {
               frame_space.insert(i," ");
               break;
             }

        }

        return frame_space.toUtf8().toUpper() ;

    }

    else
    {
        return "非法报文";
    }


}















