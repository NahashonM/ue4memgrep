#include "includes.h"


// Creates BHT table for Bad Character Heuristics
// @Param pattern: The pattern P being looked up in text T
// @Param bht:		The bad character heuristics table
// @Param szPattern: Length of the pattern P
// @Param szAlphabet: Length of the alphabet
//----------------------------------------------------------------------------
int GenBhtTable(const char* pattern, int* bhtTable, int szPattern, int szAlphabet) {
	
	// check 0 length pattern & alphabet and empty pattern
	if (!pattern || !bhtTable) return 0;

	// set the alignment values for each alphabet character in the bht
	for(int i=0; i < szAlphabet; i++) 
		bhtTable[i] = szPattern;

	// set bad character table for pattern; ignore last character
	for(int i=0; i < szPattern - 1; i++)
		bhtTable[ pattern[i] ] = szPattern - 1 - i;

	return 1;
}




int  BmpPatternMatch(const char* pattern, const int* bhtTable, int szPattern, const char* text, int szText){
	
	if (!pattern || !bhtTable || !text) return -1;

	int j = szPattern - 1;

	for (int i=0; (i + szPattern)< szText;){

		if ( pattern[j] == text[i + j] ) {		// if text and pattern characters allign

			j--;

			if (j <= 0) {								// if its a complete match j < 0
				//we only need one match for now
				return i;
				//i = i + szPattern;						// move i past the matched charactes [if matched caanot be part of next match]
				//j = szPattern - 1;						// reset j to last character of pattern
				//count++;									// just to say we found you
			}

			continue;										// search for more
															//
															//
		}else{									// current index does not allign with bht table
			i += bhtTable[ text[i+j] ];				// check in bht table how much to move to align char with equal in pattern
			j = szPattern - 1;						// reset j to last char in pattern just in case we had partial match
		}

	}

	return -1;		// return -1 if not found
}
