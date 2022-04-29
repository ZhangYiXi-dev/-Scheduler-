# Coroutinesr-Scheduler-
日志、线程、协程、协程调度

简述：封装了一个协程调度器。主要实现的功能是创建n个线程，然后将创建出来的任务（fiber或cb）加入到任务队列，空闲的线程会自动取出任务执行(如果协程执行中被换出，则未执行完成部分继续加入到任务队列)。同时，也可以指定某个任务一定要被某个线程执行。

**1.调用方法**

```cpp
make
./scheduler
```
**2.调用**

```cpp
/***
 * 协程调度器
 * 实现功能：创建几个线程；创建几个协程，观察协程在不同线程中执行
*/
#include <iostream>
#include <vector>
#include <time.h>
#include <assert.h>
#include "zyx_thread.h"
#include "measure_time.h"
#include "common.h"
#include "log.h"
#include "fiber.h"
#include "scheduler.h"
//时间 线程id-线程名称 协程id  自定义信息
zyx::Logger::ptr log_main = (new zyx::LoggerManager(zyx::LogLevel::Level::DEBUG, true, false))->Getlogger(); 

void test_sch_1()
{
    ZYX_LOG_DEBUG(log_main,"start sch");
    zyx::Fiber::ptr f=zyx::Fiber::GetThis();
    f->YieldToReady();
    ZYX_LOG_DEBUG(log_main,"med sch");
    f->YieldToReady();
    ZYX_LOG_DEBUG(log_main,"end sch");
}

int main()
{
   zyx::Scheduler::ptr sch(new zyx::Scheduler(6));//创建一个调度器，6为创建的线程个数
   for(int i=0;i<20;i++)
   {
   		//方法一：输入协程
       //zyx::Fiber::ptr f(new zyx::Fiber(test_sch_1)); 
       //sch->schedule(f);
       //方法二：输入回调函数
       sch->schedule(&test_sch_1);//向里面添加任务
   }
   sch->start();//开始执行任务
   sch->stop();//停止

}
```
**3.输出**
可以看出不同协程的不同阶段随机的被空闲线程执行
![可以看出不同协程的不同阶段随机的被空闲线程执行](https://img-blog.csdnimg.cn/56e0a86f9a9d4f9aa95c9e8404bf2358.png)

**4.其他**

```cpp
void Scheduler::run() 中的逻辑：
首先要遍历任务队列(链表实现，因为并不是真正意义的先进先出)，如果任务队列无任务，则进行idle(),
idle()是一个虚函数，可以根据不同的逻辑进行不同的实现，例如epoll就在idle()中进行epoll_wait().
如果任务队列中有任务，则判断该任务有没有指定线程执行，如果指定了且当前并不是指定的线程则寻找下一
个任务。(或者任务处于hold状态，即等待资源状态我们也跳过)。
找到可用的任务后把任务从任务队列中删除，并执行。
执行完成后判断是否是TERM状态，如果是则任务执行结束，如果不是，说明只是该任务执行一半主动放弃了
处理器，我们还要再把该任务加入任务队列，等待下一次执行。
其中有一个tickle()虚函数，该虚函数用于通知别的线程，比如有准备好的任务但是该任务在指定线程中执行，但是这不是指定的协程，则可以通过tickle()通知其他线程。

```

