#include "parse.h"
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <assert.h>

#define _URL_DEBUG__ 1


#define return_error() do {					\
	printf("ANALYSIS ERROR: file %s line %d\n", __FILE__, __LINE__);	\
	return NULL;						\
} while (0)

static inline int
string_equal(const char* pos, const char* compare, int len) {
	for (int i = 0; i < len; i++) {
		if (pos[i] != compare[i]) return -1;
	}
	return 0;
}
/**
* compare two string
*/
static inline int
find_string(const char* url, const char* compare_str) {
	size_t len_url = strlen(url);
	size_t len_compare = strlen(compare_str);
	if (len_url < len_compare) return -1;

	const char* pos = url;
	const char* pos_end = url + len_url - 1;
	// loop [len_url - 1 - len_compare]
	// printf("test : %d, %d\n", len_url - 1 - len_compare, (int)(pos_end - compare_str));
	for (; pos <= pos_end - len_compare; pos++) {
		if (string_equal(pos, compare_str, len_compare) == 0)
			return (int)(pos - url);
	}
	return -1;
}

char* parse_protocol(char* url, const char* url_copy_end_pos) {
	int analysis = 0;
	if ((analysis = find_string(url, "://")) == -1)
		return_error();
	url += analysis + 3;
	// only once
	if (url >= url_copy_end_pos) return NULL;
	if ((analysis = find_string(url, "://")) >= 0)
		return_error();
	return url;
}

char* parse_domain(char* url, const char* url_copy_end_pos) {
	int analysis = 0;
	// find domain;
	if ((analysis = find_string(url, "/")) == -1)
		return_error();
	while ((analysis = find_string(url, "/")) >= 0) {
		url += analysis + 1;
	}
	if (url >= url_copy_end_pos) return NULL;
	return url;
}

// ?
char* parse_connector(char* url, const char* url_copy_end_pos) {
	// find ?
	int analysis = 0;
	if ((analysis = find_string(url, "?")) == -1)
		return_error();
	else {
		url += analysis + 1;
	}
	if (url >= url_copy_end_pos) return NULL;
	return url;
}

int query_equal_cnt(char* url) {
	int cnt = 0;
	int url_len = strlen(url);
	for (int i = 0; i < url_len; i++) {
		if (url[i] == '=') cnt++;
	}
	return cnt;
}

// analysis parameter
int parse_param(char* url, const char* url_copy_end_pos, struct query_parameter** out) {
	
	int len = strlen(url);
	char str[MAX_URL_LEN] = { 0 };
	memcpy(str, url, len);
	
	char delim[] = " =&";
	char* token;
	int cnt = 0;
	struct query_parameter QP[MAX_URL_LEN] = {0};
	/*for (int i = 0; i < MAX_URL_LEN; i++) {
		QP[i].name = NULL;
		QP[i].value = NULL;
	}*/

	for (token = strtok(str, delim); token != NULL; token = strtok(NULL, delim)) {
		if (QP[cnt].name == NULL) QP[cnt].name = token;
		else if (QP[cnt].value == NULL && token != " ") {
			QP[cnt].value = token;
			cnt++;
		}
	}
	for (int i = 0; i < MAX_URL_LEN; i++) {
		(*out)[i].name = QP[i].name;
		(*out)[i].value = QP[i].value;
	}
	//*out = QP;
	/*for (token = strtok(str, delim); token != NULL; token = strtok(NULL, delim)) {
		if((*out)[cnt].name == NULL) (*out)[cnt].name = token;
		else if ((*out)[cnt].value == NULL) {
			(*out)[cnt].value = token;
			cnt++;
		}
	}*/
	return cnt;
}


/**
 * Parse URL query string.
 *
 * If the URL doesn't have a query string, both inputs are unmodified.
 * It's the caller's responsibility to unescape the parameters.
 *
 * @param[in,out] url The URL to parse. Ownership is borrowed temporarily from
 *                    the caller. Use strdup() to keep an original copy.
 *
 * @param[out] out Parse result. Ownership is returned to the caller.
 *
 * @return Number of parsed parameters, or -errno on error.
 */

/// @param[in] http://example.com/xxx?id=1234&data_type=article&user=john
int
parse_query_string(char* url, struct query_parameter** out)
{
	
	// copy
	char* url_copy = strdup(url);
	char* pos = url_copy;
	size_t len = strlen(url_copy);
	const char* url_copy_end_pos = url_copy + len - 1;
	
	// erase protocol
	pos = parse_protocol(pos, url_copy_end_pos);
	assert(pos != NULL);
#if _URL_DEBUG__
	printf("erase protocol : %s\n", pos);
#endif

	// erase domain
	pos = parse_domain(pos, url_copy_end_pos);
	assert(pos != NULL);
#if _URL_DEBUG__
	printf("erase domain : %s\n", pos);
#endif

	// erase connector '?'
	pos = parse_connector(pos, url_copy_end_pos);
	assert(pos != NULL);
#if _URL_DEBUG__
	printf("erase connector : %s\n", pos);
#endif

	int cnt;
	cnt = parse_param(pos, url_copy_end_pos, out);
	free(url_copy);
	/* TODO */
	return cnt;
}
