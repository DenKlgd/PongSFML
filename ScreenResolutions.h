#pragma once
#include <stdint.h>
#include <utility>
#include <string>

typedef struct Resolution
{
	uint32_t x, y;
};

extern std::pair<std::string, Resolution> resolutionList[5];
