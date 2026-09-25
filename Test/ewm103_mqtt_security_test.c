#include <EWM103/ewm103.h>
#include <assert.h>
#include <string.h>

static void CheckUserCfg(EWM103_MqttScheme scheme, const char *expected)
{
  EWM103_Content content;
  char packet[256];
  size_t length = 0U;

  memset(&content, 0, sizeof(content));
  content.type = EWM103_TYPE_MQTTUSERCFG;
  content.link_id = 0U;
  content.mode = (uint8_t)scheme;
  content.s0 = "device-01";
  content.s1 = "user";
  content.s2 = "pass";
  content.u0 = 0U;
  content.u1 = 0U;
  content.s3 = "/";
  assert(EWM103_Pack(&content, packet, sizeof(packet), &length) ==
         EWM103_RESULT_OK);
  assert(length == strlen(expected));
  assert(strcmp(packet, expected) == 0);
}

int main(void)
{
  CheckUserCfg(EWM103_MQTT_SCHEME_TCP,
      "AT+MQTTUSERCFG=0,1,\"device-01\",\"user\",\"pass\",0,0,\"/\"\r\n");
  CheckUserCfg(EWM103_MQTT_SCHEME_TLS_VERIFY_SERVER,
      "AT+MQTTUSERCFG=0,3,\"device-01\",\"user\",\"pass\",0,0,\"/\"\r\n");
  CheckUserCfg(EWM103_MQTT_SCHEME_TLS_MUTUAL,
      "AT+MQTTUSERCFG=0,5,\"device-01\",\"user\",\"pass\",0,0,\"/\"\r\n");
  return 0;
}
