#pragma once

#define STRICT
#define NOMINMAX
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#pragma comment( lib, "ntdll.lib" )

#include <delayimp.h>
#ifndef _delayimp_h
#define _delayimp_h
#endif
#pragma comment( lib, "delayimp.lib" )

EXTERN_C const IMAGE_DOS_HEADER __ImageBase;

#include <stdlib.h>
#include <stdio.h>
#include <strsafe.h>
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
