/*
Copyright © 2025 Tripp Robins

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the “Software”), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef TR_STR_EQL_H
#define TR_STR_EQL_H


#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#define TRIPP_STREQL_USE_SIMD
#elif defined(__SSE2__) || defined(_M_IX86) || defined( USE_SOFT_INTRINSICS) || defined(_M_X64)
#define TRIPP_STREQL_USE_SIMD
#include <immintrin.h>
#else
#define TRIPP_STREQL_SIMD_NOT_SUPPORTED
#endif

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/// streql() - Tests equality between two strings
/// @name streql
/// @brief Tests equality between two C strings. It differs from strcmp in 
/// the fact that it stops once immediately after a mismatch is detected, 
/// thus it is more efficient for comparison of direct equality.
static inline bool streql(const char* str1, const char* str2)
{
#ifdef TRIPP_STREQL_SIMD_NOT_SUPPORTED
    uint64_t i = 0u;
    while (true) {
        if (str1[i] != str2[i]) {
            return false;
        }
        if (str1[i] == '\0' || str2[i] == '\0') {
            return true;
        }
        i++;
    }
    return true;
#elif defined(TRIPP_STREQL_USE_SIMD)
    // TRIPP_STREQL_USE_SIMD
    
    while (true) 
    {
        // Load next 16 bytes from str1 and str2
        register __m128i va = _mm_loadu_si128((const __m128i*)str1);
        register __m128i vb = _mm_loadu_si128((const __m128i*)str2);
        // https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=4842,1047&text=cmpistr
        if (_mm_cmpistrc(va, vb, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY)) {
            return false; // not equal
        }
        // https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#ig_expand=4842,1042&text=cmpistr
        if (_mm_cmpistrz(va, vb, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH) == true) {
            // a null terminator was hit
            return true;
        }

        str1 += 16;
        str2 += 16;
    }
    

#endif
}

/// strneql() - Tests equality between two strings within range n
/// @name strneql
/// @brief Tests equality between two C strings within range n. It differs from strncmp in 
/// the fact that it stops once immediately after a mismatch is detected, 
/// thus it is more efficient for comparison of direct equality.
inline bool strneql(const char* str1, const char* str2, size_t n)
{
#ifdef TRIPP_STREQL_SIMD_NOT_SUPPORTED
    size_t i = 0;
    while (i < n) {
        if (str1[i] != str2[i]) {
            return false;
        }
        if (str1[i] == '\0' || str2[i] == '\0') {
            return true;
        }
        i++;
    }
    return true;
#elif defined(TRIPP_STREQL_USE_SIMD)
    
    while (n > 0) {
        const uint8_t buffLen = (n >= 16) ? 16 : (uint8_t)n;

        // Load next 16 bytes
        __m128i va = _mm_loadu_si128((const __m128i*)str1);
        __m128i vb = _mm_loadu_si128((const __m128i*)str2);

        // Create mask to 0 out all characters above bufflen
        __m128i indices = _mm_set_epi8(15, 14, 13, 12, 11, 10, 9, 8,
            7, 6, 5, 4, 3, 2, 1, 0);
        __m128i limit = _mm_set1_epi8((char)buffLen);
        __m128i mask = _mm_cmplt_epi8(indices, limit);

        // Mask out all characters above buffLen
        va = _mm_and_si128(va, mask);
        vb = _mm_and_si128(vb, mask);

        if (_mm_cmpistrc(va, vb,
            _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY)) {
            return false;
        }

        if (_mm_cmpistrz(va, vb, _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH)) {
            return true;
        }

        str1 += buffLen;
        str2 += buffLen;
        n -= buffLen;
    }
    return true;
    
#endif
}

#endif
