#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct contact_s{
    char name[50];
    char phone[20];
    char email[50];
} contact_t;

int main() {
    FILE *file;
    contact_t contact;

    // Open the binary file in write mode
    file = fopen("agenda.dat", "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    // Get contact information from the user
    printf("Enter name: ");
    fgets(contact.name, sizeof(contact.name), stdin);
    printf("Enter phone number: ");
    fgets(contact.phone, sizeof(contact.phone), stdin);
    printf("Enter email address: ");
    fgets(contact.email, sizeof(contact.email), stdin);

    // Write the contact to the file
    fwrite(&contact, sizeof(contact), 1, file);

    // Close the file
    fclose(file);

    return 0;
}