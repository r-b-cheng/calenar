#ifndef PROFESSOR_H
#define PROFESSOR_H

#include "Schedule.h"
#include <string>

class Professor {
private:
    std::string name;
    std::string email;
    Schedule officeHours;

public:
    Professor();
    Professor(const std::string& profName, const std::string& profEmail);

    // Getters
    std::string getEmail() const;
    std::string getName() const;
    Schedule& getOfficeHours();
    const Schedule& getOfficeHours() const;
    
    // Setters
    void setName(const std::string& profName);
    void setEmail(const std::string& profEmail);
};

#endif // PROFESSOR_H

