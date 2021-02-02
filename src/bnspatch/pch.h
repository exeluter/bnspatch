// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define RPC_USE_NATIVE_WCHAR
#include <phnt_windows.h>
#include <phnt.h>
#pragma comment( lib, "ntdll.lib" )
#pragma comment( lib, "version.lib" )
#include <Winsock2.h>
#include <ShlObj.h>
#include <KnownFolders.h>
#include <Shlwapi.h>
#pragma comment( lib, "Shlwapi.lib" )

#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <codecvt>

#include <strsafe.h>
#include <safeint.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <span>
#include <string_view>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

using namespace std::chrono_literals;
using namespace std::string_view_literals;
using namespace std::string_literals;

#if !defined(RESULT_DIAGNOSTICS_LEVEL)
#define RESULT_DIAGNOSTICS_LEVEL 0
#endif // !RESULT_DIAGNOSTICS_LEVEL

#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/compile.h>

#define PUGIXML_HEADER_ONLY
#define PUGIXML_WCHAR_MODE
#include <pugixml.hpp>

#include <fnv1a.h>
#include <ntamd64.hpp>
#include <ntmm.hpp>
#include <ntrtl.hpp>
#include "detours.hpp"

#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include <wil/resource.h>
#include <wil/result.h>

#ifdef _X86_
#define this__(x) x, std::intptr_t
#else
#define this__(x) x
#endif

#endif //PCH_H
