#pragma once

#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOGDI
#define NODRAWTEXT
#define NOMINMAX
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#pragma comment( lib, "ntdll.lib" )

#include <delayimp.h>
#ifndef _delayimp_h
#define _delayimp_h
#endif
#pragma comment( lib, "delayimp.lib" )

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <string>
using namespace std::string_literals;
#include <string_view>
using namespace std::string_view_literals;

#include <optional>
#include <memory>
#include <mutex>
#include <filesystem>
namespace fs = std::filesystem;

#include <gsl/gsl>

#define DETOURS_INTERNAL
#include <detours/detours.h>

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>

#define JM_XORSTR_DISABLE_AVX_INTRINSICS
#include <xorstr/xorstr.hpp>

#include <pugixml/pugixml.hpp>

//#include <libdeflate/libdeflate.h>
//#pragma comment( lib, "lib/libdeflatestatic.lib")

static inline ptrdiff_t make_offset(const void *Base, const void *Pointer)
{
  return ((ptrdiff_t)(((uintptr_t)(Pointer)) - ((uintptr_t)(Base))));
}
