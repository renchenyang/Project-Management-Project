#include <QApplication>
#include "List.h"
#include "widget.h"
#include "ui_widget.h"

int AvaProjId;
int AvaTaskId;
int AvaMemId;
int AvaChiId;
int option;

TList SumTaskList;
UList UserList;
PMList MemList;
PCList ProjChiefList;
PList ProjectList;
CTList CommonTaskList;
ITList InstructTaskList;
STList SecretTaskList;

ofstream &operator<<(ofstream & fout, User & a);
ifstream &operator>>(ifstream & fin, ProjMember & a);
ofstream &operator<<(ofstream & fout, Project & a);
ifstream &operator>>(ifstream & fin, Project & a);
ofstream &operator<<(ofstream & fout, Task & a);
ifstream &operator>>(ifstream & , CommonTask & );

ofstream &operator<<(ofstream & fout, User & a)
{
    User* ptr = &a;
    ptr->OutFile(fout);
}

ifstream &operator>>(ifstream & fin, ProjMember & a)
{
    int _Id, _TaskDoneNum, _KPI;
    string _Name, _Password;
    fin >> _Id >> _TaskDoneNum >> _KPI;
    fin >> _Name >> _Password;
    if (_Id > 1000)
    {
        int _ProjReleaseNum, _TaskReleaseNum;
        fin >> _ProjReleaseNum >> _TaskReleaseNum;
        ProjChief b = ProjChief(_Id, _Name, _Password, _TaskDoneNum, _KPI, _ProjReleaseNum, _TaskReleaseNum);
    }
    else
        ProjMember a = ProjMember(_Id, _Name, _Password, _TaskDoneNum, _KPI);
}

ofstream &operator<<(ofstream & fout, Project & a)
{
    fout << a.Id << " " << a.ReleaserId << " " << a.Releasetime << " " << (a.TaskList).size();
    for (auto set_it = (a.TaskList).begin(); set_it != (a.TaskList).end(); ++set_it)
        fout << " " << *set_it ;
    fout << endl;
    fout << a.Title;
}

ifstream &operator>>(ifstream & fin, Project & a)
{
    int _Id, _ReleaserId, TaskId, _Tasknum;
    time_t _Releasetime;
    Tset _TaskList;
    string _Title;
    fin >> _Id >> _ReleaserId >> _Releasetime;
    fin >> _Tasknum;
    for (int i = 0; i < _Tasknum; i++)
    {
        fin >> TaskId;
        _TaskList.insert(TaskId);
    }
    fin >> _Title;
    Project(_Id, _ReleaserId, _Releasetime, _TaskList, _Title);
}

ofstream &operator<<(ofstream & fout, Task & a)
{
    fout << a.ProjectId << " " << a.Id << " " << a.ReleaserId << " " << a.kind << endl;
    fout << a.Title << endl;
    fout << a.Releasetime << " " << a.DoTime << " " << a.TaskState << " " << a.DoerId;
}

ifstream &operator>>(ifstream & fin, CommonTask & a)
{
    int _ProjectId, _Id, _ReleaserId, _kind, _DoerId;
    time_t _Releasetime, _DoTime;
    bool _TaskState;//已发布为0，完成为1
    string _Title;
    fin >> _ProjectId >> _Id >> _ReleaserId >> _kind;
    fin >> _Title;
    fin >> _Releasetime >> _DoTime >> _TaskState >> _DoerId;
    switch (_kind)
    {
    case 1:
        CommonTask(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId);
        break;
    case 2:
        SecretTask(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId);
        break;
    case 3:
        InstructTask(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId);
        break;
    default:
        break;
    }
}

void LoadUser()
{
    try{
    ifstream fin("User.txt");//load the data in the file to memory
    if (fin.is_open())
    {
        while (fin.peek() != EOF)
        {
            ProjMember a;
            fin >> a;
        }
    }
    else
    {
        int i;
        throw i;
    }
    fin.close();
    }
    catch(int i)
    {
         QMessageBox::critical( 0, "Error！",
                 QString("The file storing the data of users is lost. Please check it."));
    }
    catch(...)
    {
        QMessageBox::critical( 0, "Error！",
                QString("There is an error during loading the data of users."));
    }
}

void LoadProj()
{
    try{
    //ifstream fin("C:\\P.txt");
    ifstream fin("P.txt");
    if (fin.is_open())
        while (fin.peek() != EOF)
        {
            Project a;
            fin >> a;
        }
    else
    {
        int i;
        throw i;
    }
    fin.close();
    }
    catch(int i)
    {
         QMessageBox::critical( 0, "Error！",
                 QString("The file storing the data of projects is lost. Please check it."));
    }
    catch(...)
    {
        QMessageBox::critical( 0, "Error！",
                QString("There is an error during loading the data of projects."));
    }
}

void  LoadTask()
{
    try{
    ifstream fin("Task.txt");
    if (fin.is_open())
        while (fin.peek() != EOF)
        {
            CommonTask a;
            fin >> a;
        }
    else
    {
        int i;
        throw i;
    }
    fin.close();
    }
    catch(int i)
    {
         QMessageBox::critical( 0, "Error！",
                 QString("The file storing the data of tasks is lost. Please check it."));
    }
    catch(...)
    {
        QMessageBox::critical( 0, "Error！",
                QString("There is an error during loading the data of tasks."));
    }
}

void LoadAvaId()
{
        try{
    ifstream fin("Available.txt");
    if (fin.is_open())
        fin >> AvaProjId >> AvaTaskId >> AvaMemId >> AvaChiId;
    fin.close();
    }
    catch(...)
    {
        QMessageBox::critical( 0, "Error！",
                QString("There is an error during loading the data of tasks. Please check it."));
    }
}

void store()
{
    try{
    ofstream fout("User.txt");
    if (fout.is_open())
    {
        auto map_it = UserList.begin();
        if (map_it != UserList.end())
        {
            fout << *(map_it->second);
            map_it++;
        }
        for (; map_it != UserList.end(); ++map_it)
        {
            fout << endl;
            fout << *(map_it->second);
        }
    }
    fout.close();
    fout.clear();
    fout.open("P.txt");
    if (fout.is_open())
    {
        auto map_it = ProjectList.begin();
        if (map_it != ProjectList.end())
        {
            fout << map_it->second;
            map_it++;
        }
        for (; map_it != ProjectList.end(); ++map_it)
        {
            fout << endl;
            fout << map_it->second;
        }
    }
    fout.close();
    fout.clear();
    fout.open("Task.txt");
    if (fout.is_open())
    {
        auto map_it = SumTaskList.begin();
        if (map_it != SumTaskList.end())
        {
            fout << *(map_it->second);
            map_it++;
        }
        for (; map_it != SumTaskList.end(); ++map_it)
        {
            fout << endl;
            fout << *(map_it->second);
        }
    }
    fout.close();
    fout.clear();
    fout.open("Available.txt");
    if (fout.is_open())
        fout << AvaProjId << " " << AvaTaskId << " " << AvaMemId << " " << AvaChiId << endl;
    fout.close();
    }
    catch(...)
    {
        QMessageBox::critical( 0, "Error！",
                QString("There is an error during storing the data of tasks."));
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    LoadUser();
    LoadProj();
    LoadTask();
    LoadAvaId();
    w.show();
    return a.exec();
}

