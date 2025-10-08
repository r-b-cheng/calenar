#include "User.h"

User::User() {
}

User::User(const std::string& userName)
    : name(userName) {
}

std::string User::getName() const {
    return name;
}

const Schedule& User::getPersonalScheduleForWeek(int phase) const {
    // 简化实现：直接返回个人日程
    // 实际应用中应该根据phase参数计算对应周的日程
    return personalSchedule;
}

Schedule& User::getCourses() {
    return courses;
}

const Schedule& User::getCourses() const {
    return courses;
}

Schedule& User::getPersonalSchedule() {
    return personalSchedule;
}

const Schedule& User::getPersonalSchedule() const {
    return personalSchedule;
}

void User::setName(const std::string& userName) {
    name = userName;
}

