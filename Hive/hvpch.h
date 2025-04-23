#pragma once

#include <functional>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <unordered_map>
#include <queue>


using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;

using float32 = float;
using float64 = double;

#ifdef _WIN32
#define API_EXPORT __declspec(dllexport)
#define API_IMPORT __declspec(dllimport)
#else
#define API_EXPORT __attribute__((visibility("default")))
#define API_IMPORT
#endif

#ifdef HIVE_EXPORTS
#define HIVE_API API_EXPORT
#elif defined HIVE_IMPORTS
#define HIVE_API API_IMPORT
#endif

#include "Core/Logging/LogManager.h"
#include "Core/Memory/MemoryAllocator.h"
#include "Core/Debug.h"
