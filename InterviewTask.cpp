#include <iostream>
#include <queue>
#include <thread>
#include "QueueProcessing.h"

std::mutex queueMutex;

int main()
{
	SimpleTaskQueueProcessor simpleTaskQueue(1, &queueMutex, "queueS1");
	DelayedTaskQueueProcessor delayedTaskQueue1(1, &queueMutex, &simpleTaskQueue, 1);
	DelayedTaskQueueProcessor delayedTaskQueue2(2, &queueMutex, &simpleTaskQueue, 2);
	DelayedTaskQueueProcessor delayedTaskQueue3(3, &queueMutex, &simpleTaskQueue, 1);

	delayedTaskQueue1.createDelayedTask();
	delayedTaskQueue2.createDelayedTask();
	delayedTaskQueue3.createDelayedTask();
	

	std::thread delayedTaskQueue1Thread([&delayedTaskQueue1]() {
		while (true) 
		{ 
			delayedTaskQueue1.process(); 
		}
	});

	std::thread delayedTaskQueue2Thread([&delayedTaskQueue2]() { 
		while (true)
		{ 
			delayedTaskQueue2.process();
		} 
	});

	std::thread delayedTaskQueue3Thread([&delayedTaskQueue3]() {
		while (true)
		{
			delayedTaskQueue3.process();
		}
	});
	
	while (true)
	{
		simpleTaskQueue.process();
	}

	return 0;
}
