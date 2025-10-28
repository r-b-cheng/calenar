#include "ResultDisplayWidget.h"
#include "ui_ResultDisplayWidget.h"
#include <ctime>
#include <QDateTime>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

ResultDisplayWidget::ResultDisplayWidget(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ResultDisplayWidget) {
    ui->setupUi(this);
    
    // 设置表格属性
    ui->resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // 连接双击事件
    connect(ui->resultTable, &QTableWidget::itemDoubleClicked, 
            this, &ResultDisplayWidget::onEmailItemDoubleClicked);
    
    // 设置列宽
    ui->resultTable->setColumnWidth(0, 150);  // 开始时间列
    ui->resultTable->setColumnWidth(1, 150);  // 结束时间列
    ui->resultTable->setColumnWidth(2, 100);  // 持续时间列
    ui->resultTable->setColumnWidth(3, 250);  // 教师信息列 - 增加宽度以显示完整邮箱
    
    // 设置表格自动调整列宽模式
    ui->resultTable->horizontalHeader()->setStretchLastSection(false);
    ui->resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    
    // 设置表格选择和悬停时的背景颜色为浅黄色
    ui->resultTable->setStyleSheet(
        "QTableWidget::item:selected { "
        "background-color: #FFFACD; "  // 浅黄色
        "color: black; "               // 黑色文字确保可读性
        "}"
        "QTableWidget::item:hover { "
        "background-color: #FFFACD; "  // 鼠标悬停时也是浅黄色
        "color: black; "               // 黑色文字确保可读性
        "}"
    );
    
    // 调整窗口大小以适应内容
    resize(700, 450);
}

ResultDisplayWidget::~ResultDisplayWidget() {
    delete ui;
}

void ResultDisplayWidget::setResults(const QString& professorName,
                                     const QString& professorEmail,
                                     const std::vector<TimeSlot>& availableSlots) {
    setWindowTitle(QString::fromUtf8("与 %1 的可用时间").arg(professorName));
    
    // 设置标题标签
    QString titleText = QString::fromUtf8("教师: %1 | 邮箱: %2 | 可用时间段: %3个 | 提示: 双击邮箱可以跳转邮箱")
                          .arg(professorName)
                          .arg(professorEmail)
                          .arg(availableSlots.size());
    ui->titleLabel->setText(titleText);
    
    // 清空表格
    ui->resultTable->setRowCount(0);
    
    // 填充可用时间段
    for (size_t i = 0; i < availableSlots.size(); ++i) {
        const auto& slot = availableSlots[i];
        auto startTime = std::chrono::system_clock::to_time_t(slot.getStartTime());
        auto endTime = std::chrono::system_clock::to_time_t(slot.getEndTime());
        
        // 使用QDateTime来正确显示时间，避免std::localtime的问题
        QDateTime startDateTime = QDateTime::fromSecsSinceEpoch(startTime);
        QDateTime endDateTime = QDateTime::fromSecsSinceEpoch(endTime);
        
        QString startStr = startDateTime.toString("yyyy-MM-dd hh:mm");
        QString endStr = endDateTime.toString("yyyy-MM-dd hh:mm");

        int row = ui->resultTable->rowCount();
        ui->resultTable->insertRow(row);
        
        ui->resultTable->setItem(row, 0, new QTableWidgetItem(startStr));
        ui->resultTable->setItem(row, 1, new QTableWidgetItem(endStr));
        ui->resultTable->setItem(row, 2, new QTableWidgetItem(QString::number(slot.durationMinutes()) + QString::fromUtf8("分钟")));
        
        // 创建邮箱项，添加提示
        QTableWidgetItem* emailItem = new QTableWidgetItem(professorEmail);
        emailItem->setToolTip(QString::fromUtf8("双击打开邮箱软件"));
        emailItem->setForeground(QColor(0, 102, 204));  // 蓝色文字提示可点击
        ui->resultTable->setItem(row, 3, emailItem);
    }

    if (availableSlots.empty()) {
        ui->resultTable->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromUtf8("没有可用时间段"));
        item->setTextAlignment(Qt::AlignCenter);
        ui->resultTable->setItem(0, 0, item);
        ui->resultTable->setSpan(0, 0, 1, 4);  // 合并单元格
    }
}

void ResultDisplayWidget::onEmailItemDoubleClicked(QTableWidgetItem* item) {
    // 检查是否是邮箱列
    if (item && item->column() == 3) {
        QString email = item->text();
        if (!email.isEmpty()) {
            // 使用mailto协议打开默认邮箱软件
            bool success = QDesktopServices::openUrl(QUrl(QString("mailto:%1").arg(email)));
            
            // 如果打开失败，显示提示信息
            if (!success) {
                QMessageBox::information(this, 
                                      QString::fromUtf8("提示"), 
                                      QString::fromUtf8("无法打开邮箱客户端，邮箱地址：%1").arg(email));
            }
        }
    }
}
