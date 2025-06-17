# streql.h
<BR>

streql.h is a highly optimized C function to compare equality between two strings. It uses Intel's SIMD instructions if available to maximize perforance.

<BR>
#Only use streql on values that are not known at compile time.
Note that some C compilers will evaluate strcmp at compile time if it's comparing constant literals. 
| streql(SIMD) | streql(No SIMD) |
| ------------- | ------------- |
|   | Content Cell  |
| Content Cell  | Content Cell  |
