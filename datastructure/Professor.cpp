#include "Professor.h"

Professor::Professor() {
}

Professor::Professor(const std::string& profName, const std::string& profEmail)
    : name(profName), email(profEmail) {
}

std::string Professor::getEmail() const {
    return email;
}

std::string Professor::getName() const {
    return name;
}

Schedule& Professor::getOfficeHours() {
    return officeHours;
}

const Schedule& Professor::getOfficeHours() const {
    return officeHours;
}

void Professor::setName(const std::string& profName) {
    name = profName;
}

void Professor::setEmail(const std::string& profEmail) {
    email = profEmail;
}

