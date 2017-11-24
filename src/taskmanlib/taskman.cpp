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
#include "taskman.h"

#include <assert.h>
#include <iostream>
#include <Windows.h>

using namespace std;

// ---------------------------------------------------------------------------
// Constructor / Destructor
// ---------------------------------------------------------------------------
TaskMan::TaskMan() {
    running = false;
}

TaskMan::~TaskMan() {
    assert( !running );
}

// ---------------------------------------------------------------------------
// onInit
// ---------------------------------------------------------------------------
void TaskMan::onInit( int workersCount ) {
    if (workersCount < 1)
        workersCount = 1;

    running = true;
    for (int i = 0; i < workersCount; ++i) {
        std::thread* th = new std::thread( TaskMan::threadWorker, this );
        threadList.push_back( th );
    }
    cout << "TaskMan::onInit with " << workersCount << " working threads" << endl;
}

// ---------------------------------------------------------------------------
// onUpdate
// ---------------------------------------------------------------------------
void TaskMan::onUpdate( void ) {
    cout << "onUpdate" << endl;

    while (!preQueue.isEmpty()) {
        ITaskPtr task = preQueue.get();     // Is it possible to get locked here? only if other thread try to use this queue
        task->onPreExecute();
        taskQueue.put( task );
    }

    while (!postQueue.isEmpty()) {
        ITaskPtr task = postQueue.get();     // Is it possible to get locked here? yes, when task is executed and worker is currently updateing queue.
        task->onPostExecute();
    }

}

// ---------------------------------------------------------------------------
// onShutdown
// ---------------------------------------------------------------------------
void TaskMan::onShutdown( void ) {
    running = false;

    cout << "TaskMan::onShutdown - waiting for threads to finish" << endl;
    taskQueue.terminate();
    for (auto tq : threadList )
        tq->join();
    cout << "TaskMan::onShutdown - done" << endl;
}

// ---------------------------------------------------------------------------
// addTask
// ---------------------------------------------------------------------------
void TaskMan::addTask( ITaskPtr task ) {
    assert( task != nullptr );
    preQueue.put( task );
}

// ---------------------------------------------------------------------------
// getTask
// ---------------------------------------------------------------------------
ITaskPtr TaskMan::getTask( void ) {
    return taskQueue.get();
}

// ---------------------------------------------------------------------------
// workerTask
// --------------------------------------------------------------= STATIC =---
void TaskMan::threadWorker( TaskMan* taskMan ) {
    assert( taskMan != nullptr );
    while (taskMan->isRunning()) {
        ITaskPtr task = taskMan->getTask();
        if (task) {
            task->onExecute();
            taskMan->postQueue.put( task );
        }
    }
}

/* EOF */