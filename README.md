# tobii-eye-interface
Tobii eye interface is a software which maps your eye movements and blinks to move the cursor and send keystrokes or buttonpresses

# How it works
I use tobii apis to get the gaze coordinate and then create a virtual absolute device with libevdev and move the cursor to absolute positions on the screen.

## Dependencies
- tobii stream engine (to be taken from tobii website)
- setup script from [github](https://github.com/Eitol/tobii_eye_tracker_linux_installer)
- libevdev (make a link in your include dir: `~sudo ln -s usr/include/libevdev-1.0/libevdev usr/include/libevdev`)

### To make it run on fedora
- install it on one ubuntu
- install libgconf `sudo dnf install GConf2`
- copy /lib/tobii in the same dir
- copy /include/tobii in the same dir
- copy /opt in the same dir
- copy /usr/share/applications/tobii_config.desktop

spoiler: it does not work hehehehheheheheheheehhe


## Query devices events on fedora
`~sudo evemu-record`

## Query devices events on debian-based OSs (ubuntu)
`~sudo libinput record`
