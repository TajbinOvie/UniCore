#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QComboBox>
#include <QStandardPaths>
#include <QDir>
#include <QHeaderView>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSettings>
#include <QProgressBar>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLayout>
#include <QDoubleSpinBox>
#include <QSizePolicy>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ==========================================
    // FIX SIDEBAR ALIGNMENT
    // ==========================================

    // The sidebar has two nested layouts in Qt Designer.
    // Keep the outer layout neutral and align the inner navigation
    // layout consistently from the top.
    if (ui->sidebar->layout()) {
        ui->sidebar->layout()->setContentsMargins(0, 0, 0, 0);
        ui->sidebar->layout()->setSpacing(0);
    }

    QVBoxLayout *sidebarItemsLayout =
        ui->sidebar->findChild<QVBoxLayout*>("verticalLayout");

    if (sidebarItemsLayout) {
        sidebarItemsLayout->setContentsMargins(16, 20, 16, 16);
        sidebarItemsLayout->setSpacing(10);
        sidebarItemsLayout->setAlignment(Qt::AlignTop);
    }

    ui->logoLabel->setMinimumHeight(55);
    ui->logoLabel->setMaximumHeight(55);
    ui->logoLabel->setAlignment(Qt::AlignCenter);

    // Give every sidebar navigation item the same height.
    ui->dashboardButton->setMinimumHeight(44);
    ui->studentInfoButton->setMinimumHeight(44);
    ui->coursesButton->setMinimumHeight(44);
    ui->cgpaButton->setMinimumHeight(44);
    ui->settingsButton->setMinimumHeight(44);

    // ==========================================
    // FIX COURSE FORM OVERLAPPING
    // ==========================================

    // The .ui file capped this container at 180 px, but it contains
    // six grid rows of controls. That cap forced the rows to overlap.
    ui->courseFormContainer->setMinimumHeight(250);
    ui->courseFormContainer->setMaximumHeight(280);

    if (QGridLayout *courseGrid =
        qobject_cast<QGridLayout*>(ui->courseFormContainer->layout())) {
        courseGrid->setContentsMargins(12, 10, 12, 10);
        courseGrid->setHorizontalSpacing(12);
        courseGrid->setVerticalSpacing(7);
    }

    // Keep each control tall enough to display its text clearly.
    ui->courseCodeInput->setMinimumHeight(32);
    ui->courseNameInput->setMinimumHeight(32);
    ui->teacherNameInput->setMinimumHeight(32);
    ui->courseCreditInput->setMinimumHeight(32);
    ui->gradeComboBox->setMinimumHeight(32);
    ui->addCourseButton->setMinimumHeight(34);

    // Search and action controls retain consistent heights.
    ui->searchCourseInput->setMinimumHeight(36);
    ui->addSemesterButton->setMinimumHeight(34);
    ui->searchCourseButton->setMinimumHeight(36);
    ui->updateCourseButton->setMinimumHeight(40);
    ui->deleteCourseButton->setMinimumHeight(40);

    // Clear old individual stylesheets
    const auto widgets = findChildren<QWidget*>();

    for (QWidget *widget : widgets) {
        widget->setStyleSheet("");
    }


    setStyleSheet(R"(
    QMainWindow, QStackedWidget, QWidget#dashboardPage, QWidget#studentsPage,
    QWidget#coursesPage, QWidget#cgpaPage, QFrame#contentArea {
        background-color: #F4F7FB;
        color: #263449;
    }

    QFrame#sidebar {
        background-color: #1C2740;
    }
    QLabel#logoLabel { color: #FFFFFF; }
    QFrame#sidebar QPushButton {
        background-color: transparent;
        color: #C7D2E3;
        text-align: left;
        padding-left: 18px;
    }
    QFrame#sidebar QPushButton:hover { background-color: #2B3955; color: #FFFFFF; }
    QFrame#sidebar QPushButton:pressed { background-color: #3B4B6A; color: #FFFFFF; }

    QLabel#dashboardTitleLabel, QLabel#studentInfoLabel,
    QLabel#coursesTitleLabel_2, QLabel#cgpaPageTitleLabel {
        color: #1B2940;
        font-size: 20px;
        font-weight: bold;
    }
    QLabel#dashboardSubtitleLabel { color: #6B7B91; }

    QFrame#cgpaCard, QFrame#completedCreditsCard, QFrame#totalCoursesCard,
    QFrame#currentSemesterCard, QFrame#frame_6, QFrame#frame_7,
    QFrame#academicProgressContainer, QWidget#todoContainer,
    QFrame#studentBasicInfoContainer, QFrame#studentContactContainer,
    QFrame#semesterContainer, QFrame#courseFormContainer,
    QFrame#frame_3, QFrame#frame_4, QFrame#frame_5, QFrame#frame_8,
    QFrame#frame_10, QFrame#frame_2, QFrame#frame_9 {
        background-color: #FFFFFF;
        border-color: #DCE4EE;
    }
    QFrame#frame_12, QFrame#frame_13, QFrame#frame_14,
    QFrame#frame_15, QFrame#frame_16 { background-color: transparent; }

    QLabel#cgpaTitleLabel, QLabel#creditsTitleLabel, QLabel#coursesTitleLabel,
    QLabel#semesterTitleLabel, QLabel#overallCgpaValueLabel,
    QLabel#completedCreditsValueLabel, QLabel#todoTitleLabel,
    QLabel#progressTitleLabel, QLabel#label_6 { color: #5F7189; }
    QLabel#cgpaValueLabel, QLabel#creditsValueLabel, QLabel#coursesValueLabel,
    QLabel#semesterValueLabel, QLabel#overallCgpaNumberLabel,
    QLabel#completedCreditsNumberLabel { color: #1B2940; }
    QLabel { color: #334155; }

    QLineEdit, QTextEdit, QComboBox, QDoubleSpinBox {
        background-color: #FFFFFF;
        color: #263449;
        border-color: #D5DFEA;
    }
    QLineEdit:focus, QTextEdit:focus, QComboBox:focus, QDoubleSpinBox:focus {
        border-color: #3B82F6;
    }

    QPushButton { background-color: #3478D4; color: #FFFFFF; }
    QPushButton:hover { background-color: #2868BE; }
    QPushButton:pressed { background-color: #1F579F; }
    QPushButton#deleteCourseButton { background-color: #D94B4B; }
    QPushButton#deleteCourseButton:hover { background-color: #C63B3B; }

    QTableWidget, QTableView {
        background-color: #FFFFFF;
        color: #263449;
        gridline-color: #E5EAF1;
        selection-background-color: #E6F0FF;
        selection-color: #1B2940;
    }
    QHeaderView::section { background-color: #F1F5F9; color: #52657D; }
    QProgressBar { background-color: #E8EEF5; color: #263449; }
    QProgressBar::chunk { background-color: #3478D4; }
    QListWidget { background-color: #FFFFFF; color: #263449; }
    QScrollBar { background-color: #F1F5F9; }
    QScrollBar::handle { background-color: #C3CFDE; }
    )");


    // ==========================================
    // TABLE CONFIGURATION
    // ==========================================

    ui->courseTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->courseTable->verticalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents
        );

    ui->cgpaCourseTable->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch
        );

    ui->cgpaCourseTable->verticalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents
        );


    // ==========================================
    // INITIAL PAGE
    // ==========================================

    ui->stackedWidget->setCurrentWidget(
        ui->dashboardPage
        );


    // ==========================================
    // COURSE DATA STORAGE
    // ==========================================

    QString dataFolder = QStandardPaths::writableLocation(
        QStandardPaths::AppDataLocation
        );

    QDir().mkpath(dataFolder);

    dataFilePath = dataFolder + "/courses.dat";


    // ==========================================
    // SEMESTER OPTIONS
    // ==========================================

    for (int i = 1; i <= 8; i++) {

        QString semesterName =
            "Semester " + QString::number(i);

        ui->semesterComboBox->addItem(semesterName);
        ui->cgpaSemesterComboBox->addItem(semesterName);
    }


    // ==========================================
    // LOAD SAVED COURSES
    // ==========================================

    if (!courseList.loadFromFile(dataFilePath)) {

        QMessageBox::warning(
            this,
            "Loading Error",
            "Could not load saved course data."
            );
    }


    // ==========================================
    // GRADE OPTIONS
    // ==========================================

    ui->gradeComboBox->addItem("A+", 4.00);
    ui->gradeComboBox->addItem("A", 3.75);
    ui->gradeComboBox->addItem("A-", 3.50);
    ui->gradeComboBox->addItem("B+", 3.25);
    ui->gradeComboBox->addItem("B", 3.00);
    ui->gradeComboBox->addItem("B-", 2.75);
    ui->gradeComboBox->addItem("C+", 2.50);
    ui->gradeComboBox->addItem("C", 2.25);
    ui->gradeComboBox->addItem("D", 2.00);
    ui->gradeComboBox->addItem("F", 0.00);
    ui->gradeComboBox->addItem("Pending", 0.00);


    ui->creditsTitleLabel->setText("Completed Credits");


    // ==========================================
    // LOAD SAVED INFORMATION
    // ==========================================

    loadTodoTasks();

    loadStudentProfile();


    // ==========================================
    // DASHBOARD TODO FUNCTIONALITY
    // ==========================================

    // Add Task

    auto addTodoTask = [this]() {

        QString task = ui->todoInput->text().trimmed();

        if (task.isEmpty()) {

            QMessageBox::warning(
                this,
                "Empty Task",
                "Please enter a task first."
                );

            return;
        }

        ui->todoListWidget->addItem(task);

        ui->todoInput->clear();

        saveTodoTasks();
    };


    connect(
        ui->addTodoButton,
        &QPushButton::clicked,
        this,
        addTodoTask
        );


    // Pressing Enter also adds a task

    connect(
        ui->todoInput,
        &QLineEdit::returnPressed,
        this,
        addTodoTask
        );


    // Delete Selected Task

    connect(
        ui->deleteTodoButton,
        &QPushButton::clicked,
        this,
        [this]() {

            int row = ui->todoListWidget->currentRow();

            if (row < 0) {

                QMessageBox::warning(
                    this,
                    "No Task Selected",
                    "Please select a task to delete."
                    );

                return;
            }

            QListWidgetItem *item =
                ui->todoListWidget->takeItem(row);

            delete item;

            saveTodoTasks();
        }
        );


    // ==========================================
    // SEARCH COURSE BUTTON
    // ==========================================

    connect(
        ui->searchCourseButton,
        &QPushButton::clicked,
        this,
        [this]() {

            QString searchText =
                ui->searchCourseInput->text().trimmed();

            if (searchText.isEmpty()) {

                refreshCourseTable();

                return;
            }

            ui->courseTable->setRowCount(0);

            Course *current = courseList.getHead();

            bool found = false;


            // Search through every semester

            while (current != nullptr) {

                if (
                    current->courseCode.contains(
                        searchText,
                        Qt::CaseInsensitive
                        )
                    ||
                    current->courseName.contains(
                        searchText,
                        Qt::CaseInsensitive
                        )
                    ) {

                    int row = ui->courseTable->rowCount();

                    ui->courseTable->insertRow(row);

                    ui->courseTable->setItem(
                        row, 0,
                        new QTableWidgetItem(current->courseCode)
                        );

                    ui->courseTable->setItem(
                        row, 1,
                        new QTableWidgetItem(current->courseName)
                        );

                    ui->courseTable->setItem(
                        row, 2,
                        new QTableWidgetItem(
                            QString::number(current->credits)
                            )
                        );

                    ui->courseTable->setItem(
                        row, 3,
                        new QTableWidgetItem(current->teacherName)
                        );

                    ui->courseTable->setItem(
                        row, 4,
                        new QTableWidgetItem(current->grade)
                        );

                    ui->courseTable->setItem(
                        row, 5,
                        new QTableWidgetItem(
                            QString::number(current->gradePoint)
                            )
                        );

                    found = true;
                }

                current = current->next;
            }


            if (!found) {

                QMessageBox::information(
                    this,
                    "Search Result",
                    "No matching course found."
                    );
            }
        }
        );


    // ==========================================
    // ADD COURSE / SAVE UPDATE BUTTON
    // ==========================================

    connect(
        ui->addCourseButton,
        &QPushButton::clicked,
        this,
        [this]() {

            QString code =
                ui->courseCodeInput->text().trimmed();

            QString name =
                ui->courseNameInput->text().trimmed();

            QString teacher =
                ui->teacherNameInput->text().trimmed();


            if (
                code.isEmpty()
                || name.isEmpty()
                || teacher.isEmpty()
                ) {

                QMessageBox::warning(
                    this,
                    "Input Error",
                    "Please fill in all course details."
                    );

                return;
            }


            // Check duplicate course codes

            if (
                !editingCourseCode.isEmpty()
                && code != editingCourseCode
                ) {

                if (courseList.searchCourse(code) != nullptr) {

                    QMessageBox::warning(
                        this,
                        "Duplicate Course",
                        "This course code already exists."
                        );

                    return;
                }
            }
            else if (
                editingCourseCode.isEmpty()
                && courseList.searchCourse(code) != nullptr
                ) {

                QMessageBox::warning(
                    this,
                    "Duplicate Course",
                    "This course code already exists."
                    );

                return;
            }


            Course newCourse;

            newCourse.courseCode = code;
            newCourse.courseName = name;

            newCourse.credits =
                ui->courseCreditInput->value();

            newCourse.teacherName = teacher;

            newCourse.grade =
                ui->gradeComboBox->currentText();

            newCourse.gradePoint =
                ui->gradeComboBox->currentData().toDouble();

            newCourse.semester =
                ui->semesterComboBox->currentIndex() + 1;


            // Update existing course or insert new course

            if (!editingCourseCode.isEmpty()) {

                courseList.updateCourse(
                    editingCourseCode,
                    newCourse
                    );

                editingCourseCode.clear();

                ui->addCourseButton->setText("Add Course");

                QMessageBox::information(
                    this,
                    "Success",
                    "Course updated successfully."
                    );
            }
            else {

                courseList.insertCourse(newCourse);

                QMessageBox::information(
                    this,
                    "Success",
                    "Course added successfully."
                    );
            }


            // Save course data

            if (!courseList.saveToFile(dataFilePath)) {

                QMessageBox::warning(
                    this,
                    "Save Error",
                    "Could not save course data."
                    );
            }


            // Refresh tables and calculations

            refreshCourseTable();
            refreshCgpaCalculator();


            // Clear input fields

            ui->courseCodeInput->clear();
            ui->courseNameInput->clear();
            ui->teacherNameInput->clear();

            ui->courseCreditInput->setValue(0.5);
        }
        );


    // ==========================================
    // DELETE COURSE BUTTON
    // ==========================================

    connect(
        ui->deleteCourseButton,
        &QPushButton::clicked,
        this,
        [this]() {

            int row = ui->courseTable->currentRow();

            if (row < 0) {

                QMessageBox::warning(
                    this,
                    "Warning",
                    "Please select a course first."
                    );

                return;
            }


            QString code =
                ui->courseTable->item(row, 0)->text();


            if (courseList.deleteCourse(code)) {

                if (!courseList.saveToFile(dataFilePath)) {

                    QMessageBox::warning(
                        this,
                        "Save Error",
                        "Could not save course data."
                        );
                }

                refreshCourseTable();
                refreshCgpaCalculator();

                QMessageBox::information(
                    this,
                    "Success",
                    "Course deleted successfully."
                    );
            }
        }
        );


    // ==========================================
    // UPDATE SELECTED COURSE BUTTON
    // ==========================================

    connect(
        ui->updateCourseButton,
        &QPushButton::clicked,
        this,
        [this]() {

            int row = ui->courseTable->currentRow();

            if (row < 0) {

                QMessageBox::warning(
                    this,
                    "Warning",
                    "Please select a course first."
                    );

                return;
            }


            editingCourseCode =
                ui->courseTable->item(row, 0)->text();

            Course *current =
                courseList.searchCourse(editingCourseCode);


            if (current == nullptr) {

                editingCourseCode.clear();

                return;
            }


            ui->courseCodeInput->setText(
                current->courseCode
                );

            ui->courseNameInput->setText(
                current->courseName
                );

            ui->courseCreditInput->setValue(
                current->credits
                );

            ui->teacherNameInput->setText(
                current->teacherName
                );

            ui->semesterComboBox->setCurrentIndex(
                current->semester - 1
                );

            ui->gradeComboBox->setCurrentText(
                current->grade
                );

            ui->addCourseButton->setText("Save Update");
        }
        );


    // ==========================================
    // ADD SEMESTER BUTTON
    // ==========================================

    connect(
        ui->addSemesterButton,
        &QPushButton::clicked,
        this,
        [this]() {

            int nextSemester =
                ui->semesterComboBox->count() + 1;

            QString semesterName =
                "Semester " + QString::number(nextSemester);

            ui->semesterComboBox->addItem(semesterName);

            ui->cgpaSemesterComboBox->addItem(semesterName);

            ui->semesterComboBox->setCurrentIndex(
                ui->semesterComboBox->count() - 1
                );

            QMessageBox::information(
                this,
                "Success",
                semesterName + " added successfully."
                );
        }
        );


    // ==========================================
    // SEMESTER SELECTION
    // ==========================================

    connect(
        ui->semesterComboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {

            refreshCourseTable();
            refreshDashboard();
        }
        );


    connect(
        ui->cgpaSemesterComboBox,
        QOverload<int>::of(&QComboBox::currentIndexChanged),
        this,
        [this](int) {

            refreshCgpaCalculator();
        }
        );


    // ==========================================
    // SIDEBAR NAVIGATION
    // ==========================================

    connect(
        ui->dashboardButton,
        &QPushButton::clicked,
        this,
        [this]() {

            ui->stackedWidget->setCurrentWidget(
                ui->dashboardPage
                );

            refreshDashboard();
        }
        );


    connect(
        ui->studentInfoButton,
        &QPushButton::clicked,
        this,
        [this]() {

            ui->stackedWidget->setCurrentWidget(
                ui->studentsPage
                );
        }
        );


    connect(
        ui->coursesButton,
        &QPushButton::clicked,
        this,
        [this]() {

            ui->stackedWidget->setCurrentWidget(
                ui->coursesPage
                );
        }
        );


    connect(
        ui->cgpaButton,
        &QPushButton::clicked,
        this,
        [this]() {

            ui->stackedWidget->setCurrentWidget(
                ui->cgpaPage
                );
        }
        );


    // ==========================================
    // STUDENT PROFILE FUNCTIONALITY
    // ==========================================

    // Edit Profile

    connect(
        ui->editProfileButton,
        &QPushButton::clicked,
        this,
        [this]() {

            setStudentProfileEditing(true);
        }
        );


    // Save Profile

    connect(
        ui->saveProfileButton,
        &QPushButton::clicked,
        this,
        [this]() {

            QString studentName =
                ui->studentNameInput->text().trimmed();

            QString studentId =
                ui->studentIdInput->text().trimmed();


            // Validate essential information

            if (
                studentName.isEmpty()
                || studentId.isEmpty()
                ) {

                QMessageBox::warning(
                    this,
                    "Incomplete Information",
                    "Please enter your name and student ID."
                    );

                return;
            }


            saveStudentProfile();

            setStudentProfileEditing(false);

            QMessageBox::information(
                this,
                "Success",
                "Student information saved successfully."
                );
        }
        );


    // Cancel Editing

    connect(
        ui->cancelProfileButton,
        &QPushButton::clicked,
        this,
        [this]() {

            QMessageBox::StandardButton reply =
                QMessageBox::question(
                    this,
                    "Discard Changes",
                    "Are you sure you want to discard your changes?",
                    QMessageBox::Yes | QMessageBox::No
                    );


            if (reply == QMessageBox::Yes) {

                loadStudentProfile();

                setStudentProfileEditing(false);
            }
        }
        );


    // ==========================================
    // INITIAL REFRESH
    // ==========================================

    refreshCourseTable();

    refreshCgpaCalculator();

    refreshDashboard();
}


// ==========================================
// DESTRUCTOR
// ==========================================

MainWindow::~MainWindow()
{
    courseList.saveToFile(dataFilePath);

    saveTodoTasks();

    delete ui;
}


// ==========================================
// REFRESH COURSE TABLE
// ==========================================

void MainWindow::refreshCourseTable()
{
    ui->courseTable->setRowCount(0);

    int selectedSemester =
        ui->semesterComboBox->currentIndex() + 1;

    Course *current = courseList.getHead();


    while (current != nullptr) {

        if (current->semester == selectedSemester) {

            int row = ui->courseTable->rowCount();

            ui->courseTable->insertRow(row);


            ui->courseTable->setItem(
                row, 0,
                new QTableWidgetItem(current->courseCode)
                );

            ui->courseTable->setItem(
                row, 1,
                new QTableWidgetItem(current->courseName)
                );

            ui->courseTable->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(current->credits)
                    )
                );

            ui->courseTable->setItem(
                row, 3,
                new QTableWidgetItem(current->teacherName)
                );

            ui->courseTable->setItem(
                row, 4,
                new QTableWidgetItem(current->grade)
                );

            ui->courseTable->setItem(
                row, 5,
                new QTableWidgetItem(
                    QString::number(current->gradePoint)
                    )
                );
        }

        current = current->next;
    }
}


// ==========================================
// REFRESH CGPA CALCULATOR
// ==========================================

void MainWindow::refreshCgpaCalculator()
{
    double totalQualityPoints = 0.0;

    double completedCredits = 0.0;

    Course *current = courseList.getHead();


    // Calculate overall CGPA and completed credits

    while (current != nullptr) {

        if (
            current->grade.compare(
                "Pending",
                Qt::CaseInsensitive
                ) != 0
            ) {

            totalQualityPoints +=
                current->credits * current->gradePoint;

            completedCredits += current->credits;
        }

        current = current->next;
    }


    // Calculate overall CGPA

    double overallCgpa = 0.0;

    if (completedCredits > 0.0) {

        overallCgpa =
            totalQualityPoints / completedCredits;
    }


    ui->overallCgpaNumberLabel->setText(
        QString::number(overallCgpa, 'f', 2)
        );

    ui->completedCreditsNumberLabel->setText(
        QString::number(completedCredits, 'g', 4)
        );


    // Calculate GPA for the selected semester

    int selectedSemester =
        ui->cgpaSemesterComboBox->currentIndex() + 1;

    double semesterQualityPoints = 0.0;

    double semesterCredits = 0.0;


    // Clear semester course table

    ui->cgpaCourseTable->setRowCount(0);

    current = courseList.getHead();


    while (current != nullptr) {

        if (current->semester == selectedSemester) {

            int row = ui->cgpaCourseTable->rowCount();

            ui->cgpaCourseTable->insertRow(row);


            ui->cgpaCourseTable->setItem(
                row, 0,
                new QTableWidgetItem(current->courseCode)
                );

            ui->cgpaCourseTable->setItem(
                row, 1,
                new QTableWidgetItem(current->courseName)
                );

            ui->cgpaCourseTable->setItem(
                row, 2,
                new QTableWidgetItem(
                    QString::number(current->credits)
                    )
                );

            ui->cgpaCourseTable->setItem(
                row, 3,
                new QTableWidgetItem(current->grade)
                );

            ui->cgpaCourseTable->setItem(
                row, 4,
                new QTableWidgetItem(
                    QString::number(
                        current->gradePoint,
                        'f',
                        2
                        )
                    )
                );


            // Exclude Pending courses from semester GPA

            if (
                current->grade.compare(
                    "Pending",
                    Qt::CaseInsensitive
                    ) != 0
                ) {

                semesterQualityPoints +=
                    current->credits * current->gradePoint;

                semesterCredits += current->credits;
            }
        }

        current = current->next;
    }


    // Calculate semester GPA

    double semesterGpa = 0.0;

    if (semesterCredits > 0.0) {

        semesterGpa =
            semesterQualityPoints / semesterCredits;
    }


    ui->semesterGpaValueLabel->setText(
        QString::number(semesterGpa, 'f', 2)
        );


    // Update Dashboard whenever CGPA changes

    refreshDashboard();
}


// ==========================================
// REFRESH DASHBOARD
// ==========================================

void MainWindow::refreshDashboard()
{
    double totalQualityPoints = 0.0;

    double completedCredits = 0.0;

    int totalCourses = 0;

    Course *current = courseList.getHead();


    // Traverse the linked list

    while (current != nullptr) {

        totalCourses++;

        // Ignore Pending courses

        if (
            current->grade.compare(
                "Pending",
                Qt::CaseInsensitive
                ) != 0
            ) {

            totalQualityPoints +=
                current->credits * current->gradePoint;

            completedCredits += current->credits;
        }

        current = current->next;
    }


    // Calculate overall CGPA

    double overallCgpa = 0.0;

    if (completedCredits > 0.0) {

        overallCgpa =
            totalQualityPoints / completedCredits;
    }


    // Update Dashboard statistics

    ui->cgpaValueLabel->setText(
        QString::number(overallCgpa, 'f', 2)
        );

    ui->creditsValueLabel->setText(
        QString::number(completedCredits, 'g', 4)
        );

    ui->coursesValueLabel->setText(
        QString::number(totalCourses)
        );

    int currentSemester =
        ui->semesterComboBox->currentIndex() + 1;

    ui->semesterValueLabel->setText(
        QString::number(currentSemester)
        );


    // ==========================================
    // ACADEMIC PERFORMANCE INDICATOR
    // ==========================================

    ui->progressBar->setRange(0, 100);

    ui->progressBar->setFormat("%p%");


    // No graded courses yet

    if (completedCredits <= 0.0) {

        ui->progressBar->setValue(0);

        ui->progressTitleLabel->setText(
            "Academic Performance - No graded courses yet"
            );

        ui->progressBar->setStyleSheet(
            "QProgressBar {"
            "   border: 1px solid #CBD5E1;"
            "   border-radius: 6px;"
            "   background-color: #E2E8F0;"
            "   text-align: center;"
            "   color: #172033;"
            "   height: 18px;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: #94A3B8;"
            "   border-radius: 5px;"
            "}"
            );

        return;
    }


    // Convert CGPA into a percentage

    int performance =
        static_cast<int>((overallCgpa / 4.0) * 100);


    // Keep the value between 0 and 100

    if (performance > 100) {

        performance = 100;
    }

    if (performance < 0) {

        performance = 0;
    }


    ui->progressBar->setValue(performance);


    // Determine performance status and color

    QString performanceStatus;

    QString performanceColor;


    if (overallCgpa >= 3.50) {

        performanceStatus = "Excellent";

        performanceColor = "#16A34A";
    }
    else if (overallCgpa >= 3.00) {

        performanceStatus = "Good";

        performanceColor = "#2563EB";
    }
    else if (overallCgpa >= 2.00) {

        performanceStatus = "Average";

        performanceColor = "#F59E0B";
    }
    else {

        performanceStatus = "Needs Improvement";

        performanceColor = "#DC2626";
    }


    // Update performance title

    ui->progressTitleLabel->setText(
        "Academic Performance - "
        + performanceStatus
        + " ("
        + QString::number(overallCgpa, 'f', 2)
        + " / 4.00)"
        );


    // Update progress bar color

    ui->progressBar->setStyleSheet(
        QString(
            "QProgressBar {"
            "   border: 1px solid #CBD5E1;"
            "   border-radius: 6px;"
            "   background-color: #E2E8F0;"
            "   text-align: center;"
            "   color: #172033;"
            "   height: 18px;"
            "}"
            "QProgressBar::chunk {"
            "   background-color: %1;"
            "   border-radius: 5px;"
            "}"
            ).arg(performanceColor)
        );
}


// ==========================================
// LOAD SAVED TODO TASKS
// ==========================================

void MainWindow::loadTodoTasks()
{
    QSettings settings("UniCore", "UniCore");

    int taskCount =
        settings.beginReadArray("TodoTasks");


    for (int i = 0; i < taskCount; i++) {

        settings.setArrayIndex(i);

        QString task =
            settings.value("task").toString();

        if (!task.isEmpty()) {

            ui->todoListWidget->addItem(task);
        }
    }

    settings.endArray();
}


// ==========================================
// SAVE TODO TASKS
// ==========================================

void MainWindow::saveTodoTasks()
{
    QSettings settings("UniCore", "UniCore");

    settings.beginWriteArray(
        "TodoTasks",
        ui->todoListWidget->count()
        );


    for (
        int i = 0;
        i < ui->todoListWidget->count();
        i++
        ) {

        settings.setArrayIndex(i);

        settings.setValue(
            "task",
            ui->todoListWidget->item(i)->text()
            );
    }

    settings.endArray();
}


// ==========================================
// STUDENT PROFILE - LOAD INFORMATION
// ==========================================

void MainWindow::loadStudentProfile()
{
    QSettings settings("UniCore", "UniCore");

    settings.beginGroup("StudentProfile");


    // Load basic information

    ui->studentNameInput->setText(
        settings.value("Name").toString()
        );

    ui->studentIdInput->setText(
        settings.value("StudentID").toString()
        );

    ui->departmentInput->setText(
        settings.value("Department").toString()
        );

    ui->semesterInput->setText(
        settings.value("Semester").toString()
        );

    ui->batchInput->setText(
        settings.value("Batch").toString()
        );


    // Load contact information

    ui->emailInput->setText(
        settings.value("Email").toString()
        );

    ui->phoneInput->setText(
        settings.value("Phone").toString()
        );

    ui->addressInput->setText(
        settings.value("Address").toString()
        );


    // Load student description

    ui->studentDescriptionInput->setPlainText(
        settings.value("Description").toString()
        );


    settings.endGroup();


    // Return profile to read-only mode

    setStudentProfileEditing(false);
}


// ==========================================
// STUDENT PROFILE - SAVE INFORMATION
// ==========================================

void MainWindow::saveStudentProfile()
{
    QSettings settings("UniCore", "UniCore");

    settings.beginGroup("StudentProfile");


    // Save basic information

    settings.setValue(
        "Name",
        ui->studentNameInput->text().trimmed()
        );

    settings.setValue(
        "StudentID",
        ui->studentIdInput->text().trimmed()
        );

    settings.setValue(
        "Department",
        ui->departmentInput->text().trimmed()
        );

    settings.setValue(
        "Semester",
        ui->semesterInput->text().trimmed()
        );

    settings.setValue(
        "Batch",
        ui->batchInput->text().trimmed()
        );


    // Save contact information

    settings.setValue(
        "Email",
        ui->emailInput->text().trimmed()
        );

    settings.setValue(
        "Phone",
        ui->phoneInput->text().trimmed()
        );

    settings.setValue(
        "Address",
        ui->addressInput->text().trimmed()
        );


    // Save student description

    settings.setValue(
        "Description",
        ui->studentDescriptionInput->toPlainText().trimmed()
        );


    settings.endGroup();

    settings.sync();


    // Check whether saving was successful

    if (settings.status() != QSettings::NoError) {

        QMessageBox::warning(
            this,
            "Save Error",
            "Could not save student information."
            );
    }
}


// ==========================================
// STUDENT PROFILE - EDITING MODE
// ==========================================

void MainWindow::setStudentProfileEditing(bool editing)
{
    // Basic information

    ui->studentNameInput->setReadOnly(!editing);

    ui->studentIdInput->setReadOnly(!editing);

    ui->departmentInput->setReadOnly(!editing);

    ui->semesterInput->setReadOnly(!editing);

    ui->batchInput->setReadOnly(!editing);


    // Contact information

    ui->emailInput->setReadOnly(!editing);

    ui->phoneInput->setReadOnly(!editing);

    ui->addressInput->setReadOnly(!editing);


    // Description

    ui->studentDescriptionInput->setReadOnly(!editing);


    // Button visibility

    ui->editProfileButton->setVisible(!editing);

    ui->saveProfileButton->setVisible(editing);

    ui->cancelProfileButton->setVisible(editing);
}