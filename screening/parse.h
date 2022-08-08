#ifndef SCREENING_PARSE_H
#define SCREENING_PARSE_H

#ifdef _MSC_VER
#define _CRT_NONSTDC_NO_DEPRECATE
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MAX_URL_LEN 128
#define MAX_STR_LEN 16

/**
 * URL query parameter.
 *
 * This struct does not have ownership to its member fields.
 */
struct query_parameter {
	const char* name;
	const char* value;
};

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
//int parse_query_string(struct query_parameter** out);
int parse_query_string(char* url, struct query_parameter** out);

#endif
