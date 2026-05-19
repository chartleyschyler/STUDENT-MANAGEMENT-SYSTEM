/*
 * ============================================================
 *  Student Management System
 *  Language : C
 *  Author   : [Your Full Name]
 *  Adm No   : [Your Admission Number]
 * ============================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* ── Constants ─────────────────────────────────────────── */
#define MAX_STUDENTS  100
#define NUM_SUBJECTS    3
#define FILENAME       "students.dat"

/* ── Grade thresholds ───────────────────────────────────── */
#define GRADE_A_MIN  70.0
#define GRADE_B_MIN  60.0
#define GRADE_C_MIN  50.0
#define GRADE_D_MIN  40.0

/* ============================================================
 *  STRUCT DEFINITIONS
 * ============================================================ */

/*
 * PersonalInfo – a nested struct that holds identity details.
 * Separating personal data from academic data keeps the design
 * clean and demonstrates the use of nested structures (bonus).
 */
typedef struct {
    char name[60];
    int  admissionNumber;
    int  age;
} PersonalInfo;

/*
 * Student – the main record struct.
 *   - embeds PersonalInfo (nested struct)
 *   - holds marks, computed average, and letter grade
 */
typedef struct {
    PersonalInfo info;                  /* nested struct          */
    float        marks[NUM_SUBJECTS];   /* marks for 3 subjects   */
    float        average;               /* auto-calculated        */
    char         grade;                 /* auto-determined        */
} Student;

/* ── Global storage ─────────────────────────────────────── */
Student students[MAX_STUDENTS];  /* array of structs  */
int     studentCount = 0;        /* live record count */

/* ============================================================
 *  HELPER / UTILITY FUNCTIONS
 * ============================================================ */

/* clearScreen – cross-platform screen clear */
void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

/* pressEnterToContinue – pause until user hits Enter */
void pressEnterToContinue(void) {
    printf("\n  Press ENTER to continue...");
    while (getchar() != '\n');   /* flush leftover newline */
    getchar();
}

/* printSeparator – prints a decorative line */
void printSeparator(void) {
    printf("  %-60s\n", "------------------------------------------------------------");
}

/* printHeader – prints the application banner */
void printHeader(void) {
    clearScreen();
    printf("\n");
    printSeparator();
    printf("  %*s\n", 40, "STUDENT MANAGEMENT SYSTEM");
    printSeparator();
    printf("\n");
}

/* ============================================================
 *  CORE LOGIC FUNCTIONS
 * ============================================================ */

/*
 * calculateGrade – takes a pointer to a Student and sets its
 * grade field based on the average.
 *
 * POINTER USAGE: We pass a pointer so the function can modify
 * the actual record in the array, not a local copy.
 */
void calculateGrade(Student *s) {
    if (s->average >= GRADE_A_MIN)       s->grade = 'A';
    else if (s->average >= GRADE_B_MIN)  s->grade = 'B';
    else if (s->average >= GRADE_C_MIN)  s->grade = 'C';
    else if (s->average >= GRADE_D_MIN)  s->grade = 'D';
    else                                  s->grade = 'E';
}

/*
 * calculateAverage – sums the three subject marks via a pointer
 * and stores the result back into the same Student record.
 */
void calculateAverage(Student *s) {
    float sum = 0.0f;
    int i;
    for (i = 0; i < NUM_SUBJECTS; i++) {
        sum += s->marks[i];
    }
    s->average = sum / NUM_SUBJECTS;
}

/*
 * findStudentByAdmNo – linear search through the array.
 * Returns the index of the matching record, or -1 if not found.
 */
int findStudentByAdmNo(int admNo) {
    int i;
    for (i = 0; i < studentCount; i++) {
        if (students[i].info.admissionNumber == admNo) {
            return i;
        }
    }
    return -1;
}

/*
 * admNoExists – returns 1 if an admission number is already
 * taken (used during add to prevent duplicates).
 */
