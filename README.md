# tobii-eye-interface
Tobii eye interface is a software which maps your eye movements and blinks to move the cursor and send keystrokes or button presses

# How it works
It uses tobii APIs to get the gaze coordinate and then create a virtual absolute device with libevdev and move the cursor to absolute positions on the screen.
This is an alpha version of the project, so the emulation stops after 2min.

## Dependencies
- tobii stream engine (to be taken from tobii website)
- setup script from [github](https://github.com/Eitol/tobii_eye_tracker_linux_installer)
- libevdev (make a link in your "include" dir: `~sudo ln -s usr/include/libevdev-1.0/libevdev usr/include/libevdev`) i don't know why this is not done automatically

### Run on Ubuntu
Unfortunately to my knowledge only an Ubutnu-based distro >= 18.04 can be used to run this software.
- execute the setup script reported in the dependencies
- One new app called tobii-config should now be on your application drawer
- build this project (CMakeLists.txt)
- You can find the executable (eye-interface) in the Build directory you choose
- Calibrate the eye-tracker using the new tobii-config app.
- Launch the eye-interface using elevated prviledges
	`sudo ./eye-interface `


## Query devices events on fedora
`~sudo evemu-record`

## Query devices events on debian-based OSs (ubuntu)
`~sudo libinput record`
