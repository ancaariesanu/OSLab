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

/*void parse_file(const char *path){
    FILE* f=fopen(path, "r"){
        
    }
}*/

//aici merge, copiaza pentru directory si pentru symbolic
void print_menu_regfile(const char *path, struct stat st){
    char option[10]; int ok=1;
    char letters_reg[7]="-ndhmal";
    printf("Choose what option do you want:\n n-file name\n d-dimension/size\n h-number of hard links\n m-time of last modification\n a-access rights\n l-create a symbolic link\n");
    scanf("%s", option);
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_reg,option[i])==NULL){
            //printf("You entered a command that is not in the command menu\n");
            //print_menu_regfile(path,st);
            ok=0;
        }
    }
    if(ok==0){
        printf("Invalid option\n");
    }
    if(ok==1 && option[0]=='-'){
        for(int i=0;i<strlen(option);i++){
            switch (option[i]){
            case 'n':
                printf("You choose to print the name of the regular file\n");
                printf("Name: %s", path);
                break;
            
            case 'd':
            printf("You choose to print the size of the regular file\n");
            printf("%ld",st.st_size);
            break;

            case 'h':
                printf("You choose to print the number of the hard links of the regular file\n");
                printf("%ld",st.st_nlink);
                break;

            case 'm': 
                printf("You choose to print the last time of modification of the regular file\n");
                printf("%ld",st.st_mtime);
                break;

            case 'a':
                printf("You choose to print the access rights\n");
                print_access_rights(path,st);
                break;

            case 'l':
                printf("You choose to create a symbolic link of the regular file\n");
                const char *new_path;
                new_path = (char*) malloc(10*sizeof(char));
                symlink(path, new_path);
                break;

            default:
                break;
            }
        }
    }
}

void print_menu_dir(const char *path, struct stat st){
    
    printf("Choose what option do you want:\n n-file name\n d-size of the directory\n a-access rights\n c-total number of files with the C extension");
    char option[10];
    scanf("%s", option);
    char letters_dir[4]="ndac";
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_dir,option[i])==NULL){
            printf("You entered a command that is not in the command menu\n");
            print_menu_dir(path,st);
        }
    }
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

void print_menu_link(const char *path, struct stat st){
    char option[10]; struct stat targeted_file;
    char letters_link[5]="nldta";
    printf("Choose what option do you want:\n n-file name\n l-delete link\n d-size of the link\n t-size of the target\n a-access rights\n ");
    scanf("%s", option);
    for(int i=0;i<strlen(option);i++){
        if(strchr(letters_link,option[i])==NULL){
            printf("You entered a command that is not in the command menu\n");
            print_menu_link(path,st);
        }
    }
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


void handle_regfile(const char *path, struct stat st){
    //int child_status;
    printf("%s is a regular file. \n", path);
    pid_t pid_reg=fork();
    if(pid_reg<0){
        perror("There is a problem with with creating the process for the regular file");
        exit(99);
    }
    else if(pid_reg==0){
        print_menu_regfile(path,st);
        exit(11);
    }
    else{
        pid_t pid_reg2 = fork();
        if(path[strlen(path)-1]=='c' && path[strlen(path)-2]=='.'){ 
            if(pid_reg2<0){
                perror("Error at the second fork in the regular file section");
                exit(EXIT_FAILURE);
            } 
            else if (pid_reg2 == 0){
                execlp("/bin/bash", "/bin/bash", "/mnt/c/Users/ancaa/Desktop/OSLab/OSLab/error_counter.sh", "arg1", NULL);
                exit(EXIT_SUCCESS);
            } 
        }
        /*else{
           
        }*/
    }
}

void handle_dir(const char *path, struct stat st){
    printf("%s is a directory. \n", path);
    pid_t pid_dir=fork();    
    //int status_dir;        
    if(pid_dir<0){
        perror("There is a problem with with creating the process for the directory");
        exit(1);
    }
    else if(pid_dir==0){
        print_menu_dir(path,st);
        exit(0);
    }
    else{
        pid_t pid_script=fork();
        if(pid_script<0){
            perror("There is a problem with the second child from directory");
            exit(1);
        }
        else if(pid_script==0){
            print_menu_dir(path,st);
        }
        /*else{
            pid_t w_link=wait(&status_dir);
            exit(EXIT_SUCCESS);
        }*/
    }    
}

void handle_sym(const char *path, struct stat st){
    printf("%s is a symbolic link. \n", path);
    pid_t pid_lnk=fork();
    //int status_sym;
    if(pid_lnk<0){
        perror("There is a problem with with creating the process for the symbolic link");
        exit(1);
    }
    else if(pid_lnk==0){
        print_menu_link(path, st);
        exit(0);
    }
    /*else{
        pid_t w_link=wait(&status_sym);
        exit(EXIT_SUCCESS);
    }*/
}

int main(int argc, char* argv[]){
    if(argc<2){
        perror("There are not enough arguments");
        exit(1);
    }
    for(int i=1;i<argc; i++){
        char *path=argv[i];
        struct stat st;
        
        if(lstat(path, &st)==-1){
            perror("There is a problem at lstat");
            continue;
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
        }
    }
    return 0;
}