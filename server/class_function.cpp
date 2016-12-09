#include "List.h"

ProjMember::ProjMember(int _Id, string _Name, string _Password, int _KPI, int _TaskDoneNum)
{
    Id = _Id;
    Name = _Name;
    Password = _Password;
    KPI = _KPI;
    online = 0;
    TaskDoneNum = _TaskDoneNum;
    MemList[Id] = (*this);
    UserList[Id] = &(MemList[Id]);
}

ProjMember::ProjMember(int a)
{
    Password = "111111";
    online = 0;
    if (a == 1)
    {
        Id = AvaMemId;
        AvaMemId++;
    }
    else
    {
        Id = AvaChiId;
        AvaChiId++;
    }
    KPI = 0;
    TaskDoneNum = 0;
    bool isOK;
    QString text = QInputDialog::getText(NULL, "Input Project ID",
                                         "Please input the new member's name:",
                                         QLineEdit::Normal,
                                         "0",
                                         &isOK);
    Name = text.toStdString();
    cout << "创建成功！该项目成员的Id为" << Id << endl;
    cout << "初始密码为111111，请通知该成员尽快修改密码" << endl;
    MemList.insert(pair<int, ProjMember>(Id, (*this)));
    UserList[Id] = &(MemList[Id]);
}

QString ProjMember::GetProjList()
{
    QString a;
    if (ProjectList.empty())
        a = "2 empty!";
    else
    {
        a = "2 Present Task:\n";
        for (auto map_it = ProjectList.begin(); map_it !=  ProjectList.end(); ++map_it)
            a += (map_it-> second).OutSelf(Id);
    }
    return a;
}

QString ProjMember:: GetTaskList()
{
    QString a;
    if (SumTaskList.empty())
    {
        a = "1 empty!";
            return a;
    }
    else
    {
        QString a = "1 Present Task:\n";
        for (auto map_it = SumTaskList.begin(); map_it != SumTaskList.end(); ++map_it)
            a += ((map_it->second)->isVisible(Id)); 
            return a;
    }

}

QString ProjMember::SubmitTask(QString datagram)
{
    QTextStream ts( &datagram, QIODevice::ReadOnly );
    int s;
    ts >> s;
    auto it = ProjectList.find(s);
    if (it == ProjectList.end())
        return QString(QLatin1String("4 This project does not exist!"));
    else
    {
        int s;
        ts >> s;
        auto TaskIt = SumTaskList.find(s);
        if (TaskIt == SumTaskList.end())
            return QString(QLatin1String("4 This task does not exist!"));
        else
        {
            return (TaskIt->second)->isExecutable(Id);
        }
    }
}
QString ProjMember::FindTaskName(QString datagram)
{
    string s = datagram.toStdString();
    bool alfind = false;
    auto map_it = SumTaskList.begin();
    QString a;
    for (; map_it != SumTaskList.end(); ++map_it)
        if ((map_it->second)->is_Title(s) == true)
        {
            alfind = true;
            a += "5 Search results:\n" + (map_it->second)->isVisible(Id);
            break;
        }
    map_it++;
    for (; map_it != SumTaskList.end(); ++map_it)
        if ((map_it->second)->is_Title(s) == true)
            a += (map_it->second)->isVisible(Id);
    if (alfind == false)
        a += "5 There is no task with this title." ;
    return a;
}

QString ProjMember::FindTaskTime(QString datagram)
//该函数能够显示所有符合规则的结果
{
    QTextStream ts( &datagram, QIODevice::ReadOnly );
    long t1, t2;
    ts >> t1 >> t2;
    bool alfind = false;
    auto map_it = SumTaskList.begin();
    QString a;
    for (; map_it != SumTaskList.end(); ++map_it)
        if (((map_it->second)->is_Time(t1, t2)) == true)
        {
            alfind = true;
            a += "5 Search results:\n" ;
            a += (map_it->second)->isVisible(Id);
            break;
        }
    map_it++;
    for (; map_it != SumTaskList.end(); ++map_it)
        if (((map_it->second)->is_Time(t1, t2)) == true)
            a += (map_it->second)->isVisible(Id);
    if (alfind == false)
        a += "5 There is no task released during this period.";
    return a;
}

ProjChief::ProjChief(int _Id, string _Name, string _Password, int _TaskDoneNum, int _KPI, int _ProjReleaseNum, int _TaskReleaseNum) : ProjMember(_Id, _Name, _Password, _TaskDoneNum, _KPI)
{
    ProjReleaseNum = _ProjReleaseNum;
    TaskReleaseNum = _TaskReleaseNum;
    ProjChiefList[Id] = (*this);
    UserList[Id] = &(ProjChiefList[Id]);
}

ProjChief::ProjChief(int a): ProjMember(2)
{
    ProjReleaseNum = 0;
    TaskReleaseNum = 0;
    ProjChiefList[Id] = (*this);
    UserList[Id] = &(ProjChiefList[Id]);
}

QString ProjChief::TaskRelease(QString datagram)
{
    QTextStream ts( &datagram, QIODevice::ReadOnly );
    int kind;
    ts >> kind;
    int s;
    ts >> s;
    auto it = ProjectList.find(s);
    if (it == ProjectList.end())
        return QString(QLatin1String("7 The project doesn't exist."));
    else
    {
        switch (kind)
        {
        case 0:
            CommonTaskRelease(Id, datagram.right(datagram.length() - 2));
            TaskReleaseNum++;
            break;
        case 1:
            SecretTaskRelease(Id, datagram.right(datagram.length() - 2));
            TaskReleaseNum++;
            break;
        case 2:
            InstructTaskRelease(Id, datagram.right(datagram.length() - 2));
            TaskReleaseNum++;
            break;
        default:
            break;
        }
        return QString(QLatin1String("7 Release successfully."));
    }
}

