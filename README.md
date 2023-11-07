# ESP32-S2 and ESP32-S3 USB MIDI Example for Arduino

For lots more debug output, set the Debug Core Level to Verbose. This can
drastically slow the code so do not do this when speed matters.

## examples/ESP32USBMIDI

This program is based on an ESP-IDF TinyUSB USB MIDI example with minimal
changes so it works with arduino-esp32. See https://github.com/espressif/esp-idf/blob/master/examples/peripherals/usb/device/tusb_midi/main/tusb_midi_main.c
for the original code.

The board must be reset or power cycled to make the new code take effect. This
has been tested on ESP32S3 and ESP32S2 DevKit boards ("ESP32S3 Dev Module"
and "ESP32S2 Dev Module").

USB Mode must be set to USB-OTG (TinyUSB) on ESP32S3.

Some knowledge of the ESP-IDF API is helpful to understand the code.

Example of output on the Arduino IDE Serial monitor.
The computer connected to the ESP32S3 sends MIDI Start, Stop, and CC 7 0.
The function midi_task_read_example() processes the incoming MIDI data and
prints the following.
```
141236968: Cable: 0 Code: F, Data: FA 00 00
141237070: Cable: 0 Code: F, Data: FC 00 00
141237158: Cable: 0 Code: B, Data: B0 07 00
```

Example of MIDI received from the ESP32S3.
The function periodic_midi_write_example_cb() sends MIDI data. The computer
connected to the ESP32S3 produced the following output.
```
channel  1   note-on          F#4 127
channel  1   note-off          D4   0
channel  1   note-on           A4 127
channel  1   note-off         F#4   0
channel  1   note-on           D5 127
channel  1   note-on          F#4 127
channel  1   note-off          D4   0
channel  1   note-on           A4 127
channel  1   note-off         F#4   0
channel  1   note-on           D5 127
channel  1   note-off          A4   0
channel  1   note-on          F#5 127
channel  1   note-off          D5   0
```

## examples/ESP32USBMIDIECHO

This program echos USB MIDI back to the source. There is very little serial console output.

SendMIDI and ReceiveMIDI are command line MIDI programs for Windows, MacOS, and Linux.

* https://github.com/gbevin/ReceiveMIDI
* https://github.com/gbevin/SendMIDI

In one terminal windows, get ready to show received MIDI with timestamps.
```
$ receivemidi list
Midi Through Port-0
ESP32S3_DEV MIDI 1
$ receivemidi dev ESP32S3_DEV timestamp
```

In another terminal window, send MIDI start and stop.
```
$ sendmidi list
Midi Through Port-0
ESP32S3_DEV MIDI 1
$ sendmidi dev ESP32S3_DEV start stop
```

On the receive window, something similar to this should appear.
```
13:18:25.506   stop
13:18:25.507   start
```

Try sending MIDI panic to see what it does.
```
$ sendmidi dev ESP32S3_DEV panic
```

panic sends note off to all 16 channels and for all 128 notes plus CC 64 0, CC 120, 0, and CC 123 0.

receivemidi should show something like this. This is the start of a very long
listing.
```
13:22:32.791   channel  1   control-change    64     0
13:22:32.792   channel  1   control-change   120     0
13:22:32.793   channel  1   control-change   123     0
13:22:32.794   channel  1   note-off         C-2   0
13:22:32.795   channel  1   note-off        C#-2   0
13:22:32.796   channel  1   note-off         D-2   0
13:22:32.797   channel  1   note-off        D#-2   0
```
