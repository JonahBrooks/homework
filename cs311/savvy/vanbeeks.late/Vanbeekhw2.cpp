/*
 * Original Author: Savannah Van Beek (vanbeeks)
 * File: uniquify.cpp
 * Created: 2012 Jan 30, 14:00 by vanbeeks
 * Last Modified: 2012 Feb 08, 22:37 by vanbeeks
 * 
 * This file contains functions that will parse a file and sort
 * it will do this with pipes.
 * I worked with Jonah Brooks. He helped me with general debugging
 * and helped explain topics in more detail.
 */
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctime>
#include <string>
#include <algorithm>
#include <sys/wait.h>
#include <cstring>
#include <cerrno>
#include <ctype.h>
#include <vector>

#define CHILDREN 50

int main (int argc, char *argv[]){
	int pfd[CHILDREN * 2][2];
	pid_t cpid;
	bool is_parent = true;
	char buff[1024];
	FILE *f_point[CHILDREN * 2];
	int Masteri = 0;
	bool is_finished = false;
	int small = 0;
	int count = 0;

	for (int i = 0; i < (CHILDREN * 2); i++){
		if(pipe(pfd[i]) == -1){
			std::cout << "Error Error Will Robinson!";
		}
		
	}
	
	//Makes all needed children
	while ((Masteri < (CHILDREN + 1)) && is_parent){
	//prevents fork bombs!
		cpid = fork();
		//if it is a child
		if (cpid == 0){
			is_parent = false;
			//if it will be body snatched by the sorter
			if (Masteri < CHILDREN){
				//closes most of the  unnecessary pipes
				for (int u = 0; u < CHILDREN; u++){
					close(pfd[u][1]);
					close(pfd[u + CHILDREN][0]);
					//closes the rest of the unnecessary pipes
					if (u != Masteri){
						close (pfd[u][0]);
						close (pfd[u + CHILDREN][1]);
					}	
				}

				//changes stdin and stdout
				dup2(pfd[Masteri][0], 0);
				dup2(pfd[Masteri + CHILDREN][1], 1);
				if (execl("/bin/sort", "sort", NULL) == -1){
					std::cout << "something bad happened";
				}
			}
			//if this child is the suppressor
			if (Masteri == CHILDREN){
				//closes the unnecessary pipes
				for (int w = 0; w < CHILDREN; w++){
					close(pfd[w][1]);
					close(pfd[w][0]);
					close(pfd[w + CHILDREN][1]);
					f_point[w] = fdopen(pfd[w + CHILDREN][0], "r");
				}
			
				std::vector<std::string> victor(CHILDREN);
				std::string last;
				//pulls from stream and stores in vector
				for (int i = 0; i < CHILDREN; i++){
					fgets(buff, 1024, f_point[i]);
					victor[i]= buff;
				}
				//loops through until all the pipes are empty
				while (is_finished == false){ 
					//goes through the array looking for smallest element
					for (int p = 0; p < CHILDREN; p++){
						if (victor[small].compare("") == 0
						    || (victor[p].compare("") != 0 
						    && victor[small].compare(victor[p]) >= 0)){
							small = p;
						}
					}
					//supppresses duplicates
					if (last.compare(victor[small]) != 0){
						std::cout << victor[small];
					}
					last = victor[small];
					//pulls up next element 
					if (fgets (buff, 1024, f_point[small]) == NULL){
						victor[small] = "";
					
					} else {
						victor[small] = buff;
					}
					//counts how many empty pipes there are
					for (int q = 0; q < CHILDREN; q++){
						if (victor[q].compare("") == 0){
							count++;
						}
					}
					//the exit condition
					if (count == CHILDREN){
						is_finished = true;
					}
					count = 0;
				}
			}		
		}
		Masteri++;
	}
	//if it is the parent
	if (cpid > 0){		
		//ANOTHER FOR LOOP! -to open streams
		for (int i = 0; i < CHILDREN; i++){
			close(pfd[i][0]);
			close(pfd[i + CHILDREN][1]);
			close(pfd[i + CHILDREN][0]);
			f_point[i] = fdopen(pfd[i][1], "w");
		}
		//parser start
		int y = 0;
		std::string things;
		while (std::cin >> things){
			things.append("\n");
			strcpy (buff, things.c_str());
			for (int i = 0; i < (int)strlen(things.c_str()); i++){
				buff[i] = (char)(tolower((int)buff[i]));
			}
			fputs(buff, f_point[y]);
			y++;
			if (y >= CHILDREN){
				y = 0;
			}	
		}

		for (int i = 0; i < CHILDREN; i++){
			fclose(f_point[i]);
		}
		//reaping souls!
		for (int i = 0; i < (CHILDREN + 1); i++){
			wait(NULL);
		}
	}
	return 0;

}

