#ifndef CLASS
#define CLASS

#include "function.h"
#include "Id.h"

class User
{
protected:
    int Id;
    string Name;
    string Password;
public:
    User() {}
    void changePassword()
    {
        cin.sync();//输入前清空输入缓存
        cout << "请输入原密码：" << endl;
        if (GetString() == Password)
        {
            cin.sync();
            cout << "请输入新密码：" << endl;
            getline(cin, Password);
            cout << "修改密码成功！" << endl;
        }
        else
            cout << "密码错误！已退出修改" << endl;
    }
    string GetString()
    {
        string ProjectName;
        cin >> ProjectName;
        return ProjectName;
    }
    void ChangeInformation()
    {
        cin.sync();
        cout << "请更正姓名：" << endl;
        getline(cin, Name);
        cout << "修改成功，新姓名为" << Name << endl;
    }
    virtual void ChangeKPI(int) = 0;
    virtual bool Login(string) = 0;
    virtual void OutFile(ofstream& ) = 0;
    friend ofstream &operator<<(ofstream &, User &);
    virtual QString menu(QString)=0;
};

class ProjMember : public User
{
protected:
    int TaskDoneNum;
    int KPI;
    bool online;
public:
    void OutFile(ofstream& fout)
    {
        fout << Id << " " << TaskDoneNum << " " << KPI << endl;
        fout << Name << " " << Password;
    }
    void ChangeKPI(int x)
    {
        KPI += x;
        TaskDoneNum++;
    }
    bool Login(string a)
    {
        if (a == Password)
            return true;
        else
            return false;
    }
    ProjMember(int , string , string , int , int );//该构造函数读取文件时调用
    ProjMember() {}
    ProjMember(int);//用参数来判断是要构造成员类还是被主管类的构造函数调用了
    QString GetProjList();
    QString GetTaskList();
    QString SubmitTask(QString);
    QString menu(QString datagram)
    {
        QTextStream ts( &datagram, QIODevice::ReadOnly );
        int option;
        ts >> option;
        QString content(datagram);
        switch (option)
        {
        case 1:
            return GetTaskList();
            break;
        case 2:
            return GetProjList();
            break;
        case 3:
            return "Id:" + QString::number(Id) + "\nName:" + QString::fromStdString(Name) + "\nTaskDoneNum:" + QString::number(TaskDoneNum) + "\nKPI:" + QString::number(KPI);
            break;
        case 4:
            content = content.right(content.length() - 2);
            return SubmitTask(content);
            break;
        case 5:
            content = content.right(content.length() - 2);
            return SearchTask(content);
            break;
        default:
            break;
        }
    }
    QString SearchTask(QString datagram)
    {
        QTextStream ts( &datagram, QIODevice::ReadOnly );
        int o;
        ts>>o;
        switch (o)
        {
        case 0:
            return FindTaskName(datagram.right(datagram.length()-2));
            break;
        case 1:
            return FindTaskTime(datagram.right(datagram.length()-2));
            break;
        default:
            break;
        }
    }
    QString FindTaskName(QString);
    QString FindTaskTime(QString);
    void Logout()
    {
        online = 0;
        //cout << "已成功退出登录。" << endl;
    }
    friend ifstream &operator>>(ifstream & , ProjMember & );
};

class ProjChief : public ProjMember
{
private:
    int ProjReleaseNum;
    int TaskReleaseNum;
public:
    ProjChief(int);
    ProjChief() {}
    ProjChief(int _Id, string _Name, string _Password, int _TaskDoneNum, int _KPI, int _ProjReleaseNum, int _TaskReleaseNum);
    void OutFile(ofstream& fout)
    {
        fout << Id << " " << TaskDoneNum << " " << KPI << endl;
        fout << Name << " " << Password << endl;
        fout << ProjReleaseNum << " " << TaskReleaseNum;
    }
    QString TaskRelease(QString);
    void RegNumber()
    {
        ProjMember(1);//1意为要构造成员类
    }
    void RegChief()
    {
        ProjChief(1);//这里的1没有实际意义，只是为避免调用无参数构造函数
    }
    QString menu(QString datagram)
    {
        QTextStream ts( &datagram, QIODevice::ReadOnly );
        int option;
        ts >> option;
        QString content(datagram);
        switch (option)
        {
        case 1:
            return GetTaskList();
            break;
        case 2:
            return GetProjList();
            break;
        case 3:
            return "3 Id:" + QString::number(Id) + "\nName:" + QString::fromStdString(Name) + "\nTaskDoneNum:" + QString::number(TaskDoneNum) + "\nKPI:" + QString::number(KPI);
            break;
        case 4:
            content = content.right(content.length() - 2);
            return SubmitTask(content);
            break;
        case 5:
            content = content.right(content.length() - 2);
            return SearchTask(content);
            break;
        case 6:
            content = content.right(content.length() - 2);
            ProjectRelease(content);
            return QString(QLatin1String("6 success"));
            break;
        case 7:
            content = content.right(content.length() - 2);
            return TaskRelease(content);
            break;
        default:
            break;
        }
    }
    void CommonTaskRelease(int, QString);
    void SecretTaskRelease(int, QString);
    void InstructTaskRelease(int, QString);
    void ProjectRelease(QString);
};

