//                                    .-""-.
//                                   / _  _ \
//                                   |(_)(_)|
// .------------------------------ooO(_ /\ _)Ooo-----------------------------.
// |                                  |====|                                 |
// |                                  '-..-'                                 |
// | Desc:     AsyncTaskQueue - Thread safe queue of tasks to execute        |
// | By:       Nuroferatu - https://github.com/Nuroferatu                    |
// '-------------------------------------------------------------------------'
// ----= Change log =---------------------------------------------------------
//   1. 2017.11.23, 10:30    [+] Initial
// ---------------------------------------------------------------------------
#include "asynctaskqueue.h"
using namespace std;

// ---------------------------------------------------------------------------
// AsyncTaskQueue
// ---------------------------------------------------------------------------
AsyncTaskQueue::AsyncTaskQueue() {
    running = true;
}

AsyncTaskQueue::~AsyncTaskQueue() {
}

// ---------------------------------------------------------------------------
// getTask
// ---------------------------------------------------------------------------
ITaskPtr AsyncTaskQueue::get( void ) {
    do {
        mtxBufferAccess.lock();
        if (taskQueue.size() > 0) {
            ITaskPtr task = taskQueue.front();
            taskQueue.pop_front();
            mtxBufferAccess.unlock();
            return task;
        }
        // Unlock buffer for put and block this thread until we get new data...
        mtxBufferAccess.unlock();
        unique_lock<mutex>  lck( mtxDataReady );
        cvDataReady.wait( lck );
    } while ( running );
    return nullptr;
}

// ---------------------------------------------------------------------------
// put
// ---------------------------------------------------------------------------
void AsyncTaskQueue::put( ITaskPtr task ) {
    mtxBufferAccess.lock();
    taskQueue.push_back( task );
    mtxBufferAccess.unlock();
    cvDataReady.notify_all();
}

// ---------------------------------------------------------------------------
// put
// ---------------------------------------------------------------------------
void AsyncTaskQueue::terminate( void ) {
    running = false;
    mtxBufferAccess.lock();
    taskQueue.clear();
    mtxBufferAccess.unlock();
    cvDataReady.notify_all();   // wake all threads, so we can exit get method
}

/* EOF */