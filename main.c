#include <stdio.h>
#include <string.h>

#define MAX_STUDENTS 100

// Structure definition
struct Student {
    char name[50];
    int admissionNumber;
    int age;
    float marks[3];
    float average;
    char grade;
};

// Global array of students
struct Student students[MAX_STUDENTS];
int studentCount = 0;

// Function prototypes
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void bestStudent();
char calculateGrade(float average);

int main() {
    int choice;
    do {
        printf("\n===== Student Management System =====\n");
        printf("1. Add Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Best Performing Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: bestStudent(); break;
            case 6: printf("Exiting program...\n"); break;
            default: printf("Invalid choice! Try again.\n");
        }
    } while(choice != 6);

    return 0;
}

// Function to add student
void addStudent() {
    if(studentCount >= MAX_STUDENTS) {
        printf("Student list full!\n");
        return;
    }

    struct Student *s = &students[studentCount]; // pointer usage
    printf("Enter name: ");
    scanf("%s", s->name);
    printf("Enter admission number: ");
    scanf("%d", &s->admissionNumber);
    printf("Enter age: ");
    scanf("%d", &s->age);

    float sum = 0;
    for(int i=0; i<3; i++) {
        printf("Enter marks for subject %d: ", i+1);
        scanf("%f", &s->marks[i]);
        sum += s->marks[i];
    }

    s->average = sum / 3.0;
    s->grade = calculateGrade(s->average);

    studentCount++;
    printf("Student added successfully!\n");
}

// Function to display all students
void displayStudents() {
    if(studentCount == 0) {
        printf("No students to display.\n");
        return;
    }

    for(int i=0; i<studentCount; i++) {
        struct Student s = students[i];
        printf("\nName: %s\nAdmission Number: %d\nAge: %d\nAverage: %.2f\nGrade: %c\n",
               s.name, s.admissionNumber, s.age, s.average, s.grade);
    }
}

// Function to search student
void searchStudent() {
    int adm;
    printf("Enter admission number to search: ");
    scanf("%d", &adm);

    for(int i=0; i<studentCount; i++) {
        if(students[i].admissionNumber == adm) {
            printf("\nName: %s\nAdmission Number: %d\nAge: %d\nAverage: %.2f\nGrade: %c\n",
                   students[i].name, students[i].admissionNumber, students[i].age,
                   students[i].average, students[i].grade);
            return;
        }
    }
    printf("Student not found!\n");
}

// Function to update student marks
void updateStudent() {
    int adm;
    printf("Enter admission number to update: ");
    scanf("%d", &adm);

    for(int i=0; i<studentCount; i++) {
        if(students[i].admissionNumber == adm) {
            float sum = 0;
            for(int j=0; j<3; j++) {
                printf("Enter new marks for subject %d: ", j+1);
                scanf("%f", &students[i].marks[j]);
                sum += students[i].marks[j];
            }
            students[i].average = sum / 3.0;
            students[i].grade = calculateGrade(students[i].average);
            printf("Student record updated successfully!\n");
            return;
        }
    }
    printf("Student not found!\n");
}

// Function to display best performing student
void bestStudent() {
    if(studentCount == 0) {
        printf("No students available.\n");
        return;
    }

    int bestIndex = 0;
    for(int i=1; i<studentCount; i++) {
        if(students[i].average > students[bestIndex].average) {
            bestIndex = i;
        }
    }

    struct Student s = students[bestIndex];
    printf("\nBest Performing Student:\nName: %s\nAdmission Number: %d\nAge: %d\nAverage: %.2f\nGrade: %c\n",
           s.name, s.admissionNumber, s.age, s.average, s.grade);
}

// Function to calculate grade
char calculateGrade(float average) {
    if(average >= 70) return 'A';
    else if(average >= 60) return 'B';
    else if(average >= 50) return 'C';
    else if(average >= 40) return 'D';
    else return 'E';
}
