#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include "NeuralNetwork.h"

#define SSID "galaxy"
#define PASSWORD "12341234"

#define PORT 30000
WiFiUDP Udp;
char Buffer[255];

NeuralNetwork *nn;

// parameters for Scaling input data
float mean_[12] = { 1.70971205, -9.09947274, -0.84459523,  0.08074532, -0.56302103,
        -0.1370391 , -3.53950139, -5.66709452,  2.31681035, -0.23059068,
        -0.41651389,  0.3579665 };
float scale_[12]  = {4.43386527, 5.29852172, 6.57840717, 0.46723938, 0.41742006,
        0.55514724, 6.06001735, 6.60135923, 4.2558025 , 0.54293752,
        0.54414737, 0.52820823};

void setup()
{
	Serial.begin(9600);
	nn = new NeuralNetwork();

	// connect to wifi
	WiFi.begin(SSID, PASSWORD);
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.printf("WiFi connected! %s \n", WiFi.localIP().toString().c_str());

	Udp.begin(PORT);
}

void loop()
{
	int packetSize = Udp.parsePacket();
	if (packetSize)
	{
		Serial.printf("Received packet of size %d from %s:%d \n", packetSize,
					  Udp.remoteIP().toString().c_str(), Udp.remotePort());
		int len = Udp.read(Buffer, sizeof(Buffer));
		if (len > 0)
			Buffer[len] = 0;
		Udp.flush();
		JsonDocument docreq;
		deserializeJson(docreq, Buffer);
		for (int i = 0; i < 12; i++)
			nn->getInputBuffer()[i] = (((float) docreq[i])-mean_[i])/scale_[i];
		nn->predict();
		float maxPred = 0;
		int maxIndex = -1;
		for (int i = 0; i < 13; i++)
		{
			float newPred = nn->getOutputBuffer()[i];
			if (newPred >= maxPred)
			{
				maxPred = newPred;
				maxIndex = i;
			}
		}
		Serial.printf("Pred=%d\n", maxIndex);
		JsonDocument docres;
		docres["pred"] = maxIndex;
		int sz = serializeJson(docres, Buffer, sizeof(Buffer));
        Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    	Udp.write((uint8_t*)Buffer, sizeof(Buffer[0])*sz);
		Udp.endPacket();
	}
}