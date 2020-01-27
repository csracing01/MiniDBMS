/******************************************DELETION*************************************/

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

char query[50];
extern FILE *fp;
extern int row;


void deletion()
{
	int condition=1,not_cond=0;
    char tabl_nm[30],tabl_nm_ext[30],col_nm[30],val[10],operation[3],type[50];
    strset(type,'\0');
    row = 0;
	fscanf(fp,"%s", query);
	fscanf(fp,"%s", query);
	strcpy(tabl_nm,query);
	strcpy(tabl_nm_ext,query);
	strcat(tabl_nm_ext,".txt");
	int n = strlen(query);
	if(query[n-1] == ';')
	{
		query[n-1] = '\0';
		strcpy(tabl_nm,query);
		strcpy(tabl_nm_ext,query);
		strcat(tabl_nm_ext,".txt");
		condition=0;           // delete all rows
	}
	else
	{
	    fscanf(fp,"%s",query);
	    if(strcmp(query,";") == 0)
	      condition=0;
    }
	FILE *table_fp, *schema_fp, *new_table, *temp_fp;
	char ref_value[30][50],ref_table[30],ref_col[30];
	if(table_exist(tabl_nm) == 0)
	{
		printf("Invalid: %s doesnot exist",tabl_nm);
		return;
	}
	int col_count=0,ref_count = 0, ref_val_count = 0;
	if(!condition)
	{
		if(remove(tabl_nm_ext) != 0)
		{
			printf("\nNo rows in table %s\n",query); ///////////////////////////////////////////////
			return;
		}
		else
	      printf("All rows deleted\n");
	      
	    schema_fp = fopen("schema.txt","r");
	    char temp[30];
	    while(!feof(schema_fp))
	    {
		   temp[0] = '\0';
		   fscanf(schema_fp,"%s",temp);
		   if(strcmp(temp,"table") == 0)
		   {
			  fscanf(schema_fp,"%s",temp);
		      if(strcmp(temp,tabl_nm) == 0)
		         break;
		   }
	    }
	    fscanf(schema_fp,"%s",temp);
	    while(strcmp(temp,"table") != 0 && !feof(schema_fp))
	    {  
		   if(strcmp(temp,"refby") == 0)
		   {
		   	 ref_count = 1;
			 fscanf(schema_fp,"%s",ref_table);
			 fscanf(schema_fp,"%s",ref_col);
			 break;
	       }
	       temp[0] = '\0';
	       fscanf(schema_fp,"%s",temp);
	    }
	    fclose(schema_fp);
	}
	else
	{
		fscanf(fp,"%s", query);
	    if(strcmp(query,"not") == 0)
	    {
		   not_cond=1;
		   fscanf(fp,"%s",query);
	    }
	    int i=0;
	    //printf("%s\n",query);
	    while(query[i] != '\0')
	    {
		   if((query[i] == '<') || (query[i] == '>') || (query[i] == '=') || (query[i] == '!'))
		      break;
		   col_nm[i] = query[i];
		   //printf("%d %c \n",i,col_nm[i]);
		   i++;
	    }
	    col_nm[i] = '\0';
	    
	    col_count = columncount(tabl_nm,col_nm);
	    if(col_count == 0)
	    {
	       printf("Invalid: %s doesnot contain %s\n",tabl_nm,col_nm);
	       return;
	    }
	    
	    schema_fp = fopen("schema.txt","r");
	    char temp[30];
	    while(!feof(schema_fp))
	    {
		   temp[0] = '\0';
		   fscanf(schema_fp,"%s",temp);
		   if(strcmp(temp,"table") == 0)
		   {
			  fscanf(schema_fp,"%s",temp);
		      if(strcmp(temp,tabl_nm) == 0)
		         break;
		   }
	    }
	
	    fscanf(schema_fp,"%s",temp);
	    int count=1;
	    while(strcmp(temp,"table") != 0 && !feof(schema_fp))
	    {
		   if(strcmp(temp,",") == 0)
		   {
		     fscanf(schema_fp,"%s",temp);
			 count++;
	       }
		   if(strcmp(temp,"refby") == 0)
		   {
		   	 ref_count = count;
			 fscanf(schema_fp,"%s",ref_table);
			 fscanf(schema_fp,"%s",ref_col);
			 //break;
	       }
	       if(count == col_count)
	       {
	       	if(strcmp(type,"\0") == 0)
	       	{
	       	
	       	   fscanf(schema_fp,"%s",temp);
	           strcpy(type,temp);
	           if(!feof(schema_fp))
	           {
	             fscanf(schema_fp,"%s",temp);
	             if(strcmp(temp,",") != 0 && strcmp(temp,"not") != 0 && strcmp(temp,"primary") != 0 && strcmp(temp,"unique") != 0 && 
			             strcmp(temp,"table") != 0 )
			     {
			     	strcat(type," ");
			     	strcat(type,temp);
			     }
		       }
		    }
	       }
	       temp[0] = '\0';
	       fscanf(schema_fp,"%s",temp);
	    }
	    fclose(schema_fp);
	    
	    //printf("%d %c \n",i,query[i]);
	    if(query[i] == '\0')
	    {
		  fscanf(fp,"%s",query);
		  i=0;
	    }
	    operation[0] = query[i++];
	    if(query[i] == '=')
	    {
	      operation[1] = query[i++];
	      operation[2] = '\0';
        }
        else
          operation[1] = '\0';
	    //printf("%s \n",operation);
	    if(query[i] == '\0')
	    {
		   fscanf(fp,"%s",query);
	 	   i = 0;
	    }
	    int j = 0;
	    strset(val,'\0');
	    while(!feof(fp))
	    {
	       while(query[i] != '\0' && query[i] != ';')
	       {
	     	  /*if((query[i] == '\'')  || (query[i] == '\"'))
		         i++;
		      else*/
		         val[j++] = query[i++];
		         //printf("\n%c\n",val[j-1]);
	       }
	       if(query[i]==';')
	         break;
	       query[0] = '\0';
	       fscanf(fp,"%s",query);
	       //printf("\n%s\n",query);
	       i = 0;
        }
        val[j]='\0';
        /*while(query[i] != '\0' && query[i] != ';')
	    {
	       /*if((query[i] == '\'')  || (query[i] == '\"'))
		       i++;
		   else
		   
		      val[j++] = query[i++];
		      //printf("\n%c\n",val[j-1]);
	    }*/
	    //printf("\ntype : %s\nvalue : %s\n",type,val);
        if(type_check(type,val))
        {
        	printf("\nInvalid syntax: type of given value doesnot match\n");
        	return;
        }
        strcpy(tabl_nm_ext,tabl_nm);
		strcat(tabl_nm_ext,".txt");
        table_fp = fopen(tabl_nm_ext,"r");
        if(table_fp == NULL)
        {
    	  printf("\nEMPTY. No row inserted in table %s.\n",tabl_nm);
    	  return;
        }
        new_table = fopen("newtable.txt","w");
        temp_fp = fopen("temp.txt","w+");
        i = 0;
	    int write = 1;
        char buff[30];
	    while(!feof(table_fp))
	    {
		    query[0] = '\0';
		    fscanf(table_fp,"%s",query);
		    //printf("%s\n",query);
	
			strset(buff,'\0'); 
		    while(strcmp(query,",") != 0 && !feof(table_fp) && strcmp(query,";") != 0 )
		    {
		    	strcat(buff,query);
		    	//printf("buff from file: %s\n",buff);
		    	fprintf(temp_fp,"%s ",query);
		    	query[0] = '\0';
		    	fscanf(table_fp,"%s",query);
		    }
		    if(!feof(temp_fp))
		       fprintf(temp_fp,"%s ",query);

			i++;
			if(i == ref_count)
			{
				  strcpy(ref_value[ref_val_count],buff);
				  //printf("ref value: %s\n",ref_value[ref_val_count]);
				  //printf("ref_val_count in i==ref_count : %d\n",ref_val_count);
				  ref_val_count++;
			}
		    if(i == col_count)
		    {
	    	   if(check(buff,operation,val,not_cond) == 1)
		    	      write = 0;
		    	else
		    	      ref_val_count--;
		    	//printf("ref_val_count in i==col_count : %d\n",ref_val_count);
		    }
		    
		    if(strcmp(query,";") == 0  || feof(table_fp))
		    {
		      i = 0;
		      if(write == 1)
		      {
		   	    fseek(temp_fp,0,SEEK_SET);
		   	    while(!feof(temp_fp))
		   	    {
		   	 	   buff[0]='\0';
		   	 	   fscanf(temp_fp,"%s",buff);
		   	 	   //printf("buff from temp: %s ",buff);
		   	 	   fprintf(new_table,"%s ",buff);
		   	    }
		      }
		      else
		        write = 1;
		      fclose(temp_fp);
		      temp_fp = fopen("temp.txt","w+");
		    }		
        }
    
        fclose(temp_fp);
        fclose(new_table);
        fclose(table_fp);
        if(remove("temp.txt") != 0)
        printf("Error in deleting temperary file\n");
        if(remove(tabl_nm_ext) != 0)
           printf("Error in deleting old table\n");
        if(rename("newtable.txt",tabl_nm_ext) != 0)
           printf("Error in renaming new table\n");
        
        if(row == 0)
          printf("No row deleted\n");
        else
        if(row == 1)
           printf("\n1 row deleted\n");
        else
          printf("%d rows deleted",row);
    }
    
    if(ref_count)
    {
    	ref_count = columncount(ref_table,ref_col);
    	strcpy(tabl_nm_ext,ref_table);
    	strcat(tabl_nm_ext,".txt");
    	table_fp = fopen(tabl_nm_ext,"r");
    	if(table_fp==NULL)
    	{
    		printf("table noooooooooooot opened\n");
    		return;
    	}
    	char temp[30],buff[30];
    	int i=0;
    	new_table = fopen("newtable.txt","w");
        if(table_fp != NULL)
        {
        	while(!feof(table_fp))
	        {
		       query[0] = '\0';
               fscanf(table_fp,"%s",query);
	           strset(buff,'\0');
			   strset(temp,'\0'); 
		       while(strcmp(query,",") != 0 && !feof(table_fp) && strcmp(query,";") != 0 )
		       {
		    	  strcat(buff,query);
		          strcat(temp,query);
		          strcat(temp," ");
		    	  query[0] = '\0';
		    	  fscanf(table_fp,"%s",query);
		       }
		       
               i++;
              // printf("ref_value :%s\n",ref_value[0]);
               if(i == ref_count)
               {
                 if(!condition)
                    strcpy(temp,"null");
                 else
                 {
                  if(same_colnm_check(ref_value,buff,ref_val_count+1) == 1)
                         strcpy(temp,"null");
                 }
               }
               if(!feof(table_fp))
		   	 	   fprintf(new_table,"%s %s ",temp,query);
		   	   if(strcmp(query,";") == 0)
		   	      i = 0;
	
		   }
        }

        fclose(new_table);
        fclose(table_fp);
    
        if(remove(tabl_nm_ext) != 0)
            printf("Error in deleting old table\n");
        if(rename("newtable.txt",tabl_nm_ext) != 0)
            printf("Error in renaming new table\n");
    }
    return;
}	
