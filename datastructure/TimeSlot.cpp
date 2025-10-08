#include "TimeSlot.h"

TimeSlot::TimeSlot()
    : isCourse(false) {
}

TimeSlot::TimeSlot(const std::chrono::system_clock::time_point& start,
                   const std::chrono::system_clock::time_point& end,
                   bool course)
    : startTime(start), endTime(end), isCourse(course) {
}

bool TimeSlot::isOverlappingWith(const TimeSlot& other) const {
    return !(endTime <= other.startTime || startTime >= other.endTime);
}

long long TimeSlot::durationMinutes() const {
    auto duration = std::chrono::duration_cast<std::chrono::minutes>(endTime - startTime);
    return duration.count();
}

bool TimeSlot::getIsCourse() const {
    return isCourse;
}

std::chrono::system_clock::time_point TimeSlot::getStartTime() const {
    return startTime;
}

std::chrono::system_clock::time_point TimeSlot::getEndTime() const {
    return endTime;
}

void TimeSlot::setStartTime(const std::chrono::system_clock::time_point& start) {
    startTime = start;
}

void TimeSlot::setEndTime(const std::chrono::system_clock::time_point& end) {
    endTime = end;
}

void TimeSlot::setIsCourse(bool course) {
    isCourse = course;
}

