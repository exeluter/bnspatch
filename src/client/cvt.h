#pragma once
#include "pch.h"

namespace cvt
{
  std::wstring_view to_wstring_view(const PUNICODE_STRING String)
  {
    if ( !String )
      return L"(null)"sv;

    return { String->Buffer, String->Length / sizeof(WCHAR) };
  }

  std::string_view to_string_view(const PANSI_STRING String)
  {
    if ( !String )
      return "(null)"sv;

    return { String->Buffer, String->Length / sizeof(CHAR) };
  }

  std::wstring to_wstring(const POBJECT_ATTRIBUTES ObjectAttributes)
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
