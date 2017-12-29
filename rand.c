#include <stdio.h>

long long _rand(){

	static const char filename[] = "Random Number.txt";
	FILE *file = fopen(filename, "r");
	int count = 0;
	static int lineNumber = 0;
	long long rett = -1;
	
	if ( file != NULL ){
    	char line[256]; /* or other suitable maximum line size */
    	while (fgets(line, sizeof(line), file) != NULL){
    		if(count == lineNumber){
    			long long ret = 0 , i;
	        	for(i=0;line[i] != '\0';i++){
    	    		ret = ret*10 + (line[i]-'0');
        		}
        		lineNumber = (lineNumber + 1)%10000;
        		rett = ret;
    		}else{
    			count++;
    		}
	    }
	    fclose(file);
	}
	else{
    	printf("Unable to access file\n");
    	fclose(file);
	}
	return rett;
}

int main(){
	
}
