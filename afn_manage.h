#ifndef AFN_MANAGE_H
#define AFN_MANAGE_H
#include <QByteArray>
#include <QDebug>
#include "database.h"
#include "frame.h"

class Afn_Manage :public Frame
{

public:
   Afn_Manage();



  void AFN_Conf_Deny(QByteArray ,  QByteArray &);                                      //00H 确认否认
  void AFN_Initialize(QByteArray,  QByteArray &);                                     // 01H 初始化
  void AFN_Data_Inquiry(QByteArray,  QByteArray &);                                     //  03H 数据查询
  void AFN_Contral(QByteArray,  QByteArray &,QByteArray);                             //  05H 控制命令
  void AFN_Report(QByteArray,  QByteArray &,QByteArray);                               //06H 主动上报
  void AFN_Router_Inquiry(QByteArray,  QByteArray &,QByteArray);                         // 10H 路由查询
  void AFN_Router_Set(QByteArray,  QByteArray &,QByteArray);                            // 11H 路由设置
  void AFN_Router_Contral(QByteArray,  QByteArray &);                                  // 12H 路由控制
  void AFN_Data_Transmit(QByteArray,  QByteArray &,QByteArray);                        //  13H 路由数据转发
  void AFN_Data_Achieve(QByteArray,  QByteArray &,QByteArray);                         //14H 路由数据请求

  void AFN_Ti_03HF10(QByteArray &);

private:
  int AFN_Ti_Fn(QByteArray);


  void AFN_Ti_00HF01(QByteArray &);
  void AFN_Ti_00HF02(QByteArray &);

  void AFN_Ti_01HF01(QByteArray &);
  void AFN_Ti_01HF02(QByteArray &);
  void AFN_Ti_01HF03(QByteArray &);

  void AFN_Ti_03HF01(QByteArray &);
  void AFN_Ti_03HF04(QByteArray &);


  void AFN_Ti_05HF01(QByteArray &,QByteArray);

  void AFN_Ti_06HF02(QByteArray &,QByteArray);

  void AFN_Ti_10HF01(QByteArray &);
  void AFN_Ti_10HF02(QByteArray &,QByteArray);
  void AFN_Ti_10HF04(QByteArray &);

  void AFN_Ti_11HF01(QByteArray &,QByteArray);
  void AFN_Ti_11HF02(QByteArray &,QByteArray);

  void AFN_Ti_12HF01(QByteArray &);
  void AFN_Ti_12HF02(QByteArray &);
  void AFN_Ti_12HF03(QByteArray &);

  void AFN_Ti_13HF01(QByteArray &,QByteArray);

  void AFN_Ti_14HF01(QByteArray &,QByteArray);
  void AFN_Ti_14HF02(QByteArray &,QByteArray);







private:







};

extern  Database_meter_id  Meter_id;
extern  int                Router_Type;
static QString Version1="TCRJ";
static QString Version2="0309198803";

#endif // AFN_MANAGE_H
