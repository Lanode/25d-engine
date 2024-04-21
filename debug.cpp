#include "debug.h"

std::string Debug::debugText;

void Debug::AddDebugLine(std::string line)
{
	Debug::debugText.append(line + '\n');
}

std::string Debug::GetDebugText()
{
	return Debug::debugText;
}

void Debug::ClearDebugText()
{
	Debug::debugText = "";
}
