/*
 * Copyright (C) 2018 German Mendez Bravo (Kronuz)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <cstdint>
#include <locale>

template <typename T, T Prime, T OffsetBasis>
struct basic_fnv1a
{
  struct details
  {
    template <class Char>
    static constexpr Char tolower(Char c)
    {
      if ( c >= 'A' && c <= 'Z' )
        return c - ('A' - 'a');
      return c;
    }

    template <class Char>
    static constexpr Char toupper(Char c)
    {
      if ( c >= 'a' && c <= 'z' )
        return c - ('a' - 'A');
      return c;
    }
  };

  template <class Char>
  static constexpr T make_hash(const Char *s)
  {
    T hash = OffsetBasis;
    while ( *s ) {
      hash ^= static_cast<T>(*s++);
      hash *= Prime;
    }
    return hash;
  }

  template <class Char>
  static constexpr T make_hash_lower(const Char *s)
  {
    T hash = OffsetBasis;
    while ( *s ) {
      hash ^= static_cast<T>(details::tolower(*s++));
      hash *= Prime;
    }
    return hash;
  }

  template <class Char>
  static constexpr T make_hash_upper(const Char *s)
  {
    T hash = OffsetBasis;
    while ( *s ) {
      hash ^= static_cast<T>(details::toupper(*s++));
      hash *= Prime;
    }
    return hash;
  }
};
using fnv1a32 = basic_fnv1a<std::uint32_t, 0x1000193UL, 2166136261UL>;
using fnv1a64 = basic_fnv1a<std::uint64_t, 0x100000001b3ULL, 14695981039346656037ULL>;

constexpr auto operator"" _fnv1a32(const char *s, std::size_t len)
{
  return fnv1a32::make_hash(s);
}
constexpr auto operator"" _fnv1a32(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s);
}
constexpr auto operator"" _fnv1a32l(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash_lower(s);
}
constexpr auto operator"" _fnv1a32u(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash_upper(s);
}
constexpr auto operator"" _fnv1a64(const char *s, std::size_t len)
{
  return fnv1a64::make_hash(s);
}
constexpr auto operator"" _fnv1a64(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s);
}
constexpr auto operator"" _fnv1a64l(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash_lower(s);
}
constexpr auto operator"" _fnv1a64u(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash_upper(s);
}

#ifdef _M_X64
using fnv1a = fnv1a64;
#else
using fnv1a = fnv1a32;
#endif

constexpr auto operator"" _fnv1a(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s);
}
constexpr auto operator"" _fnv1al(const char *s, std::size_t len)
{
  return fnv1a::make_hash_lower(s);
}
constexpr auto operator"" _fnv1au(const char *s, std::size_t len)
{
  return fnv1a::make_hash_upper(s);
}

constexpr auto operator"" _fnv1a(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s);
}
constexpr auto operator"" _fnv1al(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash_lower(s);
}
constexpr auto operator"" _fnv1au(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash_upper(s);
}
