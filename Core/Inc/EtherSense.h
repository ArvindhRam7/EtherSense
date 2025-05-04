#ifndef ETHERSENSE_H
#define ETHERSENSE_H

#include "stdint.h"
#include "cmsis_os.h"

void StartEthernetTask(void *argument);  // FreeRTOS task entry point

#endif
