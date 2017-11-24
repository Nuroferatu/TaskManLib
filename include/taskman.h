//                                    .-""-.
//                                   / _  _ \
//                                   |(_)(_)|
// .------------------------------ooO(_ /\ _)Ooo-----------------------------.
// |                                  |====|                                 |
// |                                  '-..-'                                 |
// | Desc:     TaskMan - Async Task Manager                                  |
// | By:       Nuroferatu - https://github.com/Nuroferatu                    |
// '-------------------------------------------------------------------------'
// ----= Change log =---------------------------------------------------------
//   2. 2017.11.24, 11:50    [*] ITask interface changed. TaskMan must support onPreExecute/onPostExecute
//                           [+] onUpdate - to execute onPre/onPost on TaskMan thread
//   1. 2017.11.22, 19:10    [+] Initial
// ---------------------------------------------------------------------------
#pragma once
#ifndef __TASKMAN_H__
#define __TASKMAN_H__

#include "asynctaskqueue.h"
#include <list>
#include <thread>

// ---------------------------------------------------------------------------
// TaskMan
// ---------------------------------------------------------------------------
class TaskMan {
public:
    TaskMan();
    ~TaskMan();

    void onInit( int workersCount );
    void onUpdate( void );
    void onShutdown( void );

    void addTask( ITask* task );
    volatile bool isRunning( void ) const { return running; }

protected:
    ITaskPtr    getTask( void );
    static void threadWorker( TaskMan* taskMan );

    volatile bool   running;
    AsyncTaskQueue  taskQueue;

    // Tasks in this queue are executed on TaskMan thread, synchronization for preQueue is a bit overused, but required for postQueue as workers will have access to it.
    AsyncTaskQueue  preQueue;
    AsyncTaskQueue  postQueue;

    std::list<std::thread*> threadList;
};

#endif /* ndef __TASKMAN_H__ */
/* EOF */