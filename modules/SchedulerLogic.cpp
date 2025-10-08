#include "SchedulerLogic.h"
#include <algorithm>

std::vector<TimeSlot> SchedulerLogic::findAvailableSlots(
    const Schedule& studentSchedule,
    const Schedule& officeHour) {
    
    std::vector<TimeSlot> availableSlots;
    
    // 获取老师的所有办公时间
    const auto& officeEvents = officeHour.getAllEvents();
    
    for (const auto& officeEvent : officeEvents) {
        TimeSlot officeSlot = officeEvent.getTimeSlot();
        bool hasConflict = false;
        
        // 检查该办公时间是否与学生日程冲突
        const auto& studentEvents = studentSchedule.getAllEvents();
        
        for (const auto& studentEvent : studentEvents) {
            TimeSlot studentSlot = studentEvent.getTimeSlot();
            
            if (officeSlot.isOverlappingWith(studentSlot)) {
                hasConflict = true;
                break;
            }
        }
        
        // 如果没有冲突，添加到可用时间段
        if (!hasConflict) {
            availableSlots.push_back(officeSlot);
        }
    }
    
    return availableSlots;
}

