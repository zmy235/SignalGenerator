#pragma once

#include <string>
#include <vector>
class Task
{
public:
	Task();
	~Task(); 
public:
	std::string type;
	std::string size;
	std::string time;
	std::string state;
	std::string progress;
	std::string filePath;
	std::string info;
};

extern std::vector<Task*> taskList;
