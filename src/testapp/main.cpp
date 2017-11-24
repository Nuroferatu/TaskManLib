#include <Windows.h>
#include <iostream>
#include "taskman.h"

using namespace std;

class SampleTask : public ITask {
    int     id;

public:
    SampleTask( int i ) : id(i) {}

    virtual void onPreExecute( void ) {
        cout << "Prepare task to execute for id " << id << " by worker " << std::this_thread::get_id() << endl;
    }

    virtual void onExecute( void ) {
        Sleep( (id * 20) );
        cout << "execute SampleTask very long task for id " << id << " by worker " << std::this_thread::get_id() << endl;
    }

    virtual void onPostExecute( void ) {
        cout << "Task is done, terminate for id " << id << " by worker " << std::this_thread::get_id() << endl;
        delete this;
    }
};

int main() {
    TaskMan     taskManager;

    taskManager.onInit( 10 );

    for (int i = 0; i < 100; i++) {
        taskManager.addTask( new SampleTask( i ) );
        Sleep( 1 );
    }

    for (int i = 0; i < 100; i++) {
        taskManager.onUpdate();
        Sleep( 100 );
    }
    taskManager.onShutdown();
}

/* EOF */