## 数据结构

### 1. `TimeSlot`
**作用**：表示一个时间段，用于课程、办公时间或个人安排。  

- **成员变量**  
  - `std::chrono::system_clock::time_point startTime` → 开始时间  
  - `std::chrono::system_clock::time_point endTime` → 结束时间  
  - `bool isCourse` → 是否属于课程/办公时间（true 表示课程/office hour，false 表示个人安排）  

- **方法**  
  - `bool isOverlappingWith(const TimeSlot& other) const`  
    - **参数**: `other` → 另一个时间段
	- **返回值**: `true` 表示时间段有重叠，`false` 表示没有
	- **作用**: 检查两个时间段是否冲突  
- `long long durationMinutes() const`  
    返回时间段的持续时长（分钟数）。  
- `bool getIsCourse() const`
- `std::chrono::system_clock::time_point getStartTime() const;`
- `std::chrono::system_clock::time_point getEndTime() const;`
- 还有两个函数用于set start和end time

---

### 2. `ScheduleEvent`
**作用**：在时间段基础上增加事件信息。  

```
enum Weekday {
    MONDAY = 1,    // 周一
    TUESDAY = 2,   // 周二  
    WEDNESDAY = 3, // 周三
    THURSDAY = 4,  // 周四
    FRIDAY = 5,    // 周五
    SATURDAY = 6,  // 周六
    SUNDAY = 7     // 周日
};
```

- **成员变量**  
  - `int id` → 事件唯一标识  
  - `std::string eventName` → 事件名称  
  - `std::string location` → 地点  
  - `std::string description` → 备注  
  - `int weekday;` 标注改日程位于一周的星期几
  - `TimeSlot timeSlot;`
- 函数：
	- 每个成员变量都应该有对应的get和set方法
---

### 3. `Schedule`
**作用**：存储和管理多个日程事件。  

- **成员变量**  
  - `std::vector<ScheduleEvent> events` → 存储事件的容器  

- **方法**  
  - `void addEvent(const ScheduleEvent& event)`  
    添加新事件。  
  - `bool removeEvent(int eventId)`  
    根据事件编号删除事件。  
  - `std::vector<ScheduleEvent> getEventsForDate(const std::chrono::system_clock::time_point& date) const`  
    获取某一天的事件列表。  
  - `const std::vector<ScheduleEvent>& getEventsForWeek(int weekoffset) const`  
    获取一周的所有事件。 参数对应的是偏移量，因为每次只展示一周的日程。
  - `std::vector<ScheduleEvent> Schedule::getEventsInRange(const std::chrono::system_clock::time_point& start,const std::chrono::system_clock::time_point& end) const`
	   这个函数的作用是封装给上面的这个函数调用
  - `Schedule& operator+(const Schedule& another)`
	  

---

### 4. `Professor`
**作用**：表示老师的信息及其办公时间表。  

- **成员变量**  
  - `std::string name` → 老师姓名  
  - `std::string email` → 老师邮箱  
  - `Schedule officeHours` → 办公时间  

- **方法**  
  - `std::string getEmail() const`  
    获取邮箱。  
  - `std::string getName() const`  
    获取姓名。  
  - `Schedule& getOfficeHours()`  
    获取办公时间表。  

---

### 5. `User`
**作用**：表示学生用户及其个人日程安排。  

- **成员变量**  
  - `std::string name` -> 学生姓名  
  - `Schedule personalSchedule` -> 学生日程  
  - `Schedule cources` ->学生课程

- **方法**  
  - `std::string getName() const`  
    获取姓名。  
  - `const Schedule& getPersonalScheduleForWeek(int phase)`  
    获取一周的日程。  
  - `Schedule& getCources()`
	 获取课程
-  `Schedule& getPersonalSchedule()`

ps: 最后面的这两个函数是用于对外提供获取成员变量引用的方法，当然这不是一个很好的做法，应该再加一层套娃

---
==office hour 和 course hour只存一周的，然后学生的日程额外存储==

## 模块

### 🔹 1. 数据管理模块 (`DataManager`)
**作用**：统一管理学生与教师数据，提供加载、保存和访问接口。  

- **成员变量**  
  - `User user` → 唯一学生用户  
  - `std::vector<Professor> professors` → 教师列表  

- **方法**  
  - `bool saveUserData(const User& user, const std::string& filePath)`  
    保存学生数据到文件。  
  - `bool loadUserData(User& user, const std::string& filePath)`  
    从文件加载学生数据。  
  - `User& getUser()` / `const User& getUser() const`  
    获取用户对象。  用于对外封装  

  - `bool loadProfessorsData(const std::string& filePath)`  
    从文件加载教师数据。  
  - `const std::vector<Professor>& getProfessors() const`  
    获取教师列表。  用于对外封装
  - `Professor getProfessorByName(const std::string& name) const`  
    根据姓名获取教师信息。  
- `bool saveProfessorsData(const vector<Professor>& professors, const std::string& filePath)`
	保存老师的信息

==当然这里也是同理，要对user的数据进行修改的话，最好还是再加一层套娃，方便后期的维护==

