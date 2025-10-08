#include "ScheduleEvent.h"

ScheduleEvent::ScheduleEvent()
    : id(0), weekday(MONDAY) {
}

ScheduleEvent::ScheduleEvent(int eventId, const std::string& name,
                             const std::string& loc, const std::string& desc,
                             int day, const TimeSlot& slot)
    : id(eventId), eventName(name), location(loc),
      description(desc), weekday(day), timeSlot(slot) {
}

int ScheduleEvent::getId() const {
    return id;
}

std::string ScheduleEvent::getEventName() const {
    return eventName;
}

std::string ScheduleEvent::getLocation() const {
    return location;
}

std::string ScheduleEvent::getDescription() const {
    return description;
}

int ScheduleEvent::getWeekday() const {
    return weekday;
}

TimeSlot ScheduleEvent::getTimeSlot() const {
    return timeSlot;
}

void ScheduleEvent::setId(int eventId) {
    id = eventId;
}

void ScheduleEvent::setEventName(const std::string& name) {
    eventName = name;
}

void ScheduleEvent::setLocation(const std::string& loc) {
    location = loc;
}

void ScheduleEvent::setDescription(const std::string& desc) {
    description = desc;
}

void ScheduleEvent::setWeekday(int day) {
    weekday = day;
}

void ScheduleEvent::setTimeSlot(const TimeSlot& slot) {
    timeSlot = slot;
}

