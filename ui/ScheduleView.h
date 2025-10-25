#ifndef SCHEDULEVIEW_H
#define SCHEDULEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include "../datastructure/ScheduleEvent.h"
#include <vector>

class ScheduleView : public QWidget {
    Q_OBJECT

private:
    QTableView* tableView;
    QStandardItemModel* model;
    QPushButton* prevWeekButton;
    QPushButton* nextWeekButton;
    QLabel* weekLabel;
    int currentWeekOffset;
    
    std::vector<ScheduleEvent> currentEvents;

    void setupUI();
    void updateWeekLabel();
    QStringList getWeekHeaders();

public:
    explicit ScheduleView(QWidget* parent = nullptr);
    ~ScheduleView();

    void setWeekOffset(int offset);
    void setSchedule(const std::vector<ScheduleEvent>& events);
    
    int getCurrentWeekOffset() const;

signals:
    void weekChanged(int newOffset);
    void eventDoubleClicked(int eventId);

private slots:
    void onPrevWeekClicked();
    void onNextWeekClicked();
    void onCellDoubleClicked(const QModelIndex& index);
};

#endif // SCHEDULEVIEW_H

