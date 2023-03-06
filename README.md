# CourseDesign


### 项目结构

/main.cpp 项目主要代码

/server.cpp 若是时间足够，将项目分为S/C（Server端和Client端）

/data/ 存放数据文件

/data/account.txt

/data/user.txt


### 开发日志

##### 构建抽象数据结构

(数据体+操作方法)

采用控制反转思想，将比较函数交给调用方去完成，程序中只需要传入比较函数的指针

##### 开发用户界面

##### 修改BUG

1、使用 `DistroyList()` 函数释放 `users` 单链表时，程序出现 <span style="color:red;">Unknown signal</span> 错误（未解决）

