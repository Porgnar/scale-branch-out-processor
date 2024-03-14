#include <stdio.h>
#include <stdlib.h>
#include <string.h>




int main(int argc, char* argv[]){
	
	FILE *in;
	FILE *out;
	
	//we get input output file paths prepared by our main program ncomp
	in = fopen(argv[1],"r");
	out = fopen(argv[2],"w");
	
	char buffer[256];
	
	//read file line by line
	while( (fgets(buffer,256,in)) != NULL ){
		
		char ihatescale[64];
		int cntr=0;	
		
		if(strlen(buffer)>10){
			
			//painstakingly counted out the character positions of scale standard output lines, this can and should be modified in case it is necessary to do so!
			for(int i=15;i<25;i++){
				
				//copy character by character because the standard SCALE output is inconsistent with data types.... >:( 
				ihatescale[cntr]=buffer[i];			
				cntr++;
				
			}
			
			//these are the delimiter characters rewrite these to change the output formatting
			ihatescale[cntr]=' ';
			cntr++;			
		
			for(int i=40;i<46;i++){
				
				if(buffer[i]!=' '){
					ihatescale[cntr]=buffer[i];
					cntr++;
				}
			}
			
			ihatescale[cntr]=' ';
			cntr++;			
			
			for(int i=63;i<69;i++){
				
				ihatescale[cntr]=buffer[i];
				cntr++;
				
			}
			
			ihatescale[cntr]=' ';
			cntr++;
			
			for(int i=74;i<79;i++){
				
				ihatescale[cntr]=buffer[i];
				cntr++;
				
			}
			
			ihatescale[cntr]=' ';
			cntr++;
			
			for(int i=84;i<90;i++){
				
				ihatescale[cntr]=buffer[i];
				cntr++;
				
			}
			
			ihatescale[cntr]=' ';
			cntr++;
			
			for(int i=97;i<100;i++){
				
				ihatescale[cntr]=buffer[i];
				cntr++;
				
			}

			//manuálisan lezárjuk a stringet, hogy ne maradjon bent korábbi érték sneaky módon :"D
			ihatescale[cntr]='\0';
			//dump the whole line to out outfile and we're done! :)
			fprintf(out,"%s\n",ihatescale);
		}
		
	}
	
	
	fclose(in);
	fclose(out);
	
	return 0;
}
