#include <Windows.h>
#include <iostream>
#include "taskman.h"

using namespace std;


int main() {
    TaskMan     taskManager;

    taskManager.onInit( 2 );

    taskManager.onShutdown();
}

/* EOF */