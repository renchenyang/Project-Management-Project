#include "id.h"
#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);
    receiver->bind(45457, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(receiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));
         connect(receiver,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(SeverError()));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_clicked()
{
    Id = ui ->userlineEdit->text().trimmed().toInt();
    _Id = Id;
    QByteArray datagram = QString::number(Id).toLatin1() + QByteArray(" 0 ") + QString::number(Id).toLatin1() + QByteArray(" ") + ui->pwdlineEdit->text().toLatin1();
    sender->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, 45455);
}

void Dialog::processPendingDatagram() //处理等待的数据报
{
    try
    {
        while (receiver->hasPendingDatagrams()) //拥有等待的数据报
        {
            QByteArray datagram; //拥于存放接收的数据报
            datagram.resize(receiver->pendingDatagramSize());
            //pendingDatagramSize为返回第一个在等待读取报文的size，resize函数是把datagram的size归一化到参数size的大小一样
            //让 datagram 的大小为等待处理的数据报的大小，这样才能接收到完整的数据
            receiver->readDatagram(datagram.data(), datagram.size());
            //接收数据报，将其存放到 datagram 中
            QTextStream ts( datagram, QIODevice::ReadOnly );
            int o, s;
            ts >> s >> o;
                if (s == _Id)
                    switch (o)
                    {
                    case 0:
                        QMessageBox::warning(this, QString(QLatin1String("Warning")), QString(QLatin1String("user name or password error!")), QMessageBox::Yes);
                        break;
                    case 1:
                        accept();
                        break;
                    default:
                        throw 1;
                        break;
                    }

        }
    }
    catch (...)
    {
        QMessageBox::warning(this, QString(QLatin1String("Warning")), QString(QLatin1String("Transport error!")), QMessageBox::Yes);
    }
}
void Dialog::on_pushButton_2_clicked()
{
    reject();
}

void Dialog::SeverError()
{
QMessageBox::warning(this, QString(QLatin1String("Warning")), QString(QLatin1String("Disconnection from the sever!")), QMessageBox::Yes);
}
