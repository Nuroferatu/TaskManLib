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
        std::thread* th = new std::thread( TaskMan::threadWorker, this, (500*i)+500 );
        threadList.push_back( th );
    }
    cout << "TaskMan::onInit with " << workersCount << " working threads" << endl;
}

// ---------------------------------------------------------------------------
// onShutdown
// ---------------------------------------------------------------------------
void TaskMan::onShutdown( void ) {
    running = false;

    cout << "TaskMan::onShutdown - waiting for threads to finish" << endl;
    for (auto tq : threadList )
        tq->join();
    cout << "TaskMan::onShutdown - done" << endl;
}

// ---------------------------------------------------------------------------
// addTask
// ---------------------------------------------------------------------------
void TaskMan::addTask( ITask* task ) {
    assert( task != nullptr );
    if (task)
        task->execute();
}

void TaskMan::addTask( int i ) {
    taskQueue.put( i );
}

// ---------------------------------------------------------------------------
// getTask
// ---------------------------------------------------------------------------
int TaskMan::getTask( void ) {
    return taskQueue.get();
}

// ---------------------------------------------------------------------------
// workerTask
// --------------------------------------------------------------= STATIC =---
void TaskMan::threadWorker( TaskMan* taskMan, int delay ) {
    assert( taskMan != nullptr );

    cout << "Worker started: " << std::this_thread::get_id() << endl;
    while (taskMan->isRunning()) {
        cout << "Worker [" << std::this_thread::get_id() << "] is executing task: " << taskMan->getTask() << endl;
        Sleep( delay );
    }
    cout << "Worker " << std::this_thread::get_id() << " stoped" << endl;
}

/* EOF */