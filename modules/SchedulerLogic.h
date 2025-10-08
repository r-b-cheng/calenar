#ifndef SCHEDULERLOGIC_H
#define SCHEDULERLOGIC_H

#include "../datastructure/Schedule.h"
#include "../datastructure/TimeSlot.h"
#include <vector>

class SchedulerLogic {
public:
    // 计算学生和老师之间的可用时间段
    static std::vector<TimeSlot> findAvailableSlots(
        const Schedule& studentSchedule,
        const Schedule& officeHour);
};

#endif // SCHEDULERLOGIC_H

