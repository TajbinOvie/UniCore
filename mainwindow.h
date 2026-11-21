#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "courseList.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;

    CourseList courseList;

    // Course functionality
    void refreshCourseTable();
    void refreshCgpaCalculator();
    void refreshDashboard();

    QString editingCourseCode;
    QString dataFilePath;

    // To-do list functionality
    void loadTodoTasks();
    void saveTodoTasks();

    // Student Information functionality
    void loadStudentProfile();
    void saveStudentProfile();
    void setStudentProfileEditing(bool editing);
};

#endif // MAINWINDOW_H