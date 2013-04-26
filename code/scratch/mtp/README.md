gotta do some software install and pairing bullshit


sudo -i

apt-get install bluetooth bluez-tools
/etc/init.d/bluetooth status
/etc/init.d/bluetooth start
/etc/init.d/bluetooth restart

hciconfig -a
hciconfig hci0 up
hcitool scan
bluez-simple agent hci0 <bt address>
bluez-test-device trusted <bt address> yes
bluez-test-input connect <bt address>


Input driver version is 1.0.1
Input device ID: bus 0x5 vendor 0x5ac product 0x30e version 0x160
Input device name: "ky’s trackpad"
Supported events:
  Event type 0 (EV_SYN)
  Event type 1 (EV_KEY)
    Event code 272 (BTN_LEFT)
    Event code 325 (BTN_TOOL_FINGER)
    Event code 328 (BTN_TOOL_QUINTTAP)
    Event code 330 (BTN_TOUCH)
    Event code 333 (BTN_TOOL_DOUBLETAP)
    Event code 334 (BTN_TOOL_TRIPLETAP)
    Event code 335 (BTN_TOOL_QUADTAP)
  Event type 3 (EV_ABS)
    Event code 0 (ABS_X)
      Value   -464
      Min    -2909
      Max     3167
      Fuzz       4
      Resolution      46
    Event code 1 (ABS_Y)
      Value    182
      Min    -2456
      Max     2565
      Fuzz       4
      Resolution      45
    Event code 47 (ABS_MT_SLOT)
      Value      5
      Min        0
      Max       15
    Event code 48 (ABS_MT_TOUCH_MAJOR)
      Value      0
      Min        0
      Max     1020
      Fuzz       4
    Event code 49 (ABS_MT_TOUCH_MINOR)
      Value      0
      Min        0
      Max     1020
      Fuzz       4
    Event code 52 (ABS_MT_ORIENTATION)
      Value      0
      Min      -31
      Max       32
      Fuzz       1
    Event code 53 (ABS_MT_POSITION_X)
      Value      0
      Min    -2909
      Max     3167
      Fuzz       4
      Resolution      46
    Event code 54 (ABS_MT_POSITION_Y)
      Value      0
      Min    -2456
      Max     2565
      Fuzz       4
      Resolution      45
    Event code 57 (ABS_MT_TRACKING_ID)
      Value      0
      Min        0
      Max    65535
  Event type 4 (EV_MSC)
    Event code 4 (MSC_SCAN)
  Event type 20 (EV_REP)
Properties:
  Property type 0 (INPUT_PROP_POINTER)
  Property type 2 (INPUT_PROP_BUTTONPAD)
Testing ... (interrupt to exit)
