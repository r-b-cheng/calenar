#ifndef SCHEDULE_H
#define SCHEDULE_H

#include "ScheduleEvent.h"
#include <vector>
#include <chrono>

class Schedule {
private:
    std::vector<ScheduleEvent> events;

public:
    Schedule();

    // 添加新事件
    void addEvent(const ScheduleEvent& event);
    
    // 根据事件编号删除事件
    bool removeEvent(int eventId);
    
    // 获取某一天的事件列表
    std::vector<ScheduleEvent> getEventsForDate(const std::chrono::system_clock::time_point& date) const;
    
    // 获取一周的所有事件
    const std::vector<ScheduleEvent>& getEventsForWeek(int weekOffset) const;
    
    // 获取时间范围内的事件
    std::vector<ScheduleEvent> getEventsInRange(
        const std::chrono::system_clock::time_point& start,
        const std::chrono::system_clock::time_point& end) const;
    
    // 重载+运算符，合并两个日程
    Schedule operator+(const Schedule& another) const;
    
    // 获取所有事件
    const std::vector<ScheduleEvent>& getAllEvents() const;
    
    // 清空所有事件
    void clear();
};

#endif // SCHEDULE_H

