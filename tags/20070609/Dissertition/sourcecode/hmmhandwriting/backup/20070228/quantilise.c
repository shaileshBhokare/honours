/* this problem quantilises the training data
 * 
 * We divide the coordination into 16 equal area, and give identify each area from 0
 * to 15. Any two dots in the trainning set will form a shor line, and we call it featue.
 * What we do here is quantilise each feature. In other words, we use number 0 to 15 to
 * represent each feature.
 * 
 * we use arctan(y/x) to calculate the tangle.
 * case x>0, y>0    arctan(y/x) = A
 * case x<0, y>0	arctan(y/x) = pi+A
 * case x<0, y<0	arctan(y/x) = pi+A
 * case x>0, y<0	arctan(y/x) = 2pi+A
 */
 
  #include <stdio.h>
  #include <math.h>
  #include <string.h>
  #include <stdlib.h>
  
  int vector [16];
  
  int main()
  {
  	//initialize the array
  	int i;
  	for(i=0; i<16; i++){
  		vector[i] = 0;
  	}
  	
  	double x1,y1,x2,y2 = -1;
  	char *xtemp = "..........";
  	char *ytemp = "..........";
  
	double pi = 4.0*atan(1.0);
  	
  	char *file = "../../data/trainingData/1.1.txt";
  	
  	FILE * pFile;
  	char string[100];
  	
 	pFile = fopen(file,"r");
 	if(pFile == NULL) 
 	{
 		perror("Error opening file");
 		exit(1);
 	}		
  	
  	while (!feof(pFile))
  	{
  		
  		fgets(string, 100, pFile);
  		
  		//string handling
  		
  		if(strcmp(string, "<s>")==0){
  			printf("Stroke start\n");
  		}
  		else if(strcmp(string, "</s>")==0){
  			printf("Stroke end\n");
  		}
  		else //data handling
  		{
 			char *startBracket, *endingBracket, *comma;

  			//find three important positions
  			startBracket = strstr(string, "(");
  			endingBracket = strstr(string, ")");
  			comma = strstr(string, ",");
  			
  			printf("The String is %s", string);
  			
  			if(startBracket == NULL)
  			{
  				printf("No match was found.");
  				printf("\n");
  			}
  			else{
  				printf("( was found at position %d.", startBracket-string);
  				printf("\n");
  				printf(", was found at position %d.", comma-string);
  				printf("\n");
  				printf(") was found at position %d.", endingBracket-string);
  				printf("\n");
  				
  				//retrieve the x, y
  				
  				//isolate the useful data
  				printf("Running ok1");
  				strncpy(xtemp, string, comma-string-1);
  				printf("Running ok2");
  				strncpy(ytemp, comma+1, endingBracket-comma-1);
  				printf("Running ok3");
  				//cast data to double
//  				if(x1==-1){//initialise
//  					x1 = atof(xtemp);
//  					y1 = atof(ytemp);
//  				}else{
//  					x1 = x2;
//  					y1 = y2;
//  					x2 = atof(xtemp);
//  					y2 = atof(ytemp);
//  					
//  					double deltaX = x2 - x1;
//  					double deltaY = y2 - y1;
//  					double result = atan2(deltaY,deltaX);
//  					
//  					if(result<0){
//  						result = result + 2pi;
//  					}
//  					
//  					if(result<(pi/8)){
//  						vector[0]++;
//  					}else if(result<(2*pi/8)){
//  						vector[1]++;
//  					}else if(result<(3*pi/8)){
//  						vector[2]++;
//  					}else if(result<(4*pi/8)){
//  						vector[3]++;
//  					}else if(result<(5*pi/8)){
//  						vector[4]++;
//  					}else if(result<(6*pi/8)){
//  						vector[5]++;
//  					}else if(result<(7*pi/8)){
//  						vector[6]++;
//  					}else if(result<(8*pi/8)){
//  						vector[7]++;
//  					}else if(result<(9*pi/8)){
//  						vector[8]++;
//  					}else if(result<(10*pi/8)){
//  						vector[9]++;
//  					}else if(result<(11*pi/8)){
//  						vector[10]++;
//  					}else if(result<(12*pi/8)){
//  						vector[11]++;
//  					}else if(result<(13*pi/8)){
//  						vector[12]++;
//  					}else if(result<(14*pi/8)){
//  						vector[13]++;
//  					}else if(result<(15*pi/8)){
//  						vector[14]++;
//  					}else vector[15]++;;
// 				}
  			}
  		}
  	}
//  	printf("Thi this is the final result i=%d, ii=%d, iii=%d, iv=%d, v=%d, vi=%d, vii=%d, viii=%d, ix=%d, x=%d, xi=%d, xii=%d, xiii=%d, xiv=%d, xv=%d, xvi=%d",vector[0],vector[1],vector[2],vector[3],vector[4],vector[5],vector[6],vector[7],vector[8],vector[9],vector[10],vector[11],vector[12],vector[13],vector[14],vector[15]);
  	fclose(pFile);
  	
//  	printf("before");
//  	printf("%g",viii);
//  	printf("after");	
  	return 0;
  }