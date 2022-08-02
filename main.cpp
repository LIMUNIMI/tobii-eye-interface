#pragma once
#include <tobii/tobii.h>
#include <tobii/tobii_streams.h>
#include <stdio.h>
#include <assert.h>
#include <cstring>

#include <iostream>
//#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
//#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libevdev-1.0/libevdev/libevdev-uinput.h>
#include <unistd.h>

#include <chrono>
#define CLICK_THRESHOLD 1.5f //s

struct libevdev_uinput *uidev;

class Timer {
public:
    Timer(){}
    ~Timer(){}

    void Start(){
        isRunning = true;
        start = std::chrono::high_resolution_clock::now();
    }

    void Stop(){
        isRunning = false;
    }

    bool CheckIfPassed(float amountS){
        if(isRunning){
            //printf("ciaone %f",(std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count()));
            return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - start).count() > amountS;
        }
        else
            return false;
    }

    bool isRunning = false;

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start,end;
    std::chrono::duration<float> duration;
};

bool mouseWheelMode = false;

void gaze_point_callback(tobii_gaze_point_t const *gaze_point, void *user_data) {
    if (gaze_point->validity == TOBII_VALIDITY_VALID) {
        if (mouseWheelMode){
            //TODO: ruotare rotella un su o in giù
        }else{
            //printf("Gaze point: %f, %f\n", gaze_point->position_xy[0], gaze_point->position_xy[1]);
            libevdev_uinput_write_event(uidev, EV_ABS, ABS_X, gaze_point->position_xy[0]*3100);
            libevdev_uinput_write_event(uidev, EV_ABS, ABS_Y, gaze_point->position_xy[1]*1700);
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
        }
    }
}

Timer rEyeTimer = Timer();// = std::make_unique<Timer>();
Timer lEyeTimer = Timer();

void gaze_origin_callback( tobii_gaze_origin_t const* gaze_origin, void* user_data )
{
    if( gaze_origin->left_validity == TOBII_VALIDITY_INVALID ){ //dopo 3 volte (?)
        if(lEyeTimer.isRunning & lEyeTimer.CheckIfPassed(CLICK_THRESHOLD)){
            printf("left click \n");
            libevdev_uinput_write_event(uidev, EV_KEY, BTN_LEFT, 0);
            libevdev_uinput_write_event(uidev, EV_SYN, SYN_REPORT, 0);
            lEyeTimer.Stop();
        }else if(!lEyeTimer.isRunning){
            lEyeTimer.Start();
        }
    }else{
        lEyeTimer.Stop();
    }

    if( gaze_origin->right_validity == TOBII_VALIDITY_INVALID ){
        if(rEyeTimer.isRunning & rEyeTimer.CheckIfPassed(CLICK_THRESHOLD)){
            printf("right click \n");
            mouseWheelMode = !mouseWheelMode;
            rEyeTimer.Stop();
        }else if(!rEyeTimer.isRunning){
            rEyeTimer.Start();
        }
    }else{
        rEyeTimer.Stop();
    }

}

static void url_receiver(char const *url, void *user_data) {
    char *buffer = (char *) user_data;
    if (*buffer != '\0') return; // only keep first value

    if (strlen(url) < 256)
        strcpy(buffer, url);
}


int main() {

    printf("starting \n");
    //SETUP FAKE INPUTS
    //310mm x 170mm are just my screen dimensions, however they are not much relevant since they're just here to represent the ratio
    //both maximum could have been 1000, but
    struct libevdev *dev;
    int err;
    struct input_absinfo absinfoX{
        .value = 0,
                .minimum = 0,
                .maximum = 3100,
                .fuzz = 50,
                .flat = 0,
                .resolution = 1
    };
    struct input_absinfo absinfoY{
        .value = 0,
                .minimum = 0,
                .maximum = 1700,
                .fuzz = 50,
                .flat = 0,
                .resolution = 1
    };

    dev = libevdev_new();
    libevdev_set_name(dev, "eye tracker pointer");
    libevdev_enable_event_type(dev, EV_REL);
    libevdev_enable_event_code(dev, EV_REL, REL_WHEEL, NULL);

    libevdev_enable_event_type(dev, EV_KEY);
    libevdev_enable_event_code(dev, EV_KEY, BTN_LEFT, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_MIDDLE, NULL);
    libevdev_enable_event_code(dev, EV_KEY, BTN_RIGHT, NULL);

    libevdev_enable_event_type(dev, EV_ABS);
    libevdev_enable_event_code(dev, EV_ABS, ABS_X, &absinfoX);
    libevdev_enable_event_code(dev, EV_ABS, ABS_Y, &absinfoY);

    err = libevdev_uinput_create_from_device(dev, LIBEVDEV_UINPUT_OPEN_MANAGED, &uidev);
    if (err != 0) return err;
    printf("interface configuration complete \n");

    //SETUP TOBII
    tobii_api_t *api;
    tobii_error_t error = tobii_api_create(&api, NULL, NULL);
    assert(error == TOBII_ERROR_NO_ERROR);

    char url[256] = {0};
    error = tobii_enumerate_local_device_urls(api, url_receiver, url);
    assert(error == TOBII_ERROR_NO_ERROR && *url != '\0');

    tobii_device_t *device;
    error = tobii_device_create(api, url, &device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_gaze_point_subscribe(device, gaze_point_callback, 0);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_gaze_origin_subscribe( device, gaze_origin_callback, 0 );
    assert( error == TOBII_ERROR_NO_ERROR );

    printf("tobii tracker configuration complete \n");
    int is_running = 1000; // in this sample, exit after some iterations
    while (--is_running > 0) {

        error = tobii_wait_for_callbacks(1, &device);
        assert(error == TOBII_ERROR_NO_ERROR || error == TOBII_ERROR_TIMED_OUT);

        error = tobii_device_process_callbacks(device);
        assert(error == TOBII_ERROR_NO_ERROR);
    }

    error = tobii_gaze_origin_unsubscribe( device );
    assert( error == TOBII_ERROR_NO_ERROR );

    error = tobii_gaze_point_unsubscribe(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_device_destroy(device);
    assert(error == TOBII_ERROR_NO_ERROR);

    error = tobii_api_destroy(api);
    assert(error == TOBII_ERROR_NO_ERROR);


    usleep(100000UL);
    libevdev_uinput_destroy(uidev);
    return 0;
}
