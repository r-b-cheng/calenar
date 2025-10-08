#include "FileParser.h"
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>

Schedule FileParser::parseCsv(const std::string& filePath) {
    Schedule schedule;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        return schedule;
    }

    std::string line;
    // 跳过表头
    std::getline(file, line);
    
    int eventId = 1;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string name, location, description, weekdayStr, startTimeStr, endTimeStr, isCourseStr;
        
        std::getline(iss, name, ',');
        std::getline(iss, location, ',');
        std::getline(iss, description, ',');
        std::getline(iss, weekdayStr, ',');
        std::getline(iss, startTimeStr, ',');
        std::getline(iss, endTimeStr, ',');
        std::getline(iss, isCourseStr, ',');
        
        if (!name.empty()) {
            // 解析时间（假设格式为 YYYY-MM-DD HH:MM）
            std::tm start_tm = {};
            std::tm end_tm = {};
            
            std::istringstream start_ss(startTimeStr);
            start_ss >> std::get_time(&start_tm, "%Y-%m-%d %H:%M");
            
            std::istringstream end_ss(endTimeStr);
            end_ss >> std::get_time(&end_tm, "%Y-%m-%d %H:%M");
            
            std::time_t start_t = std::mktime(&start_tm);
            std::time_t end_t = std::mktime(&end_tm);
            
            bool isCourse = (isCourseStr == "1" || isCourseStr == "true");
            
            TimeSlot slot(std::chrono::system_clock::from_time_t(start_t),
                        std::chrono::system_clock::from_time_t(end_t),
                        isCourse);
            
            int weekday = std::stoi(weekdayStr);
            ScheduleEvent event(eventId++, name, location, description, weekday, slot);
            
            schedule.addEvent(event);
        }
    }

    file.close();
    return schedule;
}

std::vector<Professor> FileParser::parseProfessorsCsv(const std::string& filePath) {
    std::vector<Professor> professors;
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        return professors;
    }

    std::string line;
    // 跳过表头
    std::getline(file, line);
    
    std::string currentProfName;
    std::string currentProfEmail;
    Professor* currentProf = nullptr;
    int eventId = 1;
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        std::istringstream iss(line);
        std::string profName, profEmail, eventName, location, description, weekdayStr, startTimeStr, endTimeStr;
        
        std::getline(iss, profName, ',');
        std::getline(iss, profEmail, ',');
        std::getline(iss, eventName, ',');
        std::getline(iss, location, ',');
        std::getline(iss, description, ',');
        std::getline(iss, weekdayStr, ',');
        std::getline(iss, startTimeStr, ',');
        std::getline(iss, endTimeStr, ',');
        
        // 如果是新教师
        if (!profName.empty() && (profName != currentProfName)) {
            currentProfName = profName;
            currentProfEmail = profEmail;
            professors.push_back(Professor(profName, profEmail));
            currentProf = &professors.back();
        }
        
        // 添加办公时间
        if (currentProf != nullptr && !eventName.empty()) {
            std::tm start_tm = {};
            std::tm end_tm = {};
            
            std::istringstream start_ss(startTimeStr);
            start_ss >> std::get_time(&start_tm, "%Y-%m-%d %H:%M");
            
            std::istringstream end_ss(endTimeStr);
            end_ss >> std::get_time(&end_tm, "%Y-%m-%d %H:%M");
            
            std::time_t start_t = std::mktime(&start_tm);
            std::time_t end_t = std::mktime(&end_tm);
            
            TimeSlot slot(std::chrono::system_clock::from_time_t(start_t),
                        std::chrono::system_clock::from_time_t(end_t),
                        true);  // 办公时间标记为true
            
            int weekday = std::stoi(weekdayStr);
            ScheduleEvent event(eventId++, eventName, location, description, weekday, slot);
            
            currentProf->getOfficeHours().addEvent(event);
        }
    }

    file.close();
    return professors;
}

