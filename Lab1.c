#include <stdio.h>
#include <stdlib.h>
#define MAX_PATH_LENGHT 1024

void print_menu_regfile(const char* path){
    char mode;
    do{
        printf("Choose what option do you want:\n n-file name\n d-dimension/size\n h-number of hard links\n m-time of last modification\n a-access rights\n l-create a symbolic link\n");
        scanf("%s", &mode);
    }while(strcmp(mode, "n")==0 || strcmp(mode, "d")==0 || strcmp(mode, "h")==0 || strcmp(mode, "m")==0 || strcmp(mode, "a")==0 || strcmp(mode, "l")==0 );
    switch (mode){
    case 'n':
        printf("You choose to print the name of the regular file %s", path);
        break;
    
    case 'd':
        break;

    case 'h':
        break;

    case 'm':
        break;

    case 'a':
        break;

    case 'l':
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
            printf("%s is a regular file. \n", path);
            //print_menu(path);
        }
        if(S_ISDIR(st.st_mode)){
            printf("%s is a directory. \n", path);
            //print_menu(path,2);
        }
        if(S_ISLNK(st.st_mode)){
            printf("%s is a symbolic link. \n", path);
            //print_menu(path,3);
        }
    }
    return 0;
}