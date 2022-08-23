#include <iostream>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <unistd.h>

int main() {

int err;
struct libevdev *dev;
struct libevdev_uinput *uidev;
struct input_absinfo absinfo{
	.value = 0,
	.minimum = 0,
	.maximum = 12372,
	.fuzz = 0,
	.flat = 0,
	.resolution = 40
};

dev = libevdev_new();
libevdev_set_name(dev, "eye tracker pointer");
libevdev_enable_event_type(dev, EV_REL);
libevdev_enable_event_code(dev, EV_REL, REL_WHEEL, NULL);
libevdev_enable_event_type(dev, EV_KEY);
libevdev_enable_event_code(dev, EV_KEY, KEY_O, NULL);
libevdev_enable_event_type(dev, EV_ABS);
libevdev_enable_event_code(dev, EV_ABS, ABS_X, &absinfo);
libevdev_enable_event_code(dev, EV_ABS, ABS_Y, &absinfo);

err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
if (err != 0) return err;

std::cout << "noerr" << std::endl;
usleep(50000000UL);//s
libevdev_uinput_write_event(uidev, EV_REL, REL_WHEEL, 1);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
usleep(1000000UL);
libevdev_uinput_write_event(uidev, EV_KEY, KEY_O, 1);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
libevdev_uinput_write_event(uidev, EV_KEY, KEY_O, 0);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
usleep(1000000UL);
libevdev_uinput_write_event(uidev, EV_REL, REL_WHEEL, 1);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
usleep(1000000UL);
libevdev_uinput_write_event(uidev, EV_ABS, ABS_X, 100);
libevdev_uinput_write_event(uidev, EV_ABS, ABS_Y, 100);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
libevdev_uinput_write_event(uidev, EV_ABS, ABS_X, 200);
libevdev_uinput_write_event(uidev, EV_ABS, ABS_Y, 200);
libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

usleep(2000000UL);

libevdev_uinput_destroy(uidev);

return 0;
// libevdev_enable_event_code(dev, EV_REL, REL_Y, NULL);
// libevdev_enable_event_type(dev, EV_KEY);
// libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
// libevdev_enable_event_code(dev, EV_KEY, BTN_MIDDLE, NULL);
// libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);

/*
//DEVICE READER
struct libevdev *dev = NULL;
int fd;
int rc = 1;

fd = open("/dev/input/event9", O_RDONLY|O_NONBLOCK);
rc = libevdev_new_from_fd(fd, &dev);
if (rc < 0) {
        fprintf(stderr, "Failed to init libevdev (%s)\n", strerror(-rc));
        exit(1);
}
std::cout << "bella2" << std::endl;
printf("Input device name: \"%s\"\n", libevdev_get_name(dev));
printf("Input device ID: bus %#x vendor %#x product %#x\n",
       libevdev_get_id_bustype(dev),
       libevdev_get_id_vendor(dev),
       libevdev_get_id_product(dev));
if (!libevdev_has_event_type(dev, EV_REL) ||
    !libevdev_has_event_code(dev, EV_KEY, BTN_LEFT)) {
        printf("This device does not look like a mouse\n");
        exit(1);
}

do {
        struct input_event ev;
        rc = libevdev_next_event(dev, LIBEVDEV_READ_FLAG_NORMAL, &ev);
        if (rc == 0)
                printf("Event: %s %s %d\n",
                       libevdev_event_type_get_name(ev.type),
                       libevdev_event_code_get_name(ev.type, ev.code),
                       ev.value);
} while (rc == 1 || rc == 0 || rc == -EAGAIN);

*/


/*
    int err;
    struct libevdev *dev;
    struct libevdev_uinput *uidev;

    dev = libevdev_new();
    libevdev_set_name(dev, "fake keyboard device");

    libevdev_enable_event_type(dev, EV_KEY);
    libevdev_enable_event_code(dev, EV_KEY, KEY_A, NULL);

    err = libevdev_uinput_create_from_device(dev,
        LIBEVDEV_UINPUT_OPEN_MANAGED,
        &uidev);

    if (err != 0)
        return err;

    libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 1);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
    libevdev_uinput_write_event(uidev, EV_KEY, KEY_A, 0);
    libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);

    //sleep(2);

    libevdev_uinput_destroy(uidev);
    printf("Complete\n");

*/


/*
struct libevdev *dev;
int err;
dev = libevdev_new();
if (!dev)
        return ENOMEM;
err = libevdev_set_fd(dev, 2);
if (err < 0)
        printf("Failed (errno %d): %s\n", -err, strerror(-err));
libevdev_free(dev);


*/







	return 0;
}
