#ifndef FRAME_H
#define FRAME_H
#include <QByteArray>
#include "database.h"






class Frame
{

public:
      Frame(uchar FRM_Cs,bool FRM_DIR,bool FRM_PRM,char FRM_TX_mode,bool FRM_TX_flag, char FRM_Relay_Lv,char FRM_Envent_Flag,char FRM_Frm_number,
            char FRM_AFN,bool FRM_Auto_flag);
      Frame()=default;
     ~Frame();

    QByteArray FRM_Rec_Frame;                           //接收报文
    QByteArray FRM_Send_Frame;                          //发送报文






public:

    void        FRM_Analysis_Frame(QByteArray,QByteArray &);             //报文解析函数
    QByteArray  FRM_Report(int);
    bool        FRM_Cs_test (QByteArray);                                //校验位检测 报文合法性
    uchar       FRM_Cs_count(QByteArray);                                // 发送报文校验位计算
    QByteArray  FRM_ToByteArray(int,int,bool);                          //将int转换成bytearray数组
    QByteArray  FRM_ToByteArray(QString);                 //将String转换成bytearray数组
    QByteArray  FRM_ByteArray_Reverse(QByteArray);                      //高低位转换



private:
     void FRM_Length_Frame(QByteArray &);                                            //报文长度
     void FRM_Add_R_C();                                                            //添加信息域R,控制域C
     char FRM_Chanel();                                                              //信道标识




private:                                       //报文控制信息位
     uchar FRM_Cs;                              //校验位
     bool FRM_DIR;                              //传输方向位
     bool FRM_PRM;                             //启动标志位
     char FRM_TX_mode;                        //通信方式
     bool FRM_TX_flag;                       //通信模块标识
     char FRM_Relay_Lv;                      //中继级别
     char FRM_Envent_Flag;                    //事件标志位
     char FRM_Frm_number;                     //报文序列号
     QByteArray FRM_Slave_node;              //从节点地址
     char FRM_AFN;                           //应用功能码
     QByteArray FRM_Ti;                      //数据单元标识
     bool FRM_Auto_flag;                      //主动上报标志位


public:                                      //报文读取信息位

     int Node_Count;                    //从节点总数量


};

extern  Database_meter_id  Meter_id;

#endif // FRAME_H
