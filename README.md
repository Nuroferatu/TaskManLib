# TaskManLib

Next steps / ideas:
  - threadWorker - moved to own class, that is passed 'instead of' or 'next to' TaskMan at thread construction
  - keep track of execution time of each ITask by worker - stats and/or stuck thread control
  - Better lifecycle control of ITask (sample now lakes memory after being executed)
  - Resuable ITask - that after execution is added to end of queue
  - Delayed ITask - that will be executed after some timestamp
  - Priority HIGH/LOW when adding ITask to queue (HIGH push_front / LOW push_back)
  - TaskMan - onUpdate method, that will check (and if needed restart) blocked workers (requires some R&D)
  - [?] Use CreateThread instead of std::thread ?
  - Task execution callback - for example per ITask listener that may/maynot be attached to task
  
