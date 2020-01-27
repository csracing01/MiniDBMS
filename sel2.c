/***********************************SELECTION*********************************/


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

extern FILE *fp;
extern int row;
FILE *schema_fp,*table_fp,*tabl_fp,*temp_fp,*newtable,*new_table;
int printcol_counter = 0;
char ch1[30];

int same_colnm_check1(int attr[30],int col,int col_count)
{
	int i=0;
	while(i < col_count)
	{
		if(attr[i] == col)
		  return 1;
		i++;
	}
	return 0;
}


void select()
{
	char tabl_nm[30],colm_nm[30],query[30],tabl_nm_ext[30],col_nm[30],val[10],operation[3],type[50];
	char printcol[30][30];//array contain the col which are selected
	int condition = 1;
	int no_cond = 0,i ;
	int colcount[30];
    
	for(i=0;i<30;i++)
	{
	   strset(printcol[i],'\0');
	   colcount[i] = 0;
    }
	printcol_counter=0;	
	fscanf(fp,"%s", query);  //read col or *
	
	if(strcmp(query,"*") == 0)
	{
		strcpy(printcol[printcol_counter],query);
		fscanf(fp,"%s",query);        //read from
    }
	else//means after select there are column names 
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
				   temp[j] = '\0';
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
		     	temp[j] = '\0';
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
	    i=0;
	    while(i<printcol_counter)
	    {
	    	//printf("col %d : %s\n",i+1,printcol[i]);
	    	i++;
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
	           
	}

	else
	{
	    fscanf(fp,"%s",query);//read ; or where
	    if(strcmp(query,";") == 0)
	     condition = 0;
    }
	
	if(table_exist(tabl_nm) == 0)//checking the existance of table name
	{
		printf("Invalid: %s doesnot exist",tabl_nm);
		return;
	}
	int col_count = 0;
	i=0;
//	printf("total col to display : %d\n",printcol_counter);
    while(i<printcol_counter)
    {    
          col_count = columncount(tabl_nm,printcol[i]);//checking the existance of projection column in the schema file
          //printf("colcolunt : %d\n",col_count);
          if(col_count == 0)
	      {
	          printf("Invalid: %s doesnot contain %s\n", tabl_nm, printcol[i]);
	          return;
	      }
	      else
	      {
              colcount[i] = col_count;	
   	          //printf("col %s: %d\n", printcol[i], colcount[i]);
   	          i++;
   	      }
    }
	if(condition)
	{
		strset(query,'\0');
        fscanf(fp,"%s", query);
	    if(strcmp(query,"not") == 0)
	    {
		   no_cond=1;
		   fscanf(fp,"%s",query);
	    }
	    int i=0;
	   // printf("condition issssssssss: %s\n",query);
	    while(query[i] != '\0')
	    {
		   if((query[i] == '<') || (query[i] == '>') || (query[i] == '=') || (query[i] == '!'))
		      break;
		   col_nm[i] = query[i];
		   //printf("%d %c \n",i,col_nm[i]);
		   i++;
	    }
	    col_nm[i] = '\0';
	    col_count = 0;
	    //printf("col : %s\n",col_nm);
	    col_count = columncount(tabl_nm,col_nm);
	    //printf("colcount : %d\n",col_count);
	    if(col_count == 0)
	    {
	       printf("Invalid: %s doesnot contain %s\n",tabl_nm,col_nm);
	       return;
	    }
	    else
	    {
	    	char temp[50];
	    	schema_fp=fopen("schema.txt","r");
	    	int j = 1;
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
	        if(j != col_count)
	        while(strcmp(temp,"table") != 0 && !feof(schema_fp))
	        {
		        temp[0] = '\0';
		        fscanf(schema_fp,"%s",temp);
		       //printf("temp : %s\n",temp);
		        if(strcmp(temp,",") == 0)
		        {
			       fscanf(schema_fp,"%s",temp);
			       j++;
			       if(j == col_count)
			          break;
	            }
	    	
	        }
	        fscanf(schema_fp,"%s",temp);
	        strcpy(type,temp);
	        
	        if(!feof(schema_fp))
	        {
	             fscanf(schema_fp,"%s",temp);
	             if(strcmp(temp,",") != 0 && strcmp(temp,"not") != 0 && strcmp(temp,"primary") != 0 && strcmp(temp,"unique") != 0 && strcmp(temp,"table") != 0 )
			     {
			     	strcat(type," ");
			     	strcat(type,temp);
			     }
		    }
		  
		   fclose(schema_fp);
	    }
		// printf("predicate colcount : %s \n",col_nm);    
        
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
		        // printf("\n%c\n",query[i-1]);
	       }
	       if(query[i]==';')
	         break;
	       strset(query,'\0');
	       fscanf(fp,"%s",query);
	       //printf("\nafter scanf query: %s\n",query);
	       i = 0;
        }
        /*while(query[i] != '\0' && query[i] != ';')
	    {
	       /*if((query[i] == '\'')  || (query[i] == '\"'))
		       i++;
		   else
		      {
		         val[j++] = query[i++];
		         printf("\nhiiiiiiiiiii\n");
		   // }
	    }*/
	    val[j] = '\0';
	    //printf("value : %s\n",val);
	    //printf("table name : %s\n",tabl_nm_ext);
	    if(type_check(type,val))
	    {
	    	printf("\nInvalid Syntax: type of given value doesnot match\n");
	    	return;
	    }
	    //printf("value : %s\n",val);
    }
    //printf("table name : %s\n",tabl_nm_ext);
    strcpy(tabl_nm_ext,tabl_nm);
	strcat(tabl_nm_ext,".txt");
    table_fp = fopen(tabl_nm_ext,"r");
    if(table_fp == NULL)
    {
    	printf("\nEMPTY TABLE. No row has been inserted in table %s.\n",tabl_nm);
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
		    		//printf("compare %s on %s with given val %s\n",operation,buff,val);
	    	      if(check(buff,operation,val,no_cond) == 1)
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
    
    //if(row)
    //{
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
		printf("\n");
		
		i = 0;
		int total_cols ;
		strcpy(buff,",");
		while(strcmp(buff,"table") != 0 && !feof(schema_fp))
		{
			if(strcmp(buff,",") == 0)
			{
				fscanf(schema_fp,"%s",buff);
				if(strcmp(buff,"foreign") != 0)
				{
	                i++;
	          		if(printcol_counter)
	    		    {
	    		       if(same_colnm_check1(colcount,i,printcol_counter) == 1)      // int
	    		          printf("  %-13s",buff);
	    	        }
	    	        else
	    	             printf("  %-13s",buff);
			    }
		    }
		    buff[0] = '\0';
			fscanf(schema_fp,"%s",buff);
			 
	    }
	    total_cols = i;
	    fclose(schema_fp);
	    printf("\n");
	    
	    if(printcol_counter)
	      for (i=0;i<printcol_counter;i++)
	        printf("  -------------");
	    else
	      for(i=0;i<total_cols;i++)
	        printf("  -------------");
	    printf("\n");
	        
	    
	    int length=0;
	    i = 1;
	    new_table = fopen("newtable.txt","r");
	    if(new_table == NULL)
	    {
	    	printf("error in opening new table\n");
	    	return;
	    }
	    fscanf(table_fp,"%s",buff);
	    printf("  ");
	    while(!feof(new_table))
	    {
			if(printcol_counter)
	    	{
	    		if(same_colnm_check1(colcount,i,printcol_counter) == 1)   
				{
				 
	    	      while(strcmp(buff,",") != 0 && strcmp(buff,";") != 0 )
			      {
			    	length = length + strlen(buff) + 1;
			    	printf("%s ",buff);
			    	buff[0] = '\0';
			        fscanf(table_fp,"%s",buff);
			      }
			      i++;
			      while(length<15)
			      {
			    	printf(" ");
			    	length++;
			      }
			      length = 0;
			      if(strcmp(buff,";") == 0)
				  {
				      i = 1;
				      printf("\n  ");
				  }
			    }
			    else
			    {
			        while(strcmp(buff,",") != 0 && strcmp(buff,";") != 0 )
			        {
			        	buff[0] = '\0';
			            fscanf(table_fp,"%s",buff);
				    }
				    i++;
				    if(strcmp(buff,";") == 0)
				    {
				    	i = 1;
				    	printf("\n  ");
				    }
			    }
		    }
		    else
		    {
		    	while(strcmp(buff,",") != 0 && strcmp(buff,";") != 0 )
			      {
			    	length = length + strlen(buff) + 1;
			    	printf("%s ",buff);
			    	buff[0] = '\0';
			        fscanf(table_fp,"%s",buff);
			      }
			      while(length<15)
			      {
			    	printf(" ");
			    	length++;
			      }
			      length = 0;
			      if(strcmp(buff,";") == 0)
			        printf("\n  ");
			      
		    }
		    buff[0] = '\0';
	    	fscanf(table_fp,"%s",buff);
	    }
	    fclose(new_table);     //remove
	//}
	
    if(condition)
    {
       if(row == 0)
          printf("\nNo row selected\n");
       else
       if(row == 1)
          printf("\n1 row selected\n");
        else
          printf("\n%d rows selected\n",row);
    }
    else
       printf("\n%d rows selected\n",total_rows-1);
//   	 remove("temp.txt");
   	// remove("newtable.txt");
    return ;   
 }//end of main
