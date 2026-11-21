#ifndef COURSE_H
#define COURSE_H

#include <QString>

struct Course {
    QString courseCode;
    QString courseName;
    double credits;
    QString teacherName;
    QString grade;
    double gradePoint;
    int semester;

    Course* next;

    Course() {
        credits = 0.0;
        gradePoint = 0.0;
        semester = 0;
        next = nullptr;
    }
};

#endif // COURSE_H
