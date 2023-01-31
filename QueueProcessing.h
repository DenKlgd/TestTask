#pragma once

#include <queue>
#include <thread>
#include <mutex>
#include "PrintTime.h"
#include "Tasks.h"

using std::queue;
using std::priority_queue;
using std::lock_guard;

class QueueProcessorBase;
class DelayedTaskQueueProcessor;
class SimpleTaskQueueProcessor;


class QueueProcessorBase
{
protected:
	std::string m_queueName = "";
	uint8_t m_queueIndex = 0;
	std::mutex* m_mutex = nullptr;
	QueueProcessorBase() = default;

public:
	const std::string& getName();
};

class DelayedTaskQueueProcessor : public QueueProcessorBase
{
private:
	queue<DelayedTask> m_delayedTaskQueue;
	SimpleTaskQueueProcessor* m_simpleTaskQueue;
	priority m_taskPriority = 0;

public:
	DelayedTaskQueueProcessor(uint8_t index, std::mutex* mutex, SimpleTaskQueueProcessor* simpleTaskQueue, priority p);
	void createDelayedTask();
	void process();

};

class SimpleTaskQueueProcessor : public QueueProcessorBase
{
private:
	priority_queue<SimpleTask, std::vector<SimpleTask>, std::less<SimpleTask>> m_simpleTaskQueue;

public:
	SimpleTaskQueueProcessor(uint8_t index, std::mutex* mutex, std::string name);
	void createSimpleTask(uint8_t index, priority p, DelayedTaskQueueProcessor* source);
	void process();
};