void ProjChief::CommonTaskRelease(int Id, QString datagram)
{
    CommonTask a(Id, datagram);
}

void ProjChief::SecretTaskRelease(int Id, QString datagram)
{
    SecretTask a(Id, datagram);
}

void ProjChief::InstructTaskRelease(int Id, QString datagram)
{
    InstructTask a(Id, datagram);
}

void ProjChief::ProjectRelease(QString text)
{
    Project a(Id, text);
    ProjReleaseNum++;
}

Project::Project(int _Id, int _ReleaserId, time_t _Releasetime, Tset _TaskList, string _Title)
{
    Id = _Id;
    ReleaserId = _ReleaserId;
    Releasetime = _Releasetime;
    Title = _Title;
    ProjectList[Id] = (*this);
}

Project::Project(int CheifId, QString text)
{
    Title = text.toStdString();
    ReleaserId = CheifId;
    Releasetime = time(NULL);
    Id = AvaProjId;
    AvaProjId++;
    ProjectList[Id] = (*this);
}

QString Project::OutSelf(int MId)
{
    struct tm *p;
    p = localtime(&Releasetime);
    char s[100];
    strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
    QString a;
    a += "ProjectTitle: " + QString::fromStdString(Title) + "\tId: " + QString::number(Id) + "\tReleaserId: " + QString::number(ReleaserId) + "\tReleasetime: " + s + "\n";
    if (TaskList.empty() == true)
        a += "No task!\n";
    else
    {
        a += "There are " + QString::number(TaskList.size()) + " task(s).The ones isvisable to you are as followed:\n";
        for (auto it = TaskList.begin(); it != TaskList.end(); ++it)
            a += SumTaskList[(*it)]-> isVisible(MId);
    }
    a += "\n";
    return a;
}

Task::Task(int _ReleaserId, int _kind, QString datagram)
{

    int s;
    QTextStream ts( &datagram, QIODevice::ReadOnly );
    ts >> s;
    auto it = ProjectList.find(s);
    ProjectId = s;
    QString _Title;
    ts >> _Title;
    Title = _Title.toStdString();
    Releasetime = time(NULL);
    TaskState = false;
    Id = AvaTaskId;
    AvaTaskId++;
    ReleaserId = _ReleaserId;
    kind = _kind;
    if (kind == 2 || kind == 3)
        Initialize_DoerId();
}

CommonTask::CommonTask(int _ProjectId, int _Id, int _ReleaserId, int _kind, time_t _Releasetime, time_t _DoTime, bool _TaskState, string _Title, int _DoerId):
    Task(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId)
{
    kind = _kind;
    ProjectList[ProjectId].InsertTask(Id);
    CommonTaskList[Id] = *(this);
    SumTaskList[Id] = &CommonTaskList[Id];
}

time_t CommonTask:: CalculateKPI()
{
    UserList[DoerId]->ChangeKPI( perKPI / (3 * CostTime()));
    UserList[ReleaserId]->ChangeKPI(perKPI / ( 30 * CostTime()));
}

CommonTask::CommonTask(int _ReleaserId, QString datagram): Task(_ReleaserId, 1, datagram)
{
    ProjectList[ProjectId].InsertTask(Id);
    CommonTaskList[Id] = *(this);
    SumTaskList[Id] = &CommonTaskList[Id];
}

SecretTask::SecretTask(int _ProjectId, int _Id, int _ReleaserId, int _kind, time_t _Releasetime, time_t _DoTime, bool _TaskState, string _Title, int _DoerId):
    Task(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId)
{
    ProjectList[ProjectId].InsertTask(Id);
    SecretTaskList[Id] = *this;
    SumTaskList[Id] = &SecretTaskList[Id];
}

time_t SecretTask::CalculateKPI()
{
    UserList[DoerId]->ChangeKPI(perKPI / (2 * CostTime()));
    UserList[ReleaserId]->ChangeKPI(perKPI / ( 20 * CostTime()));
}

SecretTask::SecretTask(int _ReleaserId, QString datagram): Task(_ReleaserId, 2, datagram)
{
    ProjectList[ProjectId].InsertTask(Id);
    SecretTaskList[Id] = *(this);
    SumTaskList[Id] = &SecretTaskList[Id];
}

time_t InstructTask::CalculateKPI()
{
    UserList[DoerId]->ChangeKPI(perKPI / CostTime());
    UserList[ReleaserId]->ChangeKPI(perKPI / ( 10 * CostTime()));
}

InstructTask::InstructTask(int _ProjectId, int _Id, int _ReleaserId, int _kind, time_t _Releasetime, time_t _DoTime, bool _TaskState, string _Title, int _DoerId):
    Task(_ProjectId, _Id, _ReleaserId, _kind, _Releasetime, _DoTime, _TaskState, _Title, _DoerId)
{
    ProjectList[ProjectId].InsertTask(Id);
    InstructTaskList[Id] = *(this);
    SumTaskList[Id] = &(InstructTaskList[Id]);
}

InstructTask::InstructTask(int _ReleaserId, QString datagram): Task(_ReleaserId, 3, datagram)
{
    ProjectList[ProjectId].InsertTask(Id);
    InstructTaskList[Id] = *(this);
    SumTaskList[Id] = &InstructTaskList[Id];
}
