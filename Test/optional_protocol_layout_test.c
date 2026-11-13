#include <MQTT/core_mqtt_serializer.h>
#include <pb/pb_encode.h>
#include <pb/pb_decode.h>
#include <assert.h>
#include <stdint.h>

int main(void)
{
    uint8_t bytes[16] = {0};
    pb_ostream_t out = pb_ostream_from_buffer(bytes, sizeof bytes);
    assert(pb_encode_varint(&out, 300U));
    assert(out.bytes_written == 2U && bytes[0] == 0xacU && bytes[1] == 2U);
    pb_istream_t in = pb_istream_from_buffer(bytes, out.bytes_written);
    uint64_t value = 0;
    assert(pb_decode_varint(&in, &value) && value == 300U && !in.bytes_left);
    size_t packet_size = 0;
    assert(MQTT_GetPingreqPacketSize(&packet_size) == MQTTSuccess && packet_size == 2U);
    MQTTFixedBuffer_t buffer = {bytes, sizeof bytes};
    assert(MQTT_SerializePingreq(&buffer) == MQTTSuccess);
    assert(bytes[0] == 0xc0U && bytes[1] == 0U);
    buffer.size = 1U;
    assert(MQTT_SerializePingreq(&buffer) == MQTTNoMemory);
    return 0;
}
