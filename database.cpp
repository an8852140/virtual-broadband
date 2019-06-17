#include "database.h"

Database_meter_id::Database_meter_id()
{

}


/* 建立数据库*/

 void Database_meter_id:: DB_BuildDatabase()                                  //建立并连接数据库
 {
     if (QSqlDatabase::contains("qt_sql_default_connection"))                  //若有则连接数据库
     {
         database = QSqlDatabase::database("qt_sql_default_connection");
     }
     else                                                                      //若没有则添加数据库
     {
         database = QSqlDatabase::addDatabase("QSQLITE");
         database.setDatabaseName("MyDataBase.db");
         database.setUserName("root");
         database.setPassword("123456");
     }
     if (!database.open())                 //没打开的情况下  打开
     {
         database.open();
     }
     QSqlQuery sql_query(database);        //建立查询类和数据库的连接
     sql_query.exec(QString("select count(*) from sqlite_master where type='table' and name='%1'").arg("archives"));
     if(sql_query.next())                  //查询数据库中表是否存在，不存在则添加,存在则清空数据。
     {
         if(sql_query.value(0).toInt() == 0)
         {
             sql_query.exec("create table archives (id int, meter_id int64_t, protocol_type int,done_flag int,primary key(id,meter_id))");
             sql_query.exec("create table master_node (id int , master_id int64_t primary key, router_type int)");
             sql_query.exec("insert into master_node values(1,010000000361,1)");
             sql_query.exec("insert into archives values(1,000000000012,1,0)");
             sql_query.exec("insert into archives values(2,000000000013,1,0)");
             sql_query.exec("insert into archives values(3,110000000099,1,0)");

             qDebug() << "Table Created";
         }
         else
         {
             sql_query.exec("delete from archives");
             sql_query.exec("delete from master_node");
             sql_query.exec("insert into master_node values(1,010000000361,1)");
             sql_query.exec("insert into archives values(1,000000000012,1,1)");
             sql_query.exec("insert into archives values(2,000000000013,1,1)");
             sql_query.exec("insert into archives values(3,189963591712,1,0)");
             qDebug() << "Table already exists,BuildDatabase()完成";
         }
     }

     database.close();
 }











/* 查询从节点数量
 * add_del 为true 添加节点
 * add_del为false 删除节点*/

 bool Database_meter_id:: DB_UpdateData(QByteArray meter_addr,bool add_del,int archives_type)
 {
     sql_query=QSqlQuery(database);
     database.open();
     int64_t   Intmeter_id=DB_ByteArray_To_Int(meter_addr);
     QString stb=QString ::number(Intmeter_id,10);
     qDebug()<<stb;
     if(add_del)
     {
       QString select_maxid_sql="select max(id) from archives ";
       int max_id=0;
       sql_query.prepare(select_maxid_sql);
       if(!sql_query.exec())
       {
           qDebug()<<sql_query.lastError();
       }
       else
       {
           while(sql_query.next())
              {
                  max_id = sql_query.value(0).toInt();
                  qDebug() << QString("max id:%1").arg(max_id);
              }
       }
       QString insert_sql="insert into archives values(?,?,?,?)";
       sql_query.prepare(insert_sql);
       sql_query.addBindValue(max_id+1);
       sql_query.addBindValue(Intmeter_id);
       sql_query.addBindValue(archives_type);
       sql_query.addBindValue(0);

       if(!sql_query.exec())
       {
           qDebug()<<sql_query.lastError();

       }
       else
       {
           qDebug()<<"添加从节点成功";
       }
     }
   else
   {
       QString delete_sql="delete from archives where meter_id = ?";
       sql_query.prepare(delete_sql);
       sql_query.addBindValue(Intmeter_id);
       if(!sql_query.exec())
       {
           qDebug()<<sql_query.lastError();
       }
       else
       {
           qDebug()<<"删除从节点成功";
       }
   }
   database.close();
   return  true;
 }













/* 查询从节点数量*/

int Database_meter_id::  DB_Node_Count()
 {
    int count = 0;
    sql_query = QSqlQuery(database);
    database.open();
    if(!sql_query.exec("select * from archives "))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        sql_query.last();
        count = sql_query.at()+1;
    }
    database.close();
    return count ;
 }



















