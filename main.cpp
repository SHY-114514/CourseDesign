#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
#define DTTPACT 1
#define DTTPUSR 2
#define ACCOUNTDATA "data/account.txt"
#define USERDATA "data/user.txt"
#define LOGV(msg) printf("\033[0;32;40m【INFO    】 %s\033[0m\n", msg);
#define LOGW(msg) printf("\033[0;33;40m【WARNING 】 %s\033[0m\n", msg);
#define LOGE(msg)                                                  \
    {                                                              \
        printf("\033[0;31;40m【ERROR   】 %s,退出\033[0m\n", msg); \
        exit(-1);                                                  \
    }
#define PRINT(name, flag)                              \
    {                                                  \
        printf("\033[0;34;40m【%s】># \033[0m", name); \
        scanf("%d", &flag);                            \
    }

int LOGINCNT = 1;

typedef struct
{
    long id;              // 主键
    char name[21];        // 疫苗名称
    char date[21];        // 疫苗日期
    unsigned short batch; // 批次
    long inventory;       // 库存
} Vaccine;

typedef struct
{
    long uid;
    long vaccineId; // 疫苗ID
    char date;      // 接种日期
} InoculationInfo;

typedef struct
{
    long uid;
    char name[20];     // 姓名
    short age;         // 年龄
    char gender;       // 性别,1:man 2:women
    char phone[12];    // 手机号
    char disease[201]; // 基础疾病
    char allergy[201]; // 过敏信息
} User;                // 用户个人信息节点

typedef struct
{
    long id;         // 主键
    char mail[51];   // 账号
    char passwd[21]; // 密码
    char role;       // 0：普通用户（默认）   1：管理员
} Account;           // 账号

typedef union
{
    User user;
    Account account;
} Data;

typedef struct _LinkedList
{
    int type;
    Data data;
    struct _LinkedList *next;
} Node, *List; // 链表结构

