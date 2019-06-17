#include "setport.h"











void Port:: scanport()   //扫描当前可用串口
{

        QList<QSerialPortInfo> serialPortInfo = QSerialPortInfo::availablePorts();
        int PotNum=serialPortInfo.count();
         QString PotName;   
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

void Port:: recframe()
{


     if(serial.bytesAvailable()>MaxRecBuffer)
              QMessageBox::warning(this,tr("提示"),tr("数据长度超限"),QMessageBox::Ok);
      else
         {

              Time_delay.start(100);
              Baowen.Rec_Frame.append(serial.readAll());
         }
}




void Port:: TimeUpdate()    //通过添加定时器 来获取串口的完整报文，否则一帧报文会分开多次接收
{


      Time_delay.stop();
      if( Baowen.Cs_test(Baowen.Rec_Frame))
     {
          Baowen.Analysis_Frame(Baowen.Rec_Frame,Baowen.Send_Frame);   // 报文分析函数
          ui->baowenjiaohu->appendPlainText ( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz")+" [接收报文  "+QString::number(rec_count++ )+"]: "+Add_Space(Baowen.Rec_Frame));
          sendframe(Baowen.Send_Frame);

     }

     Baowen.Rec_Frame.clear();
}





void Port:: sendframe(QByteArray & send_frame)    //报文发送函数
{

    send_frame.push_back(Baowen.Cs_count(send_frame));
    send_frame.push_back(0x16);
    serial.write(send_frame);
    ui->baowenjiaohu->appendPlainText ( QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss:zzz")+" [发送报文  "+QString::number(send_count++)+"]: "+Add_Space(Baowen.Send_Frame));
    ui->baowenjiaohu->appendPlainText ("=======================================================================================================");
     send_frame.clear();




}


void Port:: UpdataPort()     //更新串口设置
{
  //  serial = new QSerialPort();
  //  Baowen =new Frame();
  //  Time_delay = new QTimer();
    int Router_Type;
    QByteArray Master_node;
    Master_node.resize(6);
    Master_node[0]=0x01;
    Master_node[1]=0x00;
    Master_node[2]=0x00;
    Master_node[3]=0x00;
    Master_node[4]=0x03;
    Master_node[5]=0x61;



    serial.setPortName(ui->chuankouhao->currentText());
    serial.setBaudRate(ui->chuankousulv ->currentData().toInt());
    serial.setDataBits(QSerialPort::Data8);
    serial.setParity(QSerialPort::OddParity);
    serial.setStopBits(QSerialPort::OneStop);
    serial.setFlowControl(QSerialPort::NoFlowControl);
    serial.setReadBufferSize(MaxRecBuffer);
    Router_Type=ui->fangan->currentData().toInt() ;


    if (serial.open(QIODevice::ReadWrite))
      {
        QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::recframe);
        QObject::connect(Time_delay, &QTimer::timeout, this, &MainWindow::TimeUpdate);
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






QByteArray Port:: Add_Space(QByteArray rec_frame ) //格式处理 每字节加入空格符
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
