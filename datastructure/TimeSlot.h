#ifndef TIMESLOT_H
#define TIMESLOT_H

#include <chrono>

class TimeSlot {
private:
    std::chrono::system_clock::time_point startTime;
    std::chrono::system_clock::time_point endTime;
    bool isCourse;

public:
    TimeSlot();
    TimeSlot(const std::chrono::system_clock::time_point& start,
             const std::chrono::system_clock::time_point& end,
             bool course = false);

    // 检查两个时间段是否冲突
    bool isOverlappingWith(const TimeSlot& other) const;
    
    // 返回时间段的持续时长（分钟数）
    long long durationMinutes() const;
    
    // Getters
    bool getIsCourse() const;
    std::chrono::system_clock::time_point getStartTime() const;
    std::chrono::system_clock::time_point getEndTime() const;
    
    // Setters
    void setStartTime(const std::chrono::system_clock::time_point& start);
    void setEndTime(const std::chrono::system_clock::time_point& end);
    void setIsCourse(bool course);
};

#endif // TIMESLOT_H

