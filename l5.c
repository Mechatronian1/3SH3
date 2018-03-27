/*--- Chris Adams ---*/
/*---- 3SH3 Lab5 ----*/
/*-- March 27 2018 --*/

/*-------- Necessary Libraries ------------*/
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h> 
#include <stdio.h> 
#include <grp.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <limits.h>

/*----------- Initialize ---------------*/
void dirFunc(const char *name);

/*------------- Main Loop --------------*/
int main(int argc, char* argv[]){

	/*---- Ensure Right Number of Input Given -----*/
	//two expected
	if (argc != 2){
		//exit if incorrect amount
		printf("Two arguements expected");
		exit(0);
	}
	//Notify user correct amount inputed
	else {
		printf("the arg given is %s \n", argv[1]);
	}


	/*---- Make Data Structure For Data -----*/
	struct stat fileData;
	if(stat(argv[1], &fileData)<0){
		perror("error, no stat\n");
		exit(0);
	}	
	

	/*------------- Check if File --------------*/
	if(S_ISREG(fileData.st_mode)){
	//search used database for a user ID
	struct passwd *pwd;
	if ((pwd = getpwuid(fileData.st_uid)) != NULL)
		printf("User ID: %-8.8s \n", pwd ->pw_name);
	else
		printf(" %-8d \n", fileData.st_uid);
	
	//group database entry for a group ID
	struct group *grp; 
	if ((grp = getgrgid(fileData.st_gid)) != NULL)
		printf("Group ID: %-8.8s \n", grp -> gr_name);
	else
		printf( "%-8d \n", fileData.st_gid);

	//Print statements
		printf("Name: %s\n",argv[1]);
		printf("Permissions: %o\n", fileData.st_mode);
		printf("Number of Links: %lu\n",fileData.st_nlink);
		printf("File Size: %ld\n",fileData.st_size);
		printf("Size in Blocks: %ld\n",fileData.st_blocks/8);
		printf("Last Modification Time: %s",ctime(&fileData.st_mtime));
	}

	/*----------------- Check if Directory ------------*/
	if(S_ISDIR(fileData.st_mode)){
		char cwd[1024];
		getcwd(cwd, sizeof(cwd));
		strcat(cwd, "/");
		strcat(cwd, argv[1]);
		dirFunc(cwd);
		exit(0);
	}
}

/*----------- Recursive Function for Checking Directory --------------*/
void dirFunc(const char *name){
	char cwd[1024];
	printf("This is a Directory \n");
	DIR *pDir;
	struct dirent *pDirent;
	
	pDir = opendir(name);
	if (pDir == NULL){
		perror("cannot open directory\n");
	}
	
	while ((pDirent = readdir(pDir)) != NULL){
		//Print file names
		if ((strcmp(".",pDirent->d_name)!=0) && (strcmp("..",pDirent->d_name)!=0)){
			printf("%s\n", pDirent->d_name);
			}
		//if its a directory go into it
		if ((strcmp(".",pDirent->d_name)!=0) && (strcmp("..",pDirent->d_name)!=0) && (pDirent->d_type == DT_DIR)){
			char path[PATH_MAX];
			snprintf(path, PATH_MAX, "%s/%s",name,pDirent->d_name);
			dirFunc(path);
		}
	}
	closedir (pDir);
}
