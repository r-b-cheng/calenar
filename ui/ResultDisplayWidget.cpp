#include "ResultDisplayWidget.h"
#include "ui_ResultDisplayWidget.h"
#include <ctime>

ResultDisplayWidget::ResultDisplayWidget(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ResultDisplayWidget) {
    ui->setupUi(this);
    
    // 设置表格属性
    ui->resultTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->resultTable->horizontalHeader()->setStretchLastSection(true);
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
        
        std::tm* start_tm = std::localtime(&startTime);
        std::tm* end_tm = std::localtime(&endTime);
        
        char startBuffer[100];
        char endBuffer[100];
        std::strftime(startBuffer, sizeof(startBuffer), "%Y-%m-%d %H:%M", start_tm);
        std::strftime(endBuffer, sizeof(endBuffer), "%Y-%m-%d %H:%M", end_tm);

        int row = ui->resultTable->rowCount();
        ui->resultTable->insertRow(row);
        
        ui->resultTable->setItem(row, 0, new QTableWidgetItem(QString::fromUtf8(startBuffer)));
        ui->resultTable->setItem(row, 1, new QTableWidgetItem(QString::fromUtf8(endBuffer)));
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
