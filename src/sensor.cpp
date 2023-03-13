#include "sensor.h"

SimpleDHT22 dht;

void readDHT(float *temperature, float *humidity)
{
    float t, h;
    int status = dht.read2(DHT_PIN, &t, &h, NULL);

    if (status == SimpleDHTErrSuccess)
    {
        *temperature = t;
        *humidity = h;
    }
}