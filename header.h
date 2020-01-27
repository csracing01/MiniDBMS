/**************************************HEADER FILE**************************************/


#include <stdio.h>
#include <string.h>
#include <math.h>
int row;

int same_colnm_check(char attr[][50], char *col, int col_count)
{
	// To check if user entered name of same attribute more than once
	
	int i = 0;
	while(i < col_count)
	{
		//printf("col: %d\n",attr[i]);
		if(strcmp(attr[i],col) == 0)
		  return 1;
		i++;
	}
	return 0;
}

int table_exist(char tabl_nm[30])
{
	// To check if table exists
	// If it exists, it returns 1 and 0 otherwise
	
	char table_name[30];
	FILE *table_fp = fopen("table.txt", "a+");
	while(!feof(table_fp))
	{
		fscanf(table_fp, "%s", table_name);
		if(strcmp(table_name,tabl_nm) == 0)
		{
			fclose(table_fp);
			return 1;
		}
		   
	}
	fclose(table_fp);
	return 0;
}

int columncount(char tabl_nm[30],char col_nm[30])
{
	FILE *schema_fp = fopen("schema.txt","r");
	char temp[30];
	int column_count = 0;
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
	column_count++;
	if(strcmp(temp,col_nm) != 0)
	while(strcmp(temp,"table") != 0 && !feof(schema_fp))
	{
		temp[0] = '\0';
		fscanf(schema_fp,"%s",temp);
		//printf("temp : %s\n",temp);
		if(strcmp(temp,",") == 0)
		{
			fscanf(schema_fp,"%s",temp);
			column_count++;
	    }
	    
	    
	    if(strcmp(temp,col_nm) == 0)
	       break;
		 
    }
    if(feof(schema_fp) || (strcmp(temp,"table") == 0))
       column_count=0;
    fclose(schema_fp);
    //printf("colcnt: %d",column_count);
    return column_count;
}

