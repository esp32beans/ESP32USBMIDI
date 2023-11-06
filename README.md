# ESP32-S2 and ESP32-S3 USB MIDI Example for Arduino

This program is based on an ESP-IDF TinyUSB USB MIDI example with minimal
changes so it works with arduino-esp32. See https://github.com/espressif/esp-idf/blob/master/examples/peripherals/usb/device/tusb_midi/main/tusb_midi_main.c
for the original code.

The board must be reset or power cycled to make the new code take effect. This
has been tested on ESP32-S3 and ESP32-S2 DevKit boards ("ESP32S3 Dev Module"
and "ESP32S2 Dev Module").

USB Mode must be set to USB-OTG (TinyUSB) on ESP32S3.

For lots more debug output, set the Debug Core Level to Verbose. It helps to be
familiar with the ESP-IDF API.

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
