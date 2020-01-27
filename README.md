# MiniDBMS
A mini version of a database mangement system

We are providing a MINIDBMS prompt to execute the following four commands. User should type exit and press enter key to exit.
The commands are as under:
1.	CREATE
•	SYNTAX
create table table_name
(attribute_name attribute_type unique/not null/primary key,
foreign key(attribute_name) references other_table_name(its_attribute_name));
Note: User can enter foreign key constraint at end only (after all the attribute names with their types). And other constraints (not null/unique/primary key) must be entered immediately after the type of the respective attribute.
•	DATA TYPES ALLOWED
	Int
	Char
	Date
	Boolean
	Varchar() – string with variable length
	Char() – string with fixed length
	Double() – floating-point number with precision

2.	INSERT
•	SYNTAX

insert into table_name(attributes optional)
values(string/char/date/boolean/int/double);

Note: User can either enter attribute names in which values are to be inserted after table name or the user can skip attribute names in which values are to be inserted in which case values must be assigned to all the attributes.

•	VALUES ALLOWED
	String - either of type char() or varchar(), user must give length within brackets; string must be in double quotes
	Character – of type char and must be in single quotes
	Date – of type date, format : DD-MM-YYYY
	Boolean – of type Boolean, can be true or false
	Digit – of type int
	Floating point number – of type double(), user must give precision within brackets
3.	SELECT
•	SYNTAX
select attribute_names(or *) from table_name
where (not optional) attribute_name ==/>=/<=/>/</!= value;

Note: User can select attributes using where clause with single predicate.

4.	DELETE
•	SYNTAX
delete from table_name
where (not optional) attribute_name ==/>=/<=/>/</!= value;

Note: User can select attributes using where clause with single predicate.
In case of foreign key constraint, if primary key attribute is referenced by some other attribute in some other table (foreign key). Then if some of the values of the primary key attribute are deleted then NULL cascading is done in the other table (in values of foreign key).
