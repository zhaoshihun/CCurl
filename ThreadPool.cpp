#include "XCThreadPool.h"
namespace XingCloud{
namespace ThreadPool
{


#ifndef WIN32
pthread_mutex_t ThreadPool::mutexSync = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ThreadPool::mutexTaskCompletion = PTHREAD_MUTEX_INITIALIZER;
#endif
ThreadPool::ThreadPool() 
{
	ThreadPool(2);
}

ThreadPool::ThreadPool(int maxThreads)
{
    
	if (maxThreads < 1)
		maxThreads = 1;
#ifndef WIN32
	pthread_mutex_lock(&mutexSync);
#endif
	this->maxThreads = maxThreads;
	this->queueSize = maxThreads+2;

	incompleteTask = 0;
#ifndef WIN32
	availableTask=new IlmThread::Semaphore(0);
	availableThreads=new IlmThread::Semaphore(maxThreads);
#endif
    initializeThreads();
#ifndef WIN32
	pthread_mutex_unlock(&mutexSync);

#endif
}

void ThreadPool::initializeThreads()
{

	for (int i = 0; i < maxThreads; ++i)
	{
#ifndef WIN32
		pthread_t tempThread;
		pthread_create(&tempThread, NULL, &ThreadPool::threadExecute,(void *) this);
        threadIdVec[i] = tempThread;
#endif
	}

}

ThreadPool::~ThreadPool()
{
    destroyPool();
}

void ThreadPool::destroyPool(int maxPollSecs)
{
#ifndef WIN32
    for(int i=0;i<maxThreads;i++)
    {
        pthread_join(threadIdVec[i],NULL);
    }
    delete availableTask;
    delete availableThreads;
	
	pthread_mutex_destroy(&mutexSync);
	pthread_mutex_destroy(&mutexTaskCompletion);
#endif
}

bool ThreadPool::assignTask(Task task/*unsigned int (*task)()*/,void *argument)
{
#ifndef WIN32
	pthread_mutex_lock(&mutexTaskCompletion);
	incompleteTask++;

	pthread_mutex_unlock(&mutexTaskCompletion);
	availableThreads->wait();
	pthread_mutex_lock(&mutexSync);
    
    Work work;
    work.task=task;
    work.argument=argument;
	taskQueue.push_back(work);
  
	
	availableTask->post();
	pthread_mutex_unlock(&mutexSync);

#endif

	return true;
}

bool ThreadPool::fetchTask(Work *w/*unsigned int (**task)()*/)
{
#ifndef WIN32
	
	availableTask->wait();
    pthread_mutex_lock(&mutexSync);
     if(!taskQueue.empty())
    {
        Work work=taskQueue.front();
        w->task=work.task;
        w->argument=work.argument;
        taskQueue.erase(taskQueue.begin());


    }
    else
    {
        
        pthread_mutex_unlock(&mutexSync);
        return false;
    }
	    
    availableThreads->post();
	pthread_mutex_unlock(&mutexSync);

#endif
	return true;
}
#ifndef WIN32
void *ThreadPool::threadExecute(void *param)
{
  
    ThreadPool *tp=(ThreadPool*)param;
   
	while (1)
	{
        Work tasker;
        if(!(tp->fetchTask(&tasker)))
        {
            continue;
        }
        (tasker.task)(tasker.argument);

    	pthread_mutex_lock(&(((ThreadPool *) param)->mutexTaskCompletion));
		((ThreadPool *) param)->incompleteTask--;
		pthread_mutex_unlock(&(((ThreadPool *) param)->mutexTaskCompletion));
        
	}
	return (void*)0;
}
#endif
}/*XingCloud*/
}/*ThreadPool*/
