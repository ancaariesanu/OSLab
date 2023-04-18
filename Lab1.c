#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_PATH_LENGHT 1024

void print_menu_dir(const char *path, struct stat st, struct dirent *d){
    char mode;
    do{
        printf("Choose what option do you want:\n n-file name\n d-size of the directory\n a-access rights\n c-total number of files with the C extension");
        scanf("%s", &mode);
    }while(strcmp(mode, "n")==0 ||strcmp(mode, "d")==0 || strcmp(mode, "a")==0 || strcmp(mode, "c")==0);
    switch (mode){
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

    case 'd':
        printf("You choose to print the total number of files with the C extension\n");
        int counter=0; DIR* dir;
        char* extension=".c";
        if((dir=opendir("."))!=NULL){
            while((dir=readdir(d))!=NULL){
                if(strstr(dir->d_name, extension)!=NULL) counter++;
            }
            closedir(dir);
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

void print_menu_link(const char *path, struct stat st){
    char mode; struct stat targeted_file;
    do{
        printf("Choose what option do you want:\n n-file name\n l-delete link\n d-size of the link\n t-size of the target\n a-access rights\n ");
        scanf("%s", &mode);
    }while(strcmp(mode, "n")==0 || strcmp(mode, "l")==0 || strcmp(mode, "d")==0 || strcmp(mode, "t")==0 || strcmp(mode, "a")==0);
    switch (mode){
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
        struct dirent *dir;

        if(lstat(path, &st)==-1){
            perror("There is a problem at lstat");
            continue;
        }
        if(S_ISREG(st.st_mode)){
            printf("%s is a regular file. \n", path);
            print_menu_regfile(path, st);
        }
        if(S_ISDIR(st.st_mode)){
            printf("%s is a directory. \n", path);
            print_menu_dir(path,st, dir);
        }
        if(S_ISLNK(st.st_mode)){
            printf("%s is a symbolic link. \n", path);
            print_menu_link(path, st);
        }
    }
    return 0;
}