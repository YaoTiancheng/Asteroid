#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             
#define NOMINMAX
#include <windows.h>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <d3d11.h>

#include <cassert>
#include <cstdint>
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
#include <list>

#include <DirectXMath.h>
#include <wrl/client.h>
#include <DbgHelp.h>

#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/vector.hpp"

#include "Asteroid.h"