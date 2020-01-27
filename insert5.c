/*******************************************INSERTION********************************************************/


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

extern int yyparse(void);
extern FILE *yyin;

char query[50];
FILE *fp;
long int pos1_temp, pos1_schema, val_pos;

void save_query()
{
	// to save query with spaces in b/w (and without brackets) in temp.txt
	// for easy access later
	
	FILE *temp_fp;
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
		    if(strcmp(query, "(") == 0 || strcmp(query, ")") == 0)
		       continue;
		    
	        char token[100];
	        for(i = 0; i < 100; i++)
			      token[i] = '\0';
			      
	        i = 0;
	        int j = 0;
	        while(query[i] != '\0')
	        {
			   	if(query[i] == ',' || query[i] == '\"')
		   	    {
	                token[j++] = ' ';
	                
	                if(query[i] == ',')
	                   token[j++] = ',';
	                else
	                   token[j++] = '\"';
	                   
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
    fclose(fp); // of input.txt
	fclose(temp_fp);
}

int count_attr_entered()
{
	// counts the no. of attributes entered by user
	
	FILE *temp_fp;
	int no_of_attr = 0;
	
	char query1[50];
	int i;
	for(i = 0; i < 50; i++)
	     query1[i] = '\0';
	
	temp_fp = fopen("temp.txt","r");
    if(!temp_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
    fseek(temp_fp, pos1_temp, SEEK_SET);
    
	while(strcmp(query1,"values") != 0)
	{				
		fscanf(temp_fp, "%s", query1); // 1st fscanf gives 'values' or some attribute name
		
		if(strcmp(query1, ",") != 0 && strcmp(query1, "values") != 0)
		    no_of_attr++;
		    
	}
	// exits while after reading 'values' from temp.txt
	
	// => next fscanf reads 1st value
	val_pos = ftell(temp_fp);
	fclose(temp_fp);
    return no_of_attr;
}


int count_total_attr(char my_table_name[30])
{
	// counts the total no. of attributes in the table in which
	// insertion is to be done
	
	int n = 0; // actual no. of attributes in table 'my_table_name'
	char query1[50], query2[50];
	FILE *schema_fp;
	
	schema_fp = fopen("schema.txt", "r");
	if(!schema_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
	
	int j;
	for(j = 0; j < 50; j++)
	{
	     query1[j] = '\0';
		 query2[j] = '\0';	
	}
	
	while(strcmp(query1, my_table_name) != 0)
	{
		fscanf(schema_fp, "%s", query2);
		
		if(strcmp(query2,"table") == 0)
		    fscanf(schema_fp, "%s", query1);
	}
	// table name should come after "table" word only
	// because even some attribute can have same name as that of the table.
	
	// Upper while was just to position the file pointer where the table exists
	// Now 1st fscanf gives name of 1st attribute in table
	
	for(j = 0; j < 50; j++)
	     query1[j] = '\0';
	
	pos1_schema = ftell(schema_fp);
	while(strcmp(query1, "table") != 0 && !feof(schema_fp))
	{    
		fscanf(schema_fp, "%s", query1);
		
		if(strcmp(query1, ",") == 0)
		{
			fscanf(schema_fp, "%s", query1);
			if(strcmp(query1, "foreign") != 0)
		        n++;
	    }    
	}
	n += 1;
//	1 ',' => 2 attributes; 0 ',' => 1 attribute

	fclose(schema_fp);
	return n;
}


int count_values()
{
	// count the no. of values entered by the user
	
	FILE *temp_fp;
	char query2[50];
	int no_of_val = 0, j;
	
	for(j = 0; j < 50; j++)
	     query2[j] = '\0';
	
	temp_fp = fopen("temp.txt", "r");
    if(!temp_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
    fseek(temp_fp, val_pos, SEEK_SET);
	
	while(!feof(temp_fp))
	{
		fscanf(temp_fp, "%s", query2); // actual values
		
		if(feof(temp_fp))
              break;
		
		if(strcmp(query2,",") == 0)
            no_of_val++;
	}
	no_of_val++; // no. of values is 1 + no. of commas
	fclose(temp_fp);
	return no_of_val;
}


void insert()
{
	// Performs various basic checks, then performs type checking and constraint checking
	// If a query passes all checks, then insertion is successful.
	
	FILE *schema_fp, *temp_fp;
	int j;
	char my_table_name[30];
	char query1[50], query2[50], query3[50];
	int no_of_attr = 0; // no. of attributes provided by user
	int n = 0; // actual no. of attributes in table 'my_table_name'
	int no_of_val = 0; // no. of values entered by user
	int ins_case = 0;
	// a value of 0 implies attribute names to which values are to be assigned are provided
	// a value of 1 implies attribute names to which values are to be assigned are not provided,
	// i.e. if values are to be assigned to all attributes
	   
	for(j = 0; j < 50; j++)
	{
		query1[j] = '\0';
	    query2[j] = '\0';
	    query3[j] = '\0';
	}
	
	fscanf(fp,"%s", query); // 2nd word in query here it should be into
	
    save_query(); // in temp.txt with spaces in b/w starting from table name and without brackets
	
	temp_fp = fopen("temp.txt", "r");
	if(!temp_fp)
    {
    	perror("\nFile could not be opened in read mode.\n");
    	exit(1);
    }
	
	fscanf(temp_fp, "%s", my_table_name); // table name	
	pos1_temp = ftell(temp_fp);
    fclose(temp_fp);
	
    if(table_exist(my_table_name) == 0)
    {
		printf("\nInvalid : Table %s does not exist in the database!!!", my_table_name);
		//printf("\n\n\t\tINVALID INSERTION\n");
		return;
	}
    
    no_of_attr = count_attr_entered();
    
    n = count_total_attr(my_table_name);
	
	if(no_of_attr == 0)
	{
	    no_of_attr = n;
	    ins_case = 1;
    }
	else if(no_of_attr > n)
	{
		printf("\nInvalid : Table %s has only %d attributes...\nCan't assign values to %d attributes.. ", my_table_name, n, no_of_attr);
		//printf("\n\n\t\tINSERTION INVALID\n");
		return;
	}
	// otherwise ins_case is already 0 (as initialized)
	
	no_of_val = count_values();
    
    if(ins_case == 0) // user gave attributes
    {
    	if(no_of_attr != no_of_val)
    	{
    		printf("\nInvalid : No. of values should be equal to no. of attributes given!!!");
    		//printf("\n\n\t\tINSERTION INVALID\n");
    		return;
    	}
    }
    else
    {
    	if(no_of_attr != no_of_val)
    	{
    		printf("\nInvalid : No. of values should be equal to no. of attributes in %s i.e. %d!!!", my_table_name, no_of_attr);
    		//printf("\n\n\t\tINSERTION INVALID\n");
    		return;
    	}
    }
	
	
	char att_nm[20][20], attr_nm[20][20], type[50][50], constraints[50][50], constraints1[50][50];
    // att_nm to store attributes entered by user
    // attr_nm to store actual attributes in table
    // type to store data type of attributes to which values are to be assigned
    // constraints to store constraints on attributes to which values are to be assigned
    // constraints1 to store constraints on all attributes (it is needed later even when ins_case is 0)
    
    int i = 0; // for attr_nm
	char attr_foreign[20][20], table_ref[20][20], attr_ref[20][20];
	int foreign = 0;
	// a value of 0 => no foreign key constraint
	// a non-zero value => 1 or more foreign key constraints
	// Actually, it gives exactly the no. of foreign key constraints.
	// In this case, attr_foreign saves those attributes which serve as foreign keys
	// table_ref stores the corresponding table names to which attributes in attr_foreign refer
	// attr_ref stores the referenced attributes
	
	
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
	for(g = 0; g < 20; g++)
    {
        for(h = 0; h < 20; h++)
        {
	       attr_foreign[g][h] = '\0';
	       table_ref[g][h] = '\0';
	       attr_ref[g][h] = '\0';
	    }  
	}
    
	
	// Save attributes of table in attr_nm
	// Also if a table has foreign key constraint, fill attr_foreign, table_ref, attr_ref appropriately
    
	schema_fp = fopen("schema.txt", "r");
	if(!schema_fp)
	{
		perror("\nCould not open file in read mode!!!\n");
		exit(1);
	}
	fseek(schema_fp, pos1_schema, SEEK_SET); // done so that 1st fscanf gives name of 1st attribute in table as in schema
	fscanf(schema_fp, "%s", attr_nm[0]);
	
	while(strcmp(query3, "table") != 0 && !feof(schema_fp))
	{
		fscanf(schema_fp, "%s", query3);
		
		if(strcmp(query3, ",") == 0)
		{
			fscanf(schema_fp, "%s", query3);

			if(strcmp(query3, "foreign") != 0)
			    strcpy(attr_nm[++i], query3);
			else
			{
				int k = 0;
				while(strcmp(query2, "table") != 0 && !feof(schema_fp))
				{
					if(k != 0)
					{
					    fscanf(schema_fp, "%s", query2); //, or table
					    if(strcmp(query2, "table") == 0 || feof(schema_fp))
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
	
	// Save constraints of all attributes in constraints1
	
	int c = 0;
	int count = 0;
	for(i = 0; i < 50; i++)
	    query3[i] = '\0';
	    
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
	
	if(ins_case == 0) // i.e. if user gave attributes 
	{
        // Save attributes provided by the user in att_nm
    
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
		
	   	// Checking whether user entered name of same attribute more than once
	   	
	    int k;
	    for(k = 0; k < no_of_attr ; k++)
	    {
	    		for(j = k + 1; j < no_of_attr ; j++)
	    		{
	    			    if(strcmp(att_nm[k], att_nm[j]) == 0)
	    			    {
	    				    printf("\nInvalid : Name of same attribute entered more than once!!!");
	    				    //printf("\n\n\t\tINSERTION INVALID\n");
	    				    return;
	    			    }
	    		}
	    }
	    
	    // Checking whether attributes entered by user are present in table or not
	    
	    for(k = 0; k < no_of_attr; k++)
	    {
	            for(j = 0; j < n; j++)
	            {
	    	             if(strcmp(att_nm[k], attr_nm[j]) == 0)
	    	                 break;
	    	    }
	    	    if(j == n)
	    	    {
	    	    	printf("\nInvalid : Attibute %s is not present in table %s.", att_nm[k], my_table_name);
	    	    	break;
	    	    }
	    }
	    
	    if(k != no_of_attr)
	    {
	    	printf("\nINSERTION INVALID\n");
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
		int count; // keeps count of the no. of constraints on an attribute
		// used just to put spaces in b/w in case of more than 1 constraint
		// on an attribute
		
		// type stores data type of attributes provided by the user whereas
		// constraints stores the constraints on them.  
		
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
								if(feof(schema_fp))
						            break;
						            
								if(strcmp(query3, ",") != 0 && strcmp(query3, "table") != 0 && strcmp(query3, "unique") != 0 && strcmp(query3, "not") != 0 && strcmp(query3, "primary") != 0 && strcmp(query3, "refby") != 0)
								{
									strcat(type[t], " ");
									strcat(type[t], query3); // length in case of char and varchar
									                         // precision in case of double
								}
							}
						}
						
						
						if(strcmp(query3, "unique") == 0 || strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
						{	
							if(count > 0)
							     strcat(constraints[c], " ");

							strcat(constraints[c], query3);
							
							if(strcmp(query3, "not") == 0 || strcmp(query3, "primary") == 0)
							       fscanf(schema_fp, "%s", query3); // not needed, just save not for not null constraint,
							                                        // and primary for primary key constraint
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
						if(feof(schema_fp))
						    break;
				
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
	    
	char values[50][50]; // to store values entered by the user
	
	for(g = 0; g < 50; g++)
    	for(h = 0; h < 50; h++)
	       values[g][h] = '\0';
    
	// Save values entered by the user in values
	
	i = 0;
	while(!feof(temp_fp))	
	{
		fscanf(temp_fp, "%s", query1);
		if(feof(temp_fp))
		    break;

		if(strcmp(query1, ",") != 0)
		{
			if(strcmp(query1, "\"") == 0)
			{
				strcpy(values[i], query1);
				
				fscanf(temp_fp, "%s", query1);
				strcat(values[i], query1);
				
				fscanf(temp_fp, "%s", query1);
				
				if(strcmp(query1, "\"") == 0)		
		            strcat(values[i], query1);
		        
		        else
		        {
		        	while(strcmp(query1, "\"") != 0)
		        	{
			        	strcat(values[i], " ");
				        strcat(values[i], query1);
				        fscanf(temp_fp, "%s", query1);
				    }
				    strcat(values[i], query1);
		        }		        
		        i++;
		    }
		    else
		    {
		    	strcpy(values[i], query1);
		    	i++;
		    }
		    
		}
	}

    fclose(temp_fp);
    
    // remove("temp.txt");
	
	
	// TYPE CHECKING
	
	
	int t;
	for(i = 0; i < no_of_attr; i++)
	{
		t = type_check(type[i], values[i]);
		if(t == 1)
		{
			if(ins_case == 0)
			    printf("\nInvalid : %s is of type %s. You entered %s NOT OF TYPE %s\n", att_nm[i], type[i], values[i], type[i]);
			else
			    printf("\nInvalid : %s is of type %s. You entered %s NOT OF TYPE %s\n", att_nm[i], type[i], values[i], type[i]);
			    
			//printf("\n\n\t\tINSERTION INVALID\n");
			return;
		}
		else if(t == 2)
		{
			printf("\nINSERTION INVALID\n");
			return;
		}
	}

    // Construct file name in which values are to be inserted same as that of table name appended by .txt

    FILE *new_fp;
    char filename[30];
    for(i = 0; i < 30; i++)
        filename[i] = '\0';
        
    strcpy(filename, my_table_name);
    strcat(filename, ".txt");
    
    int pos[50];
    for(i = 0; i < 50; i++)
        pos[i] = 0;
        
	char new_val[50][50];
	// new_val stores values of attributes given by user
	// and space as the value for the rest of the attributes
	
	for(i = 0; i < 50; i++)
		for(j = 0; j < 50; j++)
		      new_val[i][j] = '\0';
    
    // CONSTRAINT CHECKING
        
    if(ins_case == 0)
    {
        // Fill new_val appropriately (put spaces as attribute value for attributes not entered by the user)
			
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
    	
    	// Checking for NOT NULL constraint
    	// If user did not enter an attribute name which has not null constraint
    	
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
    				printf("\nInvalid : %s cannot be null because of not null constraint.",attr_nm[i]);
    				//printf("\n\n\t\tINSERTION INVALID. \n");
    				return;
    			}
    		}
    	}
    	
    	// Find position of attributes provided by user in table
	
	
        for(i = 0; i < no_of_attr; i++)
    	{
    		for(j = 0; j < n; j++)
    		{
    			if(strcmp(att_nm[i], attr_nm[j]) == 0)
				      break;	
    		}
    		pos[i] = j;
        }
    }
    else
    {
    	// Since every attribute is provided a value
    	// So, position of every attribute is its index itself
    
    	for(i = 0; i < n; i++)
    	   pos[i] = i;
    }
    	    
    	for(i = 0; i < 50; i++)
    	    query2[i] = '\0';
    	
    	int flag = 0;
    	new_fp = fopen(filename, "r");
    	if(!new_fp) // 1st insertion, no need to check against violation of unique and primary key constraint
    		flag = 1;
    		
    	
    	// Checking for NOT NULL constraint
    	// If user entered value of an attribute which has not null constraint as null
    	
    	for(i = 0; i < no_of_attr; i++)
    	{
    		if(strstr(constraints[i], "not") != NULL)
    		{
    			if(strcmp(values[i], "null") == 0)
    			{
    				if(ins_case == 0)
    				    printf("Invalid : %s cannot be null becuase of not null constraint.", att_nm[i]);
    				else
    				    printf("Invalid : %s cannot be null becuase of not null constraint.", attr_nm[i]);
    				    
    				//printf("\n\n\t\tINSERTION INVALID. \n");
    				return;
    			}
    		}
    	
    	    int ct = 0;
    	 
		    // Checking for UNIQUE or PRIMARY KEY constraint
		    
    	    if(flag == 0) // if not 1st insertion
    	    {
	    	    if(strstr(constraints[i], "unique") != NULL || strstr(constraints[i], "primary") != NULL)
	    	    {
	    	    	int flg = 0; // primary key constraint
	    	    	if(strstr(constraints[i], "unique") == 0)
	    	    	     flg = 1; // unique constraint
	    	    	     
	    	    	while(!feof(new_fp))
	    	    	{	     
	    	    	     // To get to position of attributes' values in file
	    	    	     
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
	    	    	     
	    	    	     // Compare values to see if constraint is violated
	    	    	     
	    	    	     if(ct == pos[i])
	    	    	     {
	    	    	     	 fscanf(new_fp, "%s", query2);
	    	    	     	 //printf("\nquery2 %s", query2);
	    	    	     	 fscanf(new_fp, "%s", query3);
							 if(strcmp(query3, ";") != 0)
                             {
								 //printf("\nquery3 %s", query3);
								 
								 if(strcmp(query3, ",") == 0 || feof(new_fp))
								 {
								 	if(strcmp(query2, ",") != 0)
								 	{
								 		//printf("\n comapare %s & %s",values[i], query2);
								 		if(strcmp(values[i], query2) == 0)
								 		{
								 			if(flg == 1)
			    	    	     			{
			    	    	     				if(ins_case == 0)
				    	    	                    printf("\nInalid : 2 values can not be same because of unique constraint on %s!!!", att_nm[i]);
				    	    	                else
				    	    	                    printf("\nInvalid : 2 values can not be same because of unique constraint on %s!!!", attr_nm[i]);
				    	    	                    
				    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
				    				            return;
											}
											else
											{
												if(ins_case == 0)
				    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!", att_nm[i]);
				    	    	                else
				    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!",attr_nm[i]);
				    	    	                    
				    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
				    				            return;
											}
								 		}
								 	}
								 }
								 else
								 {
		    	    	     	     while(strcmp(query3, ",") != 0 && strcmp(query3, ";") != 0 && !feof(new_fp))
		    	    	     	     {
		    	    	     	     	//printf("hi ");
		    	    	     	     	if(feof(new_fp))
		    	    	     	     	     break;
		    	    	     		     strcat(query2," ");
		    	    	     		     strcat(query2, query3);
		    	    	     		     fscanf(new_fp, "%s", query3);
		    	    	     		    // printf("\nquery3 %s",query3);
									}
									if(strcmp(query2, ",") != 0)
								 	{
								 		//printf("\n comapare %s & %s",values[i], query2);
								 		if(strcmp(values[i], query2) == 0)
								 	    {
								 	    	if(flg == 1)
			    	    	     			{
			    	    	     				if(ins_case == 0)
				    	    	                    printf("\nInvalid : 2 values can not be same because of unique constraint on %s!!!", att_nm[i]);
				    	    	                else
				    	    	                    printf("\nInvalid : 2 values can not be same because of unique constraint on %s!!!", attr_nm[i]);
				    	    	                    
				    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
				    				            return;
											}
											else
											{
												if(ins_case == 0)
				    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!", att_nm[i]);
				    	    	                else
				    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!",attr_nm[i]);
				    	    	                    
				    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
				    				            return;
											}
								 	    }
								 	}
		    	    	     	}
		    	    	     }
		    	    	     else
		    	    	     {
		    	    	     	//printf("\n comapare %s & %s",values[i], query2);
		    	    	     	if(strcmp(values[i], query2) == 0)
		    	    	     	{
		    	    	     		if(flg == 1)
	    	    	     			{
	    	    	     				if(ins_case == 0)
		    	    	                    printf("\nInvalid : 2 values can not be same because of unique constraint on %s!!!", att_nm[i]);
		    	    	                else
		    	    	                    printf("\nInvalid : 2 values can not be same because of unique constraint on %s!!!", attr_nm[i]);
		    	    	                    
		    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
		    				            return;
									}
									else
									{
										if(ins_case == 0)
		    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!", att_nm[i]);
		    	    	                else
		    	    	                    printf("\nInvalid : 2 values can not be same because of primary key constraint on %s!!!",attr_nm[i]);
		    	    	                    
		    	                    	//printf("\n\n\t\tINSERTION INVALID. \n");
		    				            return;
									}
		    	    	     	}
		    	    	     	 
		    	    	     }
	    	    	     	// For next row
	    	    	     }
	    	    	     ct = 0;
	    	    	     
	    	    	     // For next row (to get to next ; i.e. values of next row)
	    	    	     
	    	    	     while(strcmp(query3, ";") != 0 && !feof(new_fp))
	    	    	         fscanf(new_fp, "%s", query3);

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
	    
	    // Checking for FOREIGN KEY constraint
	    
	    int val; // gives the index to look at in values in order to access values of attribute attr_foreign
	    
		for(i = 0; i < foreign; i++) // does not enter this loop if no foreign key constraint i.e. if foreign = 0
		{
			     // To get to the beginning of the file everytime
			     // (needed in case of more than 1 foreign key constraint)
			
			     fseek(schema_fp, 0, SEEK_SET);
			     
			     if(ins_case == 0)
			     {
                     for(j = 0; j < no_of_attr; j++)
		    		 {
		    			if(strcmp(attr_foreign[i], att_nm[j]) == 0) // if user assigned some value to it
						      break;	
		    		 } 
		    		 if(j == no_of_attr)
					       continue; // no need to check foreign key constraint for attr_foreign[i]
					       // continue to next iteration (increment i by 1)

					 else
					    val = j;
	    	     }
	    	     else
	    	     {
	    	     	 for(j = 0; j < n; j++)
		    		 {
		    			if(strcmp(attr_foreign[i], attr_nm[j]) == 0) // if user assigned some value to it
						      break;	
		    		 }
		    		 val = j;
	    	     }
	    	     
	    		 for(j = 0; j < 50; j++)
	             {
	                 query1[j] = '\0';
	                 query2[j] = '\0';
	             }

                // Finding the referenced table in schema file

				while(strcmp(query1, table_ref[i]) != 0)
				{
					fscanf(schema_fp, "%s", query2);
					
					if(strcmp(query2, "table") == 0)
					    fscanf(schema_fp, "%s", query1);
				}
				
				// Finding the referenced attribute's position in schema file
				
				int pos = 0;
				while(strcmp(query1, attr_ref[i]) != 0)
				{
					fscanf(schema_fp, "%s", query1);
					if(strcmp(query1, ",") == 0)
					     pos++;
				}
				
				// Keep referenced table's file name in fname
				
				int f = 0;
				char fname[30];
				strcpy(fname, table_ref[i]);
				strcat(fname, ".txt");
		
				newt_fp = fopen(fname, "r");
				if(!newt_fp)
					f = 1; // possible if table referenced has been created but no values are inserted into it yet
					
				if(f == 1)
				{
					    printf("\nInvalid : Foreign key constraint violated for %s.", attr_foreign[i]);    
						printf("\nThis is because referenced table %s has no values inserted into it yet. ", table_ref[i]);
						//printf("\n\n\t\tINSERTION INVALID. \n");
    				    return;
				}
				
				int ct = 0;
				for(j = 0; j < 50; j++)
	                 query2[j] = '\0';
	             
	             if(f == 0)
	             {
						while(!feof(newt_fp))
		    	    	{
		    	    		 // To reach the referenced attribute's values
		    	    		
		    	    	     while(strcmp(query2, ";") != 0 && !feof(newt_fp))
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
		    	    	     
		    	    	     // Compare values to see if foreign key constraint is violated
		    	    	     
		    	    	     strset(query3, '\0');
		    	    	     
		    	    	     if(ct == pos)
		    	    	     {
		    	    	     	 fscanf(newt_fp, "%s", query2);
		    	    	     	 //printf("\nquery2 %s", query2);
		    	    	     	
		    	    	     	 if(feof(newt_fp))
		    	    	     	     break;
		    	    	     	
								 fscanf(newt_fp, "%s", query3);
								 if(strcmp(query3, ";") != 0)
                                 {
									 //printf("\nquery3 %s", query3);
									 
									 if(strcmp(query3, ",") == 0 || feof(newt_fp))
									 {
									 	if(strcmp(query2, ",") != 0)
									 	{
									 		//printf("\n comapare %s & %s",values[val], query2);
									 		if(strcmp(values[val], query2) == 0)
									 		    break;
									 	}
									 }
									 else
									 {
			    	    	     	     while(strcmp(query3, ",") != 0 && strcmp(query3, ";") != 0 && !feof(newt_fp))
			    	    	     	     {
			    	    	     	     	//printf("hi ");
			    	    	     	     	if(feof(newt_fp))
			    	    	     	     	     break;
			    	    	     		     strcat(query2," ");
			    	    	     		     strcat(query2, query3);
			    	    	     		     fscanf(newt_fp, "%s", query3);
			    	    	     		     //printf("\nquery3 %s",query3);
										}
										if(strcmp(query2, ",") != 0)
									 	{
									 		//printf("\n comapare %s & %s",values[val], query2);
									 		if(strcmp(values[val], query2) == 0)
									 		    break;
									 	}
			    	    	     	}
			    	    	     }
			    	    	     else
			    	    	     {
			    	    	     	//printf("\n comapare %s & %s",values[val], query2);
			    	    	     	if(strcmp(values[val], query2) == 0)
			    	    	     	     break;
			    	    	     }
	    	    	         }
	    	    	         
	    	    	         // For next row
	    	    	         
	    	    	         while(strcmp(query3, ";") != 0 && !feof(newt_fp))
		    	    	      {
		    	    	      	   //printf("\n see query3 %s", query3);
		    	    	           fscanf(newt_fp, "%s", query3);
		    	    	       }
		
							 int m; 
		    	    	     for(m = 0; m < 50; m++)
		    	    	          query2[m] = '\0';
		    	    	
						}
						if(feof(newt_fp))
						{
							printf("\nInvalid : Foreign key constraint violated for %s.", attr_foreign[i]);
							//printf("\n\n\t\tINSERTION INVALID. \n");
	    				    return;
						}
						fclose(newt_fp);
				}
		}
		
		fclose(schema_fp);
		
		new_fp = fopen(filename, "a");
		
		// Successful insertion
		// Append values in file
		
		for(i = 0; i < n; i++)
	    {
	    	if(i == n - 1)
	    	{
	    	   if(ins_case == 0)
	    	       fprintf(new_fp, "%s ", new_val[i]);
	    	    else
	    	       fprintf(new_fp, "%s ", values[i]);
	    	}
	    	else
	    	{
	            if(ins_case == 0)
	    	        fprintf(new_fp, "%s , ", new_val[i]);
	    	    else
	    	        fprintf(new_fp, "%s , ", values[i]);
	    	}
	    }
	    fprintf(new_fp, "; ");
	    printf("\nINSERTION SUCCESSFUL.\n");
	    fclose(new_fp);
}
