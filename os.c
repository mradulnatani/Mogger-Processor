#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 

#define MAX_PROCESSES 10
#define MAX_COMMAND_LENGTH 100
#define PROCESS_FILE "processes.txt"

void load_processes();
void print_help();

void print_welcome_message() {
    printf("\n");
    printf("***************************************\n");
    printf("*                                     *\n");
    printf("*  W E L C O M E   T O   M O G G E R  *\n");
    printf("*                                     *\n");
    printf("***************************************\n");
    printf("\n");
}

typedef struct {
    int id;
    char name[50];
    int status; 
    int priority; 
} Process;

Process processes[MAX_PROCESSES];
int process_count = 0;

void init_os() {
    printf("\033[1;34mSimple OS initializing...\033[0m\n");
    usleep(500000); 
    print_welcome_message(); 
    load_processes(); 
}

void create_process(const char* name, int priority) {
    if (process_count < MAX_PROCESSES) {
        printf("\033[1;32mCreating process...\033[0m\n");
        usleep(500000); 
        Process new_process;
        new_process.id = process_count;
        strcpy(new_process.name, name);
        new_process.status = 0; 
        new_process.priority = priority;
        processes[process_count++] = new_process;
        printf("Process '%s' created with ID %d and priority %d\n", name, process_count - 1, priority);
    } else {
        printf("\033[1;31mError: Maximum number of processes reached.\033[0m\n");
    }
}

void list_processes() {
    int i;
    printf("\033[1;33mCurrent processes:\033[0m\n");
    for (i = 0; i < process_count; i++) {
        printf("ID: %d, Name: %s, Status: %s, Priority: %d\n", 
               processes[i].id, 
               processes[i].name, 
               processes[i].status ? "Running" : "Stopped",
               processes[i].priority);
    }
}

void start_process(int id) {
    if (id >= 0 && id < process_count) {
        processes[id].status = 1;
        printf("\033[1;32mStarting process '%s' (ID: %d)...\033[0m\n", processes[id].name, id);
        usleep(500000); 
        printf("Process '%s' (ID: %d) started.\n", processes[id].name, id);
    } else {
        printf("\033[1;31mError: Invalid process ID.\033[0m\n");
    }
}

void stop_process(int id) {
    if (id >= 0 && id < process_count) {
        processes[id].status = 0;
        printf("\033[1;32mStopping process '%s' (ID: %d)...\033[0m\n", processes[id].name, id);
        usleep(500000); 
        printf("Process '%s' (ID: %d) stopped.\n", processes[id].name, id);
    } else {
        printf("\033[1;31mError: Invalid process ID.\033[0m\n");
    }
}

void terminate_process(int id) {
    int i;
    if (id >= 0 && id < process_count) {
        printf("\033[1;32mTerminating process '%s' (ID: %d)...\033[0m\n", processes[id].name, id);
        usleep(500000); 
        
        for (i = id; i < process_count - 1; i++) {
            processes[i] = processes[i + 1];
        }
        process_count--;
        printf("Process '%s' (ID: %d) terminated.\n", processes[id].name, id);
    } else {
        printf("\033[1;31mError: Invalid process ID.\033[0m\n");
    }
}

void update_process_priority(int id, int new_priority) {
    if (id >= 0 && id < process_count) {
        processes[id].priority = new_priority;
        printf("Updated priority of process '%s' (ID: %d) to %d.\n", processes[id].name, id, new_priority);
    } else {
        printf("\033[1;31mError: Invalid process ID.\033[0m\n");
    }
}

