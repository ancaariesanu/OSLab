#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_PATH_LENGHT 1024
#define NUMBER_OF_COMMANDS 10

//helping functions
void print_error_distribution(){
    printf("Code: 1-handle_regfile\n");
    printf("Code: 2-handle_regfile\n");
    printf("Code: 3-handle_regfile\n");
    printf("\n");
    printf("Code: 4-handle_dir\n");
    printf("Code: 5-handle_dir\n");
    printf("Code: 6-handle_dir\n");
    printf("\n");
    printf("Code: 7-handle_sym\n");
    printf("Code: 8-handle_sym\n");
    printf("Code: 9-handle_sym\n");
    printf("\n");
}

//check type of input && reenter commands && get_commands
char* get_commands() {
    char *commands;
    commands = (char*) malloc(NUMBER_OF_COMMANDS*sizeof(char));
    if(commands==NULL) {
        printf("Error malloc() for the commands string in get_commands\n");
        exit(1);
    }
    printf("Please insert the options: ");
    scanf("%s", commands);

    return commands;
}

void reset_commands(char path[]) {
    printf("\n\nRenter the commands:\n");
    check_type(path);
}

void check_type(char path[]){  
    struct stat st;
    if(lstat(path, &st)==-1){
        perror("There is a problem at lstat");
    }
    switch (st.st_mode & S_IFMT){
        case S_IFREG:
            handle_regfile(path);
            break;
        case S_IFDIR:
            handle_dir(path);
            break;
        case S_IFLNK:
            handle_sym(path);
            break;
        default: 
            printf("The file is an unknown one\n");
            break;
    }
}

//permission part
void print_access_rights(mode_t mode){
    printf("User:\n");
    printf("Read - %s\n", ((mode & S_IRUSR)!=0) ? "Yes" : "No");
    printf("Write - %s\n", ((mode & S_IWUSR)!=0) ? "Yes" : "No");
    printf("Exec - %s\n", ((mode & S_IXUSR)!=0) ? "Yes" : "No");
    printf("Group:\n");
    printf("Read - %s\n", ((mode & S_IRGRP)!=0) ? "Yes" : "No");
    printf("Write - %s\n", ((mode & S_IWGRP)!=0) ? "Yes" : "No");
    printf("Exec - %s\n", ((mode & S_IXGRP)!=0) ? "Yes" : "No");
    printf("Others:\n");
    printf("Read - %s\n", ((mode & S_IROTH)!=0) ? "Yes" : "No");
    printf("Write - %s\n", ((mode & S_IWOTH)!=0) ? "Yes" : "No");
    printf("Exec - %s\n", ((mode & S_IXOTH)!=0) ? "Yes" : "No");
}

void change_permissions(char path[]) {
    mode_t permissions = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP;
    int changed = chmod(path, permissions);

    if(changed == -1) {
        printf("error at changing permissions");
        exit(3);
    }
}

//C-file part
void c_file(char path[]) {
    if(path[strlen(path)-1]=='c' && path[strlen(path)-2]=='.') {
        int no_errors = 0;
        int no_warnings = 0;
        //create pipe
        // execlp("bash", "bash", "compile.sh", path, NULL);
        int score;
        if(no_errors==0) {
            if(no_warnings==0){
                score=10;
            }
            else if(no_warnings>10){
                score=2;
            }
            else{
                score=2+8*(10-no_warnings)/10;
            }
        } 
        else if(no_errors>=1) {
            score=1;
        }
        int fd=open("file_tester.txt", O_RDWR);
        if(fd==-1) {
            printf("error for fd()");
            exit(3);
        }
        char score_text[3];
        score_text[0] = score/10+'0';
        score_text[1] = score%10+'0';

        char file_text[100];
        strcpy(file_text, path);
        strcat(file_text, " : ");
        strcat(file_text, score_text);

        int wrote=write(fd, file_text, strlen(file_text));
        if(wrote==-1) {
            printf("error at writting\n");
            exit(3);
        }
        close(fd);
    }
    else {
        int lines=count_lines(path)+1;
        printf("Number of lines: %d\n", lines);
    }
    
}

