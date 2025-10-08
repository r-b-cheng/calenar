#include "Schedule.h"
#include <algorithm>

Schedule::Schedule() {
}

void Schedule::addEvent(const ScheduleEvent& event) {
    events.push_back(event);
}

bool Schedule::removeEvent(int eventId) {
    auto it = std::find_if(events.begin(), events.end(),
                          [eventId](const ScheduleEvent& e) {
                              return e.getId() == eventId;
                          });
    if (it != events.end()) {
        events.erase(it);
        return true;
    }
    return false;
}

std::vector<ScheduleEvent> Schedule::getEventsForDate(
    const std::chrono::system_clock::time_point& date) const {
    
    std::vector<ScheduleEvent> result;
    auto dateTime = std::chrono::system_clock::to_time_t(date);
    std::tm* dateTm = std::localtime(&dateTime);
    
    for (const auto& event : events) {
        auto eventTime = std::chrono::system_clock::to_time_t(event.getTimeSlot().getStartTime());
        std::tm* eventTm = std::localtime(&eventTime);
        
        if (dateTm->tm_year == eventTm->tm_year &&
            dateTm->tm_mon == eventTm->tm_mon &&
            dateTm->tm_mday == eventTm->tm_mday) {
            result.push_back(event);
        }
    }
    
    return result;
}

const std::vector<ScheduleEvent>& Schedule::getEventsForWeek(int weekOffset) const {
    // 简化实现：直接返回所有事件
    // 实际应用中应该根据weekOffset计算对应周的起止时间
    return events;
}

std::vector<ScheduleEvent> Schedule::getEventsInRange(
    const std::chrono::system_clock::time_point& start,
    const std::chrono::system_clock::time_point& end) const {
    
    std::vector<ScheduleEvent> result;
    
    for (const auto& event : events) {
        auto eventStart = event.getTimeSlot().getStartTime();
        auto eventEnd = event.getTimeSlot().getEndTime();
        
        // 检查事件是否在时间范围内
        if (!(eventEnd <= start || eventStart >= end)) {
            result.push_back(event);
        }
    }
    
    return result;
}

Schedule Schedule::operator+(const Schedule& another) const {
    Schedule result;
    result.events = this->events;
    result.events.insert(result.events.end(),
                        another.events.begin(),
                        another.events.end());
    return result;
}

const std::vector<ScheduleEvent>& Schedule::getAllEvents() const {
    return events;
}

void Schedule::clear() {
    events.clear();
}