int ListInit(List &l, int type)
{
    l = new Node();
    if (!l)
        return FALSE;
    l->next = NULL;
    l->type = type;
    return TRUE;
}
int ListInsert(List &l, Data &data)
{
    Node *np = new Node();
    if (!np)
        return FALSE;
    if (l->type == DTTPACT)
    {
        np->data.account.id = data.account.id;
        strcpy(np->data.account.mail, data.account.mail);
        strcpy(np->data.account.passwd, data.account.passwd);
        np->data.account.role = data.account.role;
    }
    else if (l->type == DTTPUSR)
    {
        np->data.user.uid = data.user.uid;
        strcpy(np->data.user.name, data.user.name);
        np->data.user.age = data.user.age;
        np->data.user.gender = data.user.gender;
        strcpy(np->data.user.phone, data.user.phone);
        strcpy(np->data.user.disease, data.user.disease);
        strcpy(np->data.user.allergy, data.user.allergy);
    }
    np->next = l->next;
    l->next = np;
    return TRUE;
}
int ListLocate(List &l, Data *dp)
{
    List p = l->next;
    while (p)
    {
        if (l->type == DTTPACT){
            if (p->data.account.id == dp->account.id || strcmp(p->data.account.mail, dp->account.mail) == 0)
            {
                dp->account.id = p->data.account.id;
                strcpy(dp->account.mail, p->data.account.mail);
                strcpy(dp->account.passwd, p->data.account.passwd);
                dp->account.role = p->data.account.role;
                return TRUE;
            }
        }else if (l->type == DTTPUSR){
            if (p->data.user.uid == dp->user.uid)
            {
                dp->user.uid = p->data.user.uid;
                strcpy(dp->user.name, p->data.user.name);
                dp->user.age = p->data.user.age;
                dp->user.gender = p->data.user.gender;
                strcpy(dp->user.phone, p->data.user.phone);
                strcpy(dp->user.disease, p->data.user.disease);
                strcpy(dp->user.allergy, p->data.user.allergy);
            }
        }
        p = p->next;
    }
    return FALSE;
}
void ListPrint(Data data, int type)
{
    if (type == DTTPACT)
    {
        printf("ID:%ld\t权限:%d\n邮箱:%s\n密码:%s\n", data.account.id, data.account.role, data.account.mail, data.account.passwd);
        printf("**************************\n");
    }
    else if (type == DTTPUSR)
    {
        printf("UID:%d\t姓名:%s\t年龄:%d\t性别:%c\n手机号:%s\n历史疾病:%s\n过敏信息:%s\n", data.user.uid, data.user.name, data.user.age, data.user.gender, data.user.phone, data.user.disease, data.user.allergy);
        printf("**************************\n");
    }
}
void ListPrints(List &l)
{
    List p = l->next;
    while (p)
    {
        if (l->type == DTTPACT)
        {
            printf("ID:%ld\t权限:%c\n邮箱:%s\n密码:%s\n", p->data.account.id, p->data.account.role, p->data.account.mail, p->data.account.passwd);
            printf("**************************\n");
        }
        else if (l->type == DTTPUSR)
        {
            printf("UID:%d\t姓名:%s\t年龄:%d\t性别:%c\n手机号:%s\n历史疾病:%s\n过敏信息:%s\n", p->data.user.uid, p->data.user.name, p->data.user.age, p->data.user.gender, p->data.user.phone, p->data.user.disease, p->data.user.allergy);
            printf("**************************\n");
        }
        p = p->next;
    }
}
int ListDelete(List &l, Data data)
{
    List p = l->next, q;
    while (p->next)
    {
        if (l->type == DTTPACT)
            if (p->next->data.account.id == data.account.id || strcmp(p->next->data.account.mail, data.account.mail))
                break;
        p = p->next;
    }
    if (!p)
        return FALSE;
    q = p->next;
    p->next = q->next;
    delete q;
    return TRUE;
}
void ListClear(List &l)
{
    List p = l->next, q;
    while (p)
    {
        q = p;
        p = p->next;
        delete q;
    }
}
int auth(List &accounts,List &users){
    if (LOGINCNT > 3){
        LOGW("验证失败次数过多，禁止登录，若忘记密码，请联系管理员修改密码")
        return FALSE;
    }
    Data data;
    char msg[31];
    int option = 2;
    data.account.id = 0;
    printf("\t\t身份验证\n\t【1】注册\t【2】登录\n");
    PRINT("请输入操作",option);
    printf("请输入邮箱:\n");
    scanf("%s", data.account.mail);
    ListLocate(accounts,&data);
    if(option == 1){
        // 匹配失败
        if(data.account.id != 0){
            LOGW("账号已存在,请直接登录")
            return auth(accounts,users);
        }
        printf("请输入密码:\n");
        scanf("%s",data.account.passwd);
        data.account.id = accounts->next->data.account.id +1;
        data.account.role = '0';
        if(ListInsert(accounts,data)){
            printf("注册成功,请登录\n");
        }else{
            LOGW("注册失败,内存分配失败")
            return FALSE;
        }
        return auth(accounts,users);
    }else{
        if (data.account.id == 0)
        {
            sprintf(msg, "账号不存在(%d/3)", LOGINCNT);
            LOGW(msg)
            LOGINCNT += 1;
            return auth(accounts,users);
        }
        printf("请输入密码:");
        scanf("%s", msg);
        if (strcmp(data.account.passwd, msg) != 0)
        {
            sprintf(msg, "密码错误(%d/3)", LOGINCNT);
            LOGW(msg)
            LOGINCNT += 1;
            return auth(accounts,users);
        }
        LOGV("登录成功")
        return data.account.id;
    }
}


int main(void)
{
    int option = FALSE, quit = FALSE;
    LOGV("初始化")
    List accounts, users;
    if (!ListInit(accounts, DTTPACT))
        LOGE("账号列表初始化失败")
    if (!ListInit(users, DTTPUSR))
        LOGE("用户列表初始化失败")
    FILE *fp = fopen(ACCOUNTDATA, "rb");
    if (!fp)
        LOGE("账号文件打开失败");
    while (!feof(fp))
    {
        Data data;
        fscanf(fp, "%d %s %s %c\n", &(data.account.id), data.account.mail, data.account.passwd, &(data.account.role));
        ListInsert(accounts, data);
    }
    fclose(fp);
    fp = fopen(USERDATA, "rb");
    if (!fp)
        LOGE("用户文件打开失败")
    while (!feof(fp))
    {
        Data data;
        fscanf(fp, "%d %s %d %c %s %s %s\n", &(data.user.uid), data.user.name, &(data.user.age), &(data.user.gender), data.user.phone, data.user.disease, data.user.allergy);
        ListInsert(users, data);
    }
    fclose(fp);


    Data _dt;
    _dt.user.uid = auth(accounts,users);
    if(_dt.user.uid == FALSE){ quit = TRUE;}
    ListLocate(users, &_dt);
    
    

    while (!quit)
    {
        printf("【0】退出系统\t【1】查看账户列表\n");
        PRINT(_dt.user.name, option);
        switch (option)
        {
        case 0:
            quit = TRUE;
            break;
        case 1:
            ListPrints(accounts);
            break;
        case 2:
            ListPrints(users);
            break;
        }
    }

    LOGV("退出");
    ListClear(accounts);
    ListClear(users);

    return 0;
}
