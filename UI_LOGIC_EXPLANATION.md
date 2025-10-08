# UI 页面运行逻辑详解

## 整体架构

```
程序启动 (main.cpp)
    ↓
MainWindow (主窗口)
    ├─→ ScheduleView (嵌入的日程视图Widget)
    ├─→ AddEventDialog (弹窗对话框)
    ├─→ ImportProfessorDialog (弹窗对话框)
    └─→ ResultDisplayWidget (弹窗对话框)
```

---

## 1. 程序启动流程

### main.cpp
```cpp
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 创建主窗口
    MainWindow mainWindow;
    mainWindow.show();
    
    // 进入事件循环
    return app.exec();
}
```

**逻辑**：
1. 创建Qt应用程序对象
2. 创建并显示MainWindow
3. 进入Qt事件循环，等待用户交互

---

## 2. MainWindow (主窗口)

### 初始化流程

```
MainWindow 构造函数
    ↓
1. ui->setupUi(this)          // 加载 MainWindow.ui
    ↓
2. setupConnections()          // 连接信号和槽
    ↓
3. loadData()                  // 从文件加载数据
    ↓
4. updateScheduleView()        // 更新日程视图显示
```

### 核心成员变量

```cpp
private:
    Ui::MainWindow *ui;               // UI界面（来自.ui文件）
    DataManager dataManager;          // 数据管理器
    int nextEventId;                  // 下一个事件ID
    QString userDataPath;             // 用户数据文件路径
    QString professorDataPath;        // 教师数据文件路径
```

### 关键方法详解

#### setupConnections() - 连接信号槽

```cpp
void MainWindow::setupConnections() {
    // 按钮 → 槽函数
    connect(ui->addEventBtn, &QPushButton::clicked, 
            this, &MainWindow::onAddEvent);
    
    // 菜单动作 → 槽函数
    connect(ui->addEventAction, &QAction::triggered, 
            this, &MainWindow::onAddEvent);
    
    // ScheduleView信号 → MainWindow槽函数
    connect(ui->scheduleView, &ScheduleView::weekChanged, 
            this, &MainWindow::onWeekChanged);
    connect(ui->scheduleView, &ScheduleView::eventDoubleClicked, 
            this, &MainWindow::onEventDoubleClicked);
}
```

**信号槽机制**：
- **按钮点击** → 触发对应的槽函数
- **菜单选择** → 触发相同的槽函数（一个功能可以有多个触发入口）
- **ScheduleView事件** → 传递到MainWindow处理

---

## 3. ScheduleView (日程视图)

### 特点
- **自定义Widget**，继承自 QWidget
- **不使用.ui文件**，纯C++代码构建UI
- **嵌入在MainWindow中**（在MainWindow.ui中定义）

### 初始化流程

```
ScheduleView 构造函数
    ↓
setupUI()
    ↓
创建UI组件：
  - QTableView (表格视图)
  - QStandardItemModel (数据模型)
  - QPushButton (上一周/下一周按钮)
  - QLabel (周标签)
    ↓
设置布局
    ↓
连接信号槽
```

### 数据展示原理

```cpp
void ScheduleView::setSchedule(const vector<ScheduleEvent>& events) {
    // 1. 清空表格
    for (int row = 0; row < 24; ++row) {
        for (int col = 1; col <= 7; ++col) {
            model->setItem(row, col, new QStandardItem(""));
        }
    }
    
    // 2. 遍历事件，填充到表格对应位置
    for (const auto& event : events) {
        int weekday = event.getWeekday();  // 1-7 (周一到周日)
        int hour = getHourFromEvent(event);  // 0-23
        
        // 3. 在表格的 [hour, weekday] 位置显示事件
        QStandardItem* item = new QStandardItem(eventName);
        item->setData(event.getId(), Qt::UserRole);  // 保存事件ID
        
        // 4. 根据事件类型设置不同颜色
        if (event.getTimeSlot().getIsCourse()) {
            item->setBackground(QColor(173, 216, 230));  // 浅蓝色
        } else {
            item->setBackground(QColor(255, 255, 224));  // 浅黄色
        }
        
        model->setItem(hour, weekday, item);
    }
}
```

