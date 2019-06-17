#include "frame.h"
#include "afn_manage.h"

 Frame ::  Frame(uchar FRM_Cs,bool FRM_DIR,bool FRM_PRM,char FRM_TX_mode,bool FRM_TX_flag, char FRM_Relay_Lv,char FRM_Envent_Flag,char FRM_Frm_number,
                      char FRM_AFN,bool FRM_Auto_flag)   :
                FRM_Cs('0'),FRM_DIR(0),FRM_PRM(0),FRM_TX_mode('0'),FRM_TX_flag(0),FRM_Relay_Lv('0'),FRM_Envent_Flag('0'),FRM_Frm_number('0'),
                  FRM_AFN('0'),FRM_Auto_flag('0')
 {

 }




 Frame :: ~Frame()
 {

 }





void Frame:: FRM_Analysis_Frame(QByteArray frame,QByteArray & Send_Frame)

{

   Afn_Manage        FRM_AFN_Manage;


  if(frame.at(0)==0x68 && frame.at(frame.size()-1)==0x16)
  {

     FRM_Auto_flag =false;
     FRM_TX_flag   = frame.at(4)&0x04;              // 信息域R中通信模块标识的标志位为 0 时,无地址域,1时有地址域
     FRM_Relay_Lv  = static_cast<char>( frame.at(4)&0xF0);      //中继级别,0时无中继
     FRM_Frm_number = frame.at(9);             //  报文序列号
     FRM_DIR       = frame.at(3)&0x80;        //O下行报文   1上行报文
     FRM_PRM       = frame.at(3)&0x40;        //0来自从动站  1来自启动站
     FRM_TX_mode   = frame.at(3)&0x3F;       //只有  1窄带路由 和  3宽带路由


      if(FRM_TX_flag)                                 //为1时  有地址域A
      {

         FRM_Slave_node=frame.mid(16,6*FRM_Relay_Lv);
         FRM_AFN=frame.at(6*FRM_Relay_Lv+11);
         FRM_Ti=frame.mid(6*FRM_Relay_Lv+12,2);
      }
      else                                        //为0时  无地址域A
      {
       FRM_AFN=frame.at(10);                          //应用功能码
       FRM_Ti =frame.mid(11,2);                        //数据单元标识



      }



      switch (FRM_AFN)
          {
              case 0x00:

                   FRM_AFN_Manage.AFN_Conf_Deny(FRM_Ti,Send_Frame);
                   FRM_Add_R_C();
                   FRM_Length_Frame(Send_Frame);
                   break;
              case 0x01:
                   FRM_AFN_Manage.AFN_Initialize(FRM_Ti,Send_Frame);
                   FRM_Add_R_C();
                   FRM_Length_Frame(Send_Frame);
                   break;
              case 0x03:
                   FRM_AFN_Manage.AFN_Data_Inquiry(FRM_Ti,Send_Frame) ;
                   FRM_Add_R_C();
                   FRM_Length_Frame(Send_Frame);
                   break;
              case 0x05:
                        FRM_AFN_Manage.AFN_Contral(FRM_Ti,Send_Frame,frame) ;
                        FRM_Add_R_C();
                        FRM_Length_Frame(Send_Frame);
                   break;
              case 0x06:
                         FRM_AFN_Manage.AFN_Report(FRM_Ti,Send_Frame,frame) ;
                         FRM_Add_R_C();
                         FRM_Length_Frame(Send_Frame);
                   break;
              case 0x10:
                         FRM_AFN_Manage.AFN_Router_Inquiry(FRM_Ti,Send_Frame,frame) ;
                         FRM_Add_R_C();
                         FRM_Length_Frame(Send_Frame);

                   break;
              case 0x11:
                         FRM_AFN_Manage.AFN_Router_Set(FRM_Ti,Send_Frame,frame) ;
                         FRM_Add_R_C();
                         FRM_Length_Frame(Send_Frame);
                   break;
              case 0x12:
                          FRM_AFN_Manage.AFN_Router_Contral(FRM_Ti,Send_Frame) ;
                          FRM_Add_R_C();
                          FRM_Length_Frame(Send_Frame);
                   break;
              case 0x13:
                          FRM_AFN_Manage.AFN_Data_Transmit(FRM_Ti,Send_Frame,frame) ;
                          FRM_Add_R_C();
                          FRM_Length_Frame(Send_Frame);
                   break;
              case 0x14:
                          FRM_AFN_Manage.AFN_Data_Achieve(FRM_Ti,Send_Frame,frame) ;
                          FRM_Add_R_C();
                          FRM_Length_Frame(Send_Frame);
                   break;
          default:  qDebug() << "暂未支持此ANF"   ;


          }

      }
  }

