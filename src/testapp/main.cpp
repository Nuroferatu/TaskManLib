#include <Windows.h>
#include <iostream>
#include "taskman.h"

using namespace std;

class SampleTask : public ITask {
    int     id;

public:
    SampleTask( int i ) : id(i) {}

    virtual void execute( void ) {
        cout << "execute SampleTask very long task for id " << id << " by worker " << std::this_thread::get_id() << endl;
        Sleep( (id * 20) );
    }
};


int main() {
    TaskMan     taskManager;

    taskManager.onInit( 10 );

    for (int i = 0; i < 100; i++) {
        taskManager.addTask( new SampleTask( i ) );
        Sleep( 1 );
    }

    Sleep( 10000 );
    taskManager.onShutdown();
}

/* EOF */