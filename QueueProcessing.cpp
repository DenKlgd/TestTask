#include "QueueProcessing.h"

const std::string& QueueProcessorBase::getName()
{
	return m_queueName;
}

///  Delayed task processor functions

DelayedTaskQueueProcessor::DelayedTaskQueueProcessor(uint8_t index, std::mutex* mutex, SimpleTaskQueueProcessor* simpleTaskQueue, priority p)
{
	m_queueIndex = index;
	m_mutex = mutex;
	m_simpleTaskQueue = simpleTaskQueue;
	m_taskPriority = p;
}

void DelayedTaskQueueProcessor::createDelayedTask()
{
	m_delayedTaskQueue.emplace(m_queueIndex);
}

void DelayedTaskQueueProcessor::process()
{
	m_mutex->lock();
	if (!m_delayedTaskQueue.size())
	{
		m_mutex->unlock();
		return;
	}

	DelayedTask task = m_delayedTaskQueue.front();
	m_delayedTaskQueue.pop();

	printTime();
	std::cout << ": " << task.getName() << " - " << std::to_string(task.getDelay().count()) << " : " << "created.\n";

	m_mutex->unlock();


	task.sleep();


	m_mutex->lock();

	std::string simpleTaskName = "taskS" + std::to_string(task.getIndex());

	printTime();
	std::cout << ": " << task.getName() << " - " << std::to_string(task.getDelay().count()) << " : ( " << simpleTaskName << " : " << m_simpleTaskQueue->getName() << " ) pushed.\n";
	m_simpleTaskQueue->createSimpleTask(m_queueIndex, m_taskPriority, this);

	m_mutex->unlock();
}


///  Simple task processor functions

SimpleTaskQueueProcessor::SimpleTaskQueueProcessor(uint8_t index, std::mutex* mutex, std::string name)
{
	m_queueName = name;
	m_queueIndex = index;
	m_mutex = mutex;
}

void SimpleTaskQueueProcessor::createSimpleTask(uint8_t index, priority p, DelayedTaskQueueProcessor* source)
{
	m_simpleTaskQueue.emplace(index, p, (void*)source);
}

void SimpleTaskQueueProcessor::process()
{
	m_mutex->lock();
	if (!m_simpleTaskQueue.size())
	{
		m_mutex->unlock();
		return;
	}
	
	const SimpleTask task = m_simpleTaskQueue.top();
	m_simpleTaskQueue.pop();
	
	printTime();
	std::cout << ": " << m_queueName << " : " << task.getName() << " - " << std::to_string(task.getDelay().count()) << " : " << "running...\n";

	m_mutex->unlock();


	task.sleep();


	m_mutex->lock();

	printTime();
	std::cout << ": " << m_queueName << " : " << task.getName() << " - " << std::to_string(task.getDelay().count()) << " : " << "completed.\n";

	DelayedTaskQueueProcessor* source = (DelayedTaskQueueProcessor*)task.getSourceQueue();
	source->createDelayedTask();

	m_mutex->unlock();
}

