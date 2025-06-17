# streql.h
<BR>

streql.h is a highly optimized pair of C functions to compare equality between two strings. It uses Intel's SIMD instructions if available to maximize perforance.

<ins><B>Only use streql on values that are not known at compile time.</B></ins> Some C compilers will evaluate strcmp at compile time if it's comparing constant literals. This cannot be done with streql (yet.)

<BR>

        Time comparison (milliseconds) @ 100 million iterations
|String size| streql(SIMD)  | streql(No SIMD) | strneql(SIMD) | strcmp == 0 |
|-------------| ------------- | -------------   | -------------    | ----------  |   
| 16            | 1706          | 4565            | 889             | 3389        |
| 25            | 1696          | 6706            | 1791             | 4918        |
| 64            | 4180          | 16328            | 3445             | 12912        |