ps:load和save学生的数据的意思是，程序可以从另外的一个文件夹或者输入来获取老师和学生的日程，然后存在当前项目下的一个固定文件夹下的一个文件内，然后每次程序启动时就从这个文件加载用户和老师的数据。 


### 🔹 2. 文件解析模块 (`FileParser`)
**作用**：解析外部输入文件（CSV），生成日程对象。 因为C++要实现读入excel要引入外部库，很麻烦，已尝试，报错多的改不完。

- **方法**  
  - `static Schedule parseCsv(const std::string& filePath)`  
    解析 CSV 文件，生成 `Schedule`。  

---

### 🔹 3. 调度逻辑模块 (`SchedulerLogic`)
**作用**：计算学生与老师之间的可用时间。  

- **方法**  
  - `static std::vector<TimeSlot> findAvailableSlots(const Schedule& studentSchedule, const Schedule& Officehour )`  
    输入学生日历和老师日历，返回排除冲突后的可拜访时间段。
	  - 把学生的课程和当前周的计划相加然后作为studentSchedule传进去

## view

### **`MainWindow` (主窗口)**:

- 作为程序的主框架，可以包含一个工具栏（学生添加自己的日程，导入老师的办公时间段，计算时间段）
- 持有datamanager、QStandardItemModel对象

- **`ScheduleView` (日程视图)**: 主窗口直接显示一周的日程，使用Qt的Model-View框架实现
  - 基于`QTableView`和Qt提供的现成模型QStandardItemModel
  - 支持周切换功能（上一周/下一周按钮）
  - 只读模式：点击查看详情信息，不支持直接滑动编辑
  - 通过`DataManager`获取数据并实时更新显示  这个model只是存储和显示一周的数据，而dataloader可以给这个model计算并提供一周的日程，与datamanager的功能可以不冲突。
  - mainwindow在初始化的时候通过调用这个日程视图来显示日程。
  - ScheduleView 不持有 DataManager，只暴露 `setWeekOffset(int)、setSchedule(const vector<ScheduleEvent>&)。`
  - 通过点击表格中的一个cell，可以查看这个日程的更详细的信息，这个得到的是一个弹窗。此功能得使用connect手动连接doubleClicked(QModelIndex) 信号和一个相应函数。这个相应函数其实用的是datamanager的接口，因为

- mainwindow在初始化的时候一定要从之前保存用户和老师的信息的文件里读取数据，也就是调用datamanager的loadUserData...

### **`InputDialogs` (输入对话框)**:
用弹窗实现
- `AddEventDialog`: 一个用于添加或编辑学生日程的 `QDialog`。要有一个是否为课程的checkbox

- `ImportProfessorDialog`: 一个让用户选择文件、并导入教师时间的对话框。

### **`ResultDisplayWidget` (结果显示区)**:

- 当计算出空闲时间后，用一个`QTableView` 来清晰地展示这些时间段，以及老师的姓名和邮箱地址。这个用弹窗实现。



附录：
我现在想的是先走一个最简单的路线，先不用controller这个中间层去解耦合view和逻辑层，先用Qt的信号与槽函数，然后如果做好了一个可以运行的版本，再去考虑分层设计去解耦合，这样可以提高可维护性。但是本项目由于没有那么复杂，解耦和反而会提高本项目的复杂度，所以将此作为后续的优化目标。

日程视图的话就相当于将一天分割成1个小时或者十分钟十分钟的很多个区域，然后一个日程在对应区域进行染色和显示信息，这个用qt提供的model view来实现

因为本人还没有完全过一遍所有的项目代码，只是给出了一个最小可行性产品的架构，没有充分的考虑到每个代码的长度，在实现一个函数的过程中可以考虑将一些冗长的代码块变成函数，这样可以极大的增加在debug时代码的可读性。因为函数名一般都是能传递这块函数在干什么的，所以只要大致扫一遍，如果能明白这段代码在干什么，就可以跳过。


### 代码组织建议
```
project/
├── datastructure/           # 数据结构定义
├── modules/        # 业务逻辑模块
│   ├── DataManager
│   ├── FileParser  
│   └── SchedulerLogic
├── ui/            # Qt界面组件
│   ├── MainWindow.h/cpp
│   ├── AddEventDialog.h/cpp
│   ├── ImportProfessorDialog.h/cpp
│   ├── ResultDisplayWidget.h/cpp
│   ├── ScheduleView.h/cpp
|   ├── ImportStudentCoursesDialog.h/cpp
│   └── forms/
│       ├── MainWindow.ui
│       ├── AddEventDialog.ui
│       ├── ImportProfessorDialog.ui
│       └── ResultDisplayWidget.ui
├       |———ImportStudentCoursesDialog.ui
```

### 开发建议
1. 先实现核心数据结构和`DataManager`
2. 再实现view部分
3. 最后集成信号与槽机制
4. 在实现过程中，将复杂逻辑拆分为小函数，提高代码可读性


我到时候会用cursor生成一个大致完整的代码框架上传到github，就类似于liangzhiyao的作业一样，大家根据分工去完成自己的功能