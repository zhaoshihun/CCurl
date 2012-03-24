#ifndef XCTHREADPOOL_H_
#define XCTHREADPOOL_H_
#include "../toplevel/STDHeaders.h"
#include "../display/IlmThread/ilmThreadSemaphore.h"
namespace XingCloud
{
namespace ThreadPool
{

typedef unsigned int (*Task)(void*);
struct Work
{
		Task task;
		void *argument;
};
class ThreadPool
{
	public:
		ThreadPool();
		ThreadPool(int maxThreadsTemp);
		virtual ~ThreadPool();

		void destroyPool(int maxPollSecs = 2);

		bool assignTask(Task task, void *argument/*unsigned int (*task)()*/);

		bool fetchTask(Work *w/*taskunsigned int (**task)()*/);

		void initializeThreads();
#ifndef  WIN32

		static void *threadExecute(void *param);

		static pthread_mutex_t mutexSync;
		static pthread_mutex_t mutexTaskCompletion;
#endif

	private:
		int maxThreads;

#ifndef WIN32

		IlmThread::Semaphore *availableTask;
		IlmThread::Semaphore *availableThreads;
		pthread_t threadIdVec[10];

#endif
		list<Work> taskQueue;

		int incompleteTask;
		int queueSize;

};
}/*XingCloud*/
}/*ThreadPool*/



#endif
