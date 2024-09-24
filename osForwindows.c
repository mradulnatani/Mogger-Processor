#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#define MAX_PROCESSES 10
#define MAX_COMMAND_LENGTH 100
#define PROCESS_FILE "processes.txt"
#define PACKAGE_FILE "packages.txt"

// ANSI color codes
#define COLOR_RESET   "\x1B[0m"
#define COLOR_RED     "\x1B[31m"
#define COLOR_GREEN   "\x1B[32m"
#define COLOR_YELLOW  "\x1B[33m"
#define COLOR_BLUE    "\x1B[34m"
#define COLOR_MAGENTA "\x1B[35m"
#define COLOR_CYAN    "\x1B[36m"
#define COLOR_WHITE   "\x1B[37m"

// ANSI style codes
#define STYLE_BOLD      "\x1B[1m"
#define STYLE_UNDERLINE "\x1B[4m"

typedef struct {
    int id;
    char name[50];
    int status;
    int priority;
} Process;

typedef struct {
    char name[50];
    char version[10];
    int installed;
} Package;

Process processes[MAX_PROCESSES];
Package packages[MAX_PROCESSES]; // Reusing the array size for packages
int process_count = 0;
int package_count = 0;

void load_processes();
void save_processes(int append);
void create_process(const char* name, int priority);
void list_processes();
void start_process(int id);
void stop_process(int id);
void terminate_process(int id);
void update_process_priority(int id, int new_priority);
void find_process_by_name(const char* name);
void load_packages();
void save_packages(int append);
void install_package(const char* name, const char* version);
void uninstall_package(const char* name);
void list_packages();
void print_help();
void print_system_info();
void print_welcome_message();
void init_os();
void command_interpreter();

void print_welcome_message() {
    printf("\n");
    printf(COLOR_CYAN STYLE_BOLD);
    printf("+---------------------------------------+\n");
    printf("¦                                       ¦\n");
    printf("¦          W E L C O M E  T O           ¦\n");
    printf("¦                                       ¦\n");
    printf("¦               M O G G E R             ¦\n");
    printf("¦                                       ¦\n");
    printf("+---------------------------------------+\n");
    printf(COLOR_RESET "\n");

    printf(COLOR_MAGENTA);
    printf("   _____ _____ _____ _____    _____ _____ \n");
    printf("  |     |     |   __|   __|  |     |   __|\n");
    printf("  | | | |  |  |  |  |  |  |  |  |  |__   |\n");
    printf("  |_|_|_|_____|_____|_____|  |_____|_____|\n");
    printf(COLOR_RESET "\n");

    printf(COLOR_YELLOW STYLE_BOLD);
    printf("       Get Ready for an Exciting\n");
    printf("              Adventure!\n");
    printf(COLOR_RESET "\n");
}

void init_os() {
    printf(COLOR_BLUE STYLE_BOLD "+---- Simple OS initializing... ----+" COLOR_RESET "\n");
    usleep(500000);
    print_welcome_message();
    load_processes();
    load_packages(); // Load packages during initialization
    printf(COLOR_GREEN "? Initialization complete" COLOR_RESET "\n\n");
}

void create_process(const char* name, int priority) {
    if (process_count < MAX_PROCESSES) {
        printf(COLOR_GREEN "Creating process..." COLOR_RESET "\n");
        usleep(500000);
        Process new_process;
        new_process.id = process_count;
        strcpy(new_process.name, name);
        new_process.status = 0;
        new_process.priority = priority;
        processes[process_count++] = new_process;
        printf(COLOR_GREEN "Process '%s' created with ID %d and priority %d\n" COLOR_RESET, name, process_count - 1, priority);
    } else {
        printf(COLOR_RED "Error: Maximum number of processes reached.\n" COLOR_RESET);
    }
}

void list_processes() {
    int i;
    printf(COLOR_YELLOW "Current processes:\n" COLOR_RESET);
    for (i = 0; i < process_count; i++) {
        printf(COLOR_CYAN "ID: %d, Name: %s, Status: %s, Priority: %d\n" COLOR_RESET,
               processes[i].id,
               processes[i].name,
               processes[i].status ? "Running" : "Stopped",
               processes[i].priority);
    }
}

void start_process(int id) {
    if (id >= 0 && id < process_count) {
        processes[id].status = 1;
        printf(COLOR_GREEN "Starting process '%s' (ID: %d)...\n" COLOR_RESET, processes[id].name, id);
        usleep(500000);
        printf(COLOR_GREEN "Process '%s' (ID: %d) started.\n" COLOR_RESET, processes[id].name, id);
    } else {
        printf(COLOR_RED "Error: Invalid process ID.\n" COLOR_RESET);
    }
}

void stop_process(int id) {
    if (id >= 0 && id < process_count) {
        processes[id].status = 0;
        printf(COLOR_YELLOW "Stopping process '%s' (ID: %d)...\n" COLOR_RESET, processes[id].name, id);
        usleep(500000);
        printf(COLOR_YELLOW "Process '%s' (ID: %d) stopped.\n" COLOR_RESET, processes[id].name, id);
    } else {
        printf(COLOR_RED "Error: Invalid process ID.\n" COLOR_RESET);
    }
}

