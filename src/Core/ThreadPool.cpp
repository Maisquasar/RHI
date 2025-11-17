#include "ThreadPool.h"

std::unique_ptr<ThreadPool> ThreadPool::s_instance = nullptr;
void ThreadPool::Initialize()
{
    s_instance = std::make_unique<ThreadPool>();
    s_instance->m_threadPool = std::make_unique<BS::thread_pool<>>(std::thread::hardware_concurrency());
}

void ThreadPool::Terminate()
{
    s_instance->m_threadPool->reset();
    s_instance->m_threadPool.reset();
}
