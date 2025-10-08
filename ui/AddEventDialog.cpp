#include "AddEventDialog.h"
#include "ui_AddEventDialog.h"
#include <QDateTime>

AddEventDialog::AddEventDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::AddEventDialog) {
    ui->setupUi(this);
    
    // 设置默认时间
    ui->startTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
}

AddEventDialog::~AddEventDialog() {
    delete ui;
}

ScheduleEvent AddEventDialog::getEvent() const {
    // 创建时间段
    TimeSlot slot(
        std::chrono::system_clock::from_time_t(ui->startTimeEdit->dateTime().toSecsSinceEpoch()),
        std::chrono::system_clock::from_time_t(ui->endTimeEdit->dateTime().toSecsSinceEpoch()),
        ui->isCourseCheck->isChecked()
    );

    // 创建事件（ID将由调用者设置）
    ScheduleEvent event(
        0,
        ui->nameEdit->text().toStdString(),
        ui->locationEdit->text().toStdString(),
        ui->descriptionEdit->toPlainText().toStdString(),
        ui->weekdayCombo->currentIndex() + 1,  // 周一=1
        slot
    );

    return event;
}

void AddEventDialog::setEvent(const ScheduleEvent& event) {
    ui->nameEdit->setText(QString::fromUtf8(event.getEventName().c_str()));
    ui->locationEdit->setText(QString::fromUtf8(event.getLocation().c_str()));
    ui->descriptionEdit->setPlainText(QString::fromUtf8(event.getDescription().c_str()));
    ui->weekdayCombo->setCurrentIndex(event.getWeekday() - 1);
    
    auto startTime = std::chrono::system_clock::to_time_t(event.getTimeSlot().getStartTime());
    auto endTime = std::chrono::system_clock::to_time_t(event.getTimeSlot().getEndTime());
    
    ui->startTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(startTime));
    ui->endTimeEdit->setDateTime(QDateTime::fromSecsSinceEpoch(endTime));
    ui->isCourseCheck->setChecked(event.getTimeSlot().getIsCourse());
}

void AddEventDialog::clear() {
    ui->nameEdit->clear();
    ui->locationEdit->clear();
    ui->descriptionEdit->clear();
    ui->weekdayCombo->setCurrentIndex(0);
    ui->startTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->endTimeEdit->setDateTime(QDateTime::currentDateTime().addSecs(3600));
    ui->isCourseCheck->setChecked(false);
}