void find_process_by_name(const char* name) {
    int i;
    int found = 0;
    for (i = 0; i < process_count; i++) {
        if (strcmp(processes[i].name, name) == 0) {
            printf("Found process: ID: %d, Name: %s, Status: %s, Priority: %d\n",
                   processes[i].id, processes[i].name,
                   processes[i].status ? "Running" : "Stopped",
                   processes[i].priority);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("\033[1;31mNo process found with name '%s'.\033[0m\n", name);
    }
}

void save_processes(int append) {
    int i;
    FILE *file = fopen(PROCESS_FILE, append ? "a" : "w");
    if (!file) {
        printf("\033[1;31mError: Unable to open file for writing.\033[0m\n");
        return;
    }
    for (i = 0; i < process_count; i++) {
        fprintf(file, "%d %s %d %d\n", processes[i].id, processes[i].name, processes[i].status, processes[i].priority);
    }
    fclose(file);
    printf("Processes %s to '%s'.\n", append ? "appended" : "saved", PROCESS_FILE);
}

void load_processes() {
    printf("\033[1;33mLoading processes...\033[0m\n");
    usleep(500000); 
    FILE *file = fopen(PROCESS_FILE, "r");
    if (!file) {
        printf("No process file found. Starting with an empty process list.\n");
        return;
    }
    while (fscanf(file, "%d %49s %d %d", &processes[process_count].id, processes[process_count].name, &processes[process_count].status, &processes[process_count].priority) == 4) {
        process_count++;
        if (process_count >= MAX_PROCESSES) {
            break; 
        }
    }
    fclose(file);
    printf("Processes loaded from '%s'.\n", PROCESS_FILE);
}

void print_help() {
    printf("\033[1;34mAvailable commands:\033[0m\n");
    printf("  create <name> [priority]  - Create a new process with the given name and optional priority (default is 1).\n");
    printf("  list                       - List all processes.\n");
    printf("  start <id>                - Start the process with the given ID.\n");
    printf("  stop <id>                 - Stop the process with the given ID.\n");
    printf("  terminate <id>            - Terminate the process with the given ID.\n");
    printf("  update <id> <priority>    - Update the priority of the process with the given ID.\n");
    printf("  find <name>               - Find and display details of a process by name.\n");
    printf("  save                       - Save all processes to file (overwrites).\n");
    printf("  append                     - Append all processes to the file.\n");
    printf("  load                       - Load processes from file.\n");
    printf("  exit                       - Exit the program.\n");
}

void command_interpreter() {
    char command[MAX_COMMAND_LENGTH];
    
    while (1) {
        printf("SimpleOS> ");
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = 0; 
        
        char *token = strtok(command, " ");
        
        if (token == NULL) continue;
        
        if (strcasecmp(token, "exit") == 0) {
            save_processes(0); 
            printf("Exiting...\n");
            break;
        } else if (strcasecmp(token, "create") == 0) {
            token = strtok(NULL, " ");
            if (token) {
                int priority = 1; 
                char *priority_str = strtok(NULL, " ");
                if (priority_str) {
                    priority = atoi(priority_str);
                    if (priority < 1 || priority > 10) {
                        printf("\033[1;31mError: Priority must be between 1 and 10. Using default priority 1.\033[0m\n");
                        priority = 1;
                    }
                }
                create_process(token, priority);
            }
        } else if (strcasecmp(token, "list") == 0) {
            list_processes();
        } else if (strcasecmp(token, "start") == 0) {
            token = strtok(NULL, " ");
            if (token) start_process(atoi(token));
        } else if (strcasecmp(token, "stop") == 0) {
            token = strtok(NULL, " ");
            if (token) stop_process(atoi(token));
        } else if (strcasecmp(token, "terminate") == 0) {
            token = strtok(NULL, " ");
            if (token) terminate_process(atoi(token));
        } else if (strcasecmp(token, "update") == 0) {
            int id, priority;
            token = strtok(NULL, " ");
            if (token) id = atoi(token);
            token = strtok(NULL, " ");
            if (token) priority = atoi(token);
            update_process_priority(id, priority);
        } else if (strcasecmp(token, "find") == 0) {
            token = strtok(NULL, " ");
            if (token) find_process_by_name(token);
        } else if (strcasecmp(token, "save") == 0) {
            save_processes(0);
        } else if (strcasecmp(token, "append") == 0) {
            save_processes(1);
        } else if (strcasecmp(token, "load") == 0) {
            load_processes();
        } else if (strcasecmp(token, "help") == 0) {
            print_help();
        } else {
            printf("\033[1;31mUnknown command. Type 'help' for a list of commands.\033[0m\n");
        }
    }
}

int main() {
    init_os();
    command_interpreter();
    printf("Mogger shutting down.\n");
    return 0;
}
