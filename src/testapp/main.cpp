#include <Windows.h>
#include <iostream>
#include "taskman.h"

using namespace std;

class SampleTask : public ITask {
public:
    virtual void execute( void ) {
        cout << "execute SampleTask" << endl;
    }
};

int main() {
    TaskMan     taskManager;

    taskManager.onInit( 2 );

    SampleTask  st1;
    taskManager.addTask( &st1 );

    taskManager.onShutdown();
}

/* EOF */