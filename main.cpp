#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0
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

typedef struct _LinkedList
{
    void *data;
    struct _LinkedList *next;
} Node, *List; // 链表结构

// 初始化链表
int InitList(List &l)
{
    l = new Node();
    if (!l)
        return FALSE;
    l->data = NULL;
    l->next = NULL;
    return TRUE;
}

// 头插法插入元素
int InsertElem(List &l, void *data)
{
    Node *tempNode = new Node();
    if (!tempNode)
        return FALSE;
    tempNode->data = data;
    tempNode->next = l->next;
    l->next = tempNode;
    return TRUE;
}

// 判断Account是否相等
int AccountCmp(void *datap1, void *datap2)
{
    Account *a = (Account *)datap1;
    Account *b = (Account *)datap2;
    if (strcmp(a->mail, b->mail) == 0 || a->id == b->id) // 每个邮箱只能注册一个账号
        return TRUE;
    return FALSE;
}

// Account赋值函数
void AccountAsm(Node *n, void *p)
{
    Account *ap = (Account *)p;
    Account *np = (Account *)(n->data);
    ap->id = np->id;
    strcpy(ap->mail, np->mail);
    strcpy(ap->passwd, np->passwd);
    ap->role = np->role;
}

int UserCmp(void *data1, void *data2)
{
    User *a = (User *)data1;
    User *b = (User *)data2;
    if (a->uid == b->uid)
        return TRUE;
    return FALSE;
}

void UserAsm(Node *np, void *p)
{
    User *a = (User *)p;
    User *b = (User *)(np->data);
    a->uid = b->uid;
    a->age = b->age;
    a->gender = b->gender;
    strcpy(a->name,b->name);
    strcpy(a->phone,b->phone);
    strcpy(a->disease,b->disease);
    strcpy(a->allergy,b->allergy);
}

int Delete(List &l, void *data, int (*equal)(void *, void *))
{
    Node *p, *npt = l;
    long aid, bid;
    while (npt->next)
    {
        if (equal(data, npt->next->data))
            break;
        npt = npt->next;
    }
    if (!npt)
        return FALSE;
    p = npt->next;
    npt->next = p->next;
    free(p);
    return TRUE;
}

int IndexElem(List &l, void *data, int (*equal)(void *, void *), void (*value)(Node *, void *))
{
    Node *p = l->next;
    while (p)
    {
        if (equal(p->data, data))
            break;
        p = p->next;
    }
    if (p)
    {
        value(p, data);
        return TRUE;
    }
    return FALSE;
}

void DistroyList(List &l,void (*clear)(void *))
{
    Node *p = l->next,*q;
    while (p)
    {
        q = p;
        p = p->next;
        clear(q->data);
        free(q);
    }
    l->next = NULL;
}

void printAccountInfo(void *data)
{
    Account *a = (Account *)data;
    printf("ID:%d\t邮箱:%s\t密码:%s\t权限:%c\n", a->id, a->mail, a->passwd, a->role);
}

void printUserInfo(List &l, void *data)
{
    Account *a;
    User *u = (User *)data;
    a->id = u->uid;
    printf("UID:%d\t姓名:%s\t年龄:%d\t性别:%c\n手机号:%s\n历史疾病:%s\n过敏信息:%s\n", u->uid, u->name, u->age, u->gender, u->phone, u->disease, u->allergy);
    IndexElem(l, a, AccountCmp, AccountAsm);
    printf("邮箱:%s\n权限:%c\n", a->mail, a->role);
    // TODO: 打印接种信息
}

Account *login(List &accounts)
{
    Account *ap = new Account();
    static int LOGINCOUNT = 1;
    char mail[21], passwd[21];
    if (LOGINCOUNT > 3)
        return NULL;
    printf("请输入账号:\n");
    scanf("%s", mail);
    ap->id = 0; // 置空ID防止匹配错误
    strcpy(ap->mail, mail);
    printf("请输入密码:\n");
    scanf("%s", passwd);
    IndexElem(accounts, ap, AccountCmp, AccountAsm);
    if (strcmp(ap->passwd, passwd) == 0)
        return ap;
    printf("账号或密码错误(%d/3)\n", LOGINCOUNT);
    LOGINCOUNT += 1;
    login(accounts);
    return NULL;
}
void DistroyActNode(void* data){
    Account* node = (Account*)data;
    delete node;
}
void DistroyUserNode(void* data){
    User* node = (User*)data;
    delete node;
}


int main(void)
{

    int flag = -1;
    // 初始化
    LOGV("初始化...")
    List accounts,users,vaccines;
    if (!InitList(accounts))
        LOGE("账户列表初始化失败")
    if (!InitList(users))
        LOGE("用户列表初始化失败")

    // 读取数据
    LOGV("加载数据...")
    FILE *fp = fopen(ACCOUNTDATA, "rb");
    if (!fp)
        LOGE("账号文件打开失败");
    while (!feof(fp))
    {
        Account *a = new Account();
        fscanf(fp, "%d %s %s %c\n", &(a->id), a->mail, a->passwd, &(a->role));
        InsertElem(accounts, (void*)a);
    }
    fclose(fp);
    fp = fopen(USERDATA, "rb");
    if (!fp)
        LOGE("用户文件打开失败");
    while (!feof(fp))
    {
        User *u = new User();
        fscanf(fp, "%d %s %d %c %s %s %s\n", &(u->uid), u->name, &(u->age), &(u->gender), u->phone, u->disease, u->allergy);
        InsertElem(users, (void*)u);
    }
    fclose(fp);


    // 登录
    // Account *ap;
    // if ((ap = login(accounts)))
    // {
    //     User *up = new User();
    //     up->uid = ap->id;
    //     if (IndexElem(users, up, UserCmp, UserAsm))
    //         printf("你好,%s\n", up->name);
    //     else
    //         flag = FALSE;
    //     // 查看个人信息
    //     // 注册账号
    //     // 修改密码
    //     // 添加信息（账号+用户信息）
    //     // 查看疫苗信息
    //     // 预约疫苗
    //     // 添加删除疫苗信息
    //     // 退出登录
    //     while (flag)
    //     {
    //         printf("【0】退出\t【1】查看个人信息\n");
    //         PRINT(up->name, flag)
    //         switch (flag)
    //         {
    //         case 1:
    //             printUserInfo(accounts, up);
    //             break;
    //         default:
    //             break;
    //         }
    //     }
    // }
    
    for(List a = accounts->next;a != NULL;a = a->next){
            printAccountInfo(a->data);
            // printf("%p\n",a);
    }
    printf("\n");


    LOGV("退出...")
    DistroyList(accounts,DistroyActNode);
    DistroyList(users,DistroyUserNode);

    return 0;
}

