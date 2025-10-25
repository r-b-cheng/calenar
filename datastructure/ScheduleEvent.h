#ifndef SCHEDULEEVENT_H
#define SCHEDULEEVENT_H

#include "TimeSlot.h"
#include <string>

enum Weekday {
    MONDAY = 1,    // 周一
    TUESDAY = 2,   // 周二
    WEDNESDAY = 3, // 周三
    THURSDAY = 4,  // 周四
    FRIDAY = 5,    // 周五
    SATURDAY = 6,  // 周六
    SUNDAY = 7     // 周日
};

class ScheduleEvent {
private:
    int id;
    std::string eventName;
    std::string location;
    std::string description;
    int weekday;
    TimeSlot timeSlot;

public:
    ScheduleEvent();
    ScheduleEvent(int eventId, const std::string& name, const std::string& loc,
                  const std::string& desc, int day, const TimeSlot& slot);

    // Getters
    int getId() const;
    std::string getEventName() const;
    std::string getLocation() const;
    std::string getDescription() const;
    int getWeekday() const;
    TimeSlot getTimeSlot() const;

    // Setters
    void setId(int eventId);
    void setEventName(const std::string& name);
    void setLocation(const std::string& loc);
    void setDescription(const std::string& desc);
    void setWeekday(int day);
    void setTimeSlot(const TimeSlot& slot);
    
    // 辅助函数
    int getWeekOffset() const;  // 当月周序，从0开始
};

#endif // SCHEDULEEVENT_H