void terminate_process(int id) {
    int i;
    if (id >= 0 && id < process_count) {
        printf(COLOR_RED "Terminating process '%s' (ID: %d)...\n" COLOR_RESET, processes[id].name, id);
        usleep(500000);
        
        for (i = id; i < process_count - 1; i++) {
            processes[i] = processes[i + 1];
        }
        process_count--;
        printf(COLOR_RED "Process '%s' (ID: %d) terminated.\n" COLOR_RESET, processes[id].name, id);
    } else {
        printf(COLOR_RED "Error: Invalid process ID.\n" COLOR_RESET);
    }
}

void update_process_priority(int id, int new_priority) {
    if (id >= 0 && id < process_count) {
        processes[id].priority = new_priority;
        printf(COLOR_CYAN "Updated priority of process '%s' (ID: %d) to %d.\n" COLOR_RESET, processes[id].name, id, new_priority);
    } else {
        printf(COLOR_RED "Error: Invalid process ID.\n" COLOR_RESET);
    }
}

void find_process_by_name(const char* name) {
    int i;
    int found = 0;
    for (i = 0; i < process_count; i++) {
        if (strcmp(processes[i].name, name) == 0) {
            printf(COLOR_GREEN "Found process: ID: %d, Name: %s, Status: %s, Priority: %d\n" COLOR_RESET,
                   processes[i].id, processes[i].name,
                   processes[i].status ? "Running" : "Stopped",
                   processes[i].priority);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf(COLOR_RED "No process found with name '%s'.\n" COLOR_RESET, name);
    }
}

void load_packages() {
    FILE *file = fopen(PACKAGE_FILE, "r");
    if (!file) {
        printf(COLOR_YELLOW "No package file found. Starting with an empty package list.\n" COLOR_RESET);
        return;
    }
    while (fscanf(file, "%49s %9s %d", packages[package_count].name, packages[package_count].version, &packages[package_count].installed) == 3) {
        package_count++;
        if (package_count >= MAX_PROCESSES) {
            break;
        }
    }
    fclose(file);
    printf(COLOR_GREEN "Packages loaded from '%s'.\n" COLOR_RESET, PACKAGE_FILE);
}

void save_packages(int append) {
	int i;
    FILE *file = fopen(PACKAGE_FILE, append ? "a" : "w");
    if (!file) {
        printf(COLOR_RED "Error: Unable to open package file for writing.\n" COLOR_RESET);
        return;
    }
    for (i = 0; i < package_count; i++) {
        fprintf(file, "%s %s %d\n", packages[i].name, packages[i].version, packages[i].installed);
    }
    fclose(file);
    printf(COLOR_GREEN "Packages %s to '%s'.\n" COLOR_RESET, append ? "appended" : "saved", PACKAGE_FILE);
}

void install_package(const char* name, const char* version) {
    if (package_count < MAX_PROCESSES) {
        strcpy(packages[package_count].name, name);
        strcpy(packages[package_count].version, version);
        packages[package_count].installed = 1; // Mark as installed
        package_count++;
        printf(COLOR_GREEN "Package '%s' version '%s' installed successfully.\n" COLOR_RESET, name, version);
    } else {
        printf(COLOR_RED "Error: Maximum number of packages reached.\n" COLOR_RESET);
    }
}

void uninstall_package(const char* name) {
	int i;
    for (i = 0; i < package_count; i++) {
        if (strcmp(packages[i].name, name) == 0) {
            packages[i].installed = 0; // Mark as uninstalled
            printf(COLOR_YELLOW "Package '%s' uninstalled successfully.\n" COLOR_RESET, name);
            return;
        }
    }
    printf(COLOR_RED "Error: Package '%s' not found.\n" COLOR_RESET, name);
}

void list_packages() {
    printf(COLOR_YELLOW "Installed packages:\n" COLOR_RESET);
    int i;
    for (i = 0; i < package_count; i++) {
        printf(COLOR_CYAN "%s (version: %s) - %s\n" COLOR_RESET, 
               packages[i].name, 
               packages[i].version, 
               packages[i].installed ? "Installed" : "Not Installed");
    }
}

void command_interpreter() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf(COLOR_CYAN STYLE_BOLD "SimpleOS> " COLOR_RESET);
        fgets(command, MAX_COMMAND_LENGTH, stdin);
        command[strcspn(command, "\n")] = 0;

        char *token = strtok(command, " ");

        if (token == NULL) continue;

        if (strcasecmp(token, "exit") == 0) {
            save_processes(0);
            save_packages(0);
            printf(COLOR_YELLOW "Exiting...\n" COLOR_RESET);
            break;
        } else if (strcasecmp(token, "create") == 0) {
            char *name = strtok(NULL, " ");
            char *priority_str = strtok(NULL, " ");
            int priority = priority_str ? atoi(priority_str) : 0;
            if (name) {
                create_process(name, priority);
            } else {
                printf(COLOR_RED "Usage: create <name> [priority]\n" COLOR_RESET);
            }
        } else if (strcasecmp(token, "list") == 0) {
            list_processes();
        } else if (strcasecmp(token, "start") == 0) {
            char *id_str = strtok(NULL, " ");
            int id = id_str ? atoi(id_str) : -1;
            start_process(id);
        } else if (strcasecmp(token, "stop") == 0) {
            char *id_str = strtok(NULL, " ");
            int id = id_str ? atoi(id_str) : -1;
            stop_process(id);
        } else if (strcasecmp(token, "terminate") == 0) {
            char *id_str = strtok(NULL, " ");
            int id = id_str ? atoi(id_str) : -1;
            terminate_process(id);
        } else if (strcasecmp(token, "priority") == 0) {
            char *id_str = strtok(NULL, " ");
            int id = id_str ? atoi(id_str) : -1;
            char *new_priority_str = strtok(NULL, " ");
            int new_priority = new_priority_str ? atoi(new_priority_str) : 0;
            update_process_priority(id, new_priority);
        } else if (strcasecmp(token, "find") == 0) {
            char *name = strtok(NULL, " ");
            if (name) {
                find_process_by_name(name);
            } else {
                printf(COLOR_RED "Usage: find <process_name>\n" COLOR_RESET);
            }
        } else if (strcasecmp(token, "install") == 0) {
            char *name = strtok(NULL, " ");
            char *version = strtok(NULL, " ");
            if (name && version) {
                install_package(name, version);
            } else {
                printf(COLOR_RED "Usage: install <package_name> <version>\n" COLOR_RESET);
            }
        } else if (strcasecmp(token, "uninstall") == 0) {
            char *name = strtok(NULL, " ");
            if (name) {
                uninstall_package(name);
            } else {
                printf(COLOR_RED "Usage: uninstall <package_name>\n" COLOR_RESET);
            }
        } else if (strcasecmp(token, "listpackages") == 0) {
            list_packages();
        } else if (strcasecmp(token, "help") == 0) {
            print_help();
        } else {
            printf(COLOR_RED "Unknown command. Type 'help' for a list of commands.\n" COLOR_RESET);
        }
    }
}

