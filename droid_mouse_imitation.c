#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <fcntl.h>  
#include <linux/uinput.h>  
  
int main(){  
  struct uinput_user_dev device;  
  memset(&device, 0, sizeof device);  
  int uinputfd = open("/dev/uinput",O_WRONLY);  
  strcpy(device.name, "Hillcrest Labs Virtual Pointer");  
  device.id.bustype = BUS_USB;  
  device.id.vendor = 0x1d5a; // Hillcrest Labs VID  
  device.id.product = 0xc2b3; // FSM, but probably doesn't matter  
  device.id.version = 1;  
  int i = 0;  
  for (i = 0; i < ABS_MAX; i++) {  
    device.absmax[i] = -1;  
    device.absmin[i] = -1;  
    device.absfuzz[i] = -1;  
    device.absflat[i] = -1;  
  }  
  if (write(uinputfd,&device,sizeof(device)) != sizeof(device)) {  
    fprintf(stderr, "Error initializing the input device.\n");  
    return 1;  
  }  
  if (ioctl(uinputfd, UI_SET_EVBIT, EV_REL) < 0 ||  
    ioctl(uinputfd, UI_SET_RELBIT, REL_X) < 0 ||  
    ioctl(uinputfd, UI_SET_RELBIT, REL_Y) < 0 ||  
    ioctl(uinputfd, UI_SET_RELBIT, REL_WHEEL) < 0 ||  
    ioctl(uinputfd, UI_SET_EVBIT, EV_KEY) < 0 ||  
    ioctl(uinputfd, UI_SET_KEYBIT, BTN_LEFT) < 0 ||  
    ioctl(uinputfd, UI_SET_KEYBIT, BTN_RIGHT) < 0 ||  
    ioctl(uinputfd, UI_SET_KEYBIT, BTN_MIDDLE) < 0 ||  
    ioctl(uinputfd,UI_DEV_CREATE) < 0) {  
    fprintf(stderr, "Error configuring the input device.\n");  
    return 1;  
  }    
  while(1){  
  
  
  };  
  return 0;  
}  
