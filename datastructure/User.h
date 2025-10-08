#ifndef USER_H
#define USER_H

#include "Schedule.h"
#include <string>

class User {
private:
    std::string name;
    Schedule personalSchedule;
    Schedule courses;

public:
    User();
    User(const std::string& userName);

    // Getters
    std::string getName() const;
    const Schedule& getPersonalScheduleForWeek(int phase) const;
    Schedule& getCourses();
    const Schedule& getCourses() const;
    Schedule& getPersonalSchedule();
    const Schedule& getPersonalSchedule() const;
    
    // Setters
    void setName(const std::string& userName);
};

#endif // USER_H