**表格结构**：
```
     时间  │ 周一 │ 周二 │ 周三 │ 周四 │ 周五 │ 周六 │ 周日
    ─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────
    08:00│     │     │     │     │     │     │
    09:00│ 数学课│     │     │     │     │     │
    10:00│     │ 会议 │     │     │     │     │
     ...  │     │     │     │     │     │     │
```

### 用户交互

```cpp
// 用户双击单元格
void ScheduleView::onCellDoubleClicked(const QModelIndex& index) {
    QStandardItem* item = model->itemFromIndex(index);
    int eventId = item->data(Qt::UserRole).toInt();  // 获取事件ID
    
    // 发射信号，让MainWindow处理
    emit eventDoubleClicked(eventId);
}

// 用户点击"上一周"
void ScheduleView::onPrevWeekClicked() {
    currentWeekOffset--;
    emit weekChanged(currentWeekOffset);  // 通知MainWindow
}
```

**信号流程**：
```
ScheduleView 用户操作
    ↓ (emit signal)
MainWindow 接收信号
    ↓
处理逻辑（查找事件、显示详情等）
```

---

## 4. AddEventDialog (添加日程对话框)

### 使用 ui/forms/AddEventDialog.ui

```cpp
// 构造函数
AddEventDialog::AddEventDialog(QWidget* parent) {
    ui->setupUi(this);  // 加载.ui文件
    
    // 设置默认值
    ui->startTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
    
    // 连接按钮
    connect(ui->okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}
```

### 完整工作流程

```
MainWindow: 用户点击"添加日程"
    ↓
MainWindow::onAddEvent()
    ↓
创建 AddEventDialog dialog(this);
    ↓
dialog.exec()  // 显示模态对话框，等待用户操作
    ↓
用户填写表单：
  - 事件名称
  - 地点
  - 描述
  - 星期几
  - 开始/结束时间
  - 是否为课程
    ↓
用户点击"确定"
    ↓
dialog.accept() 返回
    ↓
MainWindow: if (dialog.exec() == QDialog::Accepted)
    ↓
ScheduleEvent event = dialog.getEvent();  // 获取用户输入
    ↓
event.setId(nextEventId++);  // 分配ID
    ↓
dataManager.getUser().getCourses/PersonalSchedule().addEvent(event);
    ↓
updateScheduleView();  // 刷新显示
    ↓
saveData();  // 保存到文件
```

### 数据获取逻辑

```cpp
ScheduleEvent AddEventDialog::getEvent() const {
    // 1. 从UI控件获取用户输入
    QString name = ui->nameEdit->text();
    QString location = ui->locationEdit->text();
    QString description = ui->descriptionEdit->toPlainText();
    int weekday = ui->weekdayCombo->currentIndex() + 1;  // 0-6 → 1-7
    
    // 2. 创建TimeSlot
    TimeSlot slot(
        std::chrono::system_clock::from_time_t(
            ui->startTimeEdit->dateTime().toTime_t()
        ),
        std::chrono::system_clock::from_time_t(
            ui->endTimeEdit->dateTime().toTime_t()
        ),
        ui->isCourseCheck->isChecked()
    );
    
    // 3. 创建ScheduleEvent
    ScheduleEvent event(0, name, location, description, weekday, slot);
    
    return event;
}
```

---

## 5. ImportProfessorDialog (导入教师对话框)

### 使用 ui/forms/ImportProfessorDialog.ui

### 工作流程

```
MainWindow: 用户点击"导入教师时间"
    ↓
MainWindow::onImportProfessor()
    ↓
创建 ImportProfessorDialog dialog(this);
    ↓
dialog.exec()
    ↓
用户点击"浏览"按钮
    ↓
onBrowseClicked()
    ↓
QFileDialog::getOpenFileName() // 打开文件选择对话框
    ↓
用户选择CSV文件
    ↓
ui->filePathEdit->setText(fileName);  // 显示文件路径
    ↓
用户点击"确定"
    ↓
返回到MainWindow
    ↓
QString filePath = dialog.getFilePath();
    ↓
FileParser::parseProfessorsCsv(filePath)  // 解析CSV
    ↓
professors = 解析结果
    ↓
dataManager.getProfessors().push_back(professors)  // 添加到数据
    ↓
saveData()  // 保存
```

### 简化的代码逻辑

