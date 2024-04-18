#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define FILE_NAME "agenda.dat"
#define MAX_NAME_LENGTH 50
#define MAX_PHONE_LENGTH 20
#define MAX_EMAIL_LENGTH 50

typedef enum cmd_id_e{
    CMD_ADD,
    CMD_DELETE,
    CMD_SEARCH,
    CMD_LIST,
    CMD_EDIT,
    CMD_HELP
} cmd_id_t;

typedef struct contact_s{
    uint8_t name[MAX_NAME_LENGTH];
    uint8_t phone[MAX_PHONE_LENGTH];
    uint8_t email[MAX_EMAIL_LENGTH];
} contact_t;

bool delete_contact(contact_t *entry);
bool search_contact(contact_t *entry);
bool edit_contact(contact_t *entry);


void database_init(void){
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        file = fopen(FILE_NAME, "wb");
        if (file == NULL) {
            printf("Failed to create file.\n");
            return;
        }
        fclose(file);
    }
    else fclose(file);
}

bool add_contact(contact_t *entry){
    bool ret = false;
    FILE *file = fopen(FILE_NAME, "r+b");

    if (file != NULL) {
       fseek(file,0,SEEK_END);

       if(fwrite(entry,sizeof(contact_t),1,file) == 1){
        printf("Contact added successfully.\n");
        ret = true;
       }
       fclose(file);
    }
    return ret;
}

bool list_contacts(){
    bool ret = false;
    FILE *file  = fopen(FILE_NAME,"rb");
    if(file != NULL){
        size_t num_contacts = 0;
        contact_t entry;

        fseek(file,0,SEEK_SET);
        while(fread(&entry,sizeof(contact_t),1,file) == 1){
            printf("<%02zu>    Name: %s\n", num_contacts, entry.name);
            printf("\tPhone: %s\n", entry.phone);
            printf("\tEmail: %s\n", entry.email);
            printf("\n");
            num_contacts++;
        }
        fclose(file);
        ret = true;
    }
    return ret;
}

void display_help(uint8_t *app_name){
    printf("Usage: %s <options>\n", app_name);
    printf("Options:\n");
    printf("  -a, --add <name> <phone> <email>   Add a new contact\n");
    printf("  -d, --delete <name>                Delete a contact\n");
    printf("  -s, --search <name>                Search for a contact\n");
    printf("  -l, --list                         List all contacts\n");
    printf("  -e, --edit <name>                  Edit a contact\n");
    printf("  -h, --help                         Display this help message\n");
}

bool command_process(cmd_id_t cmd_id, contact_t *entry){
    bool ret = false;
    switch (cmd_id) {
        case CMD_ADD:
            ret = add_contact(entry);
            break;
        case CMD_DELETE:
            ret = true; 
            break;
        case CMD_SEARCH:
            ret = true; 
            break;
        case CMD_LIST:
            ret = list_contacts();
            break;
        case CMD_EDIT:
            ret = true; 
            break;
        case CMD_HELP:
            display_help("agenda");
            ret = true;
            break;
        default:
            break;
    }
    return ret;
}

bool command_line_decode(int num_params, uint8_t *params[], cmd_id_t *cmd_id, contact_t *entry){
    bool ret = false;

    if(num_params == 4){
        if(strcmp(params[0],"-a") == 0 || strcmp(params[0],"--add") == 0){
            *cmd_id = CMD_ADD;
            strncpy((uint8_t*)entry->name, params[1], MAX_NAME_LENGTH);
            entry->name[MAX_NAME_LENGTH - 1] = '\0';
            
            strncpy((uint8_t*)entry->phone, params[2], MAX_PHONE_LENGTH);
            entry->phone[MAX_PHONE_LENGTH - 1] = '\0';
            
            strncpy((uint8_t*)entry->email, params[3], MAX_EMAIL_LENGTH);
            entry->email[MAX_EMAIL_LENGTH - 1] = '\0';

            ret = true;
        }
    }
    else if(num_params == 1){
        if(strcmp(params[0],"-l") == 0 || strcmp(params[0],"--list") == 0){
            *cmd_id = CMD_LIST;
            ret = true;
        }
        else if(strcmp(params[0],"-h") == 0 || strcmp(params[0],"--help") == 0){
            *cmd_id = CMD_HELP;
            ret = true;
        }
    }

    return ret;
}

int main(int argc, uint8_t *argv[]) {
    int ret = -1;
    contact_t entry = {0};
    cmd_id_t cmd_id;

    if(argc >= 2){
        if(command_line_decode(argc-1,&argv[1],&cmd_id,&entry)){
            printf("Command ID: %d\n", cmd_id);
            database_init();
            ret = command_process(cmd_id,&entry) ? 0:-2;
        }
        else{
            printf("Invalid command line\n");
        }
    }

    if(ret){
        display_help(argv[0]);
    }
    return ret;
}