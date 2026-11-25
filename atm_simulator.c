#include <stdio.h>
#include <string.h>

// Structure to store customer details
struct Account
{
int id;
char name[30];
int pin;
int balance;

char history[5][50];  
int count;

};

// Array of structures
struct Account customers[5];

int currentCustomer;

// Function declarations
void createDefaultData();
void loadData();
void saveData();
void balanceEnquiry();
void withdraw();
void deposit();
void miniStatement();
void changePIN();
void addTransaction(char message[]);

// Create default customer details
void createDefaultData()
{
int i;

customers[0].id = 1001;  
strcpy(customers[0].name, "Rahul");  
customers[0].pin = 1234;  
customers[0].balance = 5000;  

customers[1].id = 1002;  
strcpy(customers[1].name, "Priya");  
customers[1].pin = 5678;  
customers[1].balance = 8000;  

customers[2].id = 1003;  
strcpy(customers[2].name, "Arjun");  
customers[2].pin = 4321;  
customers[2].balance = 10000;  

customers[3].id = 1004;  
strcpy(customers[3].name, "Sneha");  
customers[3].pin = 1111;  
customers[3].balance = 6000;  

customers[4].id = 1005;  
strcpy(customers[4].name, "Kiran");  
customers[4].pin = 2222;  
customers[4].balance = 12000;  

for (i = 0; i < 5; i++)  
{  
    customers[i].count = 0;  
}

}

// Load customer details from file
void loadData()
{
FILE *fp;
int fileMark;
int i;
int valid = 1;

fp = fopen("atm_data.dat", "rb");  

if (fp == NULL)  
{  
    printf("Creating new ATM accounts...\n");  

    createDefaultData();  
    saveData();  
    return;  
}  

// Check file identification number  
if (fread(&fileMark, sizeof(int), 1, fp) != 1)  
{  
    valid = 0;  
}  
else if (fileMark != 12345)  
{  
    valid = 0;  
}  

// Read customer details  
if (valid == 1)  
{  
    if (fread(customers, sizeof(customers), 1, fp) != 1)  
    {  
        valid = 0;  
    }  
}  

// Check transaction counts  
if (valid == 1)  
{  
    for (i = 0; i < 5; i++)  
    {  
        if (customers[i].count < 0 ||  
            customers[i].count > 5)  
        {  
            valid = 0;  
            break;  
        }  
    }  
}  

fclose(fp);  

// If file is damaged, create default accounts  
if (valid == 0)  
{  
    printf("Invalid or damaged file!\n");  
    printf("Resetting ATM accounts...\n");  

    createDefaultData();  
    saveData();  
}

}

// Save customer details into file
void saveData()
{
FILE *fp;
int fileMark = 12345;

fp = fopen("atm_data.dat", "wb");  

if (fp == NULL)  
{  
    printf("Unable to open file!\n");  
    return;  
}  

// Write identification number  
fwrite(&fileMark, sizeof(int), 1, fp);  

// Write customer details  
fwrite(customers, sizeof(customers), 1, fp);  

fclose(fp);

}

// Add transaction to history
void addTransaction(char message[])
{
int i;

if (customers[currentCustomer].count < 5)  
{  
    strcpy(customers[currentCustomer].history[  
        customers[currentCustomer].count], message);  

    customers[currentCustomer].count++;  
}  
else  
{  
    for (i = 0; i < 4; i++)  
    {  
        strcpy(customers[currentCustomer].history[i],  
               customers[currentCustomer].history[i + 1]);  
    }  

    strcpy(customers[currentCustomer].history[4], message);  
}

}

// Balance enquiry
void balanceEnquiry()
{
printf("\n--- BALANCE ENQUIRY ---\n");

printf("Customer Name: %s\n",  
       customers[currentCustomer].name);  

printf("Available Balance: Rs.%d\n",  
       customers[currentCustomer].balance);

}

// Cash withdrawal
void withdraw()
{
int amount;
char message[50];

printf("\nEnter amount to withdraw: ");  
scanf("%d", &amount);  

if (amount <= 0)  
{  
    printf("Invalid amount!\n");  
}  
else if (amount > customers[currentCustomer].balance)  
{  
    printf("Insufficient balance!\n");  
}  
else  
{  
    customers[currentCustomer].balance =  
    customers[currentCustomer].balance - amount;  

    printf("\nPlease collect your cash.\n");  

    printf("Remaining Balance: Rs.%d\n",  
           customers[currentCustomer].balance);  

    sprintf(message, "Withdrawal: Rs.%d", amount);  

    addTransaction(message);  

    saveData();  
}

}

