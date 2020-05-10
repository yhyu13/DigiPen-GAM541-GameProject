/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 02/10/2020
- End Header ----------------------------*/

#pragma once

#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <memory>

#include <cstdint>
#include <string>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>

#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

#include <math.h>
//#define GLM_FORCE_SSE2
#define GLM_FORCE_AVX2 
#define GLM_RIGHT_HANDED
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp >

#include "engine/logging/LoggingCore.h"
#include "engine/EngineCore.h"
#include "engine/exception/EngineException.h"
