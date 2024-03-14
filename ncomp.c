#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */




int main(int argc, char *argv[]){
	
	FILE *in;
	FILE *out;
	FILE *burn;
	FILE *temperature;
	
		  
	char *aut;
	char *brn;
	char *tpr;
	
	
	char bf1[255];
	
	//allocate memory for output filenames
	aut = (char*) malloc( strlen(argv[2]) + 4 * sizeof(char));
	brn = (char*) malloc( strlen(argv[2]) + 8 * sizeof(char));
	tpr = (char*) malloc( strlen(argv[2]) + 8 * sizeof(char));

	//create output filenames
	sprintf(aut,"%s.txt",argv[2]);
	sprintf(brn,"%s brn.txt",argv[2]);
	sprintf(tpr,"%s tpr.txt",argv[2]);


	in = fopen(argv[1],"r");
	out = fopen(aut,"w");
	
	
	
	//select from original SCALE output only those lines, that contain the k-eff substring
	while( (fgets(bf1,255,in) != NULL) ){
		
		if(strstr(bf1,"k-eff") != NULL){
			
			fprintf(out,bf1);
			
		};
		
		
		
	};
	
	
	fclose(in);
	fclose(out);
	
	//first round of selection is done
	
	in = fopen(aut,"r");
	burn = fopen(brn,"w");
	temperature = fopen(tpr,"w");
	
	int counter=1;
	

	//we have to discard the k-eff predicted eigenvalue line
	fgets(bf1,255,in);
	//we have to discard the nominal branch, as its parameters are not listed, making it useless for graphing
	fgets(bf1,255,in);
	//we discard the first line that has branch 001 in it so our while doesn't terminate prematurely
	fgets(bf1,255,in);
	
	
	//we count how many branches we have, so we can use the program on any number of branches without editing sourcecode
	while(1){
		
		fgets(bf1,255,in);
		if( strstr(bf1,"Nominal") != NULL){
		}else if( strstr(bf1,"Branch 001") != NULL ) {
			break;
		} else { counter++;  };
		
	};
	
	//reset the file pointer to start of file and discard the k-eff prediction line
	rewind(in);
	fgets(bf1,255,in);
	int ic=0;
	
	
	//this while keeps the original scale order of data, just gets rid of unnecessary lines
	while( (fgets(bf1,255,in) != NULL) ){
		
		//with this if we discard every line that belongs to the nominal branch
		if(strstr(bf1,"Nominal") != NULL){
		}else{
		
			if(ic<counter){ fprintf(temperature,bf1); ic++; };
			if(ic==counter){ fprintf(temperature,"\n\n\n");	ic=0; };
			
		}

	};
	
	
	//this for sorts and prints the lines to the next temporary file using a different grouping method
	for(int i=0;i<counter;i++){
		
		rewind(in);
		fgets(bf1,255,in);
		
		int lc=0;
		
	
		while( (fgets(bf1,255,in) != NULL) ){
			
			if(strstr(bf1,"Nominal") != NULL){
			}else{
		
				if( (lc%counter) == i ){fprintf(burn,bf1);}
			
				lc++;
			
			}
			
		};
		
		fprintf(burn,"\n\n\n");
		
	};
	
	
	fclose(in);
	fclose(burn);
	fclose(temperature);
	
	//finish writing the 2nd generation of temporary files, and get rid of the obsolete first one
	
	remove(aut);
	
	//feedback for the user
	printf("Data trimmed from output file.\n");

	
	char *inname;
	char *outname;
	char *bfinal;
	char *tfinal;
	
	
	//declare final output name string with proper size	
	bfinal = (char*) malloc( strlen(argv[2]) + 8 * sizeof(char));
	tfinal = (char*) malloc( strlen(argv[2]) + 8 * sizeof(char));
	
	//create final output filenames
	sprintf(bfinal,"%s brn.prn",argv[2]);
	sprintf(tfinal,"%s tpr.prn",argv[2]);
	
	
	//store these names in another variable so calling shell commands, and external programs can happen ina  more uniform manner
	inname=brn;
	outname=bfinal;
	
	int pid;
	int status;
	
	
	//we fork the program, which creates two instances of itself, and in one of these we run the external command
	pid=fork();
	if(pid==0){
		//we compile the line formatter ourselves every time, so the user doesn't have to do it manually
		char *call[] = {"/usr/bin/gcc", "format.c", "-o", "puc", NULL};
		execv(call[0],call);
		exit(EXIT_SUCCESS);
		
	}
	
	//we wait for the fork to stop running
	waitpid(pid,&status,0);
	
	//this should tell us if our subprogram ran 
	if(WIFEXITED(status)){ 
	
		printf("Formatter compiled.\n");
	
	}else{ printf("Formatter compilation failed.\n"); }
	
	
	//same as before, but this time we don't compile, but rather use our freshly made executable tat will format us the text inside each line
	pid=fork();
	if(pid==0){
		
		char *call[] = {"./puc", inname, outname, NULL};
		execv(call[0],call);
		exit(EXIT_SUCCESS);
		
	}
	
	waitpid(pid,&status,0);
	
	if(WIFEXITED(status)){ 
	
		printf("Burn prn file created.\n");
	
	}else{ printf("Burn file creation failed.\n"); }
	
	inname=tpr;
	outname=tfinal;

	//same as before, but for the other type of sorted file
	pid=fork();
	if(pid==0){
		
		char *call[] = {"./puc", inname, outname, NULL};
		execv(call[0],call);
		exit(EXIT_SUCCESS);
		
	}
	
	waitpid(pid,&status,0);
	
	if(WIFEXITED(status)){
	
		printf("Temperature prn file created.\n");
	
	}else{ printf("Temperature file creation failed.\n"); }
	
	
	//we are done, we remove the second generation of temporary files, and the executable
	remove("puc");
	remove(brn);
	remove(tpr);


	
	return 0;
}