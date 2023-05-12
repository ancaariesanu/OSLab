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

#define NUMBER_OF_COMMANDS 10

//------------------------------------------------EXIT CODES--------------------------------------------------------//
//1->get_commands()                                          12->options_regfile()->creating symbolic link          //      
//2->change_permissions()                                    13->options_dir()->stat() for directories              //
//3->c_file()->pipe                                          14->options_sym()->stat() for symbolic links           //
//4->c_file()->child proccess(pid2)                          15->options_sym()->delete the symbolic link            //  
//5->c_file()->compiling .sh                                 16->options_sum()->stat() for print the size of target //                 
//6->c_file()->read from pipe                                17->handle_regfile()->create process for reg file      //
//7->c_file()->open file grades.txt                          18->handle_dir()->create process for dir               //
//8->c_file()->write in grade.txt                            19->handle_dir()->fork_1                               //
//9->count_lines()->open the .txt file to count lines        20->handle_sym()->create process sym                   //              
//10->create_new_file()->creating file for directories part  21->handle_sym()->fork_1                               //      
//11->options_regfile()->lstat() for regular file            22->main()->no arguments                               //
//------------------------------------------------------------------------------------------------------------------//

//prototypes
void reset_commands(char path[]);
void check_type(char path[]);
int count_lines(char path[]);
void handle_regfile(char path[]);
void handle_dir(char path[]);
void handle_sym(char path[]);

//wait function
void wait_for_children(){
    pid_t awaited_child;
    int status;
    for(int i=1;i<=2;i++){  
        awaited_child = wait(&status);
        if(WIFEXITED(status)){
            printf("\nChild process with PID <%d> has ended with the code <%d>.\n", awaited_child, WEXITSTATUS(status));
        }
        else{
            printf("\nChild process with PID <%d> has ended with the code <%d>.\n", awaited_child, WEXITSTATUS(status));
        }
    }
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
    printf("\n");
    return commands;
}

void reset_commands(char path[]) {
    printf("\n\nRenter the commands:\n");
    check_type(path);
}

