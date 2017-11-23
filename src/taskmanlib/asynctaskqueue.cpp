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
#include <iostream>
#include <thread>

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
int AsyncTaskQueue::get( void ) {
    do {
        mtxBufferAccess.lock();
        cout << "Queue worker [" << std::this_thread::get_id() << "] locked buffer" << endl;
        if (taskQueue.size() > 0) {
            cout << "Queue worker [" << std::this_thread::get_id() << "] will process task" << endl;
            int intTaks = taskQueue.front();
            taskQueue.pop_front();
            cout << "Queue worker [" << std::this_thread::get_id() << "] buffer unclocked" << endl;
            mtxBufferAccess.unlock();
            return intTaks;
        }

        // Unlock buffer for put and block this thread until we get new data...
        cout << "Queue worker [" << std::this_thread::get_id() << "] buffer unlocked and locking thread" << endl;
        mtxBufferAccess.unlock();
        unique_lock<mutex>  lck( mtxDataReady );
        cvDataReady.wait( lck );
    } while ( running );  // todo: Use here some state, so we can stop processing and not get stuck in infinite state
    cout << "Queue worker [" << std::this_thread::get_id() << "] get terminated" << endl;

    // todo: If we get here we will throw some exception, that app was terminated, to let owner exit thread
    return 0;
}

// ---------------------------------------------------------------------------
// put
// ---------------------------------------------------------------------------
void AsyncTaskQueue::put( int i ) {
    mtxBufferAccess.lock();
    cout << "AsyncTaskQueue::put locked buffer " << i << endl;
    taskQueue.push_back( i );
    cout << "AsyncTaskQueue::put unlocked buffer " << i << endl;
    mtxBufferAccess.unlock();

    cvDataReady.notify_all();
}

// ---------------------------------------------------------------------------
// put
// ---------------------------------------------------------------------------
void AsyncTaskQueue::terminate( void ) {
    running = false;
    mtxBufferAccess.lock();
    cout << "AsyncTaskQueue::terminate in progress " << endl;
    taskQueue.clear();
    mtxBufferAccess.unlock();
    cvDataReady.notify_all();   // wake all threads, so we can exit get method
}

/* EOF */