#include "afn_manage.h"




Afn_Manage::Afn_Manage()
{

}

  int Afn_Manage:: AFN_Ti_Fn(QByteArray AFN_Ti)    //  将数据标识转换成Fn
  {

      int fn;
      int x = AFN_Ti.at(0) & 0xFF;
      int y = AFN_Ti.at(1) & 0xFF;

      fn = y * 8 + 1;
      while (x != 1)
      {
         x= x>>1;
         fn++;
      }
      return fn;
  }


/* 00H 确认否认*/

void  Afn_Manage:: AFN_Conf_Deny(QByteArray AFN_Ti,QByteArray &Send_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:
              AFN_Ti_00HF01(Send_Frame);                 //确认
      break;
      case 2:
              AFN_Ti_00HF02(Send_Frame);                  //否认
      break;
      default:
              qDebug() << "暂未支持此数据标识"   ;
              AFN_Ti_00HF02(Send_Frame);
      }

  }
















/* 01H 初始化*/

 void  Afn_Manage:: AFN_Initialize(QByteArray AFN_Ti,QByteArray &Send_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:    AFN_Ti_01HF01(Send_Frame) ;
          break;
      case 2:    AFN_Ti_01HF02( Send_Frame) ;
          break;
      case 3:    AFN_Ti_01HF03( Send_Frame)  ;
          break;
      default:
                 qDebug() << "暂未支持此数据标识"   ;
                 AFN_Ti_00HF02(Send_Frame);
      }
  }




















/* 03H 数据查询*/

  void  Afn_Manage:: AFN_Data_Inquiry(QByteArray AFN_Ti,QByteArray &Send_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:      AFN_Ti_03HF01(Send_Frame);
          break;
      case 4:      AFN_Ti_03HF04(Send_Frame);
          break;
      case 10:     AFN_Ti_03HF10(Send_Frame);
          break;
      default:
                   AFN_Ti_00HF02(Send_Frame);
                   qDebug() << "暂未支持此数据标识"   ;
      }
  }
















/*  05H 控制命令*/

  void  Afn_Manage:: AFN_Contral(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray Rec_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:
                 AFN_Ti_05HF01(Send_Frame, Rec_Frame)  ;
          break;
      default:
                 AFN_Ti_00HF02(Send_Frame);
                 qDebug() << "暂未支持此数据标识"   ;

      }
  }














/* 06H 主动上报*/

 void  Afn_Manage:: AFN_Report(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray Rec_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {

      case 2:      AFN_Ti_06HF02(Send_Frame, Rec_Frame) ;
          break;
      default:
                   AFN_Ti_00HF02(Send_Frame);
                   qDebug() << "暂未支持此数据标识"   ;
      }
  }











/* 10H 路由查询*/

  void  Afn_Manage:: AFN_Router_Inquiry(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray frame)
  {

      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:        AFN_Ti_10HF01(Send_Frame) ;                //
          break;
      case 2:        AFN_Ti_10HF02(Send_Frame,frame);            //
          break;
      case 4:        AFN_Ti_10HF04(Send_Frame) ;           //
          break;
      default:
                     AFN_Ti_00HF02(Send_Frame);
                     qDebug() << "暂未支持此数据标识"   ;
      }
  }











/* 11H 路由设置*/

  void  Afn_Manage:: AFN_Router_Set(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:        AFN_Ti_11HF01(Send_Frame,frame) ;                //
          break;
      case 2:        AFN_Ti_11HF02(Send_Frame,frame) ;               //
          break;
      default:
                     AFN_Ti_00HF02(Send_Frame);
                     qDebug() << "暂未支持此数据标识"   ;
      }
  }














/* 12H 路由控制*/

  void  Afn_Manage:: AFN_Router_Contral(QByteArray AFN_Ti,QByteArray &Send_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:       AFN_Ti_12HF01(Send_Frame);               //
          break;
      case 2:       AFN_Ti_12HF01(Send_Frame);              //
          break;
      case 3:       AFN_Ti_12HF01(Send_Frame);              //
          break;
      default:
                    AFN_Ti_00HF02(Send_Frame);
                    qDebug() << "暂未支持此数据标识"   ;
      }
  }

















/* 13H 路由数据转发*/

  void  Afn_Manage:: AFN_Data_Transmit(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray Rec_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:      AFN_Ti_13HF01(Send_Frame, Rec_Frame) ;                  //
          break;
      default:
                   AFN_Ti_00HF02(Send_Frame);
                   qDebug() << "暂未支持此数据标识"   ;

      }
  }















