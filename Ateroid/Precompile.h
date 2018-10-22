// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define NOMINMAX
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <cstdint>
#include <cassert>
#include <d3d11.h>

#include <numeric>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iostream>
#include <functional>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>

#include <DirectXMath.h>
#include <wrl/client.h>
#include <DbgHelp.h>

#include "cereal/archives/json.hpp"
#include "cereal/types/unordered_map.hpp"