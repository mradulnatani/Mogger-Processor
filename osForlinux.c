#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysinfo.h>

#define MAX_PROCESSES 10
#define MAX_COMMAND_LENGTH 100
#define PROCESS_FILE "processes.txt"

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

Process processes[MAX_PROCESSES];
int process_count = 0;

void load_processes();
void print_help();
void print_system_info();

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

void save_processes(int append) {
    int i;
    FILE *file = fopen(PROCESS_FILE, append ? "a" : "w");
    if (!file) {
        printf(COLOR_RED "Error: Unable to open file for writing.\n" COLOR_RESET);
        return;
    }
    for (i = 0; i < process_count; i++) {
        fprintf(file, "%d %s %d %d\n", processes[i].id, processes[i].name, processes[i].status, processes[i].priority);
    }
    fclose(file);
    printf(COLOR_GREEN "Processes %s to '%s'.\n" COLOR_RESET, append ? "appended" : "saved", PROCESS_FILE);
}

void load_processes() {
    printf(COLOR_YELLOW "Loading processes...\n" COLOR_RESET);
    usleep(500000);
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

void print_system_info() {
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    printf(COLOR_YELLOW "System Information:\n" COLOR_RESET);
    printf(COLOR_CYAN "Total RAM: %lu MB\n" COLOR_RESET, memInfo.totalram / (1024 * 1024));
    printf(COLOR_CYAN "Free RAM: %lu MB\n" COLOR_RESET, memInfo.freeram / (1024 * 1024));
    printf(COLOR_CYAN "Total Swap: %lu MB\n" COLOR_RESET, memInfo.totalswap / (1024 * 1024));
    printf(COLOR_CYAN "Free Swap: %lu MB\n" COLOR_RESET, memInfo.freeswap / (1024 * 1024));
}

void print_help() {
    printf(COLOR_CYAN STYLE_BOLD STYLE_UNDERLINE "Available commands:\n" COLOR_RESET);
    printf(COLOR_GREEN "  create" COLOR_RESET " <name> [priority]  - Create a new process\n");
    printf(COLOR_GREEN "  list" COLOR_RESET "                 - List all processes\n");
    printf(COLOR_GREEN "  start" COLOR_RESET " <id>        - Start a process\n");
    printf(COLOR_GREEN "  stop" COLOR_RESET " <id>         - Stop a process\n");
    printf(COLOR_GREEN "  terminate" COLOR_RESET " <id>    - Terminate a process\n");
    printf(COLOR_GREEN "  update" COLOR_RESET " <id> <priority> - Update process priority\n");
    printf(COLOR_GREEN "  find" COLOR_RESET " <name>      - Find a process by name\n");
    printf(COLOR_GREEN "  save" COLOR_RESET " [append]   - Save processes to file\n");
    printf(COLOR_GREEN "  load" COLOR_RESET "                - Load processes from file\n");
    printf(COLOR_GREEN "  info" COLOR_RESET "                - Show system info\n");
    printf(COLOR_GREEN "  help" COLOR_RESET "                - Show this help\n");
    printf(COLOR_GREEN "  exit" COLOR_RESET "                - Exit the program\n");
}

int main() {
    char command[MAX_COMMAND_LENGTH];
    char name[50];
    int id, priority;

    init_os();

    while (1) {
        printf(COLOR_BLUE "MOGGER> " COLOR_RESET);
        fgets(command, sizeof(command), stdin);

        if (sscanf(command, "create %49s %d", name, &priority) >= 1) {
            if (priority < 0) priority = 0;  // Default priority to 0 if invalid
            create_process(name, priority);
        } else if (sscanf(command, "list") == 0) {
            list_processes();
        } else if (sscanf(command, "start %d", &id) == 1) {
            start_process(id);
        } else if (sscanf(command, "stop %d", &id) == 1) {
            stop_process(id);
        } else if (sscanf(command, "terminate %d", &id) == 1) {
            terminate_process(id);
        } else if (sscanf(command, "update %d %d", &id, &priority) == 2) {
            update_process_priority(id, priority);
        } else if (sscanf(command, "find %49s", name) == 1) {
            find_process_by_name(name);
        } else if (sscanf(command, "save %49s", name) == 1) {
            save_processes(strcmp(name, "append") == 0);
        } else if (sscanf(command, "load") == 0) {
            load_processes();
        } else if (sscanf(command, "info") == 0) {
            print_system_info();
        } else if (sscanf(command, "help") == 0) {
            print_help();
        } else if (sscanf(command, "exit") == 0) {
            break;
        } else {
            printf(COLOR_RED "Unknown command. Type 'help' for a list of commands.\n" COLOR_RESET);
        }
    }

    printf(COLOR_YELLOW "Exiting...\n" COLOR_RESET);
    return 0;
}
