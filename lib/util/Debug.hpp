#include "freertos/FreeRTOS.h"

/*
	output:

	main            X       1       1048    3       0
	IDLE0           R       0       688     4       0
	IDLE1           R       0       808     5       1
	ipc1            S       24      532     2       1
	ipc0            S       24      540     1       0
*/
constexpr void printTasks()
{
	char tasklist_buf[1024];
	vTaskList(tasklist_buf);
	printf(tasklist_buf);
}