#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STUDENTS 100
#define MAX_NAME_LENGTH 50
#define DATABASE_FILE "students.txt"

typedef struct {
    int i;
    int id;
    char name[MAX_NAME_LENGTH];
    char classe[50];
    char attendance;
    char course[50];

} Student;

void saveStudentToFile(Student student) {
    FILE *file = fopen(DATABASE_FILE, "a");
    if (file == NULL) {
        printf("failed.\n");
        return;
    }

    fprintf(file, "ID: %d\n", student.id);
    fprintf(file, "Name: %s\n", student.name);
    fprintf(file, "Class: %s\n", student.classe);
    fprintf(file, "Attendance: %c\n", student.attendance);
    fprintf(file, "course: %s\n", student.course);
    fprintf(file, "-----------------------\n");

    fclose(file);
}

void loadStudentsFromFile(Student students[], int *numStudents) {
    FILE *file = fopen(DATABASE_FILE, "r");
    if (file == NULL) {
        printf("welcome.\n");
        return;
    }

    while (fscanf(file, "ID: %d\nName: %s\nClass:  %s\nAttendance: %c\nCourse: %s\n-----------------------\n",
              &students[*numStudents].id, &students[*numStudents].name, &students[*numStudents].classe, &students[*numStudents].attendance, &students[*numStudents].course) == 5) {
    (*numStudents)++;
}


    fclose(file);
}

void addStudent(Student students[], int *numStudents) {
    if (*numStudents >= MAX_STUDENTS) {
        printf("Impossible to add more students.\n");
        return;
    }

    Student newStudent;
    printf("Student ID: ");
    scanf("%d", &newStudent.id);

    printf("Student Name: ");
    scanf("%s", &newStudent.name);

    printf("Student Class: ");
    scanf("%s", &newStudent.classe);


    //newStudent.attendance = 'A'; // Initialize attendance as absent

    students[*numStudents] = newStudent;
    (*numStudents)++;

    saveStudentToFile(newStudent);

    printf("Student added successfully.\n");
}

void markAttendance(Student students[], int numStudents) {
    int id;
    char status;
    char course[50];

    printf("Student ID: ");
    scanf("%d", &id);

    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (students[i].id == id) {
            found = 1;

            printf("Mark (P) for present or (A) for absent: ");
            scanf(" %c", &status);

            if (status != 'P' && status != 'A') {
                printf("Invalid status.\n");
                return;
            }

            students[i].attendance = status;

            printf("Enter the course: ");
            scanf("%s", course);
            strcpy(students[i].course, course);

            printf("%s is  %c in %s course.\n", students[i].name, status, course);

            // Update file with new attendance
            FILE *file = fopen(DATABASE_FILE, "r+");
            if (file == NULL) {
                printf("Error opening file.\n");
                return;
            }

            fseek(file, 0, SEEK_SET); // Move file pointer to the beginning

            // Rewrite the entire file with updated attendance
            for (int j = 0; j < numStudents; j++) {
                fprintf(file, "ID: %d\nName: %s\nClass: %s\nAttendance: %c\nCourse: %s\n-----------------------\n",
                        students[j].id, students[j].name, students[j].classe, students[j].attendance, students[j].course);
            }

            fclose(file);
            break; // No need to continue looping
        }
    }

    if (!found) {
        printf("Invalid Student ID.\n");
    }
}


void viewAttendanceList(Student students[], int numStudents) {
    printf("Attendance List:\n");
    printf("ID\tName\tClass\tPresence\tCourse\n");

    for (int i = 0; i < numStudents; i++) {
        printf("%d\t%s\t%s\t%c\t%s\n", students[i].id, students[i].name, students[i].classe, students[i].attendance, students[i].course);
    }
}

void searchStudent(Student students[], int numStudents) {
    char name[MAX_NAME_LENGTH];

    printf("Enter student name: ");
    scanf("%s", name);

    int found = 0;
    for (int i = 0; i < numStudents; i++) {
        if (strcmp(students[i].name, name) == 0) {
            printf("ID: %d, Name: %s, Class: %s, Presence: %c, Course: %s\n", students[i].id, students[i].name, students[i].classe, students[i].attendance, students[i].course);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Student not found.\n");
    }
}

int main() {
    Student students[MAX_STUDENTS];
    int numStudents = 0;
    int choice;

    loadStudentsFromFile(students, &numStudents);

    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Student\n");
        printf("2. Mark Presence/Absence\n");
        printf("3. View Attendance List\n");
        printf("4. Search Student\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addStudent(students, &numStudents);
                break;
            case 2:
                markAttendance(students, numStudents);
                break;
            case 3:
                viewAttendanceList(students, numStudents);
                break;
            case 4:
                searchStudent(students, numStudents);
                break;
            case 5:
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
