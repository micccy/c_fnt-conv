# CPI and PSF font file tools

**Standard disclaimer:** _this is some code of mine that was created for some very specific purpose and might not work as intended. However it may still be useful to someone so here it is._

**Status:** _Early development_

**Description:** A small set of tools to deal with linux's PC Screen Font (.psf, .psfu) and msdos' Code Page Information (.cpi) files

**Directory "cpdef"** contains the definitions of various 8-bit codepages to use for unicode conversion, in the following format:  
``00DF,03B2,0053+005A,0073+007A                     0xE1 (225) : 'ß','β','S'+'Z','s'+'z'``  
Plese note that this is not a correct definition (that letter doesn't work like that) but it makes an example easy to understand.  
The first line describes glyph #000, the second line glyph #001 and so on until the 256th line describing glyph #255.  
Each line contains a set of hexadecimal values separated by [,] or [+].  
I've made them all 4 figures wide and uppercase because it's easier to read, but any formatting is valid as long as it only contains [0-9 A-F a-f] characters separated only by [,+].  
Anything else will be considered an input termination, which means that you can write any useful comment after the numbers simply starting with any "illegal" character including space and tab.  
The tool bin/cpretty can read such files and output to stdout the definitions followed by the glyph number in hex and decimal at a fixed position if possible, and prints out the defined characters in utf8. Its output can be redirected to **another** file to get every line automatically formatted and commented.  
The next line begins at the first [0-9 A-F a-f] charater following a linefeed.

**WARNING**: _Do not redirect the output to the same file used as input. This will delete every line except for the first one since data is printed out while the file is being read, instead of after._

Every line must have at least the first number (0 for both undefined and NUL) which represents the unicode character to tie biunivocally (both ways) to the corresponding glyph.  
This "primary" number can be followed exclusively by a [,] before a list of other "secondary" characters (and then eventually character sequences) that can eventually be represented by the same glyph if they lack a "primary" role anywhere else.  
After all single characters, sequences can be specified by listing all the characters in them separated by a [+], and they must be separeted by [,].  
A sequence will only display said glyph if no other way to display it exists (if at least one of the characters has no representation elsewhere).

So, the (not real) example given above means:  
- The glyph in this position represents U+00DF, the german letter eszett.  
- However, it could also be used to represent U+03B2, the greek letter lowercase beta, if it lacks a proper definition elsewhere  
- Also, if U+0053 (uppercase s) and/or U+005A (uppercase z) cannot be represented at all, and a 'SZ' is encountered, display this glyph instead of them  
- Also, if U+0073 (lowercase s) and/or U+007A (lowercase z) cannot be represented at all, and a 'sz' is encountered, display this glyph instead of them 
