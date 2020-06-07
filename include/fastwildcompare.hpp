// Copyright 2018 IBM Corporation
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Compares two text strings.  Accepts '?' as a single-character wildcard.  
// For each '*' wildcard, seeks out a matching sequence of any characters 
// beyond it.  Otherwise compares the strings a character at a time. 
//
#pragma once
#include <locale>

template <class Char>
bool FastWildCompare(Char const *pWild, Char const *pTame)
{
  Char const *pWildSequence; // Points to prospective wild string match after '*'
  Char const *pTameSequence; // Points to prospective tame string match
  auto const &facet = std::use_facet<std::ctype<Char>>(std::locale());

  // Find a first wildcard, if one exists, and the beginning of any  
  // prospectively matching sequence after it.
  for ( ;; ) {
    // Check for the end from the start.  Get out fast, if possible.
    if ( !*pTame ) {
      if ( *pWild ) {
        while ( *pWild++ == '*' ) {
          if ( !*pWild || *pWild == ';' )
            return true; // "ab" matches "ab*".
        }
        return false; // "abcd" doesn't match "abc".
      } else {
        return true; // "abc" matches "abc".
      }
    } else if ( *pWild == '*' ) {
      // Got wild: set up for the second loop and skip on down there.
      while ( *(++pWild) == '*' )
        continue;

      if ( !*pWild || *pWild == ';' )
        return true; // "abc*" matches "abcd".

      // Search for the next prospective match.
      if ( *pWild != '?' ) {
        while ( facet.toupper(*pWild) != facet.toupper(*pTame) ) {
          if ( !*(++pTame) )
            return false; // "a*bc" doesn't match "ab".
        }
      }

      // Keep fallback positions for retry in case of incomplete match.
      pWildSequence = pWild;
      pTameSequence = pTame;
      break;
    } else if ( facet.toupper(*pWild) != facet.toupper(*pTame) && *pWild != '?' ) {
      return false; // "abc" doesn't match "abd".
    }

    ++pWild; // Everything's a match, so far.
    ++pTame;
  }

  // Find any further wildcards and any further matching sequences.
  for ( ;; ) {
    if ( *pWild == '*' ) {
      // Got wild again.
      while ( *(++pWild) == '*' )
        continue;

      if ( !*pWild || *pWild == ';' )
        return true; // "ab*c*" matches "abcd".

      if ( !*pTame )
        return false; // "*bcd*" doesn't match "abc".

      // Search for the next prospective match.
      if ( *pWild != '?' ) {
        while ( facet.toupper(*pWild) != facet.toupper(*pTame) ) {
          if ( !*(++pTame) )
            return false; // "a*b*c" doesn't match "ab".
        }
      }

      // Keep the new fallback positions.
      pWildSequence = pWild;
      pTameSequence = pTame;
    } else if ( facet.toupper(*pWild) != facet.toupper(*pTame) && *pWild != '?' ) {
      // The equivalent portion of the upper loop is really simple.
      if ( !*pTame )
        return false; // "*bcd" doesn't match "abc".

      // A fine time for questions.
      while ( *pWildSequence == '?' ) {
        ++pWildSequence;
        ++pTameSequence;
      }
      pWild = pWildSequence;

      // Fall back, but never so far again.
      while ( facet.toupper(*pWild) != facet.toupper(*(++pTameSequence)) ) {
        if ( !*pTameSequence )
          return false; // "*a*b" doesn't match "ac".
      }
      pTame = pTameSequence;
    }

    // Another check for the end, at the end.
    if ( !*pTame )
      return !*pWild || *pWild == ';';

    ++pWild; // Everything's still a match.
    ++pTame;
  }
}