int admNoExists(int admNo) {
    return findStudentByAdmNo(admNo) != -1;
}

/* printStudentRecord – pretty-prints a single Student record */
void printStudentRecord(const Student *s) {
    printSeparator();
    printf("  Name             : %s\n",   s->info.name);
    printf("  Admission Number : %d\n",   s->info.admissionNumber);
    printf("  Age              : %d\n",   s->info.age);
    printf("  Subject 1 Marks  : %.2f\n", s->marks[0]);
    printf("  Subject 2 Marks  : %.2f\n", s->marks[1]);
    printf("  Subject 3 Marks  : %.2f\n", s->marks[2]);
    printf("  Average Marks    : %.2f\n", s->average);
    printf("  Grade            : %c\n",   s->grade);
}

/* ============================================================
 *  MENU FEATURE FUNCTIONS
 * ============================================================ */

/*
 * addStudent – prompts the user for all fields, validates input,
 * then appends a new Student to the global array.
 */
void addStudent(void) {
    printHeader();
    printf("  [ ADD NEW STUDENT ]\n\n");

    if (studentCount >= MAX_STUDENTS) {
        printf("  ERROR: Maximum student capacity (%d) reached.\n", MAX_STUDENTS);
        pressEnterToContinue();
        return;
    }

    Student newStudent;   /* temporary record built on the stack */
    int i;

    /* ── Name ── */
    printf("  Enter Name            : ");
    scanf(" ");                               /* skip leading whitespace */
    fgets(newStudent.info.name, sizeof(newStudent.info.name), stdin);
    /* strip trailing newline from fgets */
    newStudent.info.name[strcspn(newStudent.info.name, "\n")] = '\0';

    if (strlen(newStudent.info.name) == 0) {
        printf("  ERROR: Name cannot be empty.\n");
        pressEnterToContinue();
        return;
    }

    /* ── Admission Number ── */
    printf("  Enter Admission No    : ");
    if (scanf("%d", &newStudent.info.admissionNumber) != 1 ||
        newStudent.info.admissionNumber <= 0) {
        printf("  ERROR: Invalid admission number.\n");
        pressEnterToContinue();
        return;
    }
    if (admNoExists(newStudent.info.admissionNumber)) {
        printf("  ERROR: Admission number %d already exists.\n",
               newStudent.info.admissionNumber);
        pressEnterToContinue();
        return;
    }

    /* ── Age ── */
    printf("  Enter Age             : ");
    if (scanf("%d", &newStudent.info.age) != 1 ||
        newStudent.info.age < 1 || newStudent.info.age > 120) {
        printf("  ERROR: Invalid age.\n");
        pressEnterToContinue();
        return;
    }

    /* ── Marks ── */
    for (i = 0; i < NUM_SUBJECTS; i++) {
        printf("  Enter Subject %d Marks : ", i + 1);
        if (scanf("%f", &newStudent.marks[i]) != 1 ||
            newStudent.marks[i] < 0 || newStudent.marks[i] > 100) {
            printf("  ERROR: Marks must be between 0 and 100.\n");
            pressEnterToContinue();
            return;
        }
    }

    /* ── Auto-calculations ── */
    calculateAverage(&newStudent);
    calculateGrade(&newStudent);

    /* ── Store ── */
    students[studentCount++] = newStudent;

    printf("\n  SUCCESS: Student \"%s\" added. (Record #%d)\n",
           newStudent.info.name, studentCount);
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * displayAllStudents – iterates the array and prints every record.
 */
void displayAllStudents(void) {
    printHeader();
    printf("  [ ALL STUDENT RECORDS ]  (Total: %d)\n", studentCount);

    if (studentCount == 0) {
        printf("\n  No records found.\n");
        pressEnterToContinue();
        return;
    }

    int i;
    for (i = 0; i < studentCount; i++) {
        printf("\n  Record %d of %d", i + 1, studentCount);
        printStudentRecord(&students[i]);
    }
    printSeparator();
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * searchStudent – asks for an admission number and displays the
 * matching record, or an error if not found.
 */
void searchStudent(void) {
    printHeader();
    printf("  [ SEARCH STUDENT ]\n\n");

    int admNo;
    printf("  Enter Admission Number to search: ");
    if (scanf("%d", &admNo) != 1) {
        printf("  ERROR: Invalid input.\n");
        pressEnterToContinue();
        return;
    }

    int idx = findStudentByAdmNo(admNo);
    if (idx == -1) {
        printf("\n  No student found with admission number %d.\n", admNo);
    } else {
        printf("\n  Student found:");
        printStudentRecord(&students[idx]);
        printSeparator();
    }
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * updateStudent – finds a student by admission number, then lets
 * the user enter new marks; recalculates average and grade.
 *
 * POINTER USAGE: `target` points directly into the global array,
 * so all writes go straight to the stored record.
 */
void updateStudent(void) {
    printHeader();
    printf("  [ UPDATE STUDENT MARKS ]\n\n");

    int admNo;
    printf("  Enter Admission Number to update: ");
    if (scanf("%d", &admNo) != 1) {
        printf("  ERROR: Invalid input.\n");
        pressEnterToContinue();
        return;
    }

    int idx = findStudentByAdmNo(admNo);
    if (idx == -1) {
        printf("\n  No student found with admission number %d.\n", admNo);
        pressEnterToContinue();
        return;
    }

    Student *target = &students[idx];   /* pointer to the actual record */

    printf("\n  Updating marks for: %s\n", target->info.name);

    int i;
    for (i = 0; i < NUM_SUBJECTS; i++) {
        printf("  New Subject %d Marks (current %.2f): ", i + 1, target->marks[i]);
        float newMark;
        if (scanf("%f", &newMark) != 1 ||
            newMark < 0 || newMark > 100) {
            printf("  ERROR: Marks must be between 0 and 100. Update aborted.\n");
            pressEnterToContinue();
            return;
        }
        target->marks[i] = newMark;
    }

    /* Recalculate using pointers */
    calculateAverage(target);
    calculateGrade(target);

    printf("\n  SUCCESS: Marks updated.\n");
    printStudentRecord(target);
    printSeparator();
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * bestStudent – scans all records to find the highest average.
 *
 * POINTER USAGE: `best` is a pointer that tracks which element
 * in the array currently holds the top average.
 */
void bestStudent(void) {
    printHeader();
    printf("  [ BEST PERFORMING STUDENT ]\n\n");

    if (studentCount == 0) {
        printf("  No records available.\n");
        pressEnterToContinue();
        return;
    }

    Student *best = &students[0];   /* pointer to current best */
    int i;
    for (i = 1; i < studentCount; i++) {
        if (students[i].average > best->average) {
            best = &students[i];    /* update pointer, not a copy */
        }
    }

    printf("  Top student is:\n");
    printStudentRecord(best);
    printSeparator();
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */
/*  ── BONUS FEATURES ──────────────────────────────────────── */
/* ─────────────────────────────────────────────────────────── */

/*
 * deleteStudent – removes a record by shifting the array left,
 * keeping elements contiguous.
 */
void deleteStudent(void) {
    printHeader();
    printf("  [ DELETE STUDENT ]\n\n");

    int admNo;
    printf("  Enter Admission Number to delete: ");
    if (scanf("%d", &admNo) != 1) {
        printf("  ERROR: Invalid input.\n");
        pressEnterToContinue();
        return;
    }

    int idx = findStudentByAdmNo(admNo);
    if (idx == -1) {
        printf("\n  No student found with admission number %d.\n", admNo);
        pressEnterToContinue();
        return;
    }

    printf("\n  Are you sure you want to delete \"%s\"? (y/n): ",
           students[idx].info.name);
    char confirm;
    scanf(" %c", &confirm);
    if (confirm != 'y' && confirm != 'Y') {
        printf("  Deletion cancelled.\n");
        pressEnterToContinue();
        return;
    }

    /* Shift remaining elements left by one position */
    int i;
    for (i = idx; i < studentCount - 1; i++) {
        students[i] = students[i + 1];
    }
    studentCount--;

    printf("\n  SUCCESS: Record deleted. Remaining students: %d\n", studentCount);
    pressEnterToContinue();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * sortStudentsByAverage – simple bubble sort (descending order).
 * Swaps whole Student structs in the array.
 */
void sortStudentsByAverage(void) {
    printHeader();
    printf("  [ SORT BY AVERAGE MARKS (Descending) ]\n\n");

    if (studentCount < 2) {
        printf("  Not enough records to sort.\n");
        pressEnterToContinue();
        return;
    }

    int i, j;
    Student temp;
    for (i = 0; i < studentCount - 1; i++) {
        for (j = 0; j < studentCount - 1 - i; j++) {
            if (students[j].average < students[j + 1].average) {
                temp           = students[j];
                students[j]     = students[j + 1];
                students[j + 1] = temp;
            }
        }
    }

    printf("  Records sorted by average marks (highest first).\n");
    pressEnterToContinue();

    /* Display the freshly-sorted list */
    displayAllStudents();
}

/* ─────────────────────────────────────────────────────────── */

/*
 * saveToFile – writes all records to a binary file so data
 * persists between program runs.
 */
void saveToFile(void) {
    FILE *fp = fopen(FILENAME, "wb");
    if (fp == NULL) {
        printf("  ERROR: Could not open file for writing.\n");
        return;
    }
    fwrite(&studentCount, sizeof(int),     1,            fp);
    fwrite(students,      sizeof(Student), studentCount, fp);
    fclose(fp);
    printf("  Records saved to \"%s\".\n", FILENAME);
}

/*
 * loadFromFile – reads records from the binary file on startup.
 */
void loadFromFile(void) {
    FILE *fp = fopen(FILENAME, "rb");
    if (fp == NULL) return;   /* file does not yet exist – that is fine */

    int count = 0;
    fread(&count, sizeof(int), 1, fp);

    if (count > 0 && count <= MAX_STUDENTS) {
        fread(students, sizeof(Student), count, fp);
        studentCount = count;
        printf("  Loaded %d record(s) from \"%s\".\n", studentCount, FILENAME);
    }
    fclose(fp);
}

/* ============================================================
 *  MAIN MENU
 * ============================================================ */

void showMenu(void) {
    printHeader();
    printf("  MAIN MENU\n\n");
    printf("    1.  Add Student\n");
    printf("    2.  Display All Students\n");
    printf("    3.  Search Student\n");
    printf("    4.  Update Student Marks\n");
    printf("    5.  Best Performing Student\n");
    printf("    6.  Delete Student\n");
    printf("    7.  Sort by Average Marks\n");
    printf("    8.  Save Records to File\n");
    printf("    0.  Exit\n\n");
    printSeparator();
    printf("  Enter your choice: ");
}

/* ============================================================
 *  ENTRY POINT
 * ============================================================ */

int main(void) {
    loadFromFile();   /* attempt to restore any previously saved data */

    int choice;

    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            /* non-integer input – flush and retry */
            while (getchar() != '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:  addStudent();          break;
            case 2:  displayAllStudents();  break;
            case 3:  searchStudent();       break;
            case 4:  updateStudent();       break;
            case 5:  bestStudent();         break;
            case 6:  deleteStudent();       break;
            case 7:  sortStudentsByAverage(); break;
            case 8:
                saveToFile();
                pressEnterToContinue();
                break;
            case 0:
                saveToFile();   /* auto-save on clean exit */
                printHeader();
                printf("  Goodbye! All records have been saved.\n\n");
                break;
            default:
                printf("\n  Invalid choice. Please try again.\n");
                pressEnterToContinue();
                break;
        }
    } while (choice != 0);

    return 0;
}
