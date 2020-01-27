//Implementing select
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

extern FILE *fp;
FILE *schema_fp,*table_fp,*tabl_fp,*temp_fp,*newtable,*new_table;
int row,printcol_counter = 0;
char ch1[30];

int same_colnm_check(int attr[30],int col,int col_count)
{
	int i=0;
	while(i<col_count)
	{
		if(attr[i] == col)
		  return 1;
		i++;
	}
	return 0;
}

int do_select(char buff[30],char op[3],char val[10], int not)
{
	int n = strlen(op);
	switch(n)
	{
		case 1: if(strcmp(op,"<") == 0)
                 if(not)
                 {  if(strcmp(buff,val) == 1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) == -1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  if(strcmp(op,">") == 0)
		          if(not)
				  {
		            if(strcmp(buff,val) == -1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) == 1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  break;
        case 2: if(strcmp(op,"<=") == 0)
		          if(not)
				  {
		            if(strcmp(buff,val) == 1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) == 0 || strcmp(buff,val) == -1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }  
                  else
                  if(strcmp(op,">=") == 0)
		          if(not)
				  {
		            if(strcmp(buff,val) == -1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) == 0 || strcmp(buff,val) == 1)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  if(strcmp(op,"==") == 0)
		          if(not)
				  {
		            if(strcmp(buff,val) != 0)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) == 0)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  if(strcmp(op,"!=") == 0)
		          if(not)
				  {
		            if(strcmp(buff,val) == 0)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  else
                  {
                  	 if(strcmp(buff,val) != 0)
		            {
		            	row++;
		            	return 1;
		            }
		            else
		              return 0;
                  }
                  break;
        default: printf("Operator Incorrect");
                
	}
}


void select()
{
	char tabl_nm[30],colm_nm[30],query[30],tabl_nm_ext[30],col_nm[30],val[10],operation[3];
	char printcol[30][30];//array contain the col which are selected
	int condition = 1;
	int no_cond ;
	int colcount[30];
	
	fscanf(fp,"%s", query);  //read col or *
	
	if(strcmp(query,"*") == 0)//means selcase 1 or 2
	{
		strcpy(printcol[printcol_counter],query);
		fscanf(fp,"%s",query);         //read from
    }
	else//means after select their is a column names i.e selcase 3 or 4
	{
		char temp[30];
		int i = 0, j = 0;
		while(1)
		{
		  if(strcmp(query,",") != 0)
		  {
		  
		     while(query[i] != '\0')
		     {
		     	if(i == 0 && query[i] == ',')
		     	{
		     	    i++;
		     	    continue;
		        }
		  	    if(query[i] != ',')
			      temp[j++] = query[i++];
			    else
			    {
				   strcpy(printcol[printcol_counter],temp);
				   printcol_counter++;
				   strset(temp,'\0');
				   j = 0;
				   if(query[++i] == '\0')
				   {
				    	query[0] = '\0';
					    fscanf(fp,"%s",query);
					    if(strcmp(query,"from") == 0)
					       break;
					    else
					       i = 0;
				   }
			    }
		     }
		     if(strcmp(query,"from") == 0)
		        break;
		     else
		     {
		     
		     	strcpy(printcol[printcol_counter],temp);
		     	printcol_counter++;
		     	query[0] = '\0';
		        fscanf(fp,"%s",query);
		        if(strcmp(query,"from") == 0)
		           break;
		        else
		           i=j=0;
		     }
		     
		  }
		  else
		  {
		  	query[0] = '\0';
		  	fscanf(fp,"%s",query);
		  	strset(temp,'\0');
		  	j = i = 0;
		  }
	    }
	}
    fscanf(fp,"%s",query);//read table name
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
		condition = 0;
		//selcase=1;           
	}

	else
	{
	    fscanf(fp,"%s",query);//read ; or where
	    if(strcmp(query,";") == 0)//means selcase 1 or 3 
	     condition = 0;
    }
	
	if(check_table_exists(tabl_nm) == 0)   //checking the existence of table name
	{
		printf("\nTable %s does not exist in the database!!!", tabl_nm);
		printf("\n\n\t\tINVALID OPERATION\n");
		return;
	}

	
	int i,col_count;
	i=0;
    while(i<printcol_counter)
    {    
          col_count = columncount(tabl_nm,printcol[i]);//checking the existance of predicate column in the schema file
          if(col_count == 0)
	      {
	          printf("Invalid: %s doesnot contain %s\n",tabl_nm,col_nm);
	          return;
	      }
	      else
	      {
              colcount[i] = col_count;	
   	          printf("col %d : %s\n",i+1,printcol[i]);
   	          i++;
   	      }
    }
	if(condition)
	{
        fscanf(fp,"%s", query);//read column name of predicate condition
        if(strcmp(query,"not"))
        {
           fscanf(fp,"%s",query);
           no_cond=1;
        }
         i=0;
         while(query[i] != '\0')
         {      
		    if((query[i] == '<') || (query[i] == '>') || (query[i] == '=') || (query[i] == '!'))
		      break;
           
		   col_nm[i] = query[i];
		   i++;
         }
         col_nm[i] = '\0';	    
         col_count = columncount(tabl_nm,col_nm);//checking the existance of predicate column in the schema file
         if(col_count == 0)
         {
	        printf("Invalid: %s doesnot contain %s\n",tabl_nm,col_nm);
	        return;
         }	    
         if(query[i] == '\0')
         {
		    fscanf(fp,"%s",query);//read operator
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
	     	    if((query[i] == '\'')  || (query[i] == '\"'))
		           i++;
		        else
		           val[j++] = query[i++];//val contain the value of the predicate column condition
		         
	         }
	         query[0] = '\0';
	         fscanf(fp,"%s",query);//read ;
	         i = 0;
         }
         while(query[i] != '\0' && query[i] != ';')
         {
	        if((query[i] == '\'')  || (query[i] == '\"'))
		       i++;
		    else
		      val[j++] = query[i++];
         }
    }
    table_fp = fopen(tabl_nm_ext,"r");
    if(table_fp == NULL)
    {
    	printf("%s cannot be opened\n",tabl_nm);
    	return;
    }
    new_table = fopen("newtable.txt","w");
    temp_fp = fopen("temp.txt","w+");
    i = 0, row = 0;
    int write = 0, total_rows = 0;
    char buff[30];
    while(!feof(table_fp))
    {
		    query[0] = '\0';
		    fscanf(table_fp,"%s",query);//read values of the columns
			strset(buff,'\0'); 
		    while(strcmp(query,",") != 0 && !feof(table_fp) && strcmp(query,";") != 0 )
		    {
		    	strcat(buff,query);
		    	fprintf(temp_fp,"%s ",query);//write to the temporary file
		    	query[0] = '\0';
		    	fscanf(table_fp,"%s",query);//read next column value
		    }
		    if(!feof(temp_fp))
		       fprintf(temp_fp,"%s ",query);
     		i++;
     		if(condition)
		    {
		    	if(i == col_count)
		    	{
	    	      if(do_select(buff,operation,val,no_cond) == 1)
	    	        write = 1;
                }
            }
            else
                write = 1;
		    if(strcmp(query,";") == 0  || feof(table_fp))
		    {
		      i = 0;
		      total_rows++;
		      if(write == 1)
		      {
		   	    fseek(temp_fp,0,SEEK_SET);
		   	    while(!feof(temp_fp))
		   	    {
		   	 	   buff[0]='\0';
		   	 	   fscanf(temp_fp,"%s",buff);
		   	 	   fprintf(new_table,"%s ",buff);
		   	    }
		   	    write = 0;
		      }
		      fclose(temp_fp);
		      temp_fp = fopen("temp.txt","w+");
		    }		
    }
    
    fclose(temp_fp);
    fclose(new_table);
    fclose(table_fp);
    //PROJECTION
    
    schema_fp = fopen("schema.txt","r");
    if(schema_fp == NULL)
    {
    	printf("error in opening schema file\n");
    	return;
    }
    while(!feof(schema_fp))
    {
    	
		buff[0] = '\0';
	    fscanf(schema_fp,"%s",buff);
		if(strcmp(buff,"table") == 0)
		{
			fscanf(schema_fp,"%s",buff);
		    if(strcmp(buff,tabl_nm) == 0)
		       break;
		}
	}
	
	i = 0;
	strcpy(buff,",");
	while(strcmp(buff,"table") != 0 && !feof(schema_fp))
	{
		if(strcmp(buff,",") == 0)
		{
			i++;
			fscanf(schema_fp,"%s",buff);
			if(printcol_counter)
    		{
    		  if(same_colnm_check(colcount,i,printcol_counter) == 1)      // int
    		    printf("%8s%",buff);
    	    }
    	    else
    	      printf("%8s%",buff); 
	    }
	    buff[0] = '\0';
		fscanf(schema_fp,"%s",buff);
		 
    }
    
    fclose(schema_fp);
    printf("\n\n");
    
    i = 0;
    strcpy(buff,",");
    new_table = fopen("newtable.txt","r");
    if(new_table == NULL)
    {
    	printf("error in opening new table\n");
    	return;
    }
    while(!feof(new_table))
    {
    	if(strcmp(buff,",") == 0)
    	{
    		i++;
    		if(printcol_counter)
    		{
    		  if(same_colnm_check(colcount,i,printcol_counter) == 1)      // int
    		    write = 1;
    		  else
    		    write = 0;
    	    }
    	    else
    	       write = 1;
    	}
    	else if(strcmp(buff,";") == 0)
    	{
    		i = 0;
    		strcpy(buff,",");
    		printf("\n");
    	}
    	else
    	{
    		if(write)
    		  printf("%8s",buff);
    	}
    	buff[0] = '\0';
    	fscanf(new_table,"%s",buff);
    }
    fclose(new_table);     //remove
    if(condition)
    {
       if(row == 0)
          printf("\nNo row selected\n");
       else
          printf("\n%d rows selected\n",row);
    }
    else
       printf("\n%d rows selected\n",total_rows-1);
   	 remove("tamp.txt");
   	 remove("newtable.txt");
    return ;   
 }//end of main
