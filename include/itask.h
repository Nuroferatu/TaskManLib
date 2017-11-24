//                                    .-""-.
//                                   / _  _ \
//                                   |(_)(_)|
// .------------------------------ooO(_ /\ _)Ooo-----------------------------.
// |                                  |====|                                 |
// |                                  '-..-'                                 |
// | Desc:     ITask - interface for task run by Task Manager                |
// | By:       Nuroferatu - https://github.com/Nuroferatu                    |
// '-------------------------------------------------------------------------'
// ----= Change log =---------------------------------------------------------
//   2. 2017.11.24, 11:50    [*] Interface changed. Added onPreExecute and onPostExecute
//                               for better control of task lifecycle (executed on TaskMan thread)
//   1. 2017.11.22, 19:15    [+] Initial
// ---------------------------------------------------------------------------
#pragma once
#ifndef __ITASK_H__
#define __ITASK_H__

// ---------------------------------------------------------------------------
// ITask
// ---------------------------------------------------------------------------
class ITask {
public:
    virtual ~ITask() = default;
    virtual void onPreExecute( void ) = 0;
    virtual void onExecute( void ) = 0;
    virtual void onPostExecute( void ) = 0;
};

typedef ITask*  ITaskPtr;

#endif /* ndef __ITASK_H__ */
/* EOF */