# 课程设计


### 项目结构

/main.cpp 项目主要代码

/data/ 存放数据文件

/data/account.txt

/data/user.txt


### 开发日志

##### 构建抽象数据结构

(数据体+操作方法)

采用控制反转思想，将比较函数交给调用方去完成，程序中只需要传入比较函数的指针

##### 开发用户界面

##### 开发日志

###### 1、使用 `DistroyList()` 函数释放 `users` 单链表时，程序出现 `Unknown signal` 错误

错误原因：非法访问已经释放的指针

解决方法：多定义一个指针，在释放之前先迭代

###### 2、 `printUserInfo()` 函数不能正确打印节点

写不下去了，重构了


###### 3、 修改登录BUG

###### 4、修改用户名显示错误问题

if() 后面又一个if()，没加{}导致的错误


##### 5、添加注册/登录功能

用户权限默认为普通用户，仅管理员才能修改权限

##### 6、开发部分系统功能并修复一些BUG

完善用户部分系统开发
