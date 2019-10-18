// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#define STRICT
#define NOMINMAX
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#pragma comment( lib, "ntdll.lib" )

EXTERN_C const IMAGE_DOS_HEADER __ImageBase;

#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
#include <intsafe.h>
#include <stdint.h>

#include <string>
#include <filesystem>
namespace fs = std::filesystem;

#include <codecvt>
#include <gsl/gsl>
#include <wil/token_helpers.h>
#include <wil/resource.h>

#define DETOURS_INTERNAL
#include <detours/detours.h>

#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#define SPDLOG_WCHAR_TO_UTF8_SUPPORT
#include <spdlog/spdlog.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/fmt/bin_to_hex.h>


#endif //PCH_H
