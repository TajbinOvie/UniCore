
#ifndef COURSELIST_H
#define COURSELIST_H

#include "course.h"

#include <QDebug>
#include <QFile>
#include <QDataStream>

class CourseList {
private:
    Course* head;
    int count;

public:
    CourseList() {
        head = nullptr;
        count = 0;
    }

    ~CourseList() {
        clear();
    }

    // Delete all nodes from the linked list
    void clear() {
        Course* current = head;

        while (current != nullptr) {
            Course* temp = current;
            current = current->next;
            delete temp;
        }

        head = nullptr;
        count = 0;
    }

    // Insert a new course
    bool insertCourse(Course newCourse) {
        Course* newNode = new Course(newCourse);

        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        }
        else {
            Course* current = head;

            while (current->next != nullptr) {
                current = current->next;
            }

            current->next = newNode;
        }

        count++;
        return true;
    }

    // Search for a course using its course code
    Course* searchCourse(QString code) {
        Course* current = head;

        while (current != nullptr) {
            if (current->courseCode == code) {
                return current;
            }

            current = current->next;
        }

        return nullptr;
    }

    // Delete a course using its course code
    bool deleteCourse(QString code) {
        if (head == nullptr) {
            return false;
        }

        if (head->courseCode == code) {
            Course* temp = head;
            head = head->next;

            delete temp;
            count--;

            return true;
        }

        Course* current = head;

        while (current->next != nullptr) {
            if (current->next->courseCode == code) {
                Course* temp = current->next;
                current->next = temp->next;

                delete temp;
                count--;

                return true;
            }

            current = current->next;
        }

        return false;
    }

    // Update an existing course
    bool updateCourse(QString code, Course updatedCourse) {
        Course* current = searchCourse(code);

        if (current == nullptr) {
            return false;
        }

        current->courseCode = updatedCourse.courseCode;
        current->courseName = updatedCourse.courseName;
        current->credits = updatedCourse.credits;
        current->teacherName = updatedCourse.teacherName;
        current->grade = updatedCourse.grade;
        current->gradePoint = updatedCourse.gradePoint;
        current->semester = updatedCourse.semester;

        return true;
    }

    // Traverse and display all courses
    void displayCourses() {
        Course* current = head;

        while (current != nullptr) {
            qDebug() << "Course Code:" << current->courseCode
                     << "Course Name:" << current->courseName
                     << "Credits:" << current->credits
                     << "Teacher:" << current->teacherName
                     << "Grade:" << current->grade
                     << "Grade Point:" << current->gradePoint;

            current = current->next;
        }
    }

    // Return the first node for UI table traversal
    Course* getHead() {
        return head;
    }

    int getCount() {
        return count;
    }

    bool isEmpty() {
        return head == nullptr;
    }


    // Save all courses to a file
    bool saveToFile(QString fileName) {

        QFile file(fileName);

        if (!file.open(QIODevice::WriteOnly)) {
            return false;
        }

        QDataStream out(&file);
        out.setVersion(QDataStream::Qt_6_0);

        Course* current = head;

        while (current != nullptr) {

            out << current->courseCode
                << current->courseName
                << current->credits
                << current->teacherName
                << current->grade
                << current->gradePoint
                << current->semester;

            current = current->next;
        }

        file.close();

        return out.status() == QDataStream::Ok;
    }


    // Load courses from a file
    bool loadFromFile(QString fileName) {

        QFile file(fileName);

        if (!file.exists()) {
            return true;
        }

        if (!file.open(QIODevice::ReadOnly)) {
            return false;
        }

        clear();

        QDataStream in(&file);
        in.setVersion(QDataStream::Qt_6_0);

        while (!in.atEnd()) {

            Course newCourse;

            in >> newCourse.courseCode
                >> newCourse.courseName
                >> newCourse.credits
                >> newCourse.teacherName
                >> newCourse.grade
                >> newCourse.gradePoint
                >> newCourse.semester;

            if (in.status() != QDataStream::Ok) {
                clear();
                file.close();
                return false;
            }

            insertCourse(newCourse);
        }

        file.close();

        return true;
    }

};

#endif // COURSELIST_H
