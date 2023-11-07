/*
 * SPDX-FileCopyrightText: 2019 Ha Thach (tinyusb.org)
 *
 * SPDX-License-Identifier: MIT
 *
 * SPDX-FileContributor: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-FileContributor: 2023 esp32beans@gmail.com
 */

// This program echos USB MIDI back to the source.

#if ARDUINO_USB_MODE
#error This sketch must be used when USB is in OTG mode
void setup() {}
void loop() {}
#else
#include "USB.h"

#include "esp32-hal-tinyusb.h"

static const char *TAG = "midiecho";

/** TinyUSB descriptors **/

extern "C" uint16_t tusb_midi_load_descriptor(uint8_t *dst, uint8_t *itf) {
  uint8_t str_index = tinyusb_add_string_descriptor("TinyUSB MIDI");
  uint8_t ep_num = tinyusb_get_free_duplex_endpoint();
  TU_VERIFY(ep_num != 0);
  uint8_t descriptor[TUD_MIDI_DESC_LEN] = {
      // Interface number, string index, EP Out & EP In address, EP size
      TUD_MIDI_DESCRIPTOR(*itf, str_index, ep_num, (uint8_t)(0x80 | ep_num),
                          64)};
  *itf += 1;
  memcpy(dst, descriptor, TUD_MIDI_DESC_LEN);
  return TUD_MIDI_DESC_LEN;
}

static void midi_echo(void *arg) {
  // The MIDI interface always creates input and output port/jack descriptors
  // regardless of these being used or not. Therefore incoming traffic should be
  // read (possibly just discarded) to avoid the sender blocking in IO
  uint8_t packet[4];
  while(true) {
    delay(1);
    while (tud_midi_available()) {
      if (tud_midi_packet_read(packet)) {
        ESP_LOGI(TAG,
                 "Read - Time (ms since boot): %lld, Data: %02hhX %02hhX "
                 "%02hhX %02hhX",
                 esp_timer_get_time(), packet[0], packet[1], packet[2],
                 packet[3]);
        if (!tud_midi_n_packet_write(0, packet)) {
          ESP_LOGI(TAG, "tud_midi_n_packet_write failed");
        }
      }
    }
  }
}

static void usbEventCallback(void *arg, esp_event_base_t event_base,
                             int32_t event_id, void *event_data) {
  if (event_base == ARDUINO_USB_EVENTS) {
    arduino_usb_event_data_t *data = (arduino_usb_event_data_t *)event_data;
    switch (event_id) {
    case ARDUINO_USB_STARTED_EVENT:
      Serial.println("USB PLUGGED");
      break;
    case ARDUINO_USB_STOPPED_EVENT:
      Serial.println("USB UNPLUGGED");
      break;
    case ARDUINO_USB_SUSPEND_EVENT:
      Serial.printf("USB SUSPENDED: remote_wakeup_en: %u\n",
                    data->suspend.remote_wakeup_en);
      break;
    case ARDUINO_USB_RESUME_EVENT:
      Serial.println("USB RESUMED");
      break;

    default:
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);

  USB.onEvent(usbEventCallback);
  tinyusb_enable_interface(USB_INTERFACE_MIDI, TUD_MIDI_DESC_LEN,
                           tusb_midi_load_descriptor);
  USB.begin();
  while (!Serial && millis() < 5000)
    delay(10);
  // Read recieved MIDI packets then write them out
  ESP_LOGI(TAG, "MIDI echo task");
  xTaskCreate(midi_echo, "midi_echo", 2 * 1024, NULL, 5, NULL);
}

void loop() {}
#endif /* ARDUINO_USB_MODE */
