#include "ScheduleView.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QDate>
#include <QDebug>
#include <ctime>
#include <iomanip>
#include <sstream>

ScheduleView::ScheduleView(QWidget* parent)
    : QWidget(parent), currentWeekOffset(0) {
    setupUI();
}

ScheduleView::~ScheduleView() {
}

void ScheduleView::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // 周切换控制栏
    QHBoxLayout* controlLayout = new QHBoxLayout();
    prevWeekButton = new QPushButton(QString::fromUtf8("上一周"), this);
    nextWeekButton = new QPushButton(QString::fromUtf8("下一周"), this);
    weekLabel = new QLabel(QString::fromUtf8("当前周"), this);
    weekLabel->setAlignment(Qt::AlignCenter);

    controlLayout->addWidget(prevWeekButton);
    controlLayout->addWidget(weekLabel, 1);
    controlLayout->addWidget(nextWeekButton);

    // 创建表格视图
    tableView = new QTableView(this);
    model = new QStandardItemModel(24, 8, this);  // 24小时 x 8列（时间+7天）

    // 设置表头
    QStringList headers;
    headers << QString::fromUtf8("时间");
    
    // 添加带日期的星期列头
    QStringList weekHeaders = getWeekHeaders();
    for (int i = 0; i < 7; ++i) {
        headers << weekHeaders[i];
    }
    
    model->setHorizontalHeaderLabels(headers);

    // 填充时间列
    for (int i = 0; i < 24; ++i) {
        QString timeStr = QString("%1:00").arg(i, 2, 10, QChar('0'));
        model->setItem(i, 0, new QStandardItem(timeStr));
    }

    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(false);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->verticalHeader()->setVisible(false);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 布局
    mainLayout->addLayout(controlLayout);
    mainLayout->addWidget(tableView);

    setLayout(mainLayout);

    // 连接信号
    connect(prevWeekButton, &QPushButton::clicked, this, &ScheduleView::onPrevWeekClicked);
    connect(nextWeekButton, &QPushButton::clicked, this, &ScheduleView::onNextWeekClicked);
    connect(tableView, &QTableView::doubleClicked, this, &ScheduleView::onCellDoubleClicked);

    updateWeekLabel();
}

void ScheduleView::updateWeekLabel() {
    if (currentWeekOffset == 0) {
        weekLabel->setText(QString::fromUtf8("本周"));
    } else if (currentWeekOffset > 0) {
        weekLabel->setText(QString::fromUtf8("未来第 %1 周").arg(currentWeekOffset));
    } else {
        weekLabel->setText(QString::fromUtf8("过去第 %1 周").arg(-currentWeekOffset));
    }
}

void ScheduleView::setWeekOffset(int offset) {
    currentWeekOffset = offset;
    updateWeekLabel();
    emit weekChanged(offset);
}

void ScheduleView::setSchedule(const std::vector<ScheduleEvent>& events) {
    currentEvents = events;
    
    // 清空表格（除了时间列）
    for (int row = 0; row < 24; ++row) {
        for (int col = 1; col <= 7; ++col) {
            model->setItem(row, col, new QStandardItem(""));
        }
    }
    
    // 重置所有合并的单元格
    tableView->clearSpans();

    // 填充事件
    for (const auto& event : events) {
        // 使用辅助函数进行周过滤
        if (event.getWeekOffset() != currentWeekOffset) {
            continue;
        }
        
        int weekday = event.getWeekday();
        if (weekday < 1 || weekday > 7) continue;

        auto startTime = std::chrono::system_clock::to_time_t(event.getTimeSlot().getStartTime());
        
        std::tm* start_tm = std::localtime(&startTime);
        int startHour = start_tm->tm_hour;
        
        // 计算持续时间（小时）- 使用chrono直接计算
        auto duration = std::chrono::duration_cast<std::chrono::hours>(event.getTimeSlot().getEndTime() - event.getTimeSlot().getStartTime());
        int durationHours = duration.count();
        
        // 确保至少显示1小时
        if (durationHours == 0) {
            durationHours = 1;
        }
        
        
        if (startHour < 0 || startHour >= 24) continue;

        QString displayText = QString::fromUtf8(event.getEventName().c_str());
        if (!event.getLocation().empty()) {
            displayText += "\n@" + QString::fromUtf8(event.getLocation().c_str());
        }

        // 只在开始时间的位置创建事件项
        QStandardItem* item = new QStandardItem(displayText);
        item->setData(event.getId(), Qt::UserRole);
        
        // 根据是否为课程设置不同颜色
        if (event.getTimeSlot().getIsCourse()) {
            item->setBackground(QColor(173, 216, 230));  // 浅蓝色
        } else {
            item->setBackground(QColor(255, 255, 224));  // 浅黄色
        }

        model->setItem(startHour, weekday, item);
        
        // 合并单元格：使用精确计算的持续时间
        if (durationHours > 1) {
            tableView->setSpan(startHour, weekday, durationHours, 1);
        }
    }
}

int ScheduleView::getCurrentWeekOffset() const {
    return currentWeekOffset;
}

void ScheduleView::onPrevWeekClicked() {
    setWeekOffset(currentWeekOffset - 1);
}

void ScheduleView::onNextWeekClicked() {
    setWeekOffset(currentWeekOffset + 1);
}

void ScheduleView::onCellDoubleClicked(const QModelIndex& index) {
    if (!index.isValid() || index.column() == 0) return;

    QStandardItem* item = model->itemFromIndex(index);
    if (item && item->text().isEmpty()) return;

    int eventId = item->data(Qt::UserRole).toInt();
    if (eventId > 0) {
        emit eventDoubleClicked(eventId);
    }
}

QStringList ScheduleView::getWeekHeaders() {
    QStringList headers;
    QStringList weekNames = {QString::fromUtf8("周一"), QString::fromUtf8("周二"), 
                            QString::fromUtf8("周三"), QString::fromUtf8("周四"), 
                            QString::fromUtf8("周五"), QString::fromUtf8("周六"), 
                            QString::fromUtf8("周日")};
    
    // 获取当前日期
    QDate today = QDate::currentDate();
    
    // 计算当前周的开始日期（周一）
    int daysToMonday = today.dayOfWeek() - 1;  // Qt中周一=1，所以减1
    QDate weekStart = today.addDays(-daysToMonday);
    
    // 根据周偏移量调整
    QDate targetWeekStart = weekStart.addDays(currentWeekOffset * 7);
    
    // 生成一周的日期
    for (int i = 0; i < 7; ++i) {
        QDate currentDate = targetWeekStart.addDays(i);
        QString header = QString("%1\n(%2/%3)")
                        .arg(weekNames[i])
                        .arg(currentDate.month())
                        .arg(currentDate.day());
        headers << header;
    }
    
    return headers;
}

