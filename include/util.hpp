#pragma once
#include <phnt_windows.h>
#include <phnt.h>
#include <span>
#include <vector>
#include <ntmm.hpp>

[[nodiscard]] inline std::vector<PVOID> duplicate_vftable(PVOID thisptr)
{
  std::vector<PVOID> v;
  MEMORY_BASIC_INFORMATION mbi;
  for ( auto vfptr = *reinterpret_cast<PVOID **>(thisptr); VirtualQuery(*vfptr, &mbi, sizeof(MEMORY_BASIC_INFORMATION)); ++vfptr ) {
    if ( (mbi.Protect & nt::mm::page_execute_any) == 0 )
      break;
    v.emplace_back(*vfptr);
  }
  return v;
}

struct patternbyte
{
  const unsigned char value;
  const unsigned char mask;

  patternbyte() : value(0), mask(0) {}
  patternbyte(const unsigned char value, const unsigned char mask = 0xff) : value(value), mask(mask) {}

  inline bool operator==(const unsigned char &rhs) const
  {
    return (rhs & mask) == (value & mask);
  }

  friend inline bool operator==(const unsigned char &lhs, const patternbyte &rhs)
  {
    return (lhs & rhs.mask) == (rhs.value & rhs.mask);
  }
};

inline bool wildcard_eq(PCWSTR String, PCWSTR Pattern)
{
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
  PCWSTR pPatternSequence; // Points to prospective wild string match after '*'
  PCWSTR pStringSequence;  // Points to prospective tame string match

  // Find a first wildcard, if one exists, and the beginning of any  
  // prospectively matching sequence after it.
  //
  for ( ;; ) {
    // Check for the end from the start.  Get out fast, if possible.
    if ( !*String ) {
      if ( *Pattern ) {
        while ( *Pattern++ == '*' ) {
          if ( !*Pattern )
            return true; // "ab" matches "ab*".
        }
        return false; // "abcd" doesn't match "abc".
      } else {
        return true; // "abc" matches "abc".
      }
    } else if ( *Pattern == '*' ) {
      // Got wild: set up for the second loop and skip on down there.
      while ( *(++Pattern) == '*' )
        continue;

      if ( !*Pattern )
        return true; // "abc*" matches "abcd".

      // Search for the next prospective match.
      if ( *Pattern != '?' ) {
        while ( __ascii_towupper(*Pattern) != __ascii_towupper(*String) ) {
          if ( !*(++String) )
            return false; // "a*bc" doesn't match "ab".
        }
      }

      // Keep fallback positions for retry in case of incomplete match.
      pPatternSequence = Pattern;
      pStringSequence = String;
      break;
    } else if ( __ascii_towupper(*Pattern) != __ascii_towupper(*String) && *Pattern != '?' ) {
      return false; // "abc" doesn't match "abd".
    }

    ++Pattern; // Everything's a match, so far.
    ++String;
  }

  // Find any further wildcards and any further matching sequences.
  for ( ;; ) {
    if ( *Pattern == '*' ) {
      // Got wild again.
      while ( *(++Pattern) == '*' )
        continue;

      if ( !*Pattern )
        return true; // "ab*c*" matches "abcd".

      if ( !*String )
        return false; // "*bcd*" doesn't match "abc".

      // Search for the next prospective match.
      if ( *Pattern != '?' ) {
        while ( __ascii_towupper(*Pattern) != __ascii_towupper(*String) ) {
          if ( !*(++String) )
            return false; // "a*b*c" doesn't match "ab".
        }
      }

      // Keep the new fallback positions.
      pPatternSequence = Pattern;
      pStringSequence = String;
    } else if ( __ascii_towupper(*Pattern) != __ascii_towupper(*String) && *Pattern != '?' ) {
      // The equivalent portion of the upper loop is really simple.
      if ( !*String )
        return false; // "*bcd" doesn't match "abc".

      // A fine time for questions.
      while ( *pPatternSequence == '?' ) {
        ++pPatternSequence;
        ++pStringSequence;
      }
      Pattern = pPatternSequence;

      // Fall back, but never so far again.
      while ( __ascii_towupper(*Pattern) != __ascii_towupper(*(++pStringSequence)) ) {
        if ( !*pStringSequence )
          return false; // "*a*b" doesn't match "ac".
      }
      String = pStringSequence;
    }

    // Another check for the end, at the end.
    if ( !*String )
      return !*Pattern;

    ++Pattern; // Everything's still a match.
    ++String;
  }
}
