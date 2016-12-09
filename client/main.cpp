#include "dialog.h"
#include"member.h"
#include"chief.h"
#include"id.h"

int Id;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog login;
    if(login.exec()==QDialog::Accepted)
    {
        if(Id<1000)
        {
            member b;
            b.exec();
        }
        else
        {
            chief b;
            b.exec();
        }
        return 0;
    }
    else
        return 0;
}
