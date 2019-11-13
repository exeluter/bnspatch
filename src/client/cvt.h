#pragma once
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#include <string>
#include <string_view>

namespace cvt
{
  inline std::wstring_view to_wstring_view(PCUNICODE_STRING String)
  {
    if ( !String )
      return L"(null)"sv;

    return { String->Buffer, String->Length / sizeof(WCHAR) };
  }

  inline std::wstring_view to_wstring_view(const UNICODE_STRING &String)
  {
    return { String.Buffer, String.Length / sizeof(WCHAR) };
  }

  inline std::string_view to_string_view(PCANSI_STRING String)
  {
    if ( !String )
      return "(null)"sv;

    return { String->Buffer, String->Length / sizeof(CHAR) };
  }

  inline std::string_view to_string_view(const ANSI_STRING &String)
  {
    return { String.Buffer, String.Length / sizeof(CHAR) };
  }

  inline std::wstring to_wstring(PCOBJECT_ATTRIBUTES ObjectAttributes)
  {
    std::wstring s;

    if ( !ObjectAttributes )
      return L"(null)"s;

    fmt::format_to(std::back_inserter(s), fmt(L"( {}, {}, {:#x}, {}, {}, {} )"),
      ObjectAttributes->Length,
      ObjectAttributes->RootDirectory,
      ObjectAttributes->Attributes,
      cvt::to_wstring_view(ObjectAttributes->ObjectName),
      ObjectAttributes->SecurityDescriptor,
      ObjectAttributes->SecurityQualityOfService);

    return s;
  }
}