int count_lines(char path[]) {
    FILE *f=fopen(path, "r");
    if(f==NULL) {
        printf("Error at opening file\n");
        exit(3);
    }
    char c;
    int no_lines=0;
    c=fgetc(f);
    while(c!=EOF) {
        if(c=='\n') {
            no_lines++;
        }
        c=fgetc(f);
    }
    return no_lines;
    printf("\n\n\n---------no_of_lines=%d\n\n\n", no_lines);
}

//directories part
void create_new_file(char name[]) {
    char file_name[100];
    char path[150];

    strcpy(file_name, name);
    strcat(file_name, "_file.txt");

    strcpy(path, name);
    strcat(path, "/");
    strcat(path, file_name);

    int created=creat(path, S_IRUSR);
    if(created==-1) {
        printf("error at creating file\n");
        exit(2);
    }
    close(created);
}

//menus
void menu_regular_file() {
    printf("--- MENU ---\n");
    printf("Name: -n\n");
    printf("Size: -d\n");
    printf("Hard link count: -h\n");
    printf("Time of last modification: -m\n");
    printf("Access rights: -a\n");
    printf("Create symbolic link: -l\n");
}

void menu_directories() {
    printf("---MENU---\n");
    printf("Name: -n\n");
    printf("Size: -d\n");
    printf("Access rights: -a\n");
    printf("Count how many c files there are in the directory: -c\n");
}

void menu_symbolic_link() {
    printf("--- MENU ---\n");
    printf("Name: -n\n");
    printf("Delete symbolic link: -l\n");
    printf("Size of symbolic link: -d\n");
    printf("Size of targeted file: -t\n");
    printf("Access rights: -a\n");
}

//execute commands
void options_regfile(char path[],  char commands[NUMBER_OF_COMMANDS]){
    struct stat st;
    if(stat(path, &st)==-1) {
        printf("Error stat() for regular file in options_regularfile\n");
        exit(1);
    }

    char letters[6]="ndhmal";
    for(int i=1;i<strlen(commands);i++) {
        if(strchr(letters, commands[i])==NULL) {
            printf("You entered a command that is not in the options menu\n");
            reset_commands(path);
        }
    }

    for(int i=1;i<strlen(commands);i++) {
        switch(commands[i]) {
            case 'n': 
                printf("Name of the file: %s\n", path);
                break;
            case 'd': 
                printf("Size: %ld\n", st.st_size);
                break;
            case 'h': 
                printf("Hard link count: %ld\n", st.st_nlink);
                break;
            case 'm':
                printf("Time of last modification: %s", ctime(&st.st_mtime));
                break;
            case 'a': 
                print_access_rights(st.st_mode);
                break;
            case 'l': 
                printf("Please enter the name of the link: ");
                char link_name[20];
                scanf("%s", link_name);
                if(symlink(path, link_name)==0) {
                    printf("Link created succesfully\n");
                }
                else {
                    printf("Error creating the symlink\n");
                    exit(1);
                }
                break;
        }
    }
}

void options_dir(char path[], char commands[NUMBER_OF_COMMANDS]){
    struct stat st;
    if(stat(path, &st)==-1) {
        printf("Error stat() for regular file in options_dir\n");
        exit(1);
    }

    char letters[4]="ndac";
    for(int i=1;i<strlen(commands);i++) {
        if(strchr(letters, commands[i])==NULL) {
            printf("You entered a command that is not in the commands menu\n");
            reset_commands(path);
        }
    }

    for(int i=1;i<strlen(commands);i++){
        switch (commands[i]){
        case 'n':
            printf("You choose to print the directory name\n");
            printf("Name of the file: %s\n", path);
            break;

        case 'd':
            printf("You choose to print the size of the directory\n");
            printf("Size of the directory: %ld\n", st.st_size);
            break;

        case 'a':
            printf("You choose to print the access rights\n");
            print_access_rights(st.st_mode);
            printf("\n");
            break;

        case 'c':
            printf("You choose to print the total number of files with the C extension\n");
            int counter=0; DIR* d;
            struct dirent* dir;
            char* extension=".c";
            if((d=opendir("."))!=NULL){
                while((dir=readdir(d))!=NULL){
                    if(strstr(dir->d_name, extension)!=NULL) counter++;
                }
                closedir(d);
                printf("Number of files with C extension is %d\n", counter);
            }
            else{
                perror("Couldn't open the directory\n");        
            }
            break;

            default:
                break;
        }
    }
}


