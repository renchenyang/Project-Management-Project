#include "widget.h"
#include "ui_widget.h"
#include "List.h"

void store();


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);
    receiver->bind(45455, QUdpSocket::ShareAddress);
    connect(receiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::processPendingDatagram() //处理等待的数据报
{
    while (receiver->hasPendingDatagrams()) //拥有等待的数据报
    {
        QByteArray datagram; //拥于存放接收的数据报
        QString password;
        QByteArray datagram2;
        datagram.resize(receiver->pendingDatagramSize());
        //pendingDatagramSize为返回第一个在等待读取报文的size，resize函数是把datagram的size归一化到参数size的大小一样
        //让 datagram 的大小为等待处理的数据报的大小，这样才能接收到完整的数据
        receiver->readDatagram(datagram.data(), datagram.size());
        //接收数据报，将其存放到 datagram 中
        QTextStream ts( datagram, QIODevice::ReadOnly );
        int o, s, l;
        ts >> s;
        if (s > 1000)
            l = 5;
        else
            l = 2;
        auto  iter = UserList.find(s);
        ts >> o;
        QString content(datagram);
        switch (o) {
        case 0:
            ts >> s;
            ts >> password;
            iter = UserList.find(s);
            if (iter != UserList.end() && (iter->second)->Login(password.toStdString()) == true)
            {
                datagram2 = QString::number(s).toLatin1()+" 1";
                sender->writeDatagram(datagram2.data(), datagram2.size(),
                                      QHostAddress::Broadcast, 45457);
            }
            else
            {
                datagram2 = QString::number(s).toLatin1()+" 0";
                sender->writeDatagram(datagram2.data(), datagram2.size(),
                                      QHostAddress::Broadcast, 45457);
            }
            break;
        case 1: case 2: case 3: case 4: case 5: case 6: case 7:
            if (iter != UserList.end())
            {
                content = content.right(content.length() - l);
                datagram2 = QString::number(s).toLatin1()+" "+((iter->second)->menu(content)).toLatin1();
                sender->writeDatagram(datagram2.data(), datagram2.size(),
                                      QHostAddress::Broadcast, 45456);
                //QMessageBox::information(this, QString(QLatin1String("Packet content")), QString(datagram2), QMessageBox::Yes);
            }
            break;
        default:
            break;
        }
        //show the content of the data package
    }
}

void Widget::closeEvent ( QCloseEvent * e )
{
    store();
}
