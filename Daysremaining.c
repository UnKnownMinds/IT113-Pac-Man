#include <stdio.h>
#include <math.h>

struct Date{
    int day;
    int month;
    int year;
};

struct Date Scandate();
int Daysdifference(struct Date d1,struct Date d2);

int main(){
    printf("Date-1 : \n");
    struct Date date1;
    date1 = Scandate();

    printf("Date - 2 : \n");
    struct Date date2;
    date2 = Scandate();

    int Days;
    if(date2.year > date1.year || (date2.year == date1.year && date2.month > date1.month) || (date2.year == date1.year && date2.month == date1.month && date2.day > date1.day)){
        Days = Daysdifference(date1,date2);
    }else{
        Days = Daysdifference(date2,date1);
    }
    
    printf("Difference in days : %d",Days);
    
    return 0;
}

int checkLeapYear(int Year){
    if(Year%4 == 0){
        if(Year%100 != 0 || Year%400 == 0){
            return 1;
        }
    }
    return 0;
}

struct Date Scandate(){
    struct Date date;
    char ch1,ch2,ch3;

    int ValidDate = 0;
    while(ValidDate == 0){
        printf("Enter date in (dd mm yyyy) format : ");
        if(scanf("%d%c%d%c%d%c",&date.day,&ch1,&date.month,&ch2,&date.year,&ch3) != 6 || ch1 != ' ' || ch2 != ' ' || ch3 != '\n'){
            printf("Invalid Inpiut\nEnter a valid Input\n\n");
            continue;
        }
        if(date.day<0 || date.month<0 || date.year<0 || date.month>12 || date.day > 31){
            printf("Invalid Inpiut\nEnter a valid Input\n\n");
            continue;
        }
        if(date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11){
            if(date.day > 30){
                printf("Invalid Inpiut\nEnter a valid Input\n\n");
                continue;
            }
        }
        if(checkLeapYear(date.year) && date.month == 2){
            if(date.day > 29){
                printf("Invalid Inpiut\nEnter a valid Input\n\n");
                continue;
            }else if(date.day > 28){
                printf("Invalid Inpiut\nEnter a valid Input\n\n");
                continue;
            }
        }
        ValidDate = 1;
    }
    return date;
}

int Daysdifference(struct Date d1,struct Date d2){  //d2>d1
    int Diff = 0;
    int Yeardiff = d2.year - d1.year;
    int Monthdiff = d2.month - d1.month;
    int Daydiff = d2.day - d1.day;

    while(Daydiff != 0 || Monthdiff != 0 || Yeardiff != 0){
        Yeardiff = d2.year - d1.year;
        Monthdiff = d2.month - d1.month;
        Daydiff = d2.day - d1.day;

        if(Monthdiff == 0 && Yeardiff == 0){            //Same month & year but different dates
            Diff+= Daydiff;
            Daydiff = 0;
        }
        else if(d1.day != 1){      //Setting every date of d1 to 1
            if((d1.month == 4 || d1.month == 6 || d1.month == 9 || d1.month == 11)){//For months with 30 days
                Diff += 30-d1.day +1; //Last plus one because we will initialize the day of next month with 1
                d1.day = 1;
                d1.month++;
            }
            else if(d1.month == 2){//For feb
                if(checkLeapYear(d1.year)){
                    Diff += 29 -d1.day + 1;
                    d1.day = 1;
                    d1.month++;
                }
                else{
                    Diff += 28 - d1.day + 1;
                    d1.day = 1;
                    d1.month++;
                }
            }
            else if(d1.month == 12){//For Dec
                Diff+=31 - d1.day + 1;
                d1.day = 1;
                d1.month = 1;
                d1.year++;
            }
            else{//For rest of the months
                Diff += 31 - d1.day + 1;
                d1.day = 1;
                d1.month++;
            }
        }
        else if(Monthdiff == 1 && d2.day < d1.day && Yeardiff == 0){       //Consecutive months but gap less than 30(Same year)
            if((d1.month == 4 || d1.month == 6 || d1.month == 9 || d1.month == 11)){//For months with 30 days
                Diff += 30; //Last plus one because we will initialize the day of next month with 1
                d1.day = 1;
                d1.month++;
            }
            else if(d1.month == 2){//For feb
                if(checkLeapYear(d1.year)){
                    Diff += 29;
                    d1.day = 1;
                    d1.month++;
                }
                else{
                    Diff += 28;
                    d1.day = 1;
                    d1.month++;
                }
            }
            else{//For rest of the months
                Diff += 31;
                d1.day = 1;
                d1.month++;
            }
        }
        else if(Yeardiff == 0 || (Yeardiff == 1 && d2.month < d1.month) || (Yeardiff == 1 && d2.month == d1.month && d2.day < d1.day)){ // Same year and multiple months gap |or| consecutive year with month gap less than 12 
            if((d1.month == 4 || d1.month == 6 || d1.month == 9 || d1.month == 11)){//For months with 30 days
                Diff += 30;
                d1.month++;
            }
            else if(d1.month == 2){//For feb
                if(checkLeapYear(d1.year)){
                    Diff += 29;
                    d1.month++;
                }
                else{
                    Diff += 28;
                    d1.month++;
                }
            }
            else if(d1.month == 12){//For Dec
                Diff+=31 - d1.day + 1;
                d1.day = 1;
                d1.month = 1;
                d1.year++;
            }
            else{//For rest of the months
                Diff += 31;
                d1.month++;
            }
        }
        else{   //For year gap more than 1
            if(checkLeapYear(d1.year)){
                Diff += 366;
                d1.year++;
            }
            else{
                Diff += 365;
                d1.year++;
            }
        }
    }
    return Diff;
}