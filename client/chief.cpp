#include "chief.h"
#include "ui_chief.h"
#include"id.h"


chief::chief(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::chief)
{
  _Id = Id;
  ui->setupUi(this);
  sender = new QUdpSocket(this);
  receiver = new QUdpSocket(this);
  receiver->bind(45456, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
  connect(receiver, SIGNAL(readyRead()), this, SLOT(processPendingDatagram2()));
}

chief::~chief()
{
  delete ui;
}

void chief::on_pushButton_3_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + " 3";
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void chief::processPendingDatagram2() //process the waiting data packet
{
  while (receiver->hasPendingDatagrams()) //for the waiting data packet
  {
    QByteArray datagram; //store the waiting data packet
    datagram.resize(receiver->pendingDatagramSize());
    //pendingDatagramSize为返回第一个在等待读取报文的size，resize函数是把datagram的size归一化到参数size的大小一样
    //让 datagram 的大小为等待处理的数据报的大小，这样才能接收到完整的数据
    receiver->readDatagram(datagram.data(), datagram.size());
    //receive the data packet and store it in the datagram
    QTextStream ts( datagram, QIODevice::ReadOnly );
    int o, s,l;
    ts >> s >> o;
    if (s == _Id)
    {
      if (s > 1000)
        l = 5;
      else
        l = 2;
      QString content;
      content = datagram;
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
      case 6:
        QMessageBox::information(this, QString(QLatin1String("results")), content, QMessageBox::Yes);
        break;
      case 7:
        QMessageBox::information(this, QString(QLatin1String("results")), content, QMessageBox::Yes);
        break;
      default:
        break;
      }
    }
  }
}

void chief::on_pushButton_2_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + " 2";
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void chief::on_pushButton_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + " 1";
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void chief::on_pushButton_4_clicked()
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
    datagram = QString::number(_Id).toLatin1() + " 4 " + text.toLatin1() + " ";
    bool isOK;
    QString text2 = QInputDialog::getText(NULL, "Input task name",
                                          "Please input task name:",
                                          QLineEdit::Normal,
                                          "0",
                                          &isOK);
    if (isOK)
    {
      datagram += text2.toLatin1();
      sender->writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress::Broadcast, 45455);
    }
  }
}

void chief::on_pushButton_5_clicked()
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

void chief::FindTaskName()
{
  bool isOK;
  QString text = QInputDialog::getText(NULL, "By title",
                                       "Input title:",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  QByteArray datagram = QString::number(_Id).toLatin1() + " 5 0 " + text.toLatin1();
  sender->writeDatagram(datagram.data(), datagram.size(),
                        QHostAddress::Broadcast, 45455);
}

void chief::FindTaskTime()
//This function can show all the results wanted.
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
      QByteArray datagram = QString::number(_Id).toLatin1() + QByteArray("5 1 ") + QString::number(t1).toLatin1() + QByteArray(" ") + QString::number(t2).toLatin1();
      sender->writeDatagram(datagram.data(), datagram.size(),
                            QHostAddress::Broadcast, 45455);
    }
  }
}


void chief::on_pushButton_6_clicked()
{
  bool isOK;
  QString text = QInputDialog::getText(NULL, "release project",
                                       "new project name:",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  if (isOK)
  {
    QByteArray datagram = QString::number(_Id).toLatin1() + " 6 " + text.toLatin1();
    sender->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, 45455);
  }
}

void chief::on_pushButton_7_clicked()
{
  QByteArray datagram = QString::number(_Id).toLatin1() + " 7 ";
  bool isOK;
  QString text = QInputDialog::getText(NULL, "release task",
                                       "project ID:",
                                       QLineEdit::Normal,
                                       "0",
                                       &isOK);
  QString text2;
  if (isOK)
  {
    isOK = false;
    switch ( QMessageBox::information( NULL, "release task",
                                       "Select the kind of the new task.",
                                       QString(QLatin1String("Common")), QString(QLatin1String("Private")), QString(QLatin1String("Instruct") )))
    {
    case 0:
      text2 = QInputDialog::getText(NULL, "release task",
                                    "new task name:",
                                    QLineEdit::Normal,
                                    "0",
                                    &isOK);
      datagram += "0 " + text + " " + text2;
      break;
    case 1:
      text2 = QInputDialog::getText(NULL, "release task",
                                    "new task name:",
                                    QLineEdit::Normal,
                                    "0",
                                    &isOK);
      datagram += "1 " + text + " " + text2;
      break;
    case 2:
      text2 = QInputDialog::getText(NULL, "release task",
                                    "new task name:",
                                    QLineEdit::Normal,
                                    "0",
                                    &isOK);
      datagram += "2 " + text + " " + text2;
      break;
    default:
      break;
    }
    sender->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, 45455);
  }
}

long chief::strtotime(QString s)
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

void chief::on_pushButton_8_clicked()
{
  reject();
}
