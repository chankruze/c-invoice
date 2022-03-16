/*
Author: chankruze (chankruze@gmail.com)
Created: Tue Mar 15 2022 13:04:35 GMT+0530 (India Standard Time)

Copyright (c) geekofia 2022 and beyond
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_LEN_CMD 35
#define MAX_LEN_INVOICE 25
#define MAX_LEN_SECONDS 10
#define MAX_LEN_NAME 30
#define MAX_ITEMS 10
#define EXIT 0

struct Item {
    char name[MAX_LEN_NAME];
    float price;
    int quantity;
};

void print_item(struct Item* item) {
    printf("%d\t%s\t\t\t%.2f\t\t%.2f\n", item->quantity, item->name, item->price, item->quantity * item->price);
}

struct Bill {
    time_t id;
    char name[MAX_LEN_NAME];
    struct Item items[MAX_ITEMS];
};

void get_time(char* t) {
    // get current time in seconds
    // store time in char[]
    time_t seconds = time(NULL);
    sprintf(t, "%ld", seconds);
}

void gen_invoice_name(char* invoice_name, long int invoice_id) {
    char* prefix = "invoice_";
    // concat invoice_<time>
    strcat(invoice_name, prefix);
    sprintf(invoice_name, "%ld", invoice_id);
    strcat(invoice_name, ".txt");
}

// void write_invoice(char* invoice_name, struct Bill* data) {
//     FILE* fptr;
//     // create and open the file in binary write mode
//     fptr = fopen(invoice_name, "wb");
//     // if unable to create and open file, exit
//     if (fptr == NULL) {
//         puts("Error!");
//         exit(1);
//     }
//     // write the data to file in binary mode
//     fwrite(data, sizeof(struct Bill), 1, fptr);
//     // close the file
//     fclose(fptr);
// }

// void read_invoice(char* invoice_name) {
//     // check if file exists
//     FILE* fptr;
//     struct Bill bill;

//     // open file in read mode
//     fptr = fopen(invoice_name, "r");

//     if (fptr == NULL) {
//         fprintf(stderr, "\nError opening the file\n");
//         exit(1);
//     }

//     // loop to read
//     while (fread(&bill, sizeof(struct Bill), 1, fptr)) {
//         // date
//         struct tm* timeinfo = localtime(&bill.id);
//         char buffer[80];
//         strftime(buffer, 80, "%c", timeinfo);
//         // header
//         puts("\n██████████████████████████ R-Mart ██████████████████████████");
//         puts("\t\tReliance Jio Infocomm Ltd.");
//         puts("\n5th Floor, Maker Chambers IV, 222, Nariman Point, Mumbai - 400021");
//         printf("-----------------------------------------------------------------\n");
//         // invoice id
//         printf("Invoice No: %ld\t\t%s\n", bill.id, buffer);
//         // customer name
//         printf("Bill to: %s\n", bill.name);
//         printf("-----------------------------------------------------------------\n");
//         puts("Quant.\tItem\t\t\tPrice\t\tTotal");
//         printf("-----------------------------------------------------------------\n");
//         // loop items bought
//         int i = 0;
//         while ((bill.items + i)->quantity != 0) {
//             print_item(bill.items + i++);
//         }

//         printf("-----------------------------------------------------------------\n");
//     }

//     fclose(fptr);
// }

void print_invoice(char* invoice_name, struct Bill* bill) {
    char print_cmd[MAX_LEN_CMD] = "notepad /P ";
    strcat(print_cmd, invoice_name);

    FILE* fptr;
    char cwd[PATH_MAX];
    char fullpath[1000];

    // TODO: get proper path in linux
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        // create and open the file in text write mode
        // TODO: fix to automatically handle both linux and windows
        strcat(fullpath, cwd);
        strcat(fullpath, "\\");
        strcat(fullpath, invoice_name);
        // create and open file in write mode
        fptr = fopen(fullpath, "w");
        // if unable to create and open file, exit
        if (fptr == NULL) {
            puts("Error opening file!");
            exit(1);
        }

        // date
        struct tm* timeinfo = localtime(&bill->id);
        char date[80];
        strftime(date, 80, "%c", timeinfo);

        // write the data to file in text mode
        // header
        fprintf(fptr, "\n██████████████████████████ R-Mart ██████████████████████████\n");
        fprintf(fptr, "\t\tReliance Jio Infocomm Ltd.\n");
        fprintf(fptr, "\n5th Floor, Maker Chambers IV, 222, Nariman Point, Mumbai - 400021\n");
        fprintf(fptr, "-----------------------------------------------------------------\n");
        // invoice id
        fprintf(fptr, "Invoice No: %ld\t\t\t%s\n", bill->id, date);
        // customer name
        fprintf(fptr, "Bill to: %s\n", bill->name);
        fprintf(fptr, "-----------------------------------------------------------------\n");
        fprintf(fptr, "Quant.\tItem\t\t\t\tPrice\t\tTotal\n");
        fprintf(fptr, "-----------------------------------------------------------------\n");
        // loop items bought
        int i = 0;
        while ((bill->items + i)->quantity != 0) {
            fprintf(fptr, "%d\t%s\t\t\t\t%.2f\t\t%.2f\n", (bill->items + i)->quantity, (bill->items + i)->name, (bill->items + i)->price, (bill->items + i)->quantity * (bill->items + i)->price);
            i++;
        }
        fprintf(fptr, "-----------------------------------------------------------------\n");
        // close the file
        fclose(fptr);

        system(print_cmd);
    }
}

void prepare_bill(struct Bill* bill) {
    // bill id
    bill->id = time(NULL);
    // get customer name
    printf("Customer Name: ");
    // char cname[MAX_LEN_NAME];
    scanf("%s", bill->name);

    // loop condition
    int choice = 1;
    // current loop count
    int count = 0;

    // loop until condition invalidates
    while (choice != EXIT) {
        switch (choice) {
            case 1: {
                struct Item* item = (struct Item*)malloc(sizeof(struct Item));
                printf("\nItem #%d\n", count);
                puts("-----------------------");
                char name[MAX_LEN_NAME];
                printf("Name: ");
                scanf("%s", item->name);

                float price;
                printf("Price: ");
                scanf("%f", &price);
                item->price = price;

                int quantity;
                printf("Quantity: ");
                scanf("%d", &quantity);
                item->quantity = quantity;
                puts("-----------------------");

                bill->items[count++] = *item;
                break;
            }
        }

        printf("\nChoose an option:\n");
        printf("0. Print\n");
        printf("1. Add item\n");
        printf("choice>");
        scanf("%d", &choice);
    }
}

int main() {
    char invoice_name[MAX_LEN_INVOICE];

    struct Bill* bill;
    bill = (struct Bill*)calloc(1, sizeof(struct Bill));
    // logic to prepare data
    prepare_bill(bill);
    // generate invoice file name
    gen_invoice_name(invoice_name, bill->id);
    // write bill to invoice file
    // write_invoice(invoice_name, bill);
    // read_invoice(invoice_name);
    print_invoice(invoice_name, bill);

    return 0;
}
