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
   zyx::Scheduler::ptr sch(new zyx::Scheduler(6));
   for(int i=0;i<20;i++)
   {
       //zyx::Fiber::ptr f(new zyx::Fiber(test_sch_1));
       //sch->schedule(f);
       sch->schedule(&test_sch_1);
   }
   sch->start();
   sch->stop();

}