#ifndef SETPORT_H
#define SETPORT_H

#include <QByteArray>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QDateTime>
#include "mainwindow.h"



class Port : public MainWindow
{

public:

void scanport( );
void UpdataPort();
void sendframe(QByteArray &  );
QByteArray Add_Space(QByteArray );
void  recframe();
void  TimeUpdate() ;


private:




};



#endif // SETPORT_H
