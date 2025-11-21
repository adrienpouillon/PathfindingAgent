#pragma once
#include <string>

constexpr int MinConfigs = 1;
constexpr int MaxConfigs = 3;

class GridConfigs
{
	static int mConfigNumber;

	GridConfigs* Get() { return this; }
public:
	static int GetConfigNumber() { return mConfigNumber; }
	static void ChangeConfig(int val);
	static std::string GetConfigString();
};

