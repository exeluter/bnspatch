#pragma once
#include <wchar.h>
#include <stdint.h>
#include <locale>
#include <type_traits>
#include <string>

namespace pe
{
  namespace traits
  {
    template <class Char, class Integer = std::make_unsigned_t<Char>>
    class ic_char_traits
    {
    public:
      using char_type = Char;
      using int_type = Integer;
      using off_type = ptrdiff_t;
      using pos_type = size_t;
      using state_type = void;

    private:
      static bool eq(char_type a, char_type b, const std::ctype<char_type> &facet) noexcept
      {
        return a == b || facet.tolower(a) == facet.tolower(b);
      }

    public:
      static void assign(char_type &r, const char_type &a)
      {
        r = a;
      }

      static char_type *assign(char_type *p, size_t count, char_type a)
      {
        return reinterpret_cast<char_type *>(
          memset(p, a, count * sizeof(char_type)));
      }

      static bool eq(char_type a, char_type b) noexcept
      {
        if ( a != b ) {
          const auto &facet = std::use_facet<std::ctype<char_type>>(std::locale());

          return eq(a, b, facet);
        }
        return true;
      }

      static bool lt(char_type a, char_type b) noexcept
      {
        const auto &facet = std::use_facet<std::ctype<char_type>>(std::locale());

        return facet.tolower(a) < facet.tolower(b);
      }

      static char_type *move(char_type *dest, const char_type *src, size_t count)
      {
        return reinterpret_cast<char_type *>(
          memmove(dest, src, count * sizeof(char_type)));
      }

      static char_type *copy(char_type *dest, const char_type *src, size_t count)
      {
        return reinterpret_cast<char_type *>(
          memcpy(dest, src, count * sizeof(char_type)));
      }

      static int compare(const char_type *s1, const char_type *s2, size_t count)
      {
        const auto &facet = std::use_facet<std::ctype<char_type>>(std::locale());

        for ( size_t i = 0; i < count; ++i ) {
          char_type c1 = s1[i];
          char_type c2 = s2[i];

          if ( c1 != c2 ) {
            c1 = facet.tolower(c1);
            c2 = facet.tolower(c2);

            if ( c1 != c2 )
              return (int)c1 - (int)c2;
          }
        }
        return 0;
      }

      static size_t length(const char_type *s)
      {
        size_t count = 0;
        while ( *s ) {
          ++count;
          ++s;
        }
        return count;
      }

      static const char_type *find(const char_type *p, size_t count, const char_type &ch)
      {
        const auto &facet = std::use_facet<std::ctype<char_type>>(std::locale());
        const char_type c = facet.tolower(ch);

        for ( ; 0 < count; --count, ++p ) {
          if ( *p == ch || facet.tolower(*p) == c )
            return p;
        }
        return nullptr;
      }

      static char_type to_char_type(int_type c) noexcept
      {
        return c;
      }

      static bool eq_int_type(int_type c1, int_type c2) noexcept
      {
        return eq(c1, c2);
      }

      static int_type eof() noexcept
      {
        return EOF;
      }

      static int_type not_eof(int_type e) noexcept
      {
        return e != eof() ? e : !eof();
      }
    };
  }

  using ic_wstring = std::basic_string<wchar_t, pe::traits::ic_char_traits<wchar_t>>;
  using ic_string = std::basic_string<char, pe::traits::ic_char_traits<char, int>>;
}