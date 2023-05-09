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

//check type of input
void check_type(char *path){
    struct stat st;
        
    if(lstat(path, &st)==-1){
        perror("There is a problem at lstat");
    }
    switch (st.st_mode & S_IFMT){
        case S_IFREG:
            handle_regfile(path,st);
            break;
        case S_IFDIR:
            handle_dir(path,st);
            break;
        case S_IFLNK:
            handle_sym(path, st);
            break;
        default: 
            printf("The file is an unknown one\n");
            break;
    }
}

//permission part
void print_access_rights(const char *path, struct stat st){
    printf("User:\n");
    if(st.st_mode & S_IRUSR){
        printf("Read-yes\n");
    }
    else{
        printf("Read-no\n");
    }
    if(st.st_mode & S_IWUSR){
        printf("Write-yes\n");
    }
    else{
        printf("Write-no\n");
    }
    if(st.st_mode & S_IXUSR){
        printf("Execute-yes\n");
    }
    else{
        printf("Execute-no\n");
    }

    printf("Group:\n");
    if(st.st_mode & S_IRGRP){
        printf("Read-yes\n");
    }
    else{
        printf("Read-no\n");
    }
    if(st.st_mode & S_IWGRP){
        printf("Write-yes\n");
    }
    else{
        printf("Write-no\n");
    }
    if(st.st_mode & S_IXGRP){
        printf("Execute-yes\n");
    }
    else{
        printf("Execute-no\n");
    }

    printf("Other:\n");
    if(st.st_mode & S_IROTH){
        printf("Read-yes\n");
    }
    else{
        printf("Read-no\n");
    }
    if(st.st_mode & S_IWOTH){
        printf("Write-yes\n");
    }
    else{
        printf("Write-no\n");
    }
    if(st.st_mode & S_IXOTH){
        printf("Execute-yes\n");
    }
    else{
        printf("Execute-no\n");
    }
}

void change_permissions(const char *path) {
    mode_t permissions = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP;
    int changed = chmod(path, permissions);

    if(changed == -1) {
        printf("error at changing permissions");
        exit(3);
    }
}

//C-file part
void c_file(const char *path) {
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
        int lines = count_lines(path);
        printf("Number of lines: %d\n", lines);
    }
    
}

int count_lines(const char *path) {
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
    return no_lines+1;
}

//directories part
void create_new_file(const char *name) {
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
void options_regfile(const char *path, struct stat st){
    char option[10]; int ok=1;
    char letters_reg[7]="-ndhmal";
    menu_regular_file();
    scanf("\nThe command introduced by the user: %s", option);
    printf("\n");
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_reg,option[i])==NULL){
            ok=0;
        }
    }
    if(ok==0){
        printf("Invalid option\n");
        break;
    }
    if(ok==1 && option[0]=='-'){
        for(int i=0;i<strlen(option);i++){
            switch (option[i]){
            case 'n':
                printf("You choose to print the name of the regular file\n");
                printf("Name: %s\n", path);
                break;
            
            case 'd':
            printf("You choose to print the size of the regular file\n");
            printf("%ld\n",st.st_size);
            break;

            case 'h':
                printf("You choose to print the number of the hard links of the regular file\n");
                printf("%ld\n",st.st_nlink);
                break;

            case 'm': 
                printf("You choose to print the last time of modification of the regular file\n");
                printf("%ld\n",st.st_mtime);
                printf("\n");
                break;

            case 'a':
                printf("You choose to print the access rights\n");
                print_access_rights(path,st);
                printf("\n");
                break;

            case 'l':
                printf("You choose to create a symbolic link of the regular file\n");
                const char *new_path;
                new_path = (char*) malloc(10*sizeof(char));
                symlink(path, new_path);
                printf("\n");
                break;

            default:
                break;
            }
        }
    }
}

void options_dir(const char *path, struct stat st){
    char option[10]; int ok=1;
    char letters_dir[5]="-ndac";
    menu_directories();
    scanf("\nThe command introduced by the user: %s", option);
    printf("\n");
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_dir,option[i])==NULL){
            ok=0;
        }
    }
    if(ok==0){
        printf("Invalid option\n");
        break
    }
    if(ok==1 && option[0]=='-'){
        for(int i=0;i<strlen(option);i++){
            switch (option[i]){
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
                print_access_rights(path,st);
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
}

void options_sym(const char *path, struct stat st){
    char option[10]; int ok=1; struct stat targeted_file;
    char letters_link[6]="-nldta";
    menu_symbolic_link();
    scanf("\nThe command introduced by the user: %s", option);
    printf("\n");
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_link,option[i])==NULL){
            ok=0;
        }
    }
    if(ok==0){
        printf("Invalid option\n");
        break;
    }
    if(ok==1 && option[0]=='-'){
        for(int i=0;i<strlen(option);i++){
            switch (option[i]){
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
                print_access_rights(path,st);
                break;

            default:
                break;
            }
        }
    }
}

//handle each type 
void handle_regfile(const char *path, struct stat st){
    printf("%s is a regular file. \n", path);
    pid_t pid_reg=fork();
    if(pid_reg<0){
        perror("There is a problem with with creating the process for the regular file");
        exit(1);
    }
    else if(pid_reg==0){
        options_regfile(path,st);
        exit(2);
    }
    else{
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
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
        }
    }
}

void handle_dir(const char *path, struct stat st){
    printf("%s is a directory. \n", path);
    pid_t pid_dir=fork();         
    if(pid_dir<0){
        perror("There is a problem with with creating the process for the directory");
        exit(4);
    }
    else if(pid_dir==0){
        options_dir(path,st);
        exit(5);
    }
    else{
        pid_t pid_dir2=fork();
        if(pid_dir2<0){
            printf("Error at fork() for child 1(directories)\n");
            exit(6);
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
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
        }
    }    
}

void handle_sym(const char *path, struct stat st){
    printf("%s is a symbolic link. \n", path);
    pid_t pid_lnk=fork();
    if(pid_lnk<0){
        perror("There is a problem with with creating the parent process for the symbolic link\n");
        exit(7);
    }
    else if(pid_lnk==0){
        options_sym(path, st);
        exit(8);
    }
    else{
        pid_t pid_sym2=fork();
        if(pid_sym2<0){
        printf("Error at fork() for child 1(symbolik)\n");
            exit(9);
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
        int wstatus;
        pid_t w=wait(&wstatus);
        if(WIFEXITED(wstatus)){
            printf("The process with PID <%d> has ended with the exit code <%d>\n", w, WEXITSTATUS(wstatus));
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
        char *path=argv[i];
        check_type(path);
    }
    return 0;
}