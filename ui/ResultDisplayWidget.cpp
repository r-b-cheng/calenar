#include "ResultDisplayWidget.h"
#include "ui_ResultDisplayWidget.h"
#include <ctime>
#include <QDateTime>

ResultDisplayWidget::ResultDisplayWidget(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ResultDisplayWidget) {
    ui->setupUi(this);
    
    // 设置表格属性
    ui->resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    // 设置列宽
    ui->resultTable->setColumnWidth(0, 150);  // 开始时间列
    ui->resultTable->setColumnWidth(1, 150);  // 结束时间列
    ui->resultTable->setColumnWidth(2, 100);  // 持续时间列
    ui->resultTable->setColumnWidth(3, 250);  // 教师信息列 - 增加宽度以显示完整邮箱
    
    // 设置表格自动调整列宽模式
    ui->resultTable->horizontalHeader()->setStretchLastSection(false);
    ui->resultTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    
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
    QString titleText = QString::fromUtf8("教师: %1 | 邮箱: %2 | 可用时间段: %3个")
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
        ui->resultTable->setItem(row, 3, new QTableWidgetItem(professorEmail));
    }

    if (availableSlots.empty()) {
        ui->resultTable->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem(QString::fromUtf8("没有可用时间段"));
        item->setTextAlignment(Qt::AlignCenter);
        ui->resultTable->setItem(0, 0, item);
        ui->resultTable->setSpan(0, 0, 1, 4);  // 合并单元格
    }
}
