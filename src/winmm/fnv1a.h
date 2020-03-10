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
#include <string_view>

template <typename T, T Prime, T OffsetBasis>
struct basic_fnv1a
{
  using key_type = T;
  template <class Char>
  static constexpr T hash(const Char *p, std::size_t len)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < len; ++i ) {
      hash = (hash ^ static_cast<T>(p[i])) * Prime;
    }
    return hash;
  }
  template <class Char, size_t Size>
  static constexpr T hash(const Char(&s)[Size])
  {
    return hash(s, Size - 1);
  }
  template <class Char>
  static constexpr T hash(const std::basic_string_view<Char> &str)
  {
    return hash(str.data(), str.size());
  }

  template <class Char>
  static T hash_lower(const Char *p, std::size_t len)
  {
    const auto &facet = std::use_facet<std::ctype<Char>>(std::locale());
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < len; ++i ) {
      hash = (hash ^ static_cast<T>(facet.tolower(p[i]))) * Prime;
    }
    return hash;
  }
  template <class Char, size_t Size>
  static constexpr T hash_lower(const Char(&s)[Size])
  {
    return hash_lower(s, Size - 1);
  }
  template <class Char>
  static constexpr T hash_lower(const std::basic_string_view<Char> &str)
  {
    return hash_lower(str.data(), str.size());
  }

  template <class Char>
  static T hash_upper(const Char *p, std::size_t len)
  {
    const auto &facet = std::use_facet<std::ctype<Char>>(std::locale());
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < len; ++i ) {
      hash = (hash ^ static_cast<T>(facet.tolower(p[i]))) *Prime;
    }
    return hash;
  }
  template <class Char, size_t Size>
  static constexpr T hash_upper(const Char(&s)[Size])
  {
    return hash_upper(s, Size - 1);
  }
  template <class Char>
  static constexpr T hash_upper(const std::basic_string_view<Char> &str)
  {
    return hash_upper(str.data(), str.size());
  }
};
using fnv1a32 = basic_fnv1a<std::uint32_t, 0x1000193UL, 2166136261UL>;
using fnv1a64 = basic_fnv1a<std::uint64_t, 0x100000001b3ULL, 14695981039346656037ULL>;

constexpr auto operator"" _fnv1a32(const char *s, std::size_t len)
{
  return fnv1a32::hash(s, len);
}
constexpr auto operator"" _fnv1a32(const wchar_t *s, std::size_t len)
{
  return fnv1a32::hash(s, len);
}
constexpr auto operator"" _fnv1a64(const char *s, std::size_t len)
{
  return fnv1a64::hash(s, len);
}
constexpr auto operator"" _fnv1a64(const wchar_t *s, size_t len)
{
  return fnv1a64::hash(s, len);
}

#ifdef _M_X64
using fnv1a = fnv1a64;
#else
using fnv1a = fnv1a32;
#endif
constexpr auto operator"" _fnv1a(const char *s, std::size_t len)
{
  return fnv1a::hash(s, len);
}
constexpr auto operator"" _fnv1a(const wchar_t *s, std::size_t len)
{
  return fnv1a::hash(s, len);
}
