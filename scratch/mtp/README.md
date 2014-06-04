# notes #

# install dependencies #

    sudo -s
    apt-get install bluetooth bluez-tools evtest
    /etc/init.d/bluetooth status
    /etc/init.d/bluetooth start
    /etc/init.d/bluetooth restart

# connect and pair #

    sudo -s
    hciconfig -a
    hciconfig hci0 up
    hcitool scan
    bluez-simple-agent hci0 F4:1B:A1:31:30:A3
    bluez-test-device trusted F4:1B:A1:31:30:A3 yes
    bluez-test-input connect F4:1B:A1:31:30:A3

# information on the ASUS bluetooth dongle on the pi #

note: tested in linux/macbookair. same hanging behaviour as on the pi. that
means that it's a hardware issue. not a power or driver issue. buy a different
part.

hci0: Type: BR/EDR  Bus: USB
  BD Address: 00:02:72:C0:66:02  ACL MTU: 1022:8  SCO MTU: 121:3
  UP RUNNING PSCAN ISCAN
  RX bytes:45964 acl:1714 sco:0 events:184 errors:0
  TX bytes:1918 acl:34 sco:0 commands:79 errors:0
  Features: 0xff 0xfe 0x0d 0xfe 0x98 0x7f 0x79 0x87
  Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
  Link policy: RSWITCH HOLD SNIFF
  Link mode: SLAVE ACCEPT
  Name: 'raspberrypi-0'
  Class: 0x420100
  Service Classes: Networking, Telephony
  Device Class: Computer, Uncategorized
  HCI Version: 3.0 (0x5)  Revision: 0x9999
  LMP Version: 3.0 (0x5)  Subversion: 0x9999
  Manufacturer: Atheros Communications, Inc. (69)

# my other blue bluetooth dongle #

hci0: Type: BR/EDR  Bus: USB
  BD Address: 11:11:11:11:11:11  ACL MTU: 678:8  SCO MTU: 48:10
  UP RUNNING PSCAN
  RX bytes:1189 acl:0 sco:0 events:40 errors:0
  TX bytes:434 acl:0 sco:0 commands:40 errors:0
  Features: 0xbf 0xfe 0x8d 0x78 0x08 0x18 0x00 0x00
  Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3
  Link policy: RSWITCH SNIFF
  Link mode: SLAVE ACCEPT
  Name: 'raspberrypi-0'
  Class: 0x420100
  Service Classes: Networking, Telephony
  Device Class: Computer, Uncategorized
  HCI Version: 1.2 (0x2)  Revision: 0x1fe
  LMP Version: 1.2 (0x2)  Subversion: 0x1fe
  Manufacturer: Integrated System Solution Corp. (57)

# bluetooth on my macbook air #

hci0:	Type: BR/EDR  Bus: USB
	BD Address: 10:9A:DD:AA:E3:EB  ACL MTU: 1021:8  SCO MTU: 64:1
	UP RUNNING PSCAN 
	RX bytes:828 acl:0 sco:0 events:35 errors:0
	TX bytes:2079 acl:0 sco:0 commands:35 errors:0
	Features: 0xff 0xff 0x8f 0xfe 0x9b 0xff 0x79 0x83
	Packet type: DM1 DM3 DM5 DH1 DH3 DH5 HV1 HV2 HV3 
	Link policy: RSWITCH HOLD SNIFF PARK 
	Link mode: SLAVE ACCEPT 
	Name: 'ubuntu-0'
	Class: 0x6e0100
	Service Classes: Networking, Rendering, Capturing, Audio, Telephony
	Device Class: Computer, Uncategorized
	HCI Version: 2.1 (0x4)  Revision: 0x2ad
	LMP Version: 2.1 (0x4)  Subversion: 0x4222
	Manufacturer: Broadcom Corporation (15)

# hid descriptor report from evtest #

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

# stuff you probably don't need to do #

## kernel

i had to compile kernel modules for the jetson...


## input system setup

[input system](http://sourceforge.net/apps/mediawiki/gizmod/index.php?title=HOWTO_-_Setting_Input_Device_Permissions_-_Creating_a_udev_Rule)

