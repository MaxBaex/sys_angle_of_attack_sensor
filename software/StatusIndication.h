#ifndef STATUS_INDICATION_H
#define STATUS_INDICATION_H

#include "Task.h"
#include <Arduino.h>
#include <stddef.h>

class StatusIndication : public Task {
  public:
    StatusIndication(const Task **taskList, size_t taskListLen)
        : _taskList(taskList), _taskListLen(taskListLen){};
    bool begin();

  protected:
    void run() override;

  private:
    const Task **_taskList;
    const size_t _taskListLen;
};

#endif // STATUS_INDICATION_H
