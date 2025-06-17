#include "streql.h"
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <stdlib.h>

#define TEST_ITR_COUNT 100000000ul
#define STR_SIZE 25

// Generate a random string of length 'len'
// The string is null-terminated and made of ASCII chars 32-126
char* random_string(size_t len) {
	const char charset[] = "abcdefghijklmnopqrstuvwxyz"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789"
		"!@#$%^&*()-_=+[]{}|;:,.<>?";

	size_t charset_size = sizeof(charset) - 1; // exclude terminating '\0'

	char* str = (char*) malloc(len + 1);
	if (!str) return NULL;

	for (size_t i = 0; i < len; i++) {
		srand(time(0) + i);
		int key = rand() % charset_size;
		str[i] = charset[key];
	}
	str[len] = '\0';

	return str;
}

inline bool streqlNOSIMD(const char* str1, const char* str2)
{
	while (true) {
		if (str1[0] != str2[0]) {
			return false;
		}
		if (str1[0] == '\0' || str2[0] == '\0') {
			return true;
		}
		str1++;
		str2++;
	}
}


int main() 
{
	char* str1;
	char* str2;
	char* str3;
	char* str4;
	char* str5;
	char* str6;
	char* str7;
	str1 = random_string(STR_SIZE);
	str2 = random_string(STR_SIZE);
	str3 = random_string(STR_SIZE);
	str4 = random_string(STR_SIZE);
	str5 = random_string(STR_SIZE);
	str6 = random_string(STR_SIZE);
	str7 = random_string(STR_SIZE);


	volatile bool res1 = false;
	volatile bool res2 = false;
	volatile bool res3 = false;
	volatile bool res4 = false;
	volatile bool res5 = false;
	volatile bool res6 = false;
	volatile bool res7 = false;

	auto start = std::chrono::system_clock::now();
	for (size_t i = 0; i < TEST_ITR_COUNT; ++i)
	{
		res1 = streql((const char*)str5, (const char*)str7);
		res2 = streql((const char*)str1, (const char*)str2);
		res3 = streql((const char*)str1, (const char*)str6);
		res4 = streql((const char*)str5, (const char*)str5);
		res5 = streql((const char*)str2, (const char*)str5);
		res6 = streql((const char*)str2, (const char*)str3);
		res7 = streql((const char*)str7, (const char*)str6);
	}
	auto end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	uint64_t elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("Res. sum: %d\n", res1 + res2 + res3 + res4 + res5 + res6 + res7);
	std::cout << "Elapsed time @ " << TEST_ITR_COUNT << " iterations, Tripp's streql with SIMD: on a string of " << STR_SIZE << " bytes: "  << elapsed_ms << " milliseconds" << std::endl;

	

	str1 = random_string(STR_SIZE);
	str2 = random_string(STR_SIZE);
	str3 = random_string(STR_SIZE);
	str4 = random_string(STR_SIZE);
	str5 = random_string(STR_SIZE);
	str6 = random_string(STR_SIZE);
	str7 = random_string(STR_SIZE);



	// for some reason strneql is almost twice as fast as streql if the string has a length that is a multiple of 16
	start = std::chrono::system_clock::now();
	for (size_t i = 0; i < TEST_ITR_COUNT; ++i)
	{
		res1 = strneql((const char*)str5, (const char*)str7, STR_SIZE);
		res2 = strneql((const char*)str1, (const char*)str2, STR_SIZE);
		res3 = strneql((const char*)str1, (const char*)str6, STR_SIZE);
		res4 = strneql((const char*)str5, (const char*)str5, STR_SIZE);
		res5 = strneql((const char*)str2, (const char*)str5, STR_SIZE);
		res6 = strneql((const char*)str2, (const char*)str3, STR_SIZE);
		res7 = strneql((const char*)str7, (const char*)str6, STR_SIZE);
	}
	end = std::chrono::system_clock::now();

	elapsed_seconds = end - start;
	elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("Res. sum: %d\n", res1 + res2 + res3 + res4 + res5 + res6 + res7);
	std::cout << "Elapsed time @ " << TEST_ITR_COUNT << " iterations, Tripp's strneql with SIMD: on a string of " << STR_SIZE << " bytes: " << elapsed_ms << " milliseconds" << std::endl;


	str1 = random_string(STR_SIZE);
	str2 = random_string(STR_SIZE);
	str3 = random_string(STR_SIZE);
	str4 = random_string(STR_SIZE);
	str5 = random_string(STR_SIZE);
	str6 = random_string(STR_SIZE);
	str7 = random_string(STR_SIZE);

	res1 = false;
	res2 = false;
	res3 = false;
	res4 = false;
	res5 = false;
	res6 = false;
	res7 = false;

	start = std::chrono::system_clock::now();
	for (size_t i = 0; i < TEST_ITR_COUNT; ++i)
	{
		res1 = streqlNOSIMD((const char*)str5, (const char*)str7);
		res2 = streqlNOSIMD((const char*)str1, (const char*)str2);
		res3 = streqlNOSIMD((const char*)str1, (const char*)str6);
		res4 = streqlNOSIMD((const char*)str5, (const char*)str5);
		res5 = streqlNOSIMD((const char*)str2, (const char*)str5);
		res6 = streqlNOSIMD((const char*)str2, (const char*)str3);
		res7 = streqlNOSIMD((const char*)str7, (const char*)str6);
	}
	end = std::chrono::system_clock::now();

	elapsed_seconds = end - start;
	elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("Res. sum: %d\n", res1 + res2 + res3 + res4 + res5 + res6 + res7);
	std::cout << "Elapsed time @ "<< TEST_ITR_COUNT << " iterations, Tripp's streql without SIMD on a string of " << STR_SIZE << " bytes: " << elapsed_ms << " milliseconds" << std::endl;


	str1 = random_string(STR_SIZE);
	str2 = random_string(STR_SIZE);
	str3 = random_string(STR_SIZE);
	str4 = random_string(STR_SIZE);
	str5 = random_string(STR_SIZE);
	str6 = random_string(STR_SIZE);
	str7 = random_string(STR_SIZE);

	res1 = false;
	res2 = false;
	res3 = false;
	res4 = false;
	res5 = false;
	res6 = false;
	res7 = false;

	start = std::chrono::system_clock::now();
	for (size_t i = 0; i < TEST_ITR_COUNT; ++i)
	{
		res1 = (strcmp((const char*)str5, (const char*)str7) == 0);
		res2 = (strcmp((const char*)str1, (const char*)str2) == 0);
		res3 = (strcmp((const char*)str1, (const char*)str6) == 0);
		res4 = (strcmp((const char*)str5, (const char*)str5) == 0);
		res5 = (strcmp((const char*)str2, (const char*)str5) == 0);
		res6 = (strcmp((const char*)str2, (const char*)str3) == 0);
		res7 = (strcmp((const char*)str7, (const char*)str6) == 0);
	}
	end = std::chrono::system_clock::now();

	printf("Res. sum: %d\n", res1 + res2 + res3 + res4 + res5 + res6 + res7);

	elapsed_seconds = end - start;
	elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	std::cout << "Elapsed time @ " << TEST_ITR_COUNT << " iterations, traditional (strcmp(A,B) == 0) on a string of " << STR_SIZE << " bytes: " << elapsed_ms << " milliseconds" << std::endl;
}