/* 14H 路由数据请求*/

  void  Afn_Manage:: AFN_Data_Achieve(QByteArray AFN_Ti,QByteArray &Send_Frame,QByteArray Rec_Frame)
  {
      switch (AFN_Ti_Fn(AFN_Ti))
      {
      case 1:     AFN_Ti_14HF01(Send_Frame, Rec_Frame);                    //
          break;
      case 2:     AFN_Ti_14HF02(Send_Frame, Rec_Frame);                    //
          break;
      default:
                  AFN_Ti_00HF02(Send_Frame);
                  qDebug() << "暂未支持此数据标识"   ;
      }
  }









/*确认*/

 void Afn_Manage:: AFN_Ti_00HF01(QByteArray &Send_Frame)
 {
   QString STB="000100010000000000";
   Send_Frame.append(FRM_ToByteArray(STB));

 }














/*否认*/

void Afn_Manage:: AFN_Ti_00HF02(QByteArray &Send_Frame)
{
    QString STB="000200010000000000";
    Send_Frame.append(FRM_ToByteArray(STB));
}











/*硬件初始化*/

void   Afn_Manage:: AFN_Ti_01HF01(QByteArray &Send_Frame)
{
     AFN_Ti_00HF01(Send_Frame);
}













/*参数初始化*/

void   Afn_Manage:: AFN_Ti_01HF02(QByteArray &Send_Frame)
{
    if(Meter_id.DB_Router_Init(false))
    {
        AFN_Ti_00HF01(Send_Frame);
    }
    else
    {
        AFN_Ti_00HF02(Send_Frame);
    }
}












/*数据初始化*/

void   Afn_Manage:: AFN_Ti_01HF03(QByteArray &Send_Frame)
{
    if(Meter_id.DB_Router_Init(true))
    {
        AFN_Ti_00HF01(Send_Frame);
    }
    else
    {
        AFN_Ti_00HF02(Send_Frame);
    }
}


















/*厂商代码和版本*/

void   Afn_Manage:: AFN_Ti_03HF01(QByteArray &Send_Frame)
{
    QString STB="030100";
    Send_Frame.append(FRM_ToByteArray(STB));
    Send_Frame.append(Version1);
    Send_Frame.append(FRM_ToByteArray(Version2));


}
















/*查询主节点地址*/

void   Afn_Manage:: AFN_Ti_03HF04(QByteArray &Send_Frame)
{
    QString STB="030800";
    Send_Frame.append(FRM_ToByteArray(STB));
    STB=Meter_id.DB_Query_node();
    Send_Frame.append(FRM_ToByteArray(STB));

}















/*本地通信模块运行信息*/

void   Afn_Manage:: AFN_Ti_03HF10(QByteArray &Send_Frame)
{
     QString STB="030201";
     Send_Frame.append(FRM_ToByteArray(STB));
     char ReadType =static_cast<char>( 0xF0);                            //周期抄读模式 01集中器主导，10路由主导，11兼容两种模式 是否需要下发从节点信息，1需要，0不需要
     Send_Frame.append(ReadType|static_cast<char>(Meter_id.DB_Query_Rter_Typ().toInt()));
     STB="3FC30000005AFF00FA00E20028";                                   //按实际路由数据回复
     Send_Frame.append(FRM_ToByteArray(STB));
     STB=Meter_id.DB_Query_node();
     Send_Frame.append(FRM_ToByteArray(STB));                            //添加主节点地址
     STB="0008";                                                         //支持的最大从节点数量
     Send_Frame.append(FRM_ToByteArray(STB));
     int node_cout=Meter_id.DB_Node_Count();
     Send_Frame.append(FRM_ToByteArray(node_cout,2,false));                    //调用数据库查询函数，添加从节点数量
     STB="130101130918";                                                 //等待时间及报文最大长度等
     Send_Frame.append(FRM_ToByteArray(STB));
     Send_Frame.append(Version1);
     Send_Frame.append(FRM_ToByteArray(Version2));                                          //版本信息
     STB="B004580264003200";
     Send_Frame.append(FRM_ToByteArray(STB));


}
























/*设置主节点地址*/

void   Afn_Manage:: AFN_Ti_05HF01(QByteArray &Send_Frame,QByteArray Rec_Frame)
{

    QByteArray master_id=Rec_Frame.mid(13,6);
    master_id.append(static_cast<char>(Router_Type));

    if(Meter_id.DB_Query_node(master_id))
    {
        AFN_Ti_00HF01(Send_Frame);
    }
    else
    {
        AFN_Ti_00HF02(Send_Frame);
    }
}


















/*上报抄读数据*/

void   Afn_Manage:: AFN_Ti_06HF02(QByteArray &Send_Frame,QByteArray Rec_Frame)
{
    QString STB="060200";
    Send_Frame.append(FRM_ToByteArray(STB));

}




















/*查询从节点数量*/

