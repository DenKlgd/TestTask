#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <queue>
#include "PrintTime.h"

using std::string;
using std::chrono::seconds;

typedef unsigned char priority;
typedef unsigned int objectCounter;

class TaskBase;
class DelayedTask;
class SimpleTask;


class TaskBase
{
protected:
	string m_taskName = "" ;
	seconds m_taskDelay { 0 };
	uint8_t m_taskIndex = 0;

protected:
	TaskBase() = default;

public:
	virtual ~TaskBase() = default;
	void sleep() const;
	const string& getName() const;
	seconds getDelay() const;
	uint8_t getIndex() const;
};

class DelayedTask : public TaskBase
{
public:
	DelayedTask(uint8_t index);
};

class SimpleTask : public TaskBase
{
private:
	priority m_priority = 0;
	void* m_source = nullptr;

public:
	SimpleTask() = delete;
	SimpleTask(uint8_t index, priority p, void* source);
	priority getPriority() const;
	void* getSourceQueue() const;
	SimpleTask& operator= (const SimpleTask& simpleTask);
};

bool operator< (const SimpleTask& s1, const SimpleTask& s2);
