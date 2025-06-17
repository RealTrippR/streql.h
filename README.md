# streql.h
<BR>

streql.h is a highly optimized C function to compare equality between two strings. It uses Intel's SIMD instructions if available to maximize perforance.

<BR>
<ins><B>Only use streql on values that are not known at compile time.</B></ins> - some C compilers will evaluate strcmp at compile time if it's comparing constant literals. This cannot be done with streql (yet.)


        Time comparison (milliseconds) @ 100 million iterations
| streql(SIMD)  | streql(No SIMD) | strneql(No SIMD) | strcmp == 0 |
| ------------- | -------------   | -------------    | ----------  |    
| 1696          | 6706            | 1791             | 4918        |
