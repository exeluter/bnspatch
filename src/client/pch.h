#pragma once

#define STRICT
#define NOMINMAX
#define PHNT_VERSION PHNT_THRESHOLD
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#pragma comment( lib, "ntdll.lib" )

#include <delayimp.h>
#ifndef _delayimp_h
#define _delayimp_h
#endif
#pragma comment( lib, "delayimp.lib" )

EXTERN_C const IMAGE_DOS_HEADER __ImageBase;

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include <wil/token_helpers.h>

#include <string>
using namespace std::string_literals;
#include <string_view>
using namespace std::string_view_literals;

#include <optional>
#include <memory>
#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;

#include <wil/resource.h>

#include <gsl/gsl>

#define DETOURS_INTERNAL
#include <detours/detours.h>

#define FMT_HEADER_ONLY
#define FMT_STRING_ALIAS 1
#include <fmt/format.h>

#define SPDLOG_FMT_EXTERNAL
#ifdef _DEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#else
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#endif
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/fmt/bin_to_hex.h>

#define MAGIC_ENUM_RANGE_MIN 0
#define MAGIC_ENUM_RANGE_MAX 256
#include <magic_enum/magic_enum.hpp>