void options_sym(char path[], char commands[NUMBER_OF_COMMANDS]){
    struct stat st; struct stat targeted_file;
    if(stat(path, &st)==-1) {
        printf("Error stat() for regular file in options_\n");
        exit(1);
    }
    char letters[5]="nldta";
    for(int i=1;i<strlen(commands);i++) {
        if(strchr(letters, commands[i])==NULL) {
            printf("You entered a command that is not in the commands menu\n");
            reset_commands(path);
        }
    }
    for(int i=0;i<strlen(commands);i++){
        switch (commands[i]){
            case 'n':
                printf("You choose to print the link name\n");
                printf("Name of the file: %s\n", path);
                break;
            
            case 'l':
                printf("You choose to delete link\n");
                if(unlink(path)==-1) {
                        exit(1);
                        }
                        else {
                            printf("Successfully delete the symbolic link\n");
                        }
                break;

            case 'd':
                printf("You choose to print the size of the link\n");
                printf("Size of the symbolic link: %ld\n", st.st_size);
                break;

            case 't': 
                printf("You choose to print the size of the target\n");
                if(stat(path, &targeted_file)==-1) {
                        exit(1);
                    }
                printf("Size of the targeted file: %ld\n", targeted_file.st_size);
                break;

            case 'a':
                printf("You choose to print the access rights\n");
                print_access_rights(st.st_mode);
                break;

            default:
                break;
        }
    }
}

//handle each type 
void handle_regfile(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("%s is a regular file. \n", path);
    pid_t pid_reg=fork();
    if(pid_reg<0){
        perror("There is a problem with with creating the process for the regular file");
        exit(1);
    }
    else if(pid_reg==0){
        menu_regular_file();
        strcpy(commands, get_commands());
        options_regfile(path, commands);
        exit(2);
    }
    else{
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
        }
        pid_t pid_reg2=fork();
        if(pid_reg2<0){
            printf("error at fork() for child 1\n");
            exit(3);
        }
        else if(pid_reg2==0){
            c_file(path);
        }
        else{
            int wstatus2;
            pid_t w2=wait(&wstatus2);
            if(WIFEXITED(wstatus2)) {
                printf("The process with PID <%d> has ended with the exit code <%d>\n", w2, WEXITSTATUS(wstatus2));
            }
        }   
    }
}

void handle_dir(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("%s is a directory. \n", path);
    pid_t pid_dir=fork();
    if(pid_dir<0){
        perror("There is a problem with with creating the process for the directory");
        exit(1);
    }
    else if(pid_dir==0){
        menu_directories();
        strcpy(commands, get_commands());
        options_dir(path, commands);
        exit(2);
    }
    else{
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
        }
        pid_t pid_dir2=fork();
        if(pid_dir2<0){
            printf("error at fork() for child 1\n");
            exit(3);
        }
        else if(pid_dir2==0){
            create_new_file(path);
        }
        else{
            int wstatus2;
            pid_t w2=wait(&wstatus2);
            if(WIFEXITED(wstatus2)) {
                printf("The process with PID <%d> has ended with the exit code <%d>\n", w2, WEXITSTATUS(wstatus2));
            }
        }   
    }
}

void handle_sym(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("%s is a symbolic link. \n", path);
    pid_t pid_sym=fork();
    if(pid_sym<0){
        perror("There is a problem with with creating the process for the symbolic link");
        exit(1);
    }
    else if(pid_sym==0){
        menu_symbolic_link();
        strcpy(commands, get_commands());
        options_sym(path, commands);
        exit(2);
    }
    else{
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
        }
        pid_t pid_sym2=fork();
        if(pid_sym2<0){
            printf("error at fork() for child 1\n");
            exit(3);
        }
        else if(pid_sym2==0){
            change_permissions(path);
        }
        else{
            int wstatus2;
            pid_t w2=wait(&wstatus2);
            if(WIFEXITED(wstatus2)) {
                printf("The process with PID <%d> has ended with the exit code <%d>\n", w2, WEXITSTATUS(wstatus2));
            }
        }   
    }
}

int main(int argc, char* argv[]){
    print_error_distribution();
    if(argc<2){
        perror("There are not enough arguments");
        exit(1);
    }
    for(int i=1;i<argc; i++){
        check_type(argv[i]);
    }
    return 0;
}