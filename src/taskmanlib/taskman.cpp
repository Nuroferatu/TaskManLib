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
// onShutdown
// ---------------------------------------------------------------------------
void TaskMan::onShutdown( void ) {
    cout << "TaskMan::onShutdown" << endl;
    running = false;
}

// ---------------------------------------------------------------------------
// addTask
// ---------------------------------------------------------------------------
void TaskMan::addTask( ITask* task ) {
    assert( task != nullptr );
    if (task)
        task->execute();
}

// ---------------------------------------------------------------------------
// workerTask
// ---------------------------------------------------------------------------
void TaskMan::threadWorker( TaskMan* taskMan ) {
    assert( taskMan != nullptr );
    if (!taskMan)
        return;
    cout << "Worker started: " << std::this_thread::get_id() << endl;
}

/* EOF */