/*************************************MAIN FILE****************************************/


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "header.h"


extern int yyparse(void);
extern FILE *yyin;
//extern int yydebug;
extern struct yy_buffer_state* curr_buff();
extern void yy_flush_buffer(struct yy_buffer_state*);
extern void select();
extern void create();
extern void deletion();
extern void insert();

char query[50];
FILE *fp;

int main()
{
    int parse_k, i = 0;

//    clrscr();    
    printf("\n\n..............................MINI DBMS....................................\n");
    printf("\nUse create/insert/select/delete commands and type exit and press enter key to exit.\n");
    
    
    fp = fopen("input.txt", "w");
    if(fp == NULL)
    {
	   printf("\nFile could not be opened in write mode!!!\n");
	   return 1;
    }
    printf("\nMINIDBMS>");

    // read query from standard input
    fgets(query, 50, stdin);
    
    // convert query to lower case
    while(query[i] != '\0')
    {
       query[i] = tolower(query[i]);
       i++;
    }
    
    // while user does not type exit and presses enter key
    while(strcmp(query,"exit\n") != 0)
    {
    	parse_k = 0; // a value of 0 => parsing did not work i.e. incorrect syntax
    	// This value is set if the syntax is correct
	    
	    // write query to file input.txt
	    fprintf(fp, "%s", query);
	    
	    // while the user does not enter a semicolon (which marks the end of a query)
	    // keep reading query from standard input, convert it to lower case
	    // and write it to file
	    while(strchr(query,';') == NULL)
	    {
	        fgets(query, 50, stdin);

	        i = 0;
	        while(query[i] != '\0')
		    {
		       query[i] = tolower(query[i]);
		       i++;
	        }
            fprintf(fp, "%s", query);
	    }

	    fclose(fp);
	    
	    //yydebug = 1;
	    
	    // For parsing
	    yyin = fopen("input.txt","r");
        if (yyin == NULL) 
	    {
               printf("\nFile could not be opened in read mode!!!\n");
               return 1;
        }   
        yy_flush_buffer(curr_buff());
        if (yyparse() == 0) /* parsing worked */
    	    parse_k = 1;
        else
        {
    	   printf("\nIncorrect Syntax\n");
    	   parse_k = 0; 
        } 
        while(!feof(yyin))
		   yyparse();   
		 
        fclose(yyin);
        
        // Call appropiate functions according to the query entered if parsing worked
        if(parse_k)
 	    {
	          fp = fopen("input.txt", "r");
	          if(fp == NULL)
			  {
				   printf("\nFile could not be opened in read mode!!!\n");
				   return 1;
		      }
              fscanf(fp, "%s", query);
            
              if(strcmp(query,"create") == 0)
                create();
              else if(strcmp(query,"insert") == 0)
                insert();
              else if(strcmp(query,"select") == 0)
                select();
              else
                deletion();
			   	
              fclose(fp);
	     }
	     // otherwise display correct syntax to the user.
	     else
	     {
	     	fp = fopen("input.txt", "r");
	     	if(fp == NULL)
		    {
			   printf("\nFile could not be opened in read mode!!!\n");
			   return 1;
		    }
		    
			fscanf(fp, "%s", query);
        
            if(strstr(query,"create") != NULL)
               usage(1);
            else if(strstr(query,"insert") != NULL)
               usage(2);
            else if(strstr(query,"select") != NULL)
               usage(3);
            else if(strstr(query,"delete") != NULL)
               usage(4);
            else
               usage(5); // to match default case
		   	
          fclose(fp);
	     }
	        
	     // For next query
         fp = fopen("input.txt", "w");  
		 if(!fp)
         {
    	     perror("\nFile could not be opened in write mode.\n");
    	     exit(1);
         }
		      
         printf("\n\nMINIDBMS>");
         fgets(query, 50, stdin);
         
         i = 0;
         while(query[i] != '\0')
	     {
	        query[i] = tolower(query[i]);
	        i++;
	     }
    }   
    return 0;
}