/*

QByteArray Frame:: FRM_Report(int afn_ti)
{
    Afn_Manage      FRM_AFN_Manage;
    QByteArray      Report_Frame;
    QString Ti[6] = {"0201",  };
    FRM_Auto_flag = true;
    FRM_TX_flag   = 0x00;              // 信息域R中通信模块标识的标志位为 0 时,无地址域,1时有地址域
    FRM_Relay_Lv  = 0x00;             //中继级别,0时无中继
    FRM_Frm_number= 0x01;             //  报文序列号
    FRM_DIR       = 0x80;            //O下行报文   1上行报文
    FRM_PRM       = 0x40;           //0来自从动站  1来自启动站


    switch (afn_ti)
    {
    case 0:                         //03F10主动上报
        FRM_AFN_Manage.AFN_Data_Inquiry(FRM_ToByteArray(Ti[afn_ti]),Report_Frame) ;
        FRM_Add_R_C();
        FRM_Length_Frame(Report_Frame);
        break;
    case 1:
        break;

        default:  ;
    }




}


*/


 void Frame::   FRM_Add_R_C()
 {

     QByteArray R_C;
     char C;
     char Chl=FRM_Chanel();
     R_C.fill(0x00,3);               //制作信息域的无关字节
     R_C.insert(1,Chl);
     R_C.append(FRM_Frm_number);


     switch (Meter_id.DB_Query_Rter_Typ().toInt())
     {
     case 1: FRM_TX_mode=1;break;  //窄带路由
     case 2: FRM_TX_mode=3;break;   //宽带路由
     case 3: FRM_TX_mode=10;break;   //无线路由
     }

    if (FRM_TX_flag)                     // 组合信息域R的第一个字节
      {
        FRM_Relay_Lv+=0x04;              //判断是否需要加地址域
        R_C.append(Meter_id.DB_Query_node());
        R_C.append(FRM_Slave_node);
      }
     R_C.push_front(FRM_Relay_Lv);


     if(FRM_Auto_flag)      //非主动上报情况下
     {
        C=0x80+FRM_TX_mode;
     }
     else               //主动上报情况下
     {
       C=0xC0+FRM_TX_mode;
     }
     R_C.push_front(C);

     FRM_Send_Frame.push_front(R_C);
 }






char Frame:: FRM_Chanel()
  {
      char x=std::rand()%4;
      return  x;
  }






void Frame:: FRM_Length_Frame(QByteArray & Send_Frame)
{

    int  length=Send_Frame.size()+6-1;
    char H=0x00;
    char L=0x00;
    if(length>255)
    {
       H= static_cast<char>(length-255);
       L=static_cast<char>(length);
    }
       else
    {
        H= 0x00;
        L=static_cast<char>(length);
    }


    Send_Frame.push_front(H);
    Send_Frame.push_front(L);
    Send_Frame.push_front(0x68);





}





bool Frame:: FRM_Cs_test (QByteArray frame)
{

    FRM_Cs=0x00;
    int Cs_rec=frame.at(frame.size()-2)&0xFF;
    frame.toHex();
    int leng=frame.size();
    for(int i=3;i<leng-2;i++)
    {
      FRM_Cs+=frame.at(i);

    }



    if(FRM_Cs ==Cs_rec)
        return true;
    else
        return  false;



}


uchar Frame:: FRM_Cs_count(QByteArray frame)
{
    FRM_Cs=0x00;
    frame.toHex();
    int leng=frame.size();
    for(int i=3;i<leng;i++)
    {
      FRM_Cs+=frame.at(i);

    }

    return FRM_Cs;


}




/*将 int 类型转换为 ByteArray 类型*/
/* ----------------------------
 *先将 int 转为string  并补齐数据长度
 *之后将每两个string 转为 一个char  存入ByteArray
 *number_format:bin格式转换为十六进制，BCD码格式转为十进制*/

QByteArray Frame:: FRM_ToByteArray(int source_number,int size,bool bcd_bin)
{
   QByteArray target_number;
   int number_format;
   if(bcd_bin)
   {
      number_format=10;
   }
   else
   {
      number_format=16;
   }

   QString str = QString ::number(source_number,number_format);
   if(str.size()<size)
   {
       str = str.rightJustified(size,'0');
   }
   else
   {
       str.resize(size);
   }
   int str_long=str.size();
   char middle;
   QMap<QString,int> HexList={
       {"0",0},
       {"1",1},
       {"2",2},
       {"3",3},
       {"4",4},
       {"5",5},
       {"6",6},
       {"7",7},
       {"8",8},
       {"9",9},
       {"A",10},
       {"B",11},
       {"C",12},
       {"D",13},
       {"E",14},
       {"F",15}};

   for (int i=0;i<str_long;)
   {
      middle = static_cast<char>(HexList.value(str.at(i)) * number_format + HexList.value(str.at(i+1)));
      target_number.append(middle);
      i+=2;
   }
   return target_number;

}


/*将 QString 类型转换为 ByteArray 类型*/
/* ----------------------------
 *由输入者保证数据长度和格式正确！！！
 *将每两个string 转为 一个char  存入ByteArray*/

QByteArray Frame:: FRM_ToByteArray(QString source_number)
{
   QByteArray target_number;

   if(source_number.size()%2 == 1)
   {
      source_number.push_front('0');
   }

   int str_long=source_number.size();
   char middle;
   QMap<QString,int> HexList={
       {"0",0},
       {"1",1},
       {"2",2},
       {"3",3},
       {"4",4},
       {"5",5},
       {"6",6},
       {"7",7},
       {"8",8},
       {"9",9},
       {"A",10},
       {"B",11},
       {"C",12},
       {"D",13},
       {"E",14},
       {"F",15}};

   for (int i=0;i<str_long;)
   {
      middle=static_cast<char>(HexList.value(source_number.at(i))*16+HexList.value(source_number.at(i+1)));
      target_number.append(middle);
      i+=2;

   }
   return target_number;

}






/*高低字节反转*/
/* ----------------------------
 *协议中要求高字节在前发送，实际反转后再处理*/

QByteArray Frame:: FRM_ByteArray_Reverse(QByteArray source_qb)
{
    QByteArray target_qb=source_qb;
    auto target_end = target_qb.end();
    auto source_begin=source_qb.begin();
    for (;source_begin!=source_qb.end();source_begin++)
    {
        *(--target_end)=*source_qb.begin();
    }
    return  target_qb;
}




