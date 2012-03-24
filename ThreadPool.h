#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include <pthread>
#include <semaphore.h>
#include <list>
using namespace std;

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

		semaphore *availableTask;
		semaphore *availableThreads;
		pthread_t threadIdVec[10];

#endif
		list<Work> taskQueue;

		int incompleteTask;
		int queueSize;

};
}/*ThreadPool*/



#endif
