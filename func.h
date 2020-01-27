int check_table_exists(char my_table_name[30])
{
	FILE *table_fp;
	char table_name[30];
	
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
        {
           fclose(table_fp);
           return 1;
        }
    }	
	
	fclose(table_fp);
	return 0;
}

//checking the existence of column in the schema file
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
		if(strcmp(temp,",") == 0)
		{
			fscanf(schema_fp,"%s",temp);
			column_count++;
	        }
	    if(strcmp(temp,col_nm) == 0)
	       break;
		 
       }
    if(feof(schema_fp))
       column_count=0;
    fclose(schema_fp);
    return column_count;
}

