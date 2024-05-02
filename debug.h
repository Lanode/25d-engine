#pragma once
#include <string>
#include <map>

class Debug
{
private:
	static std::map<std::string, std::string> metrics;
public:
	static void SetMetric(std::string name, std::string value);
	static std::string GetMetricsAsText();
	static void DeleteMetric(std::string name);
	static void ClearMetrics();
};