/* 查询从节点信息*/

 QByteArray Database_meter_id:: DB_Query_node(int Start_idx)
 {
     sql_query=QSqlQuery(database);
     database.open();
     QByteArray meter_id,id_makeup  ;
     QByteArray meter_info;
     QString S1="select * from archives limit 1 offset ",S2;
     S2=QString::number(Start_idx-1, 10);

     if(!sql_query.exec(S1+S2))
     {
         qDebug()<<sql_query.lastError();
     }
     else
     {
         while(sql_query.next())
             {
                 meter_id  = sql_query.value(1).toByteArray();
                 meter_id  = meter_id.rightJustified(12,'0',true);
                 int protocol_type = sql_query.value(2).toInt();
                 qDebug()<<"111";
                 switch (protocol_type)
                 {
                    case 1: meter_info="F009";
                     break;
                    case 2: meter_info="F011";
                     break;
                    case 3: meter_info="F019";
                     break;
                 }

             }
     }
     return  meter_id.append(meter_info);
 }




/* 查询主节点*/

 QByteArray Database_meter_id:: DB_Query_node()
 {
     sql_query=QSqlQuery(database);
     database.open();
     QByteArray  master_id;
     if(!sql_query.exec("select master_id from master_node"))
     {
         qDebug()<<sql_query.lastError();
     }
     else
     {
         while(sql_query.next())
         {
             master_id = sql_query.value(0).toByteArray();

         }
     }
     database.close();
     return  master_id;
 }






/* 设置主节点地址*/

bool  Database_meter_id:: DB_Query_node(QByteArray frame)
 {
     sql_query=QSqlQuery(database);
     database.open();
     QByteArray  Bmaster_id=frame.mid(1,6);
     int   router_type=frame[7];
     int64_t   Intmaster_id=DB_ByteArray_To_Int(Bmaster_id);
     qDebug()<<Intmaster_id;
     QString insert_sql="insert into master_node values(?,?,?)";
     sql_query.prepare(insert_sql);
     sql_query.addBindValue(1);
     sql_query.addBindValue(Intmaster_id);
     sql_query.addBindValue(router_type);
     sql_query.exec("delete from master_node");

     if(!sql_query.exec())
     {
         qDebug()<<sql_query.lastError();
     }
     else
     {
         qDebug()<<"设置主节点地址成功";
     }
     database.close();
     return  true;

 }






/* 查询路由类型*/

 QByteArray  Database_meter_id:: DB_Query_Rter_Typ()
 {
     sql_query=QSqlQuery(database);
      database.open();
      QByteArray  Router_Type;
      if(!sql_query.exec("select router_type from master_node"))
      {
          qDebug()<<sql_query.lastError();
      }
      else
      {
          while(sql_query.next())
          {
              Router_Type = sql_query.value(0).toByteArray();
          }

      }
     database.close();
     return Router_Type;

 }























/* 更新路由类型*/

void  Database_meter_id:: DB_Query_Rter_Typ(int Router_Type)
 {
      sql_query=QSqlQuery(database);
      database.open();
      sql_query.prepare("update master_node set router_type = :router_type where id = 1");
      sql_query.bindValue(":router_type", Router_Type);

      if(!sql_query.exec())
      {
          qDebug()<<sql_query.lastError();
      }
      else
      {
          qDebug() << "路由类型更新成功";
      }
     database.close();
 }










/* 抄读完成测量点数量*/

int    Database_meter_id::  DB_Sucess_Count()
{
    int count=0;
    sql_query=QSqlQuery(database);
    database.open();

    if(!sql_query.exec("select done_flag from archives "))
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        while(sql_query.next())
        {
            count += sql_query.value(3).toInt();
        }
    }
    database.close();
    return count ;
}



/* 初始化函数*
 * para_data为false时参数初始化，为true时数据初始化*/
bool    Database_meter_id::       DB_Router_Init(bool para_data)
{
    sql_query=QSqlQuery(database);
    database.open();
    QString delete_para1="delete from archives",
            delete_para2="delete from master_node",
            delete_data ="update archives set done_flag = 0";
    if(para_data)
    {
        if(!sql_query.exec(delete_para1) && !sql_query.exec(delete_para2))
        {
            qDebug() << sql_query.lastError();
            database.close();
            return false;
        }
        else
        {
            qDebug() << "路由参数初始化完成";
            database.close();
            return  true;
        }
    }
    else
    {
        if(!sql_query.exec(delete_data))
        {
            qDebug() << sql_query.lastError();
            database.close();
            return false;
        }
        else
        {
            qDebug() << "路由数据初始化完成";
            database.close();
            return true;
        }
    }

}





















int64_t   Database_meter_id::   DB_ByteArray_To_Int(QByteArray Bmeter_addr)
{

    int64_t To_int=0, Bsize = Bmeter_addr.size();
    for(int i=0;i<Bsize;i++)
    {
        To_int += Bmeter_addr[i] * static_cast<int>(pow(10,Bsize-1-i));
    }
    return To_int;

}







