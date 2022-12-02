int main()
{
        static int uinp_fd;
        struct uinput_user_dev uinp;
        struct input_event event;

        uinp_fd = open("/dev/uinput", O_WRONLY|O_NONBLOCK);
        if(uinp_fd == NULL) {
                printf("Unable to open /dev/uinput\n");
                return -1;
        }

        // configure touch device event properties
        memset(&uinp, 0, sizeof(uinp));
        strncpy(uinp.name, "MyFakeTouchScreen", UINPUT_MAX_NAME_SIZE);




        return ;
}

