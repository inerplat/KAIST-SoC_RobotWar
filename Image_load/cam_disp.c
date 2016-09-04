/*
 * UART_API.C
 *
 * By Kim Jong Keol (jongkkim@adc.co.kr)
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/stat.h>
#include <sys/ioctl.h>
//#include <errno.h>

#include "cam_disp.h"

#define SDK_DEV_NAME "/dev/eagle_sdk"
#define EAGLE_CAMERA_DISP_OFF		(0x12480)
#define EAGLE_CAMERA_DISP_ON		(0x12481)

int eagle_camera_off(void)
{
	int handle;

    if ((handle = open(SDK_DEV_NAME, O_RDWR)) < 0) {
        printf("Open Error %s\n", SDK_DEV_NAME);
        return -1;
    }

	ioctl(handle, EAGLE_CAMERA_DISP_OFF, 0);

	close(handle);
	
	return 0;
}

int eagle_camera_on(void)
{
	int handle;

    if ((handle = open(SDK_DEV_NAME, O_RDWR)) < 0) {
        printf("Open Error %s\n", SDK_DEV_NAME);
        return -1;
    }

	ioctl(handle, EAGLE_CAMERA_DISP_ON, 0);

	close(handle);
	
	return 0;
}




