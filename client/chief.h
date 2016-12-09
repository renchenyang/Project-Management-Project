#ifndef CHIEF_H
#define CHIEF_H

#include "id.h"

namespace Ui {
class chief;
}

class chief : public QDialog
{
    Q_OBJECT

public:
    explicit chief(QWidget *parent = 0);
    ~chief();

private slots:
    void on_pushButton_7_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::chief *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    void FindTaskName();
    void FindTaskTime();
    int _Id;
private slots:
void processPendingDatagram2();
long strtotime(QString);
};

#endif // CHIEF_H
