#ifndef MEMBER_H
#define MEMBER_H

#include "id.h"

namespace Ui {
class member;
}

class member : public QDialog
{
    Q_OBJECT

public:
    explicit member(QWidget *parent = 0);
    ~member();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::member *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    void FindTaskName();
    void FindTaskTime();
    int _Id;
private slots:
void processPendingDatagram3();
long strtotime(QString);
};

#endif // MEMBER_H
