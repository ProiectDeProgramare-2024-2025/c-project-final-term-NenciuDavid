#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "history.txt"

typedef struct {
    char name[50];
    int price, time;
} Offer;

Offer offers[] = {
    {"Consultation", 50, 60},
    {"Surgery", 500, 60},
    {"Check-up", 30, 60},
    {"Filling", 20, 60},
};

typedef struct {
    int day, month, year, hour, minutes;
    char operation[50];
} Appointment;

Appointment history[10];
int appointmentCount = 0;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void displayMenu(){
    printf("\ndadadadaMain Menu\n");
    printf("1. View offers\n");
    printf("2. Check Availability\n");
    printf("3. View Appointments History\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

void viewOffers(){
    clearScreen();
    printf("Available offers:\n");
    int numOffers = sizeof(offers) / sizeof(offers[0]);
    for (int i = 0; i < numOffers; i++) {
        printf("%d. %s - \033[0;32m%d lei\033[0m - \033[0;31m%d min\033[0m\n", i + 1, offers[i].name, offers[i].price, offers[i].time);
    }
    printf("\nPress any key to return to the menu...\n");
    getchar(); getchar();
    clearScreen();
}

void saveAppointments() {
    FILE *file = fopen(FILE_NAME, "a");
    if(file == NULL) {
        printf("Error opening file for writing in saveAppointments.\n");
        return;
    }
    fprintf(file, "%d %d %d %d %d %s\n", history[appointmentCount].day, history[appointmentCount].month, history[appointmentCount].year, history[appointmentCount].hour, history[appointmentCount].minutes, history[appointmentCount].operation);
    fclose(file);
}

void loadAppointments() {
    FILE *file = fopen(FILE_NAME, "r");
    if(file == NULL) {
        printf("Error in opening file for reading in loadAppointments.\n");
        return;
    }
    appointmentCount = 0;
    while(fscanf(file, "%d %d %d %d %d %[^\n]", 
        &history[appointmentCount].day, 
        &history[appointmentCount].month, 
        &history[appointmentCount].year, 
        &history[appointmentCount].hour, 
        &history[appointmentCount].minutes, 
        history[appointmentCount].operation) != EOF)
    {
        appointmentCount++;
    }
    fclose(file);
}

int isDateAvailable(int day, int month, int year, int hour, int minutes) {
    loadAppointments();
    for (int i = 0; i < appointmentCount; i++) {
        if (history[i].day == day && history[i].month == month && history[i].year == year && history[i].hour == hour && history[i].minutes == minutes) {
            return 0;
        }
    }
    return 1;
}

void scheduleAppointment(int day, int month, int year, int hour, int minutes) {
    clearScreen();
    if (appointmentCount >= 10) {
        printf("Appointment list is full!\n");
        return;
    }
    
    int choice;
    int numOffers = sizeof(offers) / sizeof(offers[0]);
    for (int i = 0; i < numOffers; i++) {
        printf("%d. %s - %d lei - %d min\n", i + 1, offers[i].name, offers[i].price, offers[i].time);
    }
    printf("Enter choice: ");
    scanf("%d", &choice);
    if (choice < 1 || choice > sizeof(offers) / sizeof(offers[0])) {
        printf("Invalid choice!\n");
        return;
    }
    
    strcpy(history[appointmentCount].operation, offers[choice - 1].name);
    history[appointmentCount].day = day;
    history[appointmentCount].month = month;
    history[appointmentCount].year = year;
    history[appointmentCount].hour = hour;
    history[appointmentCount].minutes = minutes;
    printf("Appointment scheduled successfully!\n\nPress any key to return to the menu...\n");
    saveAppointments();
    appointmentCount++;
    getchar(); getchar();
    clearScreen();
}

int isDateValid(const char *date)
{
    int dd, mm, yy;
    dd = (date[0]-'0')*10 + (date[1]-'0');
    mm = (date[3]-'0')*10 + (date[4]-'0');
    yy = (date[6]-'0')*1000 + (date[7]-'0')*100 + (date[8]-'0')*10 + (date[9]-'0');
    if(date[2]!='/' && date[5]!='/')
        return 0;
    if (dd < 1 || dd > 31 || mm < 1 || mm > 12 || yy < 2025)
        return 0;
    return 1;
}

int isTimeValid(const char *time)
{
    int h, min;
    h = (time[0]-'0')*10 + (time[1]-'0');
    min = (time[3]-'0')*10 + (time[4]-'0');
    if (time[2]!=':')
        return 0;
    if (h < 0 || h > 23 || min < 0 || min > 59)
        return 0;
    return 1;
}

void checkAvailability(){
    clearScreen();
    int day, month, year, hour, minutes;
    char date[10], time[10];
    printf("Enter the date (dd/mm/yy): ");
    scanf("%s", date);
    if(isDateValid(date) == 1)
    {
        day = (date[0]-'0')*10 + (date[1]-'0');
        month = (date[3]-'0')*10 + (date[4]-'0');
        year = (date[6]-'0')*1000 + (date[7]-'0')*100 + (date[8]-'0')*10 + (date[9]-'0');
        printf("Enter the time (hh:mm): ");
        scanf("%s", time);
        if(isTimeValid(time) == 1)
        {
            hour = (time[0]-'0')*10 + (time[1]-'0');
            minutes = (time[3]-'0')*10 + (time[4]-'0');
            if (isDateAvailable(day, month, year, hour, minutes))
            {
                printf("Slot available for %d/%d/%d, at %d:%d!\n\nPress 1 to schedule an appointment or 2 to go back to the menu\n", day, month, year, hour, minutes);
                int answear;
                scanf("%d", &answear);
                if(answear == 1)
                    scheduleAppointment(day, month, year, hour, minutes);
                else
                {
                    clearScreen();
                }
            }
            else
            {
                printf("No available slots on %d/%d/%d, at %d:%d\n", day, month, year, hour, minutes);
                printf("\nPress any key to return to the menu...\n");
                getchar(); getchar();
                clearScreen();
            }
        }
        else
        {
            printf("Wrong time format.");
            printf("\nPress any key to return to the menu...\n");
            getchar(); getchar();
            clearScreen();
        }
    }
    else
    {
        printf("Wrong date format.");
        printf("\nPress any key to return to the menu...\n");
        getchar(); getchar();
        clearScreen();
    }
}

void viewHistory() {
    clearScreen();
    loadAppointments();
    if (appointmentCount == 0) {
        printf("No appointments scheduled.\n");
    } else {
        printf("Appointment History:\n");
        for (int i = 0; i < appointmentCount; i++) {
            printf("%d. %s on %d/%d/%d at %d:%d\n", i + 1, history[i].operation, history[i].day, history[i].month, history[i].year, history[i].hour, history[i].minutes);
        }
    }
    printf("\nPress any key to return to the menu...\n");
    getchar(); getchar();
    clearScreen();
}

int main() {
    int choice;
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        clearScreen();
        
        switch (choice) {
            case 1: viewOffers(); break;
            case 2: checkAvailability(); break;
            case 3: viewHistory(); break;
            case 4: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }
}