void check_type(char path[]){  
    struct stat st;
    if(lstat(path, &st)==-1){
        perror("There is a problem at lstat!\n");
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
            printf("The file is an unknown one!\n");
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

int change_permissions(char path[]) {
    mode_t permissions = S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP;
    int changed = chmod(path, permissions);
    int ok=0;

    if(changed == -1) {
        perror("Error at changing permissions!\n");
        exit(2);
    }
    else{
        printf("\nPermissions changed!\n");
    }
    return ok;
}

//C-file part 
void c_file(char path[]) {
    char file_name[1024];
    int lenght=strlen(path);
    int count=0;
    int cpy_lenght=lenght;
    while(path[cpy_lenght]!='/' && cpy_lenght>=0) cpy_lenght--;
    while(lenght!=cpy_lenght) file_name[count++]=path[++cpy_lenght];
    int pfd[2];
    if(pipe(pfd)<0){
        perror("Pipe for c_file doesn't work!\n");
        exit(3);
    }
    pid_t pid_reg2=fork();
    if(pid_reg2<0){
        perror("Pid_reg2 nu e creat\n");
        exit(4);
    }
    else if(pid_reg2==0){
        if(file_name[strlen(file_name)-1]=='c' && file_name[strlen(file_name)-2]=='.') {
            close(pfd[0]);
            dup2(pfd[1], 1);
            int check=execlp("bash", "bash", "error_counter.sh", file_name, NULL);
            if(check==-1){
                perror("\nCouldn't compile the .sh file\n");
                exit(5);
            }
        }
        else {
            int lines=count_lines(path)+1;
            printf("\nNumber of lines: %d\n", lines);
        }
        exit(EXIT_SUCCESS);
    }
    else {
        wait_for_children();
        if(file_name[strlen(file_name)-1]=='c' && file_name[strlen(file_name)-2]=='.'){
            int no_errors = 0;
            int no_warnings = 0;
            int score=0;
            close(pfd[1]);
            char buff[10];
            int check;
            check=read(pfd[0], buff, 10);
            if(check==-1){
                perror("\nCouldn't read from pipe!\n");
                exit(6);
            }
            sscanf(buff, "%d %d", &no_errors, &no_warnings);
            if(no_errors==0){
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
            int f=open("grades.txt", O_RDWR);
            if(f==-1) {
                printf("\nError for opening f()\n");
                exit(7);
            }
            char score_text[3];
            score_text[0] = score/10+'0';
            score_text[1] = score%10+'0';

            char file_text[100];
            strcpy(file_text, path);
            strcat(file_text, " : ");
            strcat(file_text, score_text);

            int wrote=write(f, file_text, strlen(file_text));
            if(wrote==-1) {
                printf("\nError at writting\n");
                exit(8);
            }
            close(f);
        }
    }
}          

int count_lines(char path[]) {
    FILE *f=fopen(path, "r");
    if(f==NULL) {
        printf("Error at opening file\n");
        exit(9);
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
        printf("\nError at creating file for directories part!\n");
        exit(10);
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
    if(lstat(path, &st)==-1) {
        printf("\nError lstat() for regular file in options_regfile\n");
        exit(11);
    }

    char letters[6]="ndhmal";
    for(int i=1;i<strlen(commands);i++) {
        if(strchr(letters, commands[i])==NULL) {
            printf("\nYou entered a command that is not in the options menu\n");
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
                    exit(12);
                }
                break;
        }
    }
}

void options_dir(char path[], char commands[NUMBER_OF_COMMANDS]){
    struct stat st;
    if(stat(path, &st)==-1) {
        printf("Error stat() for directories in options_dir\n");
        exit(13);
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
            
            if((d=opendir(path))!=NULL){
                while((dir=readdir(d))!=NULL){
                    if(dir->d_name[strlen(dir->d_name)-2]=='.' && dir->d_name[strlen(dir->d_name)-1]=='c') counter++;
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
    int deleted=0;
    if(stat(path, &st)==-1) {
        printf("Error stat() for symbolic links in options_\n");
        exit(14);
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
                if(deleted==0){
                    printf("Name of the file: %s\n", path);
                }
                else{
                    printf("The symbolic link was deleted and you can not perform any operation\n");
                }
                break;
            
            case 'l':
                printf("You choose to delete the symbolic link\n");
                if(unlink(path)==-1) {
                    printf("\nCouldn't delete the symbolic link!\n");
                    exit(15);
                }
                else {
                    printf("\nSuccessfully delete the symbolic link!\n");
                }
                deleted=1;
                break;

            case 'd':
                printf("You choose to print the size of the link\n");
                if(deleted==0){
                    printf("Size of the symbolic link: %ld\n", st.st_size);
                } 
                else{
                    printf("The symbolic link was deleted and you can not perform any operation\n");
                }
                break;

            case 't': 
                printf("You choose to print the size of the target\n");
                if(deleted==0){
                    if(stat(path, &targeted_file)==-1) {
                        exit(16);
                    }
                    printf("Size of the targeted file: %ld\n", targeted_file.st_size);
                }
                else{
                    printf("The symbolic link was deleted and you can not perform any operation\n");
                }
                break;

            case 'a':
                printf("You choose to print the access rights\n");
                if(deleted==0){
                    print_access_rights(st.st_mode);
                }
                else{
                    printf("The symbolic link was deleted and you can not perform any operation\n");
                }
                break;

            default:
                break;
        }
    }
}

//handle each type 
void handle_regfile(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("\n------------------------------------------------\n");
    printf("%s is a regular file. \n", path);
    pid_t pid_reg=fork();
    if(pid_reg<0){
        perror("\nThere is a problem with with creating the process for the regular file!\n");
        exit(17);
    }
    else if(pid_reg==0){
        menu_regular_file();
        strcpy(commands, get_commands());
        options_regfile(path, commands);
        exit(EXIT_SUCCESS);
    }
    else{
        c_file(path);
    }
}

void handle_dir(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("\n------------------------------------------------\n");
    printf("%s is a directory. \n", path);
    pid_t pid_dir=fork();
    if(pid_dir<0){
        perror("\nThere is a problem with with creating the process for the directory!\n");
        exit(18);
    }
    else if(pid_dir==0){
        menu_directories();
        strcpy(commands, get_commands());
        options_dir(path, commands);
        exit(EXIT_SUCCESS);
    }
    else{
        pid_t pid_dir2=fork();
        if(pid_dir2<0){
            printf("\nError at fork() for child 1(dir)!\n");
            exit(19);
        }
        else if(pid_dir2==0){
            create_new_file(path);
        }
        else{
            wait_for_children();
        }   
    }
}

void handle_sym(char path[]){
    char commands[NUMBER_OF_COMMANDS];
    printf("\n------------------------------------------------\n");
    printf("%s is a symbolic link. \n", path);
    pid_t pid_sym=fork();
    if(pid_sym<0){
        perror("\nThere is a problem with with creating the process for the symbolic link!\n");
        exit(20);
    }
    else if(pid_sym==0){
        menu_symbolic_link();
        strcpy(commands, get_commands());
        options_sym(path, commands);
        exit(EXIT_SUCCESS);
    }
    else{
        pid_t pid_sym2=fork();
        if(pid_sym2<0){
            printf("\nError at fork() for child 1(sym)!\n");
            exit(21);
        }
        else if(pid_sym2==0){
            change_permissions(path);
            //exit(EXIT_SUCCESS);
        }
        else{
            wait_for_children();
        }   
    }
}

int main(int argc, char* argv[]){
    if(argc<2){
        perror("\nThere are not enough arguments!\n");
        exit(22);
    }
    for(int i=1;i<argc; i++){
        check_type(argv[i]);
    }
    return 0;
}