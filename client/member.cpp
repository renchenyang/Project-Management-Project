#include "member.h"
#include "ui_member.h"
#include "id.h"

member::member(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::member)
{
  _Id = Id;
  ui->setupUi(this);
  sender = new QUdpSocket(this);
  receiver = new QUdpSocket(this);
  receiver->bind(45456, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(receiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagram3()));
}

member::~member()
{
  delete ui;
}

void member::on_pushButton_3_clicked()
{
  QByteArray datagram = "3";
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void member::processPendingDatagram3() //处理等待的数据报
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
    int o, s,l;
    ts >> s >> o;
    if (s == _Id)
    {
      if (s > 1000)
        l = 5;
      else
        l = 2;
      QString content(datagram);
      content = content.right(content.length() - l - 2);
      switch (o)
      {
      case 3:
        QMessageBox::information(this, QString(QLatin1String("Personal information")), content, QMessageBox::Yes);
        break;
      case 2:
        QMessageBox::information(this, QString(QLatin1String("ProjList")), content, QMessageBox::Yes);
        break;
      case 1:
        QMessageBox::information(this, QString(QLatin1String("TaskList")), content, QMessageBox::Yes);
        break;
      case 4:
        QMessageBox::information(this, QString(QLatin1String("results")), content, QMessageBox::Yes);
        break;
      case 5:
        QMessageBox::information(this, QString(QLatin1String("Search results")), content, QMessageBox::Yes);
        break;
      default:
        break;
      }
    }
  }
}

void member::on_pushButton_2_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + QByteArray(" 2");
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void member::on_pushButton_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + QByteArray(" 1");
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void member::on_pushButton_4_clicked()
{
  bool isOK;
  QString text = QInputDialog::getText(NULL, "Input Project ID",
                                       "Please input Project ID",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  QByteArray datagram;
  if (isOK)
  {
    datagram = QString::number(_Id).toLatin1() + " 4 " + text.toLatin1() + QByteArray(" ");
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Input task name",
                                         "Please input task name:",
                                         QLineEdit::Normal,
                                         "0",
                                         &isOK);
    if (isOK)
    {
      datagram += text.toLatin1();
      sender->writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress::Broadcast, 45455);
    }
  }
}

void member::on_pushButton_5_clicked()
{
  switch ( QMessageBox::information( NULL, "Search task",
                                     "By title or by time?",
                                     QString(QLatin1String("By title")), QString(QLatin1String("By time") )))
  {
  case 0:
    FindTaskName();
    break;
  case 1:
    FindTaskTime();
    break;
  default:
    break;
  }
}

void member::FindTaskName()
{
  bool isOK;
  QString text = QInputDialog::getText(NULL, "By title",
                                       "Input title:",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  QByteArray datagram = QString::number(_Id).toLatin1() + QByteArray(" 5 0 ") + text.toLatin1();
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void member::FindTaskTime()
//该函数能够显示所有符合规则的结果
{
  long t1, t2;
  bool isOK;
  QString text = QInputDialog::getText(NULL, "By time",
                                       "Input sample:2015-5-30 10:33:50\nthe earliest time:",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  if (isOK)
  {
    isOK = false;
    t1 = strtotime(text);//将输入的标准格式时间转化为长整型
    QString text = QInputDialog::getText(NULL, "By time",
                                         "Input sample:2015-5-30 10:33:50\nthe earliest time:",
                                         QLineEdit::Normal,
                                         "0",
                                         &isOK);
    if (isOK)
    {
      t2 = strtotime(text);
      QByteArray datagram = QByteArray("5 1 " ) + QString::number(t1).toLatin1() + QByteArray(" ") + QString::number(t2).toLatin1();
      sender->writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress::Broadcast, 45455);
    }
  }
}

long member::strtotime(QString s)
{
  struct tm t;
  char*  ch;
  QByteArray ba = s.toLatin1();
  ch = ba.data();
  sscanf(ch, "%d-%d-%d %d:%d:%d", &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);
  t.tm_year -= 1900;
  t.tm_mon -= 1;
  return mktime(&t);    //转换为长整型
}

