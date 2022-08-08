#include "parse.h"
#include "./third_party/cJSON/cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char*
file_get_contents(const char* path)
{
	FILE* file;
	long size;
	char* buffer;

	if ((file = fopen(path, "r")) == NULL) {
		printf("file open error!");
		exit(0);
	}
	// int fseek(FILE *stream, long offset, int fromwhere);
	fseek(file, 0L, SEEK_END);
	// Used to obtain the offset bytes of the current position of the file position pointer relative to the file head.
	// long ftell(FILE *stream);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	buffer = malloc(size + 1);
	if (buffer == NULL) {
		fclose(file);
		return 0;
	}

	fread(buffer, 1, size, file);
	//fread(buffer, size, 1, file);
	buffer[size] = '\0';
	fclose(file);
	return buffer;
}

static int
cJSON_GetObjectSize(const cJSON* object)
{
	int size = 0;

	for (const cJSON* child = object->child; child; child = child->next) {
		size++;
	}
	return size;
}

#define return_error() do {					\
	printf("ERROR: file %s line %d\n", __FILE__, __LINE__);	\
	cJSON_Delete(json);					\
	return 1;						\
} while (0)

int
main(void)
{
	char* json_string = NULL;
	cJSON* json = NULL;
	const cJSON* testcases;
	const cJSON* testcase;

	json_string = file_get_contents("./testcases.json");
	// print test
	// printf("%s", json_string);
	if (!json_string) {
		// printf("unknown file");
		return 1;
	}
	json = cJSON_Parse(json_string);
	free(json_string);
	// print test
	// printf("%s", cJSON_Print(json));
	if (!cJSON_IsObject(json)) {
		return_error();
	}
	testcases = cJSON_GetObjectItem(json, "testcases");
	if (!cJSON_IsArray(testcases)) {
		printf("error");
		return_error();
	}
	// print test
	// printf("%s", cJSON_Print(testcases));
		
	cJSON_ArrayForEach (testcase, testcases) {
		const cJSON* input = NULL;
		const cJSON* output = NULL;
		char* url;
		//struct query_parameter* params;
		// struct query_parameter params[MAX_URL_LEN] = { 0 };
		
		// Memory must be allocated
		struct query_parameter* params = (struct query_parameter*)malloc(sizeof(struct query_parameter) * MAX_URL_LEN);
		
		int count = 0;

		input = cJSON_GetObjectItem(testcase, "input");
		output = cJSON_GetObjectItem(testcase, "output");
		url = strdup(input->valuestring);
		count = parse_query_string(url, &params);
		// free(input);
		free(url);
		

		printf("\ncount %d\n", count);
		if (count != cJSON_GetObjectSize(output)) {
			return_error();
		}
		for (int i = 0; i < count; i++) {
			const cJSON* json_value;

			output = cJSON_GetObjectItem(testcase, "output");
			//printf("%s\n", output->valuestring);
			printf("[%s] = %s\n", params[i].name, params[i].value);
			json_value = cJSON_GetObjectItem(output, params[i].name);
			if (!cJSON_IsString(json_value)) {
				return_error();
			}
			if (strcmp(json_value->valuestring, params[i].value) != 0) {
				return_error();
			}
		}
		printf("OK\n\n");
	}
	
	cJSON_Delete(json);
	return 0;
}