```cpp
void ImportProfessorDialog::onBrowseClicked() {
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择CSV文件",
        "",
        "CSV Files (*.csv);;All Files (*)"
    );
    
    if (!fileName.isEmpty()) {
        ui->filePathEdit->setText(fileName);  // 显示路径
    }
}

QString ImportProfessorDialog::getFilePath() const {
    return ui->filePathEdit->text();  // 返回路径供MainWindow使用
}
```

---

## 6. ResultDisplayWidget (结果显示窗口)

### 使用 ui/forms/ResultDisplayWidget.ui

### 工作流程

```
MainWindow: 用户点击"计算可用时间"
    ↓
MainWindow::onCalculateSlots()
    ↓
显示教师选择对话框
    ↓
用户选择教师
    ↓
合并学生日程：courses + personalSchedule
    ↓
SchedulerLogic::findAvailableSlots(studentSchedule, profOfficeHours)
    ↓
得到可用时间段列表：vector<TimeSlot>
    ↓
创建 ResultDisplayWidget resultWidget(this);
    ↓
resultWidget.setResults(profName, profEmail, availableSlots);
    ↓
resultWidget.exec()  // 显示模态对话框
```

### 结果展示逻辑

```cpp
void ResultDisplayWidget::setResults(
    const QString& professorName,
    const QString& professorEmail,
    const vector<TimeSlot>& availableSlots) {
    
    // 1. 设置窗口标题
    setWindowTitle("与 " + professorName + " 的可用时间");
    
    // 2. 设置教师信息标签
    QString info = QString("教师: %1 | 邮箱: %2 | 可用时间段: %3个")
        .arg(professorName)
        .arg(professorEmail)
        .arg(availableSlots.size());
    ui->titleLabel->setText(info);
    
    // 3. 清空表格
    ui->resultTable->setRowCount(0);
    
    // 4. 填充数据
    for (const auto& slot : availableSlots) {
        int row = ui->resultTable->rowCount();
        ui->resultTable->insertRow(row);
        
        // 格式化时间
        QString startTime = formatTime(slot.getStartTime());
        QString endTime = formatTime(slot.getEndTime());
        
        // 填充单元格
        ui->resultTable->setItem(row, 0, new QTableWidgetItem(startTime));
        ui->resultTable->setItem(row, 1, new QTableWidgetItem(endTime));
        ui->resultTable->setItem(row, 2, new QTableWidgetItem(
            QString::number(slot.durationMinutes()) + "分钟"));
        ui->resultTable->setItem(row, 3, new QTableWidgetItem(professorEmail));
    }
}
```

---

## 7. 数据流动全景

### 添加日程流程

```
┌─────────────┐
│  MainWindow │
└──────┬──────┘
       │ 1. 用户点击"添加日程"
       ↓
┌──────────────────┐
│ AddEventDialog   │
│ (弹出对话框)      │
└──────┬───────────┘
       │ 2. 用户填写表单
       ↓
┌──────────────────┐
│  getEvent()      │ 返回 ScheduleEvent
└──────┬───────────┘
       │ 3. 返回到MainWindow
       ↓
┌──────────────────┐
│  DataManager     │
│  .getUser()      │
│  .getCourses()   │
│  .addEvent()     │
└──────┬───────────┘
       │ 4. 数据已添加
       ↓
┌──────────────────┐
│ updateScheduleView()│
└──────┬───────────┘
       │ 5. 更新显示
       ↓
┌──────────────────┐
│  ScheduleView    │
│  setSchedule()   │
└──────────────────┘
       界面更新完成
```

### 计算可用时间流程

```
┌─────────────┐
│  MainWindow │
└──────┬──────┘
       │ 1. 用户点击"计算可用时间"
       ↓
┌──────────────────┐
│ QInputDialog     │ 选择教师
└──────┬───────────┘
       │ 2. 用户选择教师
       ↓
┌──────────────────┐
│  DataManager     │
│  - getUser()     │
│  - getProfessorByName()│
└──────┬───────────┘
       │ 3. 获取数据
       ↓
┌──────────────────┐
│ SchedulerLogic   │
│ findAvailableSlots()│
└──────┬───────────┘
       │ 4. 计算可用时间
       │    返回 vector<TimeSlot>
       ↓
┌──────────────────────┐
│ ResultDisplayWidget  │
│ setResults()         │
└──────────────────────┘
       显示结果
```

