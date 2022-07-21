#ifndef DATALOGGING_H
#define DATALOGGING_H

#include <stddef.h>

bool initializeLogging();
void storeData(const void *data, size_t dataSize);

#endif // DATALOGGING_H
