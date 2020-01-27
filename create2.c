#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include <math.h>
//#include "header.h"


char query[50];
extern FILE *fp;

/*int same_colnm_check(char attr[][30],char *col,int col_count)
{
	int i=0;
	while(i<col_count)
	{
		printf("col: %s\n",attr[i]);
		if(strcmp(attr[i],col) == 0)
		  return 1;
		i++;
	}
	return 0;
}*/

void create()
{
    char tabl_nm[30],colm_nm[30];
	fscanf(fp,"%s", query);
	fscanf(fp,"%s", query);
	
	if(table_exist(query) == 1)//checking the existance of table name
	{
		printf("Invalid: %s already exist",query);
		return;
	}
	if(strcmp(query,"schema") == 0 || strcmp(query,"temp") == 0 || strcmp(query,"input") == 0 || strcmp(query,"newtable") == 0 )
	{
		printf("Invalid: %s used as reserved file, cannot be file name.\n");
		return;
	}
	FILE *schema_fp,*temp_fp,*table_fp;
	temp_fp = fopen("temp.txt", "w");
	fprintf(temp_fp,"table %s ",query);
    strcpy(tabl_nm,query);
	char attr[20][50];
	int attr_name=1,foreign=0,primary=0;
	int type_check = 0,col_count=0;
	char tb_nm[10],col_nm[10];
    while(!feof(fp))
	{
		int i;
        query[0] = '\0';
	    fscanf(fp,"%s", query);
	    if(strcmp(query,"(") == 0 || strcmp(query,")") == 0)
	       continue;
	    
        char token[100];
        for(i=0; i<100; i++)
		      token[i] = '\0';
		      
        i = 0;
        int j = 0;
        while(query[i] != '\0')
        {
		   	if(query[i] == ',')
	   	    {
                token[j++] = ' ';
                token[j++] = ',';
                token[j++] = ' ';
            }
            else if(query[i] != '(' && query[i] != ')' && query[i] != ';')
		   	    token[j++] = query[i];
		   	else
		   	    token[j++] = ' ';
		   	i++;
        }	

        char *ptr;
        ptr = strtok(token, " ");
        while(ptr != NULL)
        {
        	  if(attr_name)
        	  {
        	  	if(strcmp(ptr,"foreign") != 0 && foreign < 1)
				{
        	  	  if(same_colnm_check(attr,ptr,col_count) == 0)
        	  	  {
        	  	    //attr[col_count]=(char *)malloc(sizeof(*ptr));
        	  	    strcpy((attr[col_count]),ptr);
        	  	
        	  	    //printf("col : %s",attr[col_count]);
        	  	    col_count++;
        	  	    attr_name = 0;
        	      }
        	      else
        	      {
        	      	printf("Invalid: SAME COLUMN NAME NOT ALLOEWD.\n");
        	      	fclose(temp_fp);
        	      	return;
        	      }
        	    }
        	    else
        	    {
        	    	foreign++;
        	    	if(foreign == 3)
        	    	{
        	    	  strcpy(colm_nm,ptr);
        	    	  if(same_colnm_check(attr,ptr,col_count) == 0)
        	    	  {
        	    	  	printf("Invalid: COLUMN NAME DONT MATCH\n");
        	    	  	fclose(temp_fp);
        	    	  	return;
        	    	  }
        	        }
        	    	if(foreign == 5)
        	    	  strcpy(tb_nm,ptr);
        	    	if(foreign == 6)
        	    	  strcpy(col_nm,ptr);
        	    		
        	    }
              }
              if(type_check)
              {
              	type_check = 0;
              	if(strcmp(ptr,"not") != 0 && strcmp(ptr,"unique") != 0 && strcmp(ptr,"primary") != 0 && strcmp(ptr,",") != 0 && !feof(fp))
              	{
              		int len;
              		if(*(ptr+1) == '\0')
		            {
		               len = *(ptr);
		               len -= 48;
		            }
		            else
		            {
		            	int k = 1;
                        while(*(ptr+k) != '\0')
			            k++;

			            int v = k;
			            k=1;
			            while(*(ptr+k) != '\0')
			            {
				           len += (*(ptr+k) - 48) * pow(10,v);
				           v--;
				           k++;
			            }
		            }
		            
		            if(len<=0)
		            {
		            	printf("\nInvalid syntax: size must be positive\n" );
		            	fclose(temp_fp);
		            	return;
		            }
              	}
              }
              if(strcmp(ptr,"char") == 0 || strcmp(ptr,"varchar") == 0 || strcmp(ptr,"double") == 0 )
                  type_check = 1;
        	  if(strcmp(ptr,",") == 0)
        	      attr_name = 1;
        	  if(strcmp(ptr,"primary") == 0)
        	  {
        	  	primary++;
        	  	if(primary>1)
        	  	{
        	  		printf("Invalid: TWO PRIMARY KEYS NOT ALLOWED\n");
        	  		fclose(temp_fp);
        	  		return;
        	  	}
        	  }
		      //printf("%s ", ptr); 
			  fprintf(temp_fp,"%s ",ptr);    //attr, type, constraint write to schema file 
		      ptr = strtok(NULL, " ");  
              
        }
    }
    fclose(temp_fp);
    //fclose(fp);
    
    if(foreign == 6)
    {
    	if(table_exist(tb_nm) == 0)//checking the existance of table name
	    {
		printf("Invalid: %s doesnot exist",tb_nm);
		return;
	    }
		schema_fp = fopen("schema.txt","a+");  	
        FILE *new_fp=fopen("newschema.txt","w"); 
        while(!feof(schema_fp))
        {
        	query[0]='\0';
        	fscanf(schema_fp,"%s",query);
        	if(strcmp(query,"table") == 0)
        	{
        		fscanf(schema_fp,"%s",query);
        		if(strcmp(query,tb_nm) == 0)
        		{
        		   fprintf(new_fp,"%s %s ","table",query);
        		   break;
        	    }
        	    else
        	    fprintf(new_fp,"%s %s ","table",query);
        	}
        	else
        	fprintf(new_fp,"%s ",query);
        }
        while(strcmp(query,"table") != 0)
        {
        	query[0]='\0';
            fscanf(schema_fp,"%s",query);
            fprintf(new_fp,"%s ",query);
			if(strcmp(query,col_nm) == 0)
    		   break;	
        }
        if(strcmp(query,"table") == 0)
        {
        	printf("Invalid: %s doesnot exists in table %s.",col_nm,tb_nm);
        	fclose(schema_fp);
        	fclose(new_fp);
        	if(remove("newschema.txt") != 0)
        	   printf("Error in deleting new schema\n");
        	return;
        }
        else
        {
        	primary = 0;
        	while(!feof(schema_fp))
            {
        	   query[0]='\0';
               fscanf(schema_fp,"%s",query);
               if(strcmp(query,"primary") == 0)
                  primary = 1;
               if(strcmp(query,",") == 0)
                  break;
               fprintf(new_fp,"%s ",query);
            }
            if(!primary)
            {
            	printf("Invalid : Foreign key violation.%s is not primary key",col_nm);
            	fclose(schema_fp);
        	    fclose(new_fp);
        	    if(remove("newschema.txt") != 0)
        	       printf("Error in deleting new schema\n");
        	    return;
            }
            fprintf(new_fp,"%s ","refby");
            fprintf(new_fp,"%s %s ",tabl_nm,colm_nm);
            fprintf(new_fp,"%s ",query);
            while(!feof(schema_fp))
            {
            	query[0]='\0';
        	    fscanf(schema_fp,"%s",query);
        	    fprintf(new_fp,"%s ",query);
            }
            fclose(schema_fp);
        	fclose(new_fp);
        	if(remove("schema.txt") != 0)
        	   printf("Error in deleting old schema\n");
        	if(rename("newschema.txt","schema.txt") != 0)
        	   printf("Error in renaming new schema file\n");
        	
        }
    }
    table_fp=fopen("table.txt","a+");
    fprintf(table_fp,"%s ", tabl_nm);
    fclose(table_fp);
    temp_fp=fopen("temp.txt","r");
    schema_fp=fopen("schema.txt","a+");
    while(!feof(temp_fp))
    {
    	query[0]='\0';
    	fscanf(temp_fp,"%s",query);
    	fprintf(schema_fp,"%s ",query);
    }
    fclose(schema_fp);
    fclose(temp_fp);
    printf("\nTable created\n");
    return;
}