void load_processes() {
    FILE *file = fopen(PROCESS_FILE, "r");
    if (!file) {
        printf(COLOR_YELLOW "No process file found. Starting with an empty process list.\n" COLOR_RESET);
        return;
    }
    while (fscanf(file, "%d %49s %d %d", &processes[process_count].id, processes[process_count].name, &processes[process_count].status, &processes[process_count].priority) == 4) {
        process_count++;
        if (process_count >= MAX_PROCESSES) {
            break;
        }
    }
    fclose(file);
    printf(COLOR_GREEN "Processes loaded from '%s'.\n" COLOR_RESET, PROCESS_FILE);
}

void save_processes(int append) {
    FILE *file = fopen(PROCESS_FILE, append ? "a" : "w");
    if (!file) {
        printf(COLOR_RED "Error: Unable to open process file for writing.\n" COLOR_RESET);
        return;
    }
    int i;
    for (i = 0; i < process_count; i++) {
        fprintf(file, "%d %s %d %d\n", processes[i].id, processes[i].name, processes[i].status, processes[i].priority);
    }
    fclose(file);
    printf(COLOR_GREEN "Processes %s to '%s'.\n" COLOR_RESET, append ? "appended" : "saved", PROCESS_FILE);
}

void print_help() {
    printf(COLOR_CYAN STYLE_BOLD STYLE_UNDERLINE "Available commands:\n" COLOR_RESET);
    printf(COLOR_GREEN "  create" COLOR_RESET " <name> [priority]  - Create a new process\n");
    printf(COLOR_GREEN "  list" COLOR_RESET "                     - List all processes\n");
    printf(COLOR_GREEN "  start" COLOR_RESET " <id>            - Start a process\n");
    printf(COLOR_GREEN "  stop" COLOR_RESET " <id>             - Stop a process\n");
    printf(COLOR_GREEN "  terminate" COLOR_RESET " <id>         - Terminate a process\n");
    printf(COLOR_GREEN "  priority" COLOR_RESET " <id> <value>  - Update process priority\n");
    printf(COLOR_GREEN "  find" COLOR_RESET " <process_name>   - Find a process by name\n");
    printf(COLOR_GREEN "  install" COLOR_RESET " <package_name> <version> - Install a package\n");
    printf(COLOR_GREEN "  uninstall" COLOR_RESET " <package_name>    - Uninstall a package\n");
    printf(COLOR_GREEN "  listpackages" COLOR_RESET "          - List all installed packages\n");
    printf(COLOR_RED "  exit" COLOR_RESET "                   - Exit the program\n");
}

int main() {
    init_os();
    command_interpreter();
    return 0;
}