// Cash deposit
void deposit()
{
int amount;
char message[50];

printf("\nEnter amount to deposit: ");  
scanf("%d", &amount);  

if (amount <= 0)  
{  
    printf("Invalid amount!\n");  
}  
else  
{  
    customers[currentCustomer].balance =  
    customers[currentCustomer].balance + amount;  

    printf("\nAmount deposited successfully!\n");  

    printf("Updated Balance: Rs.%d\n",  
           customers[currentCustomer].balance);  

    sprintf(message, "Deposit: Rs.%d", amount);  

    addTransaction(message);  

    saveData();  
}

}

// Mini statement
void miniStatement()
{
int i;

printf("\n================================\n");  
printf("         MINI STATEMENT\n");  
printf("================================\n");  

printf("Customer ID: %d\n",  
       customers[currentCustomer].id);  

printf("Customer Name: %s\n",  
       customers[currentCustomer].name);  

printf("\nRecent Transactions:\n");  

if (customers[currentCustomer].count == 0)  
{  
    printf("No transactions available.\n");  
}  
else  
{  
    for (i = customers[currentCustomer].count - 1;  
         i >= 0; i--)  
    {  
        printf("%s\n",  
               customers[currentCustomer].history[i]);  
    }  
}  

printf("\nAvailable Balance: Rs.%d\n",  
       customers[currentCustomer].balance);  

printf("================================\n");

}

// Change PIN
void changePIN()
{
int newPIN;
int confirmPIN;

printf("\nEnter new 4-digit PIN: ");  
scanf("%d", &newPIN);  

if (newPIN < 1000 || newPIN > 9999)  
{  
    printf("Enter a valid 4-digit PIN!\n");  
    return;  
}  

printf("Confirm new PIN: ");  
scanf("%d", &confirmPIN);  

if (newPIN == confirmPIN)  
{  
    customers[currentCustomer].pin = newPIN;  

    saveData();  

    printf("\nPIN changed successfully!\n");  
}  
else  
{  
    printf("\nPIN does not match!\n");  
}

}

// Main function
int main()
{
int choice;
int cardChoice;
int enteredPIN;
int attempts = 0;
int i;

loadData();  

printf("\n================================\n");  
printf("          WELCOME TO ATM\n");  
printf("================================\n");  

// Card selection  
printf("\nSELECT CARD TO INSERT\n");  

for (i = 0; i < 5; i++)  
{  
    printf("%d. Customer %d\n",  
           i + 1, customers[i].id);  
}  

printf("\nEnter your choice: ");  
scanf("%d", &cardChoice);  

if (cardChoice < 1 || cardChoice > 5)  
{  
    printf("Invalid card selection!\n");  
    return 0;  
}  

currentCustomer = cardChoice - 1;  

printf("\nCARD INSERTED SUCCESSFULLY!\n");  

printf("Customer ID: %d\n",  
       customers[currentCustomer].id);  

printf("Customer Name: %s\n",  
       customers[currentCustomer].name);  

// PIN verification  
while (attempts < 3)  
{  
    printf("\nEnter your PIN: ");  
    scanf("%d", &enteredPIN);  

    if (enteredPIN == customers[currentCustomer].pin)  
    {  
        printf("\nPIN verified successfully!\n");  
        break;  
    }  
    else  
    {  
        attempts++;  

        printf("Incorrect PIN!\n");  
        printf("Attempts remaining: %d\n",  
               3 - attempts);  
    }  
}  

if (attempts == 3)  
{  
    printf("\nATM session ended!\n");  
    return 0;  
}  

// ATM menu  
do  
{  
    printf("\n================================\n");  
    printf("            ATM MENU\n");  
    printf("================================\n");  

    printf("1. Balance Enquiry\n");  
    printf("2. Cash Withdrawal\n");  
    printf("3. Cash Deposit\n");  
    printf("4. Mini Statement\n");  
    printf("5. Change PIN\n");  
    printf("6. Exit\n");  

    printf("\nEnter your choice: ");  
    scanf("%d", &choice);  

    switch (choice)  
    {  
        case 1:  
            balanceEnquiry();  
            break;  

        case 2:  
            withdraw();  
            break;  

        case 3:  
            deposit();  
            break;  

        case 4:  
            miniStatement();  
            break;  

        case 5:  
            changePIN();  
            break;  

        case 6:  
            printf("\nThank you for using ATM!\n");  
            printf("Please collect your card.\n");  
            break;  

        default:  
            printf("\nInvalid choice!\n");  
    }  

} while (choice != 6);  

saveData();  

return 0;

}