#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "network_80211.h"

extern "C" {
  #include "user_interface.h"
}

#define CHAN 6
#define args6(a) a[0], a[1], a[2], a[3], a[4], a[5] // expand int[6] to argument list (for snprintf)
#define node_id  ESP.getChipId()

int selfRestart = 100000;

ESP8266WiFiMulti wifi;

void handle_pkt(uint8_t *buf, uint16 len) {
  wifi.addAP("snip","black talons loaded in the clip");
  char ssid_buf[32];
  lpframectrl_80211 framectrl;
  if (len < 64) return;
  
  wifi_promiscuous_enable(0);
  buf += sizeof(struct RxControl);
  struct probe_request_80211 *probe_buf = (struct probe_request_80211*) buf;

  if (FRAME_TYPE_MANAGEMENT == probe_buf->framectrl.Type) {

    /* Management frame */
    if (FRAME_SUBTYPE_PROBE_REQUEST == probe_buf->framectrl.Subtype) {
      uint8* tsaddr = probe_buf->tsaddr;tsaddr;
      

      char srcaddr[18];
      snprintf(srcaddr, sizeof(srcaddr), "%02x:%02x:%02x:%02x:%02x:%02x", args6(tsaddr));

      /* Probe Request */
      ptagged_parameter tag = (ptagged_parameter)(buf + sizeof(probe_request));

      if (tag->tag_length != 0) {
        os_memset(ssid_buf, 0, 32);
        os_memcpy(ssid_buf, (uint8_t *)tag + 2, tag->tag_length);

        char logbuf[81];
        /* snprintf(logbuf, sizeof(logbuf), "Probe request from %s for %s (%d octets remain)", 
          srcaddr, ssid_buf, sizeof(buf) - sizeof(tag)); */
          
        snprintf(logbuf, sizeof(logbuf), "Probe request from %s for %s", srcaddr, ssid_buf);
        Serial.println(logbuf);
        if (wifi.run() == WL_CONNECTED) {
          HTTPClient http;
          http.begin("http://snip/test.php");
          http.addHeader("Content-Type", "application/json");
          int httpCode = http.POST(logbuf);
          Serial.printf("[HTTP] code: %d\n", httpCode);
          http.end();
      }
    }
  }
  wifi_promiscuous_enable(1);
}
}


void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n\nNode ID: ");
  Serial.println(node_id);
  Serial.println("\n\n");
  
  wifi_set_opmode(WIFI_STA);// Set opmode to control to mode 
                            //     0x01 STATION_MODE
                            //     0x02 SOFTAP_MODE
                            //     0x03 STATIONAP_MODE
  
  wifi_promiscuous_enable(1);
  wifi_set_promiscuous_rx_cb(handle_pkt);
  wifi_set_channel(CHAN);
  Serial.println("\n");
}

void loop() {  }