class Project
{
private:
    int Id;
    int ReleaserId;
    time_t Releasetime;
    Tset TaskList;
    string Title;
public:
    Project() {}
    Project(int _Id, int _ReleaserId, time_t _Releasetime, Tset _TaskList, string _Title);
    Project(int,QString);
    void InsertTask(int TaskId)
    {
        TaskList.insert(TaskId);
    }
    QString OutSelf(int );
    friend ofstream &operator<<(ofstream &, Project &);
    friend ifstream &operator>>(ifstream & , Project &);
};

class Task
{
private:
    bool TaskState;//0 means released, while 1 means completed.
    string Title;
    time_t Releasetime;
    time_t DoTime;
protected:
    int ReleaserId;
    int Id;
    int ProjectId;
    int kind;
    int DoerId;
public:
    Task() {}
    Task(int , int, QString);
    Task(int _ProjectId, int _Id, int _ReleaserId, int _kind, time_t _Releasetime, time_t _DoTime, bool _TaskState, string _Title, int _DoerId)
    {
        ProjectId = _ProjectId;
        Id = _Id;
        ReleaserId = _ReleaserId;
        Releasetime = _Releasetime;
        DoTime = _DoTime;
        TaskState = _TaskState;
        Title = _Title;
        kind = _kind;
        DoerId = _DoerId;
    }
    virtual time_t CalculateKPI() = 0;
    QString isVisible(int VisitorId)
    {
        QString a;
        if (kind == 1 || kind == 3 || (kind == 2 && (DoerId == VisitorId || ReleaserId == VisitorId)))
        {
            struct tm *p;
            p = localtime(&Releasetime);
            char s[100];
            strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", p);
            a += "TaskTitle: " + QString::fromStdString(Title) + "\tId: " + QString::number(Id) + "\tProjectId: " + QString::number(ProjectId) + "\tReleaserId: " + QString::number(ReleaserId) + "\nReleasetime: " + s + "\tkind: ";
            switch (kind)
            {
            case 1:
                a += "Common";
                break;
            case 2:
                a += "Private";
                break;
            case 3:
                a += "Instruct";
                break;
            }
            a += "\nstate: ";
            if (TaskState == true)
                a += "done\tdoer id： " + QString::number(DoerId) + "\n";
            else
            {
                a += "not done\n";
                if (kind == 3 || kind == 2)
                    a += "doerid: " + QString::number(DoerId) + "\n";
            }
        }
        return a;
    }
    time_t CostTime()
    {
        return DoTime - Releasetime;
    }
    QString isExecutable(int ExecutorId)
    {
        if ((kind == 1 || DoerId == ExecutorId) && TaskState == false)//check whether the user has the privilege
        {
            TaskState = true; //0 means released, while 1 means completed.
            DoerId = ExecutorId;
            DoTime = time(NULL);
            CalculateKPI();
            return QString(QLatin1String("4 submitted successfully"));
        }
        else
            return QString(QLatin1String("4 Submission fails.You don't have permission to execute this task, or this task has been executed."));
    }
    bool is_Title(string s)
    {
        if (Title == s)
            return true;
    }
    bool is_Time(time_t t1, time_t t2)
    {
        if (Releasetime >= t1 && Releasetime <= t2 )//check whether the time is in the period with the timestamp
            return true;
        else
            return false;
    }
    void Initialize_DoerId()
    {
        bool isOK;
        QString text = QInputDialog::getText(NULL, "Input Doer ID",
                                             "Please input doer ID",
                                             QLineEdit::Normal,
                                             "0",
                                             &isOK);
        if (isOK)
            DoerId = text.toInt();
    }
    friend ofstream &operator<<(ofstream &, Task &);
};

class CommonTask : public Task
{
public:
    CommonTask() {}
    CommonTask(int , int , int , int , time_t , time_t , bool , string , int );
    time_t CalculateKPI();
    CommonTask(int, QString);
    friend ifstream &operator<<(ifstream &, Task &);
};

class SecretTask : public Task
{
public:
    SecretTask() {}
    SecretTask(int , int , int , int , time_t , time_t , bool , string , int );
    time_t CalculateKPI();
    SecretTask(int, QString);
};

class InstructTask : public Task
{
public:
    InstructTask() {}
    time_t CalculateKPI();
    InstructTask(int , int , int , int , time_t , time_t , bool , string , int );
    InstructTask(int, QString );
};

#endif // CLASS

