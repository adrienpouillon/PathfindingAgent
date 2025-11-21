#include "pch.h"
#include "GridConfigs.h"

int GridConfigs::mConfigNumber = MinConfigs;

void GridConfigs::ChangeConfig(int val)
{
	int newConfigNumber = mConfigNumber + val;

	if (newConfigNumber > MaxConfigs)
	{
		newConfigNumber = MaxConfigs;
	}
	else if (newConfigNumber < MinConfigs)
	{
		newConfigNumber = MinConfigs;
	}

	mConfigNumber = newConfigNumber;
}

std::string GridConfigs::GetConfigString()
{
	std::string finalTxt = "";

	switch (mConfigNumber)
	{
	case 1 :
		finalTxt = "1";
		break;
	case 2 :
		finalTxt = "2";
		break;
	case 3 :
		finalTxt = "3";
		break;
	}

	finalTxt += ".txt";

	return finalTxt;
}
