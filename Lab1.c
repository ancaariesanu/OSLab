#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH_LENGHT 1024

void print_menu_regfile(const char *path, struct stat st){
    char mode;
    do{
        printf("Choose what option do you want:\n n-file name\n l-delete link\n d-size of the link\n t-size of the target\n a-access rights\n ");
        scanf("%s", &mode);
    }while(strcmp(mode, "n")==0 || strcmp(mode, "l")==0 || strcmp(mode, "d")==0 || strcmp(mode, "t")==0 || strcmp(mode, "n")==0);
    switch (mode){
    case 'n':
        printf("You choose to print the link name\n");
        
        break;
    
    case 'l':
       printf("You choose to delete link\n");
       
       break;

    case 'd':
        printf("You choose to print the size of the link\n");
        
        break;

    case 't': 
        printf("You choose to print the size of the target\n");
        
        break;

    case 'a':
        printf("You choose to print the access rights\n");
        printf("User:\n")
        if(stat.st_mode & S_IRUSR){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWUSR){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXUSR){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }

        printf("Group:\n")
        if(stat.st_mode & S_IRGRP){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWGRP){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXGRP){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }

        printf("Other:\n")
        if(stat.st_mode & S_S_IROTH){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWOTH){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXOTH){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }
        break;

    default:
        break;
    }
}

void print_menu_regfile(const char *path, struct stat st){
    char mode;
    do{
        printf("Choose what option do you want:\n n-file name\n d-dimension/size\n h-number of hard links\n m-time of last modification\n a-access rights\n l-create a symbolic link\n");
        scanf("%s", &mode);
    }while(strcmp(mode, "n")==0 || strcmp(mode, "d")==0 || strcmp(mode, "h")==0 || strcmp(mode, "m")==0 || strcmp(mode, "a")==0 || strcmp(mode, "l")==0 );
    switch (mode){
    case 'n':
        printf("You choose to print the name of the regular file\n");
        printf("Name: %s", path);
        break;
    
    case 'd':
       printf("You choose to print the size of the regular file\n");
       printf("%d",st.st_size)
       break;

    case 'h':
        printf("You choose to print the number of the hard links of the regular file\n");
        printf("%d",st.st_nlink)
        break;

    case 'm': 
        printf("You choose to print the last time of modification of the regular file\n");
        printf("%d",st.st_mtime)
        break;

    case 'a':
        printf("You choose to print the access rights of the regular file\n");
        printf("User:\n")
        if(stat.st_mode & S_IRUSR){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWUSR){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXUSR){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }

        printf("Group:\n")
        if(stat.st_mode & S_IRGRP){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWGRP){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXGRP){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }

        printf("Other:\n")
        if(stat.st_mode & S_S_IROTH){
            printf("Read-yes\n");
        }
        else{
            printf("Read-no\n");
        }
        if(stat.st_mode & S_IWOTH){
            printf("Write-yes\n");
        }
        else{
            printf("Write-no\n");
        }
        if(stat.st_mode & S_IXOTH){
            printf("Execute-yes\n");
        }
        else{
            printf("Execute-no\n");
        }
        break;

    case 'l':
        printf("You choose to create a symbolic link of the regular file\n");
        const char *new_path;
        symlink(path, new_path);
        break;

    default:
        break;
    }
}

int main(int argc, char* argv[]){
    if(argc<2){
        perror("There are not enough arguments");
        exit(1);
    }
    for(int i=0;i<argc; i++){
        char path[MAX_PATH_LENGHT];
        struct stat st;

        if(lstat(path, &st)==-1){
            perror("There is a problem at lstat");
            continue;
        }
        if(S_ISREG(st.st_mode)){
            printf("%s is a regular file. \n", path, st);
            print_menu_regfile(path, st);
        }
        if(S_ISDIR(st.st_mode)){
            printf("%s is a directory. \n", path, st);
            //print_menu_dir(path,st);
        }
        if(S_ISLNK(st.st_mode)){
            printf("%s is a symbolic link. \n", path, st);
            //print_menu_link(path, st);
        }
    }
    return 0;
}