---

## 8. 信号槽连接图

```
MainWindow.ui
├─ addEventBtn (按钮) ────────→ MainWindow::onAddEvent()
├─ importProfessorBtn ────────→ MainWindow::onImportProfessor()
├─ calculateBtn ──────────────→ MainWindow::onCalculateSlots()
├─ loadDataBtn ───────────────→ MainWindow::onLoadData()
├─ saveDataBtn ───────────────→ MainWindow::onSaveData()
│
├─ addEventAction (菜单) ─────→ MainWindow::onAddEvent()
├─ importProfessorAction ─────→ MainWindow::onImportProfessor()
├─ calculateAction ───────────→ MainWindow::onCalculateSlots()
└─ exitAction ────────────────→ MainWindow::close()

ScheduleView (嵌入的Widget)
├─ prevWeekButton ────────────→ ScheduleView::onPrevWeekClicked()
├─ nextWeekButton ────────────→ ScheduleView::onNextWeekClicked()
├─ tableView.doubleClicked ──→ ScheduleView::onCellDoubleClicked()
│
└─ emit weekChanged(offset) ──→ MainWindow::onWeekChanged()
   emit eventDoubleClicked(id)→ MainWindow::onEventDoubleClicked()
```

---

## 9. 关键技术点

### Qt的模态对话框

```cpp
// 模态对话框会阻塞父窗口，直到对话框关闭
AddEventDialog dialog(this);
if (dialog.exec() == QDialog::Accepted) {
    // 用户点击了"确定"
    ScheduleEvent event = dialog.getEvent();
    // 处理事件...
} else {
    // 用户点击了"取消"或关闭对话框
}
```

### UI命名空间模式

```cpp
// .h 文件
namespace Ui {
class MainWindow;  // 前向声明
}

class MainWindow : public QMainWindow {
    Q_OBJECT
    
private:
    Ui::MainWindow *ui;  // 指向UI对象的指针
};

// .cpp 文件
#include "ui_MainWindow.h"  // qmake自动从.ui文件生成

MainWindow::MainWindow(QWidget* parent) {
    ui = new Ui::MainWindow();  // 创建UI对象
    ui->setupUi(this);          // 初始化所有UI元素
}

MainWindow::~MainWindow() {
    delete ui;  // 清理
}
```

### 自定义Widget嵌入

```xml
<!-- MainWindow.ui 中 -->
<widget class="ScheduleView" name="scheduleView" native="true">
  <!-- ScheduleView是自定义Widget类 -->
</widget>
```

这样Qt Designer知道这里要放一个自定义的Widget，运行时会创建ScheduleView实例。

---

## 10. 总结

### 各页面职责

| 页面 | 职责 | 使用.ui文件 |
|------|------|-----------|
| **MainWindow** | 主窗口，协调所有组件 | ✅ Yes |
| **ScheduleView** | 日程视图，显示周日程表 | ❌ No (纯C++构建) |
| **AddEventDialog** | 添加日程的表单对话框 | ✅ Yes |
| **ImportProfessorDialog** | 选择CSV文件对话框 | ✅ Yes |
| **ResultDisplayWidget** | 显示计算结果的表格对话框 | ✅ Yes |

### 设计模式

1. **MVC模式**：
   - Model: DataManager, Schedule, ScheduleEvent
   - View: UI层的所有组件
   - Controller: MainWindow协调逻辑

2. **信号槽机制**：
   - 松耦合，组件间通过信号槽通信
   - ScheduleView不直接修改数据，只发送信号
   - MainWindow统一处理所有业务逻辑

3. **对话框模式**：
   - 使用模态对话框收集用户输入
   - exec() 阻塞等待用户操作
   - 通过返回值判断用户选择

### 优势

- ✅ **职责清晰**：每个类只负责自己的功能
- ✅ **易于维护**：UI和逻辑分离
- ✅ **可扩展**：添加新功能只需新增对话框
- ✅ **可重用**：对话框可以在多处调用

---

希望这个详细的解释能帮助你理解整个UI层的运行逻辑！如有疑问，欢迎继续提问。

