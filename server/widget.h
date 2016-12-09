#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
protected:
    void closeEvent ( QCloseEvent * );
private:
    Ui::Widget *ui;
    QUdpSocket *sender;
    QUdpSocket *receiver;


private slots:
    void processPendingDatagram();
};

#endif // WIDGET_H
