#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]){
	
	FILE *in;
	FILE *out;
	FILE *burn;
	FILE *temperature;
	
	char *aut;
	char *brn;
	char *tpr;
	
	char bf1[255];
	
	
	aut = (char*) malloc(sizeof(argv[2]) + 4 * sizeof(char));
	brn = (char*) malloc(sizeof(argv[2]) + 8 * sizeof(char));
	tpr = (char*) malloc(sizeof(argv[2]) + 8 * sizeof(char));


	sprintf(aut,"%s.txt",argv[2]);
	sprintf(brn,"%s brn.txt",argv[2]);
	sprintf(tpr,"%s tpr.txt",argv[2]);


	in = fopen(argv[1],"r");
	out = fopen(aut,"w");
	
	
	
	
	while( (fgets(bf1,255,in) != NULL) ){
		
		if(strstr(bf1,"k-eff") != NULL){
			
			fprintf(out,bf1);
			
		};
		
		
		
	}; // */
	
	
	fclose(in);
	fclose(out);
	
	in = fopen(aut,"r");
	burn = fopen(brn,"w");
	temperature = fopen(tpr,"w");
	
	int counter=1;
	
	//printf("%d\n",counter);
	
	fgets(bf1,255,in);
	fgets(bf1,255,in);
	fgets(bf1,255,in);
	
	while(1){
		
		fgets(bf1,255,in);
		if( strstr(bf1,"Nominal") != NULL){
		}else if( strstr(bf1,"Branch 001") != NULL ) {
			break;
		} else { counter++;  };
		
	};//*/
	
	//printf("mi a faszér nem printel innentől?\n");
	//printf("%d\n",counter);
	
	
	rewind(in);
	fgets(bf1,255,in);
	int ic=0;
	
	while( (fgets(bf1,255,in) != NULL) ){
		
		
		if(strstr(bf1,"Nominal") != NULL){
		}else{
		
			if(ic<counter){ fprintf(temperature,bf1); ic++; };
			if(ic==counter){ fprintf(temperature,"\n\n\n");	ic=0; };
			
		}

	};
	
	
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
	
	remove(aut);
	
	return 0;
}