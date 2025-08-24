#pragma once

#include <unistd.h>
#include <sys/syscall.h>


// 定义CurrentThread命名空间
// 封装相关功能，防止命名冲突
namespace CurrentThread
{   
    // __thread是GCC的扩展关键字，用于定义线程局部存储（TLS）变量，每个线程都有自己独立的t_cachedTid副本
    // extern表示该变量在其他地方定义，这里只是声明
    // t_cachedTid用于缓存线程ID，避免频繁的系统调用
    extern __thread int t_cachedTid;

    void cacheTid();

    inline int tid() // 内联函数只在当前文件中起作用
    {
        // __builtin_expect是GCC的内建函数，用于分支预测优化
        if (__builtin_expect(t_cachedTid == 0, 0)) {
            cacheTid();
        }
        return t_cachedTid;
    }
}