int check(char buff[30],char op[3],char val[10], int not_condition)
{
	// To check operator used in case of select or delete operation
	
	int n = strlen(op);
	switch(n)
	{
		case 1: if(strcmp(op,"<") == 0)
                 if(not_condition)
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
		          if(not_condition)
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
		          if(not_condition)
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
		          if(not_condition)
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
		          if(not_condition)
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
		          if(not_condition)
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

int type_check(char type[50], char (values[50]))
{
	// Performs type checking
	// returns 0 if type check is successful and 1 on failure
	
	int number = 0, k, j, v[20], m;
	char ch;
	
    if(strcmp(type, "int") == 0)
	{	

	    if(values[0] == '\'' || values[0] == '"' || strchr(values, '-') != NULL || strchr(values, '.') != NULL || strcmp(values, "true") == 0 || strcmp(values, "false") == 0)
		{
			return 1;
	    }
	    
	    
		for(k = 0; values[k] != '\0'; k++)
		{
		    ch = values[k];
		    v[k] = (int) values[k];
		    v[k] -= 48; // 0's ASCII code is 48
		}

		int no = k;
		number = 0;
		for(m = 0; m < no; m++)
		{
			k--;
			number += v[k] * pow(10,m);
		}
		
		ch = (char) (number + 48);
	
		if(!isdigit(ch) || number > 65535)
			return 1;
	}
	
	else if(strcmp(type, "date") == 0)
	{
		for(k = 0; k < 2; k++)
		{
		      if(values[k] != '\0' && !isdigit(values[k]))
			        return 1;
		}
		
		if((values[2] == '\0' || (values[2] != '\0' && values[2] != '-')))
	        return 1;
		
		for(k = 3; k < 5; k++)
		{
		      if(values[k] != '\0' && !isdigit(values[k]))
			        return 1;
		}
		
		if((values[5] == '\0' || (values[5] != '\0' && values[5] != '-')))
	        return 1;

		for(k = 6; k < 10; k++)
		{
		      if(values[k] != '\0' && !isdigit(values[k]))
			        return;
		}
	}
	
	else if(strcmp(type, "boolean") == 0)
	{
		if(strcmp(values, "true") != 0 && strcmp(values, "false") != 0)
			        return 1;
	}
	
	else if(strcmp(type, "char") == 0)
	{
		if(!isalpha(values[1]) || values[0] != '\'' || values[2] != '\'' || values[3] != '\0')
			        return 1;
			        
		values[0] = values[1];
		
		for(j = 1; j < 50; j++)
		      values[j] = '\0'; // to remove ''
	}
	else // char(len) or varchar(len) or double(precision)
	{
		int k = 0;
		char data_type[20];
		for(k = 0; k < 20; k++)
		        data_type[k] = '\0';
		        
		k = 0;  
		int len = 0;
		
		for(j = 0; type[j] != ' '; j++)
		{
		    data_type[k] = type[j];
		    k++;	
		}

		if(type[j+2] == '\0')
		{
		    len = type[j+1];
		    len -= 48;
		}
		else
		{
			k = j+1;

			while(type[k] != '\0')
			    k++;

			int v = k-j-2;
			
			k = j+1; 
			while(type[k] != '\0')
			{
				len += (type[k] - 48) * pow(10,v);
				v--;
				k++;
			}
		}
		
		if(strcmp(data_type, "double") == 0)
		{
			k = 0;
			for(k = 0; values[k] != '\0'; k++)
			{
				if(values[k] == '.')
				       break;
			}
			if(values[k] == '\0')
		        return 1;
	
		    else
		    {
		    	for(j = 0; j < k; j++)
		    	{
			    	if(!isdigit(values[j]))
					        return 1;
				}
				
				for(j = k + 1; values[j] != '\0'; j++)
				{
					if(!isdigit(values[j]))
					        return 1;
				}
				
			    for(m = k+1; values[m] != '\0'; m++);
			    
			    int precision = m-k-1;
			    if(precision > len)
			    {
			    	printf("\nMaximum precision allowed is %d. You entered precision %d.", len, precision);
			    	//printf("\n\n\t\tINSERTION INVALID\n");
				    return 2;
			    }
		    }
		}
		else if(strcmp(data_type, "char") == 0)
		{
			if(values[0] != '"')
				return 1;
		
			for(k = 1; values[k] != '\0'; k++);
			if(values[k-1] != '"') 
				return 1;

			int length = k-2;
			
			if(length != len)
			{
				printf("\nLength is fixed to be %d. You entered length %d.",len,length);
				//printf("\n\n\t\tINSERTION INVALID\n");
				return 2;
			}
			
			int h = 0;
			for(j = 1; j < k-1; j++, h++)
				values[h] = values[j];

			for(j = h; j < 50; j++)
			    values[j] = '\0'; // to remove " "
		}
		else if(strcmp(data_type, "varchar") == 0)
		{
			if(values[0] != '"')
				return 1;
			
			for(k = 1; values[k] != '\0'; k++);
			if(values[k-1] != '"') 
				return 1;

			int length = k-2;
			
			if(length > len)
			{
				printf("\nMaximum length is %d. You entered length %d.",len,length);
				//printf("\n\n\t\tINSERTION INVALID\n");
				return 2;
			}
			int h = 0;
			
			for(j = 1; j < k-1; j++, h++)
				values[h] = values[j];
			
			for(j = h; j < 50; j++)
			    values[j] = '\0';  // to remove " "
		}
	}
	return 0;
}


void usage(int ch)
{
	// to print correct usage to the user if the user gives incorrect syntax
	
	switch(ch)
	{
		case 1: printf("\nCORRECT SYNTAX FOR CREATE COMMAND\n");
		        printf("\ncreate table table_name"
		               "\n(attribute_name attribute_type unique/not null/primary key,"
		               "\nforeign key(attribute_name) references other_table_name(its_attribute_name)); \n");
		               
		        break;
		        
		case 2: printf("\nCORRECT SYNTAX FOR INSERT COMMAND\n");
		        printf("\ninsert into table_name(attributes optional)"
		               "\nvalues(string/char/date/boolean/int/double); \n");
		               
		        break;
		        
		case 3: printf("\nCORRECT SYNTAX FOR SELECT COMMAND\n");
		        printf("\nselect attribute_names(or *) from table_name"
		               "\nwhere (not optional) attribute_name ==/>=/<=/>/</!= value; \n");
		               
		        break;
		        
		case 4: printf("\nCORRECT SYNTAX FOR DELETE COMMAND\n");
		        printf("\ndelete from table_name"
		               "\nwhere (not optional) attribute_name ==/>=/<=/>/</!= value; \n");
		               
		        break;
		        
		default: printf("\nEnter create/insert/select/delete command. \n");
	}
}