void   Afn_Manage:: AFN_Ti_10HF01(QByteArray &Send_Frame)
{
   QString STB="100100";
   Send_Frame.append(FRM_ToByteArray(STB));
   Node_Count=Meter_id.DB_Node_Count();
   Send_Frame.append(FRM_ToByteArray(Node_Count,2,false));
   QString S="0008";
   Send_Frame.append(FRM_ToByteArray(S));
}






/*查询从节点信息*/


void   Afn_Manage:: AFN_Ti_10HF02(QByteArray &Send_Frame,QByteArray frame)
{
    QString STB;
    int      H = frame[14];
    int      L = frame[13];
    int      Start_idx = H * 0x0F + L;                   //计算查询起始序号
    int      Query_count = frame[15];                   //查询从节点数量
    int      Response_Conut = 0;                        //本次应答从节点数量


    for (int i=0;i<Query_count;i++)
    {
        STB=Meter_id.DB_Query_node(Start_idx);
        Send_Frame.append(FRM_ToByteArray(STB));
        Start_idx++;
        if(!STB.isNull())
        {
            Response_Conut++;
        }
    }
    Node_Count = Meter_id.DB_Node_Count();
    Send_Frame.push_front(static_cast<char>(Response_Conut));
    Send_Frame.push_front(FRM_ToByteArray(Node_Count,2,false));
    STB="100200";
    Send_Frame.push_front(FRM_ToByteArray(STB));
}














/*查询路由运行状态*/

void   Afn_Manage:: AFN_Ti_10HF04(QByteArray &Send_Frame)
{
    QString STB="100800";
    Send_Frame.append(FRM_ToByteArray(STB));
    STB="0000030000000000000000";
    char    Run_Status = 0x03;
    int     Sucess_Count = Meter_id.DB_Sucess_Count();
            Node_Count=Meter_id.DB_Node_Count();
     Send_Frame.append(Run_Status);
     Send_Frame.append(FRM_ToByteArray( Node_Count,2,false));
     Send_Frame.append(FRM_ToByteArray( Sucess_Count,2,false));
     Send_Frame.append(FRM_ToByteArray( STB));
}














/* 添加从节点*/

void   Afn_Manage:: AFN_Ti_11HF01(QByteArray &Send_Frame,QByteArray frame)
{

    int        Insert_Count=frame[13];
    int        meter_type=0;
    QByteArray BMeter_addr;

    for(int i=0;i<Insert_Count;i++)
    {
       BMeter_addr=frame.mid(14+i*7,6);
       meter_type=frame[20+i*7];
       if( Meter_id.DB_UpdateData(BMeter_addr,true,meter_type))
       {
           AFN_Ti_00HF01(Send_Frame);
       }
       else
       {
           AFN_Ti_00HF02(Send_Frame);
       }

    }
}











/* 删除从节点*/

void   Afn_Manage:: AFN_Ti_11HF02(QByteArray &Send_Frame,QByteArray frame)
{
    int        Delete_Count=frame[13];
    QByteArray BMeter_addr;
    for(int i=0;i<Delete_Count;i++)
    {
       BMeter_addr=frame.mid(14+i*6,6);
       if( Meter_id.DB_UpdateData(BMeter_addr,false,0))
       {
           AFN_Ti_00HF01(Send_Frame);
       }
       else
       {
           AFN_Ti_00HF02(Send_Frame);
       }

    }
}


















/* 路由重启*/

void   Afn_Manage:: AFN_Ti_12HF01(QByteArray &Send_Frame)
{
    AFN_Ti_00HF01(Send_Frame);
}





















/* 路由暂停*/

void   Afn_Manage:: AFN_Ti_12HF02(QByteArray &Send_Frame)
{
    AFN_Ti_00HF01(Send_Frame);
}

















/* 路由恢复*/

void   Afn_Manage:: AFN_Ti_12HF03(QByteArray &Send_Frame)
{
    AFN_Ti_00HF01(Send_Frame);
}


















/* 路由数据转发*/

void   Afn_Manage:: AFN_Ti_13HF01(QByteArray &Send_Frame,QByteArray Rec_Frame)
{
    QString STB="130100";
    Send_Frame.append(FRM_ToByteArray(STB));
}




















/* 路由路由请求抄读*/

void   Afn_Manage:: AFN_Ti_14HF01(QByteArray &Send_Frame,QByteArray Rec_Frame)
{
    QString STB="140100";
    Send_Frame.append(FRM_ToByteArray(STB));
}


















/* 路由请求集中器时钟*/

void   Afn_Manage:: AFN_Ti_14HF02(QByteArray &Send_Frame,QByteArray Rec_Frame)
{
    QString STB="140200";
    Send_Frame.append(FRM_ToByteArray(STB));
}























