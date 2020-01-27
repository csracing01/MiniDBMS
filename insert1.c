#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

extern int yyparse(void);
extern FILE *yyin;

char query[50];
extern FILE *fp;

void insert()
{
	FILE *table_fp, *schema_fp, *temp_fp;
	char my_table_name[20], table_name[20];
	char query1[50], query2[50], query3[50];
	
	long int pos1_temp, pos1_schema;
	int ins_case = 0;
	// a value of 0 implies attribute names to which values are to be assigned are provided
	// a value of 1 implies attribute names to which values are to be assigned are not provided
	
	fscanf(fp,"%s", query); // 2nd word in query here it should be into
	
    temp_fp = fopen("temp.txt", "w");
    if(!temp_fp)
    {
    	perror("\nFile could not be opened in write mode.\n");
    	exit(1);
    }
	
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
	        
	        fprintf(temp_fp, "%s ", token);
    }
    fclose(fp);
	fclose(temp_fp);
	
	temp_fp = fopen("temp.txt", "r");
	if(!temp_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
	
	fscanf(temp_fp, "%s", my_table_name); // table name
	
	table_fp = fopen("table.txt", "r");
	if(!table_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
	
	
	while(!feof(table_fp))
    {
    	fscanf(table_fp, "%s", table_name);

    	if(strcmp(table_name, my_table_name) == 0)
           break;
    	    
    }	
	
	if(feof(table_fp))
	{
		printf("\nTable %s does not exist in the database!!!", my_table_name);
		printf("\n\n\t\tINSERTION INVALID\n");
		return;
	}
	
	fclose(table_fp);
	
	int no_of_attr = 0; // no. of attributes provided by user
	
	pos1_temp = ftell(temp_fp);

	while(strcmp(query1,"values") != 0)
	{				
		fscanf(temp_fp, "%s", query1); // 1st fscanf gives 'values' or some attribute name
		
		if(strcmp(query1, ",") != 0 && strcmp(query1, "values") != 0)
		    no_of_attr++;
		    
	}
	// exits while after reading 'values' from temp.txt
	
	schema_fp = fopen("schema.txt", "r");
	if(!schema_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
	
	int j;
	for(j = 0; j < 50; j++)
		    query1[j] = '\0';
	
	while(strcmp(query1, my_table_name) != 0)
	{
		fscanf(schema_fp, "%s", query2);
		
		if(strcmp(query2,"table") == 0)
		    fscanf(schema_fp, "%s", query1);
	}
	// table name should come after "table" word only
	// because even some attribute can have same name as that of the table.
	
	
	int n = 0; // actual no. of attributes in table 'my_table_name'
	
	// Upper while was just to position the file pointer where the table exists
	// Now 1st fscanf gives name of 1st attribute in table
	
	pos1_schema = ftell(schema_fp);
	while(strcmp(query3, "table") != 0 && !feof(schema_fp))
	{    
		fscanf(schema_fp, "%s", query3);
		
		if(strcmp(query3, ",") == 0)
		{
			fscanf(schema_fp, "%s", query3);
			if(strcmp(query3, "foreign") != 0)
		        n++;
	    }    
	}
	n += 1;
//	1 ',' => 2 attributes; 0 ',' => 1 attribute

	fclose(schema_fp);
	
	if(no_of_attr == 0)
	{
	    no_of_attr = n;
	    ins_case = 1;
    }
	else if(no_of_attr > n)
	{
		printf("\n\nTable %s has only %d attributes...\nCan't assign values to %d attributes.. ", my_table_name, n, no_of_attr);
		printf("\n\n\t\tINSERTION INVALID\n");
		return;
	}
	// otherwise ins_case is already 0 (as initialized)
	
	// => next fscanf reads 1st value
	long int val_pos = ftell(temp_fp);
	
	for(j = 0; j < 50; j++)
	     query2[j] = '\0';
	
	int no_of_val = 0;
	while(!feof(temp_fp))
	{
		fscanf(temp_fp, "%s", query2); // actual values
		
		if(feof(temp_fp))
              break;
		
		if(strcmp(query2,",") == 0)
            no_of_val++;
	}
	no_of_val++; // no. of values is 1 + no. of commas
    
    if(ins_case == 0) // user gave attributes
    {
    	if(no_of_attr != no_of_val)
    	{
    		printf("\nNo. of values should be equal to no. of attributes given!!!");
    		printf("\n\n\t\tINSERTION INVALID\n");
    		return;
    	}
    }
    else
    {
    	if(no_of_attr != no_of_val)
    	{
    		printf("\nNo. of values should be equal to no. of attributes in %s i.e. %d!!!", my_table_name, no_of_attr);
    		printf("\n\n\t\tINSERTION INVALID\n");
    		return;
    	}
    }
	
	fclose(temp_fp);
	
	
	char att_nm[20][20], attr_nm[20][20], type[50][50], constraints[50][50], constraints1[50][50];
    // att_nm to store attributes entered by user
    // attr_nm to store actual attributes in table
    
    int g, h;
    for(g = 0; g < 50; g++)
    {
    	for(h = 0; h < 50; h++)
    	{
	       type[g][h] = '\0';
	       constraints[g][h] = '\0';
	       constraints1[g][h] = '\0';
	    }
    }
    for(g = 0; g < 20; g++)
    {
        for(h = 0; h < 20; h++)
        {
	       attr_nm[g][h] = '\0';
	       att_nm[g][h] = '\0';
	    }  
	}
	j = 0;
    
	schema_fp = fopen("schema.txt", "r");
	if(!schema_fp)
	{
		perror("\nCould not open file in read mode!!!\n");
		exit(1);
	}
	fseek(schema_fp, pos1_schema, SEEK_SET); // done so that 1st fscanf gives name of 1st attribute in table as in schema
	fscanf(schema_fp, "%s", attr_nm[0]);


	for(j = 0; j < 50; j++)
	    query3[j] = '\0';
	    
	for(j = 0; j < 50; j++)
	    query2[j] = '\0';


	int i = 0; // for attr_nm
	char attr_foreign[20][20],table_ref[20][20],attr_ref[20][20];
	int foreign = 0;
	
	for(g = 0; g < 20; g++)
    {
        for(h = 0; h < 20; h++)
        {
	       attr_foreign[g][h] = '\0';
	       table_ref[g][h] = '\0';
	       attr_ref[g][h] = '\0';
	    }  
	}
	
	while(strcmp(query3,"table") != 0 && !feof(schema_fp))
	{
		fscanf(schema_fp, "%s", query3);
		
		if(strcmp(query3, ",") == 0)
		{
			fscanf(schema_fp, "%s", query3);

			if(strcmp(query3, "foreign") != 0)
			    strcpy(attr_nm[++i], query3);
			else
			{
				foreign = 1;
				int k = 0;
				while(strcmp(query2,"table") != 0 && !feof(schema_fp))
				{
					if(k!=0)
					{
					    fscanf(schema_fp, "%s", query2); //, or table
					    if(strcmp(query2,"table") == 0 || feof(schema_fp))
					           break;
					    fscanf(schema_fp, "%s", query2); // foreign
					}
					    
				    fscanf(schema_fp, "%s", query2); // key
				    fscanf(schema_fp, "%s", attr_foreign[k]);
				    fscanf(schema_fp, "%s", query2); // references
				    fscanf(schema_fp, "%s", table_ref[k]);
				    fscanf(schema_fp, "%s", attr_ref[k]);
				    k++;
				    foreign++;
				}
				
			}
		}
	}
	

    fclose(schema_fp);
	
	
    	int c = 0;
    	int count = 0;
    	for(i = 0; i < 50; i++)
    	    query3[i] = '\0';
    	    
    	schema_fp = fopen("schema.txt","r");
    	if(!schema_fp)
    	{
    		perror("\n\nFile could not be opened in read mode!!!");
    		exit(1);
    	}
    	fseek(schema_fp, pos1_schema, SEEK_SET);
    	fscanf(schema_fp,"%s",query3); // gives 1st attribute of table in schema

		while(strcmp(query3, ",") != 0 && strcmp(query3,"table") != 0 && !feof(schema_fp))
		{
	            fscanf(schema_fp, "%s", query3);
	            
	            if(feof(schema_fp))
	                break; // to avoid last string being read twice
				
				
				if(strcmp(query3, "unique") == 0 || strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
				{
					if(count > 0)
						 strcat(constraints1[c], " ");
					strcat(constraints1[c], query3);
					
					if(strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
					       fscanf(schema_fp, "%s", query3); // not needed, just save not for not null,
					                                        // and primary for primary key	
					count++;
				}
				
				if(strcmp(query3, ",") == 0)
				{
					fscanf(schema_fp, "%s", query3);
					    
					c++;
					count = 0;
				}

		}

    	fclose(schema_fp);
	    j = 0;
	
	
	if(ins_case == 0) // i.e. if user gave attributes 
	{
		temp_fp = fopen("temp.txt", "r");
		if(!temp_fp)
		{
			perror("\nFile could not be opened in read mode!!!\n");
			exit(1);
		}
		fseek(temp_fp, pos1_temp, SEEK_SET); // done so that 1st fscanf gives 1st attribute entered by user
		fscanf(temp_fp, "%s", att_nm[0]);
		
		
		for(j = 0; j < 50; j++)
		    query1[j] = '\0';
		
		i = 0;
		while(strcmp(query1, "values") != 0)
		{
			fscanf(temp_fp, "%s", query1);
			if(strcmp(query1, "values") != 0 && strcmp(query1, ",") != 0)
			       strcpy(att_nm[++i], query1);
		}
		
		fclose(temp_fp);
		
	   	
	    int k;
	    for(k = 0; k < no_of_attr ; k++)
	    {
	    		for(j = k + 1; j < no_of_attr ; j++)
	    		{
	    			    if(strcmp(att_nm[k], att_nm[j]) == 0)
	    			    {
	    				    printf("\n\nName of same column entered more than once!!!");
	    				    printf("\n\n\t\tINSERTION INVALID\n");
	    				    return;
	    			    }
	    		}
	    }
	    
	    for(k = 0; k < no_of_attr; k++)
	    {
	            for(j = 0; j < n; j++)
	            {
	    	             if(strcmp(att_nm[k], attr_nm[j]) == 0)
	    	                 break;
	    	    }
	    	    if(j == n)
	    	    {
	    	    	printf("\nAttibute %s is not present in table %s.", att_nm[k], my_table_name);
	    	    	break;
	    	    }
	    }
	    
	    if(k != no_of_attr)
	    {
	    	printf("\n\n\t\tINSERTION INVALID\n");
	        return;
	    }
		
		schema_fp = fopen("schema.txt", "r");
		if(!schema_fp)
		{
			perror("\nFile could not be opened in read mode!!!");
			exit(1);
		}
		
		for(j = 0; j < 50; j++)
		    query3[j] = '\0';
		
		int t = 0, c = 0; // t for type, c for constraints

		int count;
		
		for(i = 0; i < no_of_attr; i++, t++, c++)
		{
			    count = 0;
				fseek(schema_fp, pos1_schema, SEEK_SET);
				for(j = 0; j < 50; j++)
		           query3[j] = '\0';
				
				while(strcmp(query3, "table") != 0 && !feof(schema_fp))
				{
					fscanf(schema_fp, "%s", query3); // gives 1st attribute of table as in schema
		
					if(strcmp(query3, att_nm[i]) == 0)
					      break;
			    }
					    
					    
				if(strcmp(query3, "table") != 0 && !feof(schema_fp))
				{	
					while(strcmp(query3, ",") != 0 && strcmp(query3, "table") != 0 && !feof(schema_fp))
					{	
					    fscanf(schema_fp, "%s", query3);
					    
						if(strcmp(query3, "int") == 0 || strcmp(query3, "char") == 0 || strcmp(query3, "varchar") == 0 || strcmp(query3, "date") == 0 || strcmp(query3, "double") == 0 || strcmp(query3, "boolean") == 0)
						{
							strcpy(type[t], query3);
														
							if(strcmp(query3,"char") == 0 || strcmp(query3,"varchar") == 0 || strcmp(query3, "double") == 0)
							{
								fscanf(schema_fp, "%s", query3);
								if(strcmp(query3, ",") != 0 && strcmp(query3,"table") != 0 && strcmp(query3,"unique") != 0 && strcmp(query3, "not") != 0 && strcmp(query3, "primary") != 0 && strcmp(query3, "refby") != 0)
								{
									strcat(type[t], " ");
									strcat(type[t], query3);
								}
							}
						}
						
						
						if(strcmp(query3, "unique") == 0 || strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
						{	
							if(count > 0)
							     strcat(constraints[c], " ");

							strcat(constraints[c], query3);
							
							if(strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
							       fscanf(schema_fp, "%s", query3); // not needed, just save not for not null,
							                                        // and primary for primary key
							count++;
						}
						
						
					}
	
				}
		}
		    
		fclose(schema_fp);
    }
    else
    {
    	int t = 0, c = 0;
    	int count = 0;
    	for(i = 0; i < 50; i++)
    	    query3[i] = '\0';
    	    
    	schema_fp = fopen("schema.txt","r");
    	if(!schema_fp)
    	{
    		perror("\n\nFile could not be opened in read mode!!!\n");
    		exit(1);
    	}
    	fseek(schema_fp, pos1_schema, SEEK_SET);
    	fscanf(schema_fp,"%s",query3); // gives 1st attribute of table in schema

		while(strcmp(query3, ",") != 0 && strcmp(query3,"table") != 0 && !feof(schema_fp))
		{
	            fscanf(schema_fp, "%s", query3);
	            
	            if(feof(schema_fp))
	                break; // to avoid last string being read twice
	        
				if(strcmp(query3, "int") == 0 || strcmp(query3, "char") == 0 || strcmp(query3, "varchar") == 0 || strcmp(query3, "date") == 0 || strcmp(query3, "boolean") == 0 || strcmp(query3, "double") == 0)
				{
					strcpy(type[t], query3);
							
					if(strcmp(query3, "char") == 0 || strcmp(query3, "varchar") == 0 || strcmp(query3, "double") == 0)
					{
						fscanf(schema_fp, "%s", query3);
				
						if(strcmp(query3, ",") != 0 && strcmp(query3,"table") != 0 && strcmp(query3,"unique") != 0 && strcmp(query3, "not") != 0 && strcmp(query3, "primary") != 0 && strcmp(query3,"refby") != 0)
						{
							strcat(type[t], " ");
							strcat(type[t], query3);
						}
					}
				}
				
				if(strcmp(query3, "unique") == 0 || strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
				{
					if(count > 0)
						 strcat(constraints[c], " ");
					strcat(constraints[c], query3);
					
					if(strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
					       fscanf(schema_fp, "%s", query3); // not needed, just save not for not null,
					                                        // and primary for primary key
					count++;
				}
				
				if(strcmp(query3, ",") == 0)
				{
					fscanf(schema_fp, "%s", query3);
					    
					t++;
					c++;
					count = 0;
				}

		}

    	fclose(schema_fp);
    }
	
	temp_fp = fopen("temp.txt", "r");
	if(!temp_fp)
	{
		perror("\nFile could not be opened in read mode!!!\n");
		exit(1);
	}
	fseek(temp_fp, val_pos, SEEK_SET); // done so that next fscanf gives 1st value entered by the user
	
	for(i = 0; i < 50; i++)
	    query1[i] = '\0';
	    
	char values[50][50];
	
	for(g = 0; g < 50; g++)
    {
    	for(h = 0; h < 50; h++)
    	{
	       values[g][h] = '\0';
	    }
    }
	
	i = 0;
	while(!feof(temp_fp))	
	{
		fscanf(temp_fp, "%s", query1);
		if(feof(temp_fp))
		    break;

		if(strcmp(query1, ",") != 0)
		{
		    strcpy(values[i], query1);
		    i++;
		}
	}

    fclose(temp_fp);
    remove("temp.txt");
	
	if(ins_case == 0)
	{
		int number = 0, k, v[20], m;
		char ch;
		for(i = 0; i < no_of_attr; i++)
		{
			if(strcmp(type[i], "int") == 0)
			{	
				for(k = 0; values[i][k] != '\0'; k++)
				{
				    ch = values[i][k];
				    v[k] = (int) values[i][k];
				    v[k] -= 48; // 0's ASCII code is 48
				}
	
				int no = k;
				for(m = 0; m < no; m++)
				{
					k--;
					number += v[k] * pow(10,m);
				}
	
				
				ch = (char) (number + 48);

			
				if(!isdigit(ch) || number > 65535)
				{
					printf("%s is of type int. You entered %s NOT OF TYPE INT", att_nm[i], values[i]);
					printf("\nInsertion invalid\n");
					return;
				}
			}
			else if(strcmp(type[i], "date") == 0)
			{
				for(k = 0; k < 2; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", att_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
				if((values[i][2] == '\0' || (values[i][2] != '\0' && values[i][2] != '-')))
				{
					printf("%s is of type date. You entered %s NOT OF TYPE DATE", att_nm[i], values[i]);
					printf("\nNot a valid date!!!\n");
			      	printf("\nInsertion inavlid\n");
			        return;
				}
				
				for(k = 3; k < 5; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", att_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
				if((values[i][5] == '\0' || (values[i][5] != '\0' && values[i][5] != '-')))
				{
					printf("%s is of type date. You entered %s NOT OF TYPE DATE", att_nm[i], values[i]);
					printf("\nNot a valid date!!!\n");
			      	printf("\nInsertion inavlid\n");
			        return;
				}
				for(k = 6; k < 10; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", att_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
			}
			
			else if(strcmp(type[i], "boolean") == 0)
			{
				if(strcmp(values[i], "true") != 0 && strcmp(values[i], "false") != 0)
				{
					      	printf("%s is of type boolean. You entered %s NOT OF TYPE BOOLEAN", att_nm[i], values[i]);
					      	printf("\nInsertion invalid\n");
					        return;
				}
			}
			else if(strcmp(type[i], "char") == 0)
			{
				if(!isalpha(values[i][1]) || values[i][0] != '\'' || values[i][2] != '\'' || values[i][3] != '\0')
				{
					      	printf("%s is of type char. You entered %s NOT OF TYPE CHAR", att_nm[i], values[i]);
					      	printf("\nInsertion invalid\n");
					        return;
				}
				values[i][0] = values[i][1];
				
				for(j = 1; j < 50; j++)
				      values[i][j] = '\0'; // to remove ''
			}
			else // char(len) or varchar(len) or double(precision)
			{
				int k = 0;
				char data_type[20];
				for(k = 0; k < 20; k++)
				        data_type[k] = '\0';
				        
				k = 0;  
				int len = 0;
				
				len = 0;
				for(j = 0; type[i][j] != ' '; j++)
				{
				    data_type[k] = type[i][j];
				    k++;	
				}
	
				if(type[i][j+2] == '\0')
				{
				    len = type[i][j+1];
				    len -= 48;
				}
				else
				{
					k = j+1;
	
					while(type[i][k] != '\0')
					    k++;
	
					int v = k-j-2;
					
					k = j+1; 
					while(type[i][k] != '\0')
					{
						len += (type[i][k] - 48) * pow(10,v);
						v--;
						k++;
					}
				}
				
				if(strcmp(data_type, "double") == 0)
				{
					k = 0;
					for(k = 0; values[i][k] != '\0'; k++)
					{
						if(values[i][k] == '.')
						       break;
					}
					if(values[i][k] == '\0')
					{
						      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", att_nm[i], values[i]);
						      	printf("\nInsertion invalid\n");
						        return;
				    }
				    else
				    {
				    	for(j = 0; j < k; j++)
				    	{
					    	if(!isdigit(values[i][j]))
					    	{
							      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", att_nm[i], values[i]);
							      	printf("\nInsertion invalid\n");
							        return;
						    }
						}
						
						for(j = k + 1; values[i][j] != '\0'; j++)
						{
							if(!isdigit(values[i][j]))
					    	{
							      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", att_nm[i], values[i]);
							      	printf("\nInsertion invalid\n");
							        return;
						    }
						}
						
					    for(m = k+1; values[i][m] != '\0'; m++);
					    
					    int precision = m-k-1;
					    if(precision > len)
					    {
					    	printf("\nMaximum precision allowed is %d. You enetered precision %d.",len, precision);
					    	printf("\nInsertion invalid\n");
						    return;
					    }
				    }
				}
				else if(strcmp(data_type, "char") == 0)
				{
					if(values[i][0] != '"')
					{
						printf("%s is of type char with a fixed length. You entered %s NOT OF TYPE CHAR", att_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
					for(k = 1; values[i][k] != '\0'; k++);
					if(values[i][k-1] != '"') 
					{
						printf("%s is of type char with a fixed length. You entered %s NOT OF TYPE CHAR", att_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}

					int length = k-2;
					
					if(length != len)
					{
						printf("\nLength is fixed to be %d. You entered length %d.",len,length);
						printf("\nInsertion invalid\n");
						return;
					}
					int h = 0;
		
					for(j = 1; j < k-1; j++, h++)
						values[i][h] = values[i][j];
					
	
					for(j = h; j < 50; j++)
					    values[i][j] = '\0';
				}
				else if(strcmp(data_type, "varchar") == 0)
				{
					if(values[i][0] != '"')
					{
						printf("%s is of type varchar. You entered %s NOT OF TYPE VARCHAR", att_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
					for(k = 1; values[i][k] != '\0'; k++);
					if(values[i][k-1] != '"') 
					{
						printf("%s is of type varchar. You entered %s NOT OF TYPE VARCHAR", att_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
	
					int length = k-2;
					
					if(length > len)
					{
						printf("\nMaximum length is %d. You entered length %d.",len,length);
						printf("\nInsertion invalid\n");
						return;
					}
					int h = 0;
					
					for(j = 1; j < k-1; j++, h++)
						values[i][h] = values[i][j];
					
					for(j = h; j < 50; j++)
					    values[i][j] = '\0';
				}
			}
		}
    }
    
    else
	{
		int number = 0, k, v[20], m;
		char ch;
		for(i = 0; i < n; i++)
		{
			if(strcmp(type[i], "int") == 0)
			{	
				for(k = 0; values[i][k] != '\0'; k++)
				{
				    ch = values[i][k];
				    v[k] = (int) values[i][k];
				    v[k] -= 48; // 0's ASCII code is 48
				}

				int no = k;
				for(m = 0; m < no; m++)
				{
					k--;
					number += v[k] * pow(10,m);
				}
				
				ch = (char) (number + 48);
			
				if(!isdigit(ch) || number > 65535)
				{
					printf("%s is of type int. You entered %s NOT OF TYPE INT", attr_nm[i], values[i]);
					printf("\nInsertion invalid\n");
					return;
				}
			}
			else if(strcmp(type[i], "date") == 0)
			{
				for(k = 0; k < 2; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", attr_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
				if((values[i][2] == '\0' || (values[i][2] != '\0' && values[i][2] != '-')))
				{
					printf("%s is of type date. You entered %s NOT OF TYPE DATE", attr_nm[i], values[i]);
					printf("\nNot a valid date!!!\n");
			      	printf("\nInsertion inavlid\n");
			        return;
				}
				
				for(k = 3; k < 5; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", attr_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
				if((values[i][5] == '\0' || (values[i][5] != '\0' && values[i][5] != '-')))
				{
					printf("%s is of type date. You entered %s NOT OF TYPE DATE", attr_nm[i], values[i]);
					printf("\nNot a valid date!!!\n");
			      	printf("\nInsertion inavlid\n");
			        return;
				}
				for(k = 6; k < 10; k++)
				{
				      if(values[i][k] != '\0' && !isdigit(values[i][k]))
				      {
					      	printf("%s is of type date. You entered %s NOT OF TYPE DATE", attr_nm[i], values[i]);
					      	printf("\nNot a valid date!!!\n");
					      	printf("\nInsertion invalid\n");
					        return;
				      }
				}
			}
			
			else if(strcmp(type[i], "boolean") == 0)
			{
				if(strcmp(values[i], "true") != 0 && strcmp(values[i], "false") != 0)
				{
					      	printf("%s is of type boolean. You entered %s NOT OF TYPE BOOLEAN", attr_nm[i], values[i]);
					      	printf("\nInsertion invalid\n");
					        return;
				}
			}
			else if(strcmp(type[i], "char") == 0)
			{
				if(!isalpha(values[i][1]) || values[i][0] != '\'' || values[i][2] != '\'' || values[i][3] != '\0')
				{
					      	printf("%s is of type char. You entered %s NOT OF TYPE CHAR", attr_nm[i], values[i]);
					      	printf("\nInsertion invalid\n");
					        return;
				}
				values[i][0] = values[i][1];
				
				for(j = 1; j < 50; j++)
				      values[i][j] = '\0';
			}
			else // char(len) or varchar(len) or double(precision)
			{
				int k = 0;
				char data_type[20];
				for(k = 0; k < 20; k++)
				        data_type[k] = '\0';
				        
				k = 0;
				        
				int len = 0;
				
				j = 0;
				for(j = 0; type[i][j] != ' '; j++)
				{
				    data_type[k] = type[i][j];
				    k++;	
				}
	
				if(type[i][j+2] == '\0')
				{
				    len = type[i][j+1];
				    len -= 48;
				}
				else
				{
					k = j + 1;
					
					while(type[i][k] != '\0')
					    k++;
		
					int v = k-j-2;
					
					k = j + 1; 
					while(type[i][k] != '\0')
					{
						len += (type[i][k] - 48) * pow(10,v);
						v--;
						k++;
					}
				}
				
				if(strcmp(data_type, "double") == 0)
				{
					k = 0;
					for(k = 0; values[i][k] != '\0'; k++)
					{
						if(values[i][k] == '.')
						       break;
					}
					if(values[i][k] == '\0')
					{
						      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", attr_nm[i], values[i]);
						      	printf("\nInsertion invalid\n");
						        return;
				    }
				    else
				    {
				    	for(j = 0; j < k; j++)
				    	{
					    	if(!isdigit(values[i][j]))
					    	{
							      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", att_nm[i], values[i]);
							      	printf("\nInsertion invalid\n");
							        return;
						    }
						}
						
						for(j = k + 1; values[i][j] != '\0'; j++)
						{
							if(!isdigit(values[i][j]))
					    	{
							      	printf("%s is of type double. You entered %s NOT OF TYPE DOUBLE", att_nm[i], values[i]);
							      	printf("\nInsertion invalid\n");
							        return;
						    }
						}

					    for(m = k+1; values[i][m] != '\0'; m++);
					    
					    int precision = m-k-1;
					    if(precision > len)
					    {
					    	printf("\nMaximum precision allowed is %d. You enetered precision %d.",len, precision);
					    	printf("\nInsertion invalid\n");
						    return;
					    }
				    }
				}
				else if(strcmp(data_type, "char") == 0)
				{
					if(values[i][0] != '"')
					{
						printf("%s is of type char with a fixed length. You entered %s NOT OF TYPE CHAR", attr_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
					for(k = 1; values[i][k] != '\0'; k++);
					if(values[i][k-1] != '"') 
					{
						printf("%s is of type char with a fixed length. You entered %s NOT OF TYPE CHAR", attr_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
					
					int length = k-2;
					
					if(length != len)
					{
						printf("\nLength is fixed to be %d. You entered length %d.",len,length);
						printf("\nInsertion invalid\n");
						return;
					}
					int h = 0;
		
					for(j = 1; j < k-1; j++, h++)
						values[i][h] = values[i][j];
					
					for(j = h; j < 50; j++)
					    values[i][j] = '\0';
				}
				else if(strcmp(data_type, "varchar") == 0)
				{
					if(values[i][0] != '"')
					{
						printf("%s is of type varchar. You entered %s NOT OF TYPE VARCHAR", attr_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
					for(k = 1; values[i][k] != '\0'; k++);
					if(values[i][k-1] != '"') 
					{
						printf("%s is of type varchar. You entered %s NOT OF TYPE VARCHAR", attr_nm[i], values[i]);
						printf("\nInsertion invalid\n");
						return;
					}
	
					int length = k-2;
					
					if(length > len)
					{
						printf("\nMaximum length is %d. You entered length %d.",len,length);
						printf("\nInsertion invalid\n");
						return;
					}
					int h = 0;
		
					for(j = 1; j < k-1; j++, h++)
						values[i][h] = values[i][j];
	
					for(j = h; j < 50; j++)
					    values[i][j] = '\0';
				}
			}
		}
    }

    FILE *new_fp;
    char filename[30];
    for(i = 0; i < 30; i++)
        filename[i] = '\0';
        
    strcpy(filename, my_table_name);
    strcat(filename,".txt");
        
    if(ins_case == 0)
    {
    	char new_val[50][50];
    	
    	for(i = 0; i < 50; i++)
    		for(j = 0; j < 50; j++)
    		      new_val[i][j] = '\0';
    		
    	
    	for(i = 0; i < n; i++)
    	{
    		for(j = 0; j < no_of_attr; j++)
    		{
    			if(strcmp(attr_nm[i], att_nm[j]) == 0)
    			{
    			      strcpy(new_val[i], values[j]);
    			      break;
    			}
    		}
    	}
    	
    	if(new_val[i] == '\0')
    	      strcpy(new_val[i], " ");
    	
    	for(i = 0; i < n; i++)
    	{
    		for(j = 0; j < no_of_attr; j++)
    		{
    			if(strcmp(attr_nm[i], att_nm[j]) == 0)
    			       break;
    		}
    		if(j == no_of_attr)
    		{
    			if(strstr(constraints1[i], "not") != NULL)
    			{
    				printf("%s cannot be null because of not null constraint.",attr_nm[i]);
    				printf("\nInsertion inavlid. \n");
    				return;
    			}
    		}
    	}
    	
    	int pos[50];
        for(i = 0; i < 50; i++)
            pos[i] = 0;
	
	
        for(i = 0; i < no_of_attr; i++)
    	{
    		for(j = 0; j < n; j++)
    		{
    			if(strcmp(att_nm[i], attr_nm[j]) == 0)
				      break;	
    		}
    		pos[i] = j;
        }
        
    	    
    	for(i = 0; i < 50; i++)
    	    query2[i] = '\0';
    	
    	int flag = 0;
    	new_fp = fopen(filename, "r");
    	if(!new_fp) // 1st insertion
    	{
    		flag = 1;
    	}
    	
    	for(i = 0; i < no_of_attr; i++)
    	{
    		if(strstr(constraints[i], "not") != NULL)
    		{
    			if(strcmp(values[i], "null") == 0)
    			{
    				printf("%s cannot be null becuase of not null constraint.",att_nm[i]);
    				printf("\nInsertion invalid. \n");
    				return;
    			}
    		}
    	
    	    int ct = 0;
    	    
    	    if(flag == 0)
    	    {
	    	    if(strstr(constraints[i], "unique") != NULL || strstr(constraints[i], "primary") != NULL)
	    	    {
	    	    	int l = 0; // primary
	    	    	if(strstr(constraints[i],"unique") == 0)
	    	    	     l = 1; // unique
	    	    	     
	    	    	while(!feof(new_fp))
	    	    	{	     
	    	    	     while(strcmp(query2, ";") != 0 && !feof(new_fp))
	    	    	     {
	    	    	     	 if(ct == pos[i] && ct == 0)
	    	    	     	      break;
	    	    	     	
	    	    	     	 fscanf(new_fp, "%s", query2);
	    	    	     	 
	    	    	     	 if(strcmp(query2, ",") == 0)
	    	    	     	 {
	    	    	     	 	ct++;
	    	    	     	 }
	    	    	     	 if(ct == pos[i])
							       break; // from while   	 
	    	    	     }
	    	    	     if(ct == pos[i])
	    	    	     {
	    	    	     	fscanf(new_fp, "%s", query2);
	    	    	     	if(strcmp(query2,",") != 0)
	    	    	     	{
	    	    	     		if(strcmp(values[i],query2) == 0)
	    	    	     		{
	    	    	     			if(l == 1)
	    	    	     			{
		    	    	                printf("\n\n2 values can not be same because of unique constraint on %s!!!",att_nm[i]);
		    	                    	printf("\nInsertion invalid. \n");
		    				            return;
									}
									else
									{
		    	    	                printf("\n\n2 values can not be same because of primary key constraint on %s!!!",att_nm[i]);
		    	                    	printf("\nInsertion invalid. \n");
		    				            return;
									}
	    	    	     		}
	    	    	     	}
	    	    	     }
	    	    	     ct = 0;
	    	    	     
	    	    	     while(strcmp(query2,";") != 0 && !feof(new_fp))
	    	    	         fscanf(new_fp, "%s", query2);

						 int m; 
	    	    	     for(m = 0; m < 50; m++)
	    	    	          query2[m] = '\0';
	    	        }
	    	        
	    	    }
	    	}
    	}
    	fclose(new_fp);
    	
    	FILE *newt_fp;
		    	
    	schema_fp = fopen("schema.txt","r");
    	if(!schema_fp)
	    {
	    	perror("\nFile could not be opened in read mode.\n");
	    	exit(1);
	    }
    	if(foreign != 0)
    	{
    		for(i = 0; i < foreign-1; i++)
    		{
    			     fseek(schema_fp, 0, SEEK_SET);
                     for(j = 0; j < no_of_attr; j++)
		    		 {
		    			if(strcmp(attr_foreign[i], att_nm[j]) == 0) // if user assigned some value to it
						      break;	
		    		 } 
		    		 for(j = 0; j < 50; j++)
		             {
		                 query1[j] = '\0';
		                 query2[j] = '\0';
		             }
	
					while(strcmp(query1, table_ref[i]) != 0)
					{
						fscanf(schema_fp, "%s", query2);
						
						if(strcmp(query2,"table") == 0)
						    fscanf(schema_fp, "%s", query1);
					}
					
					int pos = 0;
					while(strcmp(query1,attr_ref[i]) != 0)
					{
						fscanf(schema_fp, "%s", query1);
						if(strcmp(query1,",") == 0)
						     pos++;
					}
					
					int f = 0;
					char fname[30];
					strcpy(fname, table_ref[i]);
					strcat(fname,".txt");
			
					newt_fp = fopen(fname,"r");
					if(!newt_fp)
					{
						f = 1;
					}
					
					int ct = 0;
					for(j = 0; j < 50; j++)
		             {
		                 query2[j] = '\0';
		             }
		             
		             if(f == 0)
		             {
						while(!feof(newt_fp))
		    	    	{
		    	    	     while(strcmp(query2,";") != 0 && !feof(newt_fp))
		    	    	     {
		    	    	     	if(ct == pos && ct == 0)
		    	    	     	      break;
		    	    	     	      
		    	    	     	 fscanf(newt_fp, "%s", query2);
		    	    	     	 ct = 0;
		    	    	     	
		    	    	     	 if(strcmp(query2, ",") == 0)
		    	    	     	 	ct++;
		    	    	     	 	
		    	    	     	 if(ct == pos)
								       break; // from while   	 
		    	    	     }
		    	    	     if(ct == pos)
		    	    	     {
		    	    	     	fscanf(newt_fp, "%s", query2);
		    	    	     	
	                            if(strcmp(query2,",") != 0)
		    	    	     	{
		    	    	     		if(strcmp(values[i],query2) == 0)
		    	    	     			break;			
		    	    	     	}
	    	    	         }
	    	    	         while(strcmp(query2,";") != 0 && !feof(newt_fp))
		    	    	         fscanf(newt_fp, "%s", query2);
		
							 int m; 
		    	    	     for(m = 0; m < 50; m++)
		    	    	          query2[m] = '\0';
		    	    	
						}
						if(feof(newt_fp))
						{
							printf("\nForeign key constraint violated for %s.", att_nm[i]);
							printf("\nInsertion invalid. \n");
	    				    return;
						}
						fclose(newt_fp);
					}
    		}
    		
    	}
    	fclose(schema_fp);
    	
    	new_fp = fopen(filename, "a");
    	for(i = 0; i < n; i++)
	    {
	    	if(i == n - 1)
	    	   fprintf(new_fp, "%s ", new_val[i]);
	    	else
	    	   printf("hi %s ;", new_val[i]);
	    }
	    fprintf(new_fp, "; ");
	    printf("\nInsertion successful.\n");
	    fclose(new_fp);
    	
    }
    else
    {   
       int pos[50];
       for(i = 0; i < 50; i++)
            pos[i] = 0;
            
	   for(i = 0; i < 50; i++)
    	    query2[i] = '\0';
	 	
    	int flag = 0;
    	new_fp = fopen(filename, "r");
    	if(!new_fp) // 1st insertion
    	{
    		flag = 1;
    	}
    	
    	for(i = 0; i < n; i++)
    	   pos[i] = i;
    	
    	
    	for(i = 0; i < n; i++)
    	{
    		if(strstr(constraints[i], "not") != NULL)
    		{
    			if(strcmp(values[i], "null") == 0)
    			{
    				printf("%s cannot be null.",attr_nm[i]);
    				printf("\nInsertion invalid. \n");
    				return;
    			}
    		}
    	
    	    int ct = 0;
    	    
    	    if(flag == 0)
    	    {
	    	    if(strstr(constraints[i], "unique") != NULL || strstr(constraints[i], "primary") != NULL)
	    	    {
	    	    	int l = 0; // primary
	    	    	if(strstr(constraints[i],"unique") == 0)
	    	    	     l = 1; // unique
	    	    	     
	    	    	while(!feof(new_fp))
	    	    	{	     
	    	    	     while(strcmp(query2, ";") != 0 && !feof(new_fp))
	    	    	     {
	    	    	     	 if(ct == pos[i] && ct == 0)
	    	    	     	      break;
	    	    	     	
	    	    	     	 fscanf(new_fp, "%s", query2);
	    	    	     	 
	    	    	     	 if(strcmp(query2, ",") == 0)
	    	    	     	 	ct++;
	    	    	     	 	
	    	    	     	 if(ct == pos[i])
							       break; // from while	 
	    	    	     }
	    	    	     if(ct == pos[i])
	    	    	     {
	    	    	     	fscanf(new_fp, "%s", query2);
	    	    	     	
	    	    	     	if(strcmp(query2,",") != 0)
	    	    	     	{
	    	    	     		if(strcmp(values[i],query2) == 0)
	    	    	     		{
	    	    	     			if(l == 1)
	    	    	     			{
		    	    	                printf("\n\n2 values can not be same because of unique constraint on %s!!!",attr_nm[i]);
		    	                    	printf("\nInsertion invalid. \n");
		    				            return; 	
									}
									else
									{
		    	    	                printf("\n\n2 values can not be same because of primary keyconstraint on %s!!!",attr_nm[i]);
		    	                    	printf("\nInsertion invalid. \n");
		    				            return; 	
									}
	    	    	     		}
	    	    	     	}
	    	    	     }
	    	    	     ct = 0;
	    	    	     
	    	    	     while(strcmp(query2,";") != 0 && !feof(new_fp))
	    	    	         fscanf(new_fp, "%s", query2);
	    	    	     
						 int m; 
	    	    	     for(m = 0; m < 50; m++)
	    	    	          query2[m] = '\0';
	    	        }
	    	        
	    	    }
	    	}
    	}
    	fclose(new_fp);
    	
    	FILE *newt_fp;
    	
    	schema_fp = fopen("schema.txt","r");
    	if(!schema_fp)
	    {
	    	perror("\nFile could not be opened in read mode.\n");
	    	exit(1);
	    }
    	if(foreign != 0)
    	{
    		for(i = 0; i < foreign-1; i++)
    		{
    			     fseek(schema_fp, 0, SEEK_SET);
                     for(j = 0; j < n; j++)
		    		 {
		    			if(strcmp(attr_foreign[i], attr_nm[j]) == 0) // if user assigned some value to it
						      break;	
		    		 } 
		    		 for(j = 0; j < 50; j++)
		             {
		                 query1[j] = '\0';
		                 query2[j] = '\0';
		             }
	
					while(strcmp(query1, table_ref[i]) != 0)
					{
						fscanf(schema_fp, "%s", query2);
						
						if(strcmp(query2,"table") == 0)
						    fscanf(schema_fp, "%s", query1);
					}
					
					int pos = 0;
					while(strcmp(query1,attr_ref[i]) != 0)
					{
						fscanf(schema_fp, "%s", query1);
						if(strcmp(query1,",") == 0)
						     pos++;
					}
					
					int f = 0;
					char fname[30];
					strcpy(fname, table_ref[i]);
					strcat(fname,".txt");
		
					newt_fp = fopen(fname,"r");
					if(!newt_fp)
					{
						f = 1;
					}
					
					int ct = 0;
					for(j = 0; j < 50; j++)
		             {
		                 query2[j] = '\0';
		             }
		             
		             if(f == 0)
		             {
						while(!feof(newt_fp))
		    	    	{
		    	    	     while(strcmp(query2,";") != 0 && !feof(newt_fp))
		    	    	     {
		    	    	     	if(ct == pos && ct == 0)
		    	    	     	      break;
		    	    	     	      
		    	    	     	 fscanf(newt_fp, "%s", query2);
		    	
		    	    	     	 ct = 0;
		    	    	     	 
		    	    	     	 if(strcmp(query2, ",") == 0)
		    	    	     	 	ct++;

		    	    	     	 if(ct == pos)
								       break; // from while
		    	    	     }
		    	    	     if(ct == pos)
		    	    	     {
		    	    	     	fscanf(newt_fp, "%s", query2);
		    	    
	                            if(strcmp(query2,",") != 0)
		    	    	     	{
		    	    	     		if(strcmp(values[i],query2) == 0)
		    	    	     			break;			
		    	    	     	}
	    	    	         }
	    	    	         while(strcmp(query2,";") != 0 && !feof(newt_fp))
		    	    	         fscanf(newt_fp, "%s", query2);
		    	
							 int m; 
		    	    	     for(m = 0; m < 50; m++)
		    	    	          query2[m] = '\0';
		    	    	
						}
						if(feof(newt_fp))
						{
							printf("\nForeign key constraint violated for %s.", attr_nm[i]);
							printf("\nInsertion invalid. \n");
	    				    return;
						}
						fclose(newt_fp);
					}
    		}
    		
    	}
    	fclose(schema_fp);
    	
    	new_fp = fopen(filename, "a");
	    for(i = 0; i < no_of_attr; i++)
	    {
	    	if(i == no_of_attr - 1)
	    	   fprintf(new_fp, "%s ", values[i]);
	    	else
	    	   fprintf(new_fp, "%s , ", values[i]);
	    }
	    fprintf(new_fp, "; ");
	    printf("\nInsertion successful.\n");
	    fclose(new_fp);
	}
    
}


