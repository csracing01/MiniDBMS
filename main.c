#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

extern int yyparse(void);
extern FILE *yyin;
extern int yydebug;
extern void select();
extern void create();
extern void deletion();
extern void insert();

char query[50];
FILE *fp;

int main()
{
    int parse_k;
    fp = fopen("input.txt", "w");
    if(fp == NULL)
    {
	   printf("\nFile could not be opened!!!");
	   exit(1);
    }
    printf("\nMINIDBMS>");
    fgets(query, 20, stdin);
    
    while(strcmp(query,"exit\n") != 0)
    {
    	parse_k = 0;
	    size_t n;
	    n = strlen(query);
	    int i = 0;
	    for(i=0;i<n;i++)
	       query[i]=tolower(query[i]);

	    fprintf(fp, "%s", query);	    
	    
	    while(query[n-2] != ';')
	    {
	        fgets(query, 20, stdin);
            fprintf(fp, "%s", query);
	        n = strlen(query);
	    }
	    fclose(fp);
	    yydebug = 1;
	    yyin = fopen("input.txt","r");
        if (yyin == NULL) 
	        {
               printf("\nFile could not be opened!!!");
            }   
        
        if (yyparse() == 0) /* parsing worked */
    	parse_k = 1;
        else
        {
    	   printf("Incorrect Syntax\n");
    	   parse_k = 0;
        } 
            
        fclose(yyin);
        yyin = fopen("temp.txt","w");
        fflush(yyin);
        fclose(yyin);
        remove("temp.txt");
        if(parse_k)
 	    {
	          fp = fopen("input.txt", "r");
              fscanf(fp, "%s", query);
            
              if(strcmp(query,"create") == 0)
                create();
              else if(strcmp(query,"insert")==0)
                insert();
              else if(strcmp(query,"select")==0)
                select();
              else
                delete();
			   	
              fclose(fp);
	     }
	        
         fp = fopen("input.txt", "w");       
         printf("\nMINIDBMS>");
         fgets(query, 20, stdin);
    }   
    return 0;
}
    
