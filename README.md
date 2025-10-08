# 学生日程管理系统 (Schedule Manager)

这是一个基于Qt5/C++开发的学生日程管理系统，帮助学生管理个人日程和课程，并能够计算与教师之间的可用会面时间。

## 功能特性

1. **日程管理**
   - 添加和查看课程安排
   - 添加和查看个人日程
   - 周视图展示（支持切换不同周）
   - 双击事件查看详细信息

2. **教师办公时间管理**
   - 从CSV文件导入教师办公时间
   - 存储多位教师的信息和办公时间

3. **学生课程导入**
   - 从CSV文件批量导入学生课程
   - 支持课程和个人日程的区分

4. **智能时间计算**
   - 自动计算学生与指定教师之间的可用会面时间
   - 排除学生已有的课程和个人安排
   - 显示所有不冲突的时间段

5. **数据持久化**
   - 自动保存用户数据到本地文件
   - 程序启动时自动加载历史数据

## 项目结构

```
project/
├── datastructure/         # 数据结构定义
│   ├── TimeSlot.h/cpp
│   ├── ScheduleEvent.h/cpp
│   ├── Schedule.h/cpp
│   ├── Professor.h/cpp
│   └── User.h/cpp
├── modules/              # 业务逻辑模块
│   ├── DataManager.h/cpp
│   ├── FileParser.h/cpp
│   └── SchedulerLogic.h/cpp
├── ui/                   # Qt界面组件
│   ├── MainWindow.h/cpp
│   ├── ScheduleView.h/cpp
│   ├── AddEventDialog.h/cpp
│   ├── ImportProfessorDialog.h/cpp
│   └── ResultDisplayWidget.h/cpp
├── example_data/         # 示例数据文件
│   ├── professors.csv
│   └── student_schedule.csv
├── data_storage/         # 自动生成的数据存储目录
├── main.cpp
├── ScheduleManager.pro
└── README.md
```

## 编译和运行

### 前置要求

- Qt 5.x 或更高版本
- C++11 或更高版本编译器
- qmake 构建工具

### 编译步骤

#### 使用Qt Creator（推荐）

1. 打开Qt Creator
2. 打开项目文件 `ScheduleManager.pro`
3. 选择合适的构建套件（Kit）
4. 点击 "构建" → "构建项目"
5. 点击 "运行" 或按 Ctrl+R

#### 使用命令行

```bash
# Windows
qmake ScheduleManager.pro
nmake         # 或 mingw32-make (如果使用MinGW)

# Linux/macOS
qmake ScheduleManager.pro
make

# 运行程序
./bin/ScheduleManager  # Linux/macOS
bin\ScheduleManager.exe  # Windows
```

## 使用说明

### 1. 添加个人日程

1. 点击工具栏的 "添加日程" 按钮
2. 填写事件信息：
   - 事件名称
   - 地点
   - 备注
   - 星期几
   - 开始和结束时间
   - 是否为课程（勾选"课程/办公时间"）
3. 点击 "确定" 保存

### 2. 导入教师办公时间

1. 准备CSV格式的教师数据文件（参考 `example_data/professors.csv`）
2. 点击工具栏的 "导入教师时间" 按钮
3. 选择CSV文件
4. 点击 "导入"

### 3. 导入学生课程

1. 准备CSV格式的学生课程文件（参考 `example_data/student_schedule.csv`）
2. 点击工具栏的 "导入学生课程" 按钮
3. 选择CSV文件
4. 点击 "导入"

### 4. 计算可用时间

1. 点击工具栏的 "计算可用时间" 按钮
2. 从下拉列表中选择教师
3. 系统会自动计算并显示所有可用的会面时间段
4. 结果窗口会显示教师的联系信息和可用时间列表

### 5. 查看日程

- 主界面以周视图形式展示所有日程
- 课程显示为浅蓝色背景
- 个人日程显示为浅黄色背景
- 双击事件可查看详细信息
- 使用 "上一周" / "下一周" 按钮切换周视图

## CSV文件格式

### 教师办公时间 (professors.csv)

```csv
ProfessorName,Email,EventName,Location,Description,Weekday,StartTime,EndTime
Dr. Zhang,zhang@university.edu,Office Hour,Room 301,Weekly office hour,1,2025-01-06 14:00,2025-01-06 16:00
```

字段说明：
- ProfessorName: 教师姓名
- Email: 教师邮箱
- EventName: 办公时间名称
- Location: 地点
- Description: 描述
- Weekday: 星期几（1=周一, 7=周日）
- StartTime: 开始时间（格式：YYYY-MM-DD HH:MM）
- EndTime: 结束时间（格式：YYYY-MM-DD HH:MM）

### 学生日程 (student_schedule.csv)

```csv
EventName,Location,Description,Weekday,StartTime,EndTime,IsCourse
Math Class,Building A Room 101,Calculus lecture,1,2025-01-06 10:00,2025-01-06 12:00,1
```

字段说明：
- EventName: 事件名称
- Location: 地点
- Description: 描述
- Weekday: 星期几（1=周一, 7=周日）
- StartTime: 开始时间（格式：YYYY-MM-DD HH:MM）
- EndTime: 结束时间（格式：YYYY-MM-DD HH:MM）
- IsCourse: 是否为课程（1=是, 0=否）

## 数据存储

程序会自动在 `data_storage/` 目录下保存数据：

- `user_data.txt`: 用户的课程和个人日程
- `professor_data.txt`: 教师信息和办公时间

这些文件会在程序启动时自动加载，关闭时自动保存。

## 核心类说明

### 数据结构

- **TimeSlot**: 表示时间段，包含开始时间、结束时间和是否为课程的标记
- **ScheduleEvent**: 日程事件，包含事件名称、地点、描述等信息
- **Schedule**: 日程集合，管理多个日程事件
- **Professor**: 教师信息，包含姓名、邮箱和办公时间
- **User**: 学生用户，包含课程和个人日程

### 业务逻辑模块

- **DataManager**: 数据管理器，负责加载和保存数据
- **FileParser**: 文件解析器，解析CSV格式的数据文件
- **SchedulerLogic**: 调度逻辑，计算学生和教师之间的可用时间

### UI组件

- **MainWindow**: 主窗口，程序的主界面
- **ScheduleView**: 日程视图，以周视图形式展示日程
- **AddEventDialog**: 添加事件对话框
- **ImportProfessorDialog**: 导入教师数据对话框
- **ResultDisplayWidget**: 结果展示窗口

## 技术特性

- 使用Qt Model-View框架实现UI和数据分离
- 基于C++ STL标准库
- 使用 `std::chrono` 进行时间处理
- 完全的中文界面支持（UTF-8编码）
- 模块化设计，易于维护和扩展

## 许可证

本项目仅供学习和教学使用。

## 联系方式

如有问题或建议，请联系开发团队。

---

**祝您使用愉快！**

