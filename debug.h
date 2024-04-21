#pragma once
#include <string>

class Debug
{
public:
	static std::string debugText;
	static void AddDebugLine(std::string line);
	static std::string GetDebugText();
	static void ClearDebugText();
};