#ifndef DATABASE_H
#define DATABASE_H
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QByteArray>
#include <QSqlQueryModel>
#include <cmath>

class Database_meter_id
{


public:
    Database_meter_id();






    int            DB_Node_Count();                    //查询当前从节点数量。
    int            DB_Sucess_Count();                 //查询抄读成功节点数量
    QByteArray     DB_Query_node(int);                 //查询从节点  开始序号在前  查询数据量在后
    QByteArray     DB_Query_node() ;                   //查询主节点
    bool           DB_Query_node(QByteArray frame);   //设置主节点地址
    QByteArray     DB_Query_Rter_Typ();               //查询路由类型
    bool           DB_Insert_Node(char[],int);         //增加档案
    bool           DB_Delete_Node(char[]);            //删除档案
    void           DB_BuildDatabase();                //重建数据库
    bool           DB_UpdateData(QByteArray,bool,int);  //更新档案
    void           DB_Query_Rter_Typ(int);             //查询路由类型
    bool           DB_Router_Init(bool);              //初始化命令




private:

    int64_t           DB_ByteArray_To_Int(QByteArray);
    QSqlDatabase  database;
    QSqlQuery     sql_query;



};

#endif // DATABASE_H
