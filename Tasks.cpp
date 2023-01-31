#include "Tasks.h"
#include "QueueProcessing.h"

/// Base class

void TaskBase::sleep() const
{
	std::this_thread::sleep_for(m_taskDelay);
}

const string& TaskBase::getName() const
{
	return m_taskName;
}

seconds TaskBase::getDelay() const
{
	return m_taskDelay;
}

uint8_t TaskBase::getIndex() const
{
	return m_taskIndex;
}


/// Delayed task

DelayedTask::DelayedTask(uint8_t index)
{
	m_taskName = "taskD" + std::to_string(index);
	m_taskIndex = index;
	m_taskDelay = seconds(10);
}


/// Simple task

SimpleTask::SimpleTask(uint8_t index, priority p, void* source)
{
	m_taskName = "taskS" + std::to_string(index);
	m_taskIndex = index;
	m_taskDelay = seconds(2);
	m_priority = p;
	m_source = source;
}

priority SimpleTask::getPriority() const
{
	return m_priority;
}

void* SimpleTask::getSourceQueue() const
{
	return m_source;
}

SimpleTask& SimpleTask::operator= (const SimpleTask& simpleTask)
{
	this->m_taskName = simpleTask.m_taskName;
	this->m_taskDelay = simpleTask.m_taskDelay;
	this->m_source = simpleTask.m_source;
	this->m_priority = simpleTask.m_priority;
	return *this;
}

bool operator< (const SimpleTask& s1, const SimpleTask& s2)
{
	return s1.getPriority() > s2.getPriority();
}
