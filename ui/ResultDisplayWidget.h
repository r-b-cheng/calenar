#ifndef RESULTDISPLAYWIDGET_H
#define RESULTDISPLAYWIDGET_H

#include <QDialog>
#include "../datastructure/TimeSlot.h"
#include <vector>
#include <QTableWidgetItem>

namespace Ui {
class ResultDisplayWidget;
}

class ResultDisplayWidget : public QDialog {
    Q_OBJECT

public:
    explicit ResultDisplayWidget(QWidget* parent = nullptr);
    ~ResultDisplayWidget();

    void setResults(const QString& professorName,
                   const QString& professorEmail,
                   const std::vector<TimeSlot>& availableSlots);

private slots:
    void onEmailItemDoubleClicked(QTableWidgetItem* item);

private:
    Ui::ResultDisplayWidget *ui;
};

#endif // RESULTDISPLAYWIDGET_H
