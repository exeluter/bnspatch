#pragma once

#include <cstdint>

template <typename T, T Prime, T OffsetBasis>
struct basic_fnv1a
{
  struct details
  {
    template <class T>
    static constexpr T _fnv1a_ascii_tolower(T c)
    {
      if ( c >= 'A' && c <= 'Z' )
        return c - ('A' - 'a');
      return c;
    }

    template <class T>
    static constexpr T _fnv1a_ascii_toupper(T c)
    {
      if ( c >= 'a' && c <= 'z' )
        return c - ('a' - 'A');
      return c;
    }
  };

  static constexpr T make_hash(
    const char *s,
    std::char_traits<char>::int_type(*fx)(std::char_traits<char>::int_type) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      hash ^= static_cast<T>(fx ? fx(*s) : *s);
      hash *= Prime;
    }
    return hash;
  }

  static constexpr T make_hash(
    const char *s,
    std::size_t length,
    std::char_traits<char>::int_type(*fx)(std::char_traits<char>::int_type) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < length; ++i ) {
      hash ^= static_cast<T>(fx ? fx(s[i]) : s[i]);
      hash *= Prime;
    }
    return hash;
  }

  template <std::size_t length>
  static constexpr T make_hash(
    char(&s)[length],
    std::char_traits<char>::int_type(*fx)(std::char_traits<char>::int_type) = nullptr)
  {
    return make_hash(s, length, fx);
  }

  static constexpr T make_hash(
    const wchar_t *s,
    std::char_traits<wchar_t>::int_type(*fx)(std::char_traits<wchar_t>::int_type) = nullptr)
  {
    T hash = OffsetBasis;
    for ( ; *s; ++s ) {
      wchar_t c = fx ? fx(*s) : *s;
      hash ^= static_cast<T>(HIBYTE(c));
      hash *= Prime;
      hash ^= static_cast<T>(LOBYTE(c));
      hash *= Prime;
    }
    return hash;
  }

  static constexpr T make_hash(
    const wchar_t *s,
    std::size_t length,
    std::char_traits<wchar_t>::int_type(*fx)(std::char_traits<wchar_t>::int_type) = nullptr)
  {
    T hash = OffsetBasis;
    for ( std::size_t i = 0; i < length; ++i ) {
      wchar_t c = fx ? fx(s[i]) : s[i];
      hash ^= static_cast<T>(HIBYTE(c));
      hash *= Prime;
      hash ^= static_cast<T>(LOBYTE(c));
      hash *= Prime;
    }
    return hash;
  }

  template <std::size_t length>
  static constexpr T make_hash(
    wchar_t(&s)[length],
    std::char_traits<wchar_t>::int_type(*fx)(std::char_traits<wchar_t>::int_type) = nullptr)
  {
    return make_hash(s, length);
  }

  template <typename Elem, typename Traits, typename Alloc>
  static constexpr T make_hash(
    const std::basic_string<Elem, Traits, Alloc> &s,
    typename std::char_traits<Elem>::int_type(*fx)(typename std::char_traits<Elem>::int_type) = nullptr)
  {
    return make_hash(s.c_str(), s.size(), fx);
  }
};

using fnv1a32 = basic_fnv1a<std::uint32_t, 0x1000193UL, 2166136261UL>;
using fnv1a64 = basic_fnv1a<std::uint64_t, 0x100000001b3ULL, 14695981039346656037ULL>;

constexpr auto operator"" _fnv1a32(const char *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
constexpr auto operator"" _fnv1a32(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len);
}
constexpr auto operator"" _fnv1a32u(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::details::_fnv1a_ascii_toupper);
}
constexpr auto operator"" _fnv1a32l(const wchar_t *s, std::size_t len)
{
  return fnv1a32::make_hash(s, len, fnv1a32::details::_fnv1a_ascii_tolower);
}
constexpr auto operator"" _fnv1a64(const char *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len);
}
constexpr auto operator"" _fnv1a64(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len);
}
constexpr auto operator"" _fnv1a64u(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::details::_fnv1a_ascii_toupper);
}

constexpr auto operator"" _fnv1a64l(const wchar_t *s, std::size_t len)
{
  return fnv1a64::make_hash(s, len, fnv1a64::details::_fnv1a_ascii_tolower);
}

#ifdef _M_X64
using fnv1a = fnv1a64;
#else
using fnv1a = fnv1a32;
#endif

constexpr auto operator"" _fnv1a(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
constexpr auto operator"" _fnv1au(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::details::_fnv1a_ascii_toupper);
}
constexpr auto operator"" _fnv1al(const char *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::details::_fnv1a_ascii_tolower);
}

constexpr auto operator"" _fnv1a(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len);
}
constexpr auto operator"" _fnv1au(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::details::_fnv1a_ascii_toupper);
}
constexpr auto operator"" _fnv1al(const wchar_t *s, std::size_t len)
{
  return fnv1a::make_hash(s, len, fnv1a::details::_fnv1a_ascii_tolower);
}
