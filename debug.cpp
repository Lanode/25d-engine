#include <iostream>
#include "debug.h"

std::map<std::string, std::string> Debug::metrics;

void Debug::SetMetric(std::string name, std::string value)
{
	Debug::metrics[name] = value;
}

std::string Debug::GetMetricsAsText()
{
	std::string result;
	for (auto it = Debug::metrics.cbegin(); it != Debug::metrics.cend(); it++) {
		result += it->first + ": " + it->second + '\n';
	}
	return result;
}

void Debug::DeleteMetric(std::string name)
{
	Debug::metrics.erase(name);
}

void Debug::ClearMetrics()
{
	Debug::metrics.clear();
}

void Debug::WriteLn(std::string line)
{
	std::cout << line << std::endl;
}
