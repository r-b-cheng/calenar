#ifndef ADDEVENTDIALOG_H
#define ADDEVENTDIALOG_H

#include <QDialog>
#include "../datastructure/ScheduleEvent.h"

namespace Ui {
class AddEventDialog;
}

class AddEventDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddEventDialog(QWidget* parent = nullptr);
    ~AddEventDialog();

    ScheduleEvent getEvent() const;
    void setEvent(const ScheduleEvent& event);
    void clear();

private:
    Ui::AddEventDialog *ui;
};

#endif // ADDEVENTDIALOG_H
