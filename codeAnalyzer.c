#include "codeAnalyzer.h"

char* sub_string (const char* input, int offset, int len, char* dest)
{
	if(input == NULL || dest == NULL)
		exit(-1);
  	int input_len = strlen (input);

  	if (offset + len > input_len)
  	{
     	return NULL;
  	}

  	strncpy (dest, input + offset, len);
  	dest[len] = '\0';
  	return dest;
}

char* deblank(char* input)                                         
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]=0;
    return output;
}

void parse_commands(Node* command, List* variables){
	if(command == NULL){
		return;
	}
	if(command -> tag == 1){
		type_wrapper* var_com = (type_wrapper*)parse_creater((char*)command -> value);
	 	push_list(variables -> start,  var_com, command -> tag);
	}
	if(command -> tag == 0){
		//type_wrapper* var_com = (type_wrapper*)parse_creater((char*)command -> value);
		//parse_free((char*)command -> value);
	}
	parse_commands(command -> next, variables);
}

type_wrapper* new_type_wrapper(int type, void* data_in){
	type_wrapper* res = (type_wrapper*)s_malloc(sizeof(type_wrapper));
	res -> data_type = type;
	res -> data = data_in;
	return res;
}

int get_type_of_command(char* input){
	if(input == NULL)
		return -2;
	if(strstr(input, "create") != NULL){
		return 1;
	}
	else if(strstr(input, "free") != NULL){
		return 0;
	}
	else
		return -1;
}

List* parser(char* filename){
	List* variables = new_list((void*)NULL);
	FILE *f = fopen(filename, "rb");
	if(f == NULL)
		exit(1);
	
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *str = (char*)malloc(fsize + 1);
	fread(str, fsize, 1, f);
	fclose(f);

	str[fsize] = 0;
	
	char content[strlen(str)];
	strncpy(content, str, strlen(str));
	
	char* splited = strtok(content, ";");
	List* array_of_tokens = new_list((void*)splited);
	array_of_tokens -> start -> tag = get_type_of_command(splited);

	while(splited != NULL)
	{
		splited = strtok(NULL, ";");
		if(splited != NULL)
			push_list(array_of_tokens -> start, splited, get_type_of_command(splited));
	}

	free(str);
	parse_commands(array_of_tokens -> start, variables);
	return variables;
}

void* parse_creater(char* create_block){
	char* brackets_block;
	if((brackets_block = strstr(create_block, "(")) == NULL)
		exit(-1);
	char* variable;
	if((variable = strstr(brackets_block, "int")) == NULL && (variable = strstr(brackets_block, "str")) == NULL)
		exit(-1);
	char* type;

	if((type = strstr(variable, "int")) != NULL){
		char* value;
		if((value = strstr(variable, "{")) != NULL){
			char name[MAX_VARIABLE_NAME_LEN];
			sub_string(variable, 3, strlen(variable) - strlen(value) - 3, name);
		}
		char parset_value[strlen(value)-1];
		int* res_int_value = (int*)s_malloc(sizeof(int));
		*res_int_value = atoi(sub_string(deblank(value), 1, strlen(value)-3, parset_value));

		return new_type_wrapper(1, res_int_value);
	}

	if((type = strstr(variable, "str")) != NULL){
		char* value;
		
		if((value = strstr(variable, "{")) != NULL){
			char name[MAX_VARIABLE_NAME_LEN];
			sub_string(variable, 3, strlen(variable) - strlen(value) - 3, name);
		}

		char parset_value[strlen(value)-1];
		char* res_str_value = (char*)s_malloc(sizeof(char));
		res_str_value = sub_string(deblank(value), 1, strlen(value)-3, parset_value);
		return new_type_wrapper(0, res_str_value);
	}

	return NULL;
}		

void* parse_free(char* free_block){
	//get variable name
	//request to holder
	return NULL;
}
