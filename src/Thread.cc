#include "Thread.h"
#include "CurrentThread.h"

#include <semaphore.h>


std::atomic_int Thread::numCreated_(0);

Thread::Thread(ThreadFunc func, const std::string &name)
    : started_(false)
    , joined_(false)
    , tid_(0)
    , func_(std::move(func))
    , name_(name)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        thread_->detach();  // thread类提供了设置分离线程的方法 线程运行后自动销毁（非阻塞）
    }
}

void Thread::start()    // 一个Thread对象 记录的就是一个新线程的详细信息
{
    started_ = true;
    sem_t sem;
    sem_init(&sem, false, 0);  // false指的是 不设置进程间共享
    // 创建并开启线程，其中lambda表达式函数将在子线程中执行
    thread_ = std::shared_ptr<std::thread>(new std::thread([&]() {
        tid_ = CurrentThread::tid();  // 获取线程的tid值
        sem_post(&sem);  // 将信号量的值加1，通知主线程已获取到tid_
        func_();  // 执行线程回调
    }));

    // 必须阻塞等待新创建的线程获取到其tid_
    // 即子线程中sem_post(&sem);执行后，主线程继续执行
    sem_wait(&sem);
}

// C++ std::thread 中join()和detach()的区别：
// 1. join()：阻塞当前线程，直到被调用的线程执行完毕
// 2. detach()：将当前线程与调用线程分离，允许调用线程独立执行，主线程不再等待它的结束
void Thread::join()
{
    joined_ = true;
    thread_->join();
}

void Thread::setDefaultName()
{
    int num = ++numCreated_;
    if (name_.empty())
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", num);
        name_ = buf;
    }
}