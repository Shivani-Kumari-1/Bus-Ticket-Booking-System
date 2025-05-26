/*
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 Project Name : Bus Ticket Booking System
 Author       : Shivani Kumari
 Created on   : 25/05/2025 
 Description  : A simple console-based bus ticket booking system using C
 Objective    : To implement a booking and cancellation system for bus ticketing.
 :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<time.h>
#define NUM_BUSES 3
#define CAPACITY 30
#define MAX_BOOKINGS (NUM_BUSES * CAPACITY)

typedef struct P
{
    int busNum;
    int seatNum;
    char name[30];
    char id[10];
}Passenger;

Passenger psngr[MAX_BOOKINGS];

//Helper function used in main()
//options() : This function displays a menu to the user
void options()
{
    printf("\n* * * * * * * * * * * * * * * * * * * * * * * * * *");
    printf("\n*  ::::::::::BUS TICKET BOOKING SYSTEM::::::::::: *");
    printf("\n*  1. Book tickets and Assign seat Numbers.       *");
    printf("\n*  2. Display available seats.                    *");
    printf("\n*  3. Cancel Bookings.                            *");
    printf("\n*  4. View passenger list.                        *");
    printf("\n*  5. Search By Passenger ID or Name.             *");
    printf("\n*  6. Exit.                                       *");
    printf("\n*  :::::::::::::::::::::::::::::::::::::::::::::: *");
    printf("\n* * * * * * * * * * * * * * * * * * * * * * * * * *");
}

//Helper function used in bookTicket()
//showBuses() : It shows real-time available seat capacity
int bookingCount = 0;
void showBuses()
{
    int remaining[3] = {CAPACITY,CAPACITY,CAPACITY}; 
    for(int i=0;i<bookingCount;i++) 
    {
        if(psngr[i].busNum == 111)
        {  
            remaining[0]--;  
        }
        else if(psngr[i].busNum == 112)
        {  
            remaining[1]--;  
        }
        else if(psngr[i].busNum == 113)
        {  
            remaining[2]--;  
        }
    }
    printf("\n::::::::::::::::::::::::::::Available Buses:::::::::::::::::::::::::::::::::");
    printf("\nBus No: 111 | Route: Patna to Prayagraj | Time: 05:00 AM | Seat_Capacity: %d",remaining[0]);
    printf("\nBus No: 112 | Route: Patna to Varanasi  | Time: 05:00 AM | Seat_Capacity: %d",remaining[1]);
    printf("\nBus No: 113 | Route: Patna to Delhi     | Time: 05:00 AM | Seat_Capacity: %d",remaining[2]);
    printf("\n::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
}

//Helper function used in bookTicket()
//saveBookingsToFile(): Save all bookings to file 
void saveBookingsToFile()
{
    FILE *fptr = fopen("bookings.txt","w");
    if(fptr == NULL)
    {  
        printf("File write error!\n");  
        return; 
    }
    for(int i=0;i<bookingCount;i++)
    {  
        fprintf(fptr,"%d %d %s %s\n",psngr[i].busNum,psngr[i].seatNum,psngr[i].name,psngr[i].id);  
    }
    fclose(fptr);
}

//Helper function used in bookTicket(), searchPassenger() and loadBookingsFromFile()
//toUpperCase(): To convert string to uppercase
void toUpperCase(char *str)
{
    for(int i=0;str[i];i++)
    {  
        str[i] = toupper((unsigned char)str[i]); 
    }
}

//showAvailableSeats(): Marks all booked seats and prints only the available ones.
void showAvailableSeats()
{
    int bus;
    //The array size is (CAPACITY + 1) so that seat number 1 to CAPACITY can be used directly as indices. Initially, all seats are set to 0(not booked).
    int booked[CAPACITY + 1] = {0};
    printf("\nEnter Bus Number (111,112 or 113) : ");
    scanf("%d",&bus);

    //check if the entered bus number is valid
    if(bus != 111 && bus != 112 && bus != 113)
    {  
        printf("Invalid Bus number!"); 
        return;  
    }

    //Mark the seats that are already booked for the selected bus
    for(int i=0;i<bookingCount;i++)
    {
        if(psngr[i].busNum == bus)
        {
            int seat = psngr[i].seatNum;
            if(seat >= 1 && seat <= CAPACITY)
            {  
                booked[seat] = 1;//Mark this seat as booked
            } 
        }
    }

    //display available seats
    printf("\n::::Available Seats in Bus Number %d::::\n",bus);
    int availableCount = 0;
    for(int i=1;i<=CAPACITY;i++)//Loop through all seats and print the ones that are not booked
    {
        if(!booked[i])
        {
            printf("%3d ",i);
            availableCount++;
            if(availableCount % 10 == 0)
            {  
                printf("\n");  
            }
        }
    }
    //if no seats are available, print a message
    if(availableCount == 0)
    {   
        printf("No available seats.\n");  
    }
    printf("\n::::::::::::::::::::::::::::::::::::::\n");
}

//Helper function used in bookTicket()
//generateID(): this function will generate alphanumeric unique id
void generateID(char *id, int bookcount)
{
    int randomNum = rand() % 90 + 10;
    sprintf(id,"BR%03d%02d",bookingCount+1,randomNum);
}

//bookTicket(): Takes bus number, seat number, name and ID from the user, converts name and ID to uppercase,
//checks if the seat is already booked, and if not, adds the booking and saves it.
void bookTicket()
{
    Passenger p;
    int seatTaken = 0;

    showBuses();

    printf("\nEnter Bus Number (111,112,113) : ");
    scanf("%d",&p.busNum);
    if(p.busNum != 111 && p.busNum != 112 && p.busNum != 113)
    {  
        printf("Invalid Bus Number!\n");  
        return;  
    }
    //showAvailableSeats();
    printf("\nEnter Seat Number (1 to %d) : ",CAPACITY);
    scanf("%d",&p.seatNum);
    if(p.seatNum < 1 || p.seatNum > CAPACITY)
    {  
        printf("Invalid Seat Number!\n");  
        return; 
    }

    getchar();

    printf("Enter Passenger Name : ");
    fgets(p.name,sizeof(p.name),stdin);
    p.name[strcspn(p.name,"\n")] = '\0';//to convert newline character into NULL character
    toUpperCase(p.name);

    generateID(p.id,p.busNum);
    toUpperCase(p.id);

    //check if seat is already booked
    for(int i=0;i<bookingCount;i++)
    {
        if(psngr[i].busNum == p.busNum && psngr[i].seatNum == p.seatNum)
        {  
            seatTaken = 1; 
            break;
        }
    }
    if(seatTaken)
    { 
        printf("Seat %d on Bus %d is already booked.\n",p.seatNum,p.busNum); //if booked then print error message
    }
    else//if not booked then add it into psngr array and save it 
    {
        psngr[bookingCount++] = p;
        saveBookingsToFile();
        printf("\nBooking successful! Seat Number (%d) on Bus Number (%d) booked for (%s) (ID: %s).\n",p.seatNum,p.busNum,p.name,p.id);
    }
}

//cancelBooking(): Takes bus number and seat number, finds the booking, removes it, and updates the file
void cancelBooking()
{
    int busNum,seatNum,found=0;
    printf("\nEnter bus number to cancel booking : ");
    scanf("%d",&busNum);
    printf("\nEnter seat number to cancel booking : ");
    scanf("%d",&seatNum);

    for(int i=0;i<bookingCount;i++)
    {
        if(psngr[i].busNum == busNum && psngr[i].seatNum == seatNum)
        {
            found = 1;
            for(int j=i;j<bookingCount-1;j++)
            {  
                psngr[j] = psngr[j+1];  
            }
            bookingCount--;
            printf("Booking for Seat (%d) on Bus (%d) has been cancelled.\n",seatNum,busNum);
            saveBookingsToFile();//update the file
            break;
        }
    }
    if(!found)
    {  
        printf("No booking found for Seat (%d) on Bus (%d).\n",seatNum,busNum);  
    }
}

//Helper function used in viewAllBookings()
//seatNumSort(): Sort passengers list by seat number 
void seatNumSort(Passenger arr[],int n)
{
    for(int i=0;i<n-1;i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            if(arr[j].seatNum > arr[j+1].seatNum)
            {
                Passenger temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

//viewAllBookings(): display all current Buses tickets booking details 
//sorts these bookings by seat number in ascending order using the helper function seatNumSort()
void viewAllBookings()
{
    if (bookingCount == 0)
    {
        printf("\nNo bookings available.\n");
        return;
    }
    int busNum;
    printf("\nEnter Bus number (111,112,113): ");
    if (scanf("%d", &busNum) != 1)  
    {
        printf("Invalid input.\n");
        while (getchar() != '\n'); 
        return;
    }

    if (busNum != 111 && busNum != 112 && busNum != 113)
    {
        printf("Invalid bus number\n");
        return;
    }

    printf("\nPassenger list for Bus No : %d\n", busNum);

    int count = 0;
    Passenger bookings[MAX_BOOKINGS];

    for (int i = 0; i < bookingCount; i++)
    {
        if (psngr[i].busNum == busNum)
        {
            bookings[count++] = psngr[i];
        }
    }

    if (count > 0)
    {
        seatNumSort(bookings, count);  

        for (int i = 0; i < count; i++)
        {
            printf("Seat No: %d | Name: %s | ID: %s\n",
                   bookings[i].seatNum, bookings[i].name, bookings[i].id);
        }
    }
    else
    {
        printf("No bookings found for Bus %d.\n", busNum);
    }
}

//searchPassenger(): converts the search term to uppercase and compares it with stored data(which is also uppercase)
void searchPassenger()
{
    char searchPsngr[30];
    int found = 0;

    printf("\nEnter Name or ID to search: ");
    getchar();
    fgets(searchPsngr, sizeof(searchPsngr), stdin);
    searchPsngr[strcspn(searchPsngr, "\n")] = '\0';
    toUpperCase(searchPsngr);

    for (int i = 0; i < bookingCount; i++) //loop through all bookings to find a match
    {   //check if the search term is present in the passenger's name or ID
        if (strstr(psngr[i].name, searchPsngr) != NULL || strstr(psngr[i].id, searchPsngr) != NULL) 
        {   //if matching found
            printf("\nFound: Bus %d | Seat %d | Name: %s | ID: %s\n",psngr[i].busNum,psngr[i].seatNum,psngr[i].name,psngr[i].id);
            found = 1;//set the flag to indicate a match was found
        }
    }

    if (!found) //if no matching passenger was found
    {
        printf("No passenger found with the given name or ID.\n");
    }
}

//loadBookingsFromFile(): When the program starts, it loads all previous bookings from a (bookings.txt) file.
void loadBookingsFromFile()
{
    FILE *fptr = fopen("bookings.txt", "r");
    bookingCount = 0;

    if (fptr == NULL) 
    {
        printf("bookings.txt file not found.\n");
        return;
    }

    char str[200];//buffer to store each line from the file
    while (fgets(str, sizeof(str), fptr)) //read the file line by line
    {
        str[strcspn(str, "\n")] = '\0';

        char *token = strtok(str, " ");//split the line into tokens using space as a delimiter
        if (token == NULL) continue;
        int busNum = atoi(token);//convert first token to bus number

        token = strtok(NULL, " ");
        if (token == NULL) continue;
        int seatNum = atoi(token);//convert second token to seat number

        char *namePlusId = strtok(NULL, "");//the remaining part of the line contains nane and ID.
        if (namePlusId == NULL) continue;

        char *lastSpace = strrchr(namePlusId, ' ');//find the last space in the remaining string to separate name and ID
        if (lastSpace == NULL) continue;

        *lastSpace = '\0';
        char *name = namePlusId;//NAME IS THE PART BEFORE THE LAST SPACE
        char *id = lastSpace + 1;//ID IS THE PART AFTER THE LAST SPACE

        //Store the extracted data into the passengers array
        psngr[bookingCount].busNum = busNum;
        psngr[bookingCount].seatNum = seatNum;
        strncpy(psngr[bookingCount].name, name, sizeof(psngr[bookingCount].name) - 1);
        strncpy(psngr[bookingCount].id, id, sizeof(psngr[bookingCount].id) - 1);

        //Ensure strings are null terminated
        psngr[bookingCount].name[sizeof(psngr[bookingCount].name) - 1] = '\0';
        psngr[bookingCount].id[sizeof(psngr[bookingCount].id) - 1] = '\0';

        //convert name and ID to uppercase for consistency
        toUpperCase(psngr[bookingCount].name);
        toUpperCase(psngr[bookingCount].id);

        //printf("Loaded: Bus %d | Seat %d | Name: %s | ID: %s\n", busNum, seatNum, psngr[bookingCount].name, psngr[bookingCount].id);

        bookingCount++;
        if (bookingCount >= MAX_BOOKINGS) //if maximum bookings reached, stop loading
        {
            printf("Maximum booking limit reached!\n");
            break;
        }
    }

    //printf("Total bookings loaded: %d\n", bookingCount);
    fclose(fptr);
}

//clearInputBuffer(): 
void clearInputBuffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}
//exitprg():
int exitprg()
{
    char c;
        printf("Do you really want to exit? (Y/N): ");
        scanf(" %c", &c);
        clearInputBuffer();
        if (toupper(c) == 'Y')
        {
            printf("Thank you for using the system.\nHappy Journey :)\n");
            saveBookingsToFile();
            return 1;
        }
        else if (toupper(c) == 'N')
        {
            return 0;
        }
        else
        {
            printf("Invalid input! Please enter Y or N.\n");
        }
}

//main() : The entry point of the program, Loads bookings from file, 
//then repeatedly shows the menu and calls the right function based on user choice.
int main()
{
    srand(time(NULL));
    loadBookingsFromFile();
    int choice;
    while(1)
    {
        options();
        printf("\n\nEnter your choice : ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input! Please enter a number.\n");
            clearInputBuffer(); 
            continue;
        }
        switch(choice)
        {
            case 1: bookTicket(); break;
            case 2: showAvailableSeats(); break;
            case 3: cancelBooking(); break;
            case 4: viewAllBookings(); break;
            case 5: searchPassenger(); break;
            case 6: if(exitprg()) return 0; break;
            default:printf("Invalid Choice! Please try again.\n");
        }
    }
    return 0;
}

 