#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "AsyncDelay.h"

static const char *TAG = "main";
/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-web-bluetooth/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "19b10000-e8f2-537e-4f6c-d104768a1214"
#define SENSOR_CHARACTERISTIC_UUID "19b10001-e8f2-537e-4f6c-d104768a1214"
#define LED_CHARACTERISTIC_UUID "19b10002-e8f2-537e-4f6c-d104768a1214"

BLEServer *pServer = NULL;
BLECharacteristic *pSensorCharacteristic = NULL;
BLECharacteristic *pLedCharacteristic = NULL;
static bool deviceConnected = false;
static uint32_t value = 0;
static AsyncDelay samplingInterval;
static AsyncDelay advertizingInterval;

static void setup_ble();
static void start_advertising();

const int ledPin = 2; // Use the appropriate GPIO pin for your setup

void setup()
{
	delay(2000); // Wait for the VCP to connect

	ESP_LOGI(TAG, "Build %s, %s %s\r\n", AUTO_VERSION, __DATE__, __TIME__);
	setup_ble();
}

class MyServerCallbacks : public BLEServerCallbacks
{
	void onConnect(BLEServer *pServer)
	{
		deviceConnected = true;
	};

	void onDisconnect(BLEServer *pServer)
	{
		deviceConnected = false;
	}
};

class MyCharacteristicCallbacks : public BLECharacteristicCallbacks
{
	void onWrite(BLECharacteristic *pLedCharacteristic)
	{
		std::string value = pLedCharacteristic->getValue();
		if (value.length() > 0)
		{
			Serial.print("Characteristic event, written: ");
			Serial.println(static_cast<int>(value[0])); // Print the integer value

			int receivedValue = static_cast<int>(value[0]);
			if (receivedValue == 1)
			{
				digitalWrite(ledPin, HIGH);
			}
			else
			{
				digitalWrite(ledPin, LOW);
			}
		}
	}
};

void setup_ble()
{
	pinMode(ledPin, OUTPUT);

	// Create the BLE Device
	BLEDevice::init("ESP32");

	// Create the BLE Server
	pServer = BLEDevice::createServer();
	pServer->setCallbacks(new MyServerCallbacks());

	// Create the BLE Service
	BLEService *pService = pServer->createService(SERVICE_UUID);

	// Create a BLE Characteristic
	pSensorCharacteristic = pService->createCharacteristic(
		SENSOR_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_READ |
			BLECharacteristic::PROPERTY_WRITE |
			BLECharacteristic::PROPERTY_NOTIFY |
			BLECharacteristic::PROPERTY_INDICATE);

	// Create the ON button Characteristic
	pLedCharacteristic = pService->createCharacteristic(
		LED_CHARACTERISTIC_UUID,
		BLECharacteristic::PROPERTY_WRITE);

	// Register the callback for the ON button characteristic
	pLedCharacteristic->setCallbacks(new MyCharacteristicCallbacks());

	// https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
	// Create a BLE Descriptor
	pSensorCharacteristic->addDescriptor(new BLE2902());
	pLedCharacteristic->addDescriptor(new BLE2902());

	// Start the service
	pService->start();

	// Start advertising
	BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
	pAdvertising->addServiceUUID(SERVICE_UUID);
	pAdvertising->setScanResponse(false);
	pAdvertising->setMinPreferred(0x0); // set value to 0x00 to not advertise this parameter
	start_advertising();
	ESP_LOGI(TAG, "Waiting a client connection to notify...");
	BLEDevice::
}

typedef enum connection_state
{
	DISCONNECTED,
	CONNECTED
} connection_state_t;

void loop()
{
	static connection_state_t connection_state = DISCONNECTED;

	switch (connection_state)
	{
	case DISCONNECTED:
		if (advertizingInterval.isExpired())
		{
			esp_sleep_enable_timer_wakeup(7e6);
			esp_light_sleep_start();
			//esp_deep_sleep_start();
			start_advertising();
		}
		if (deviceConnected)
		{
			connection_state = CONNECTED;
			ESP_LOGI(TAG, "Device Connected");
			samplingInterval.start(3000, AsyncDelay::MILLIS);
		}
		break;
	case CONNECTED:
		if (deviceConnected)
		{
			if (samplingInterval.isExpired())
			{
				samplingInterval.repeat();
				// notify changed value
				pSensorCharacteristic->setValue(String(value).c_str());
				pSensorCharacteristic->notify();
				ESP_LOGI(TAG, "New value notified: %d", value);
				value++;
			}
		}
		else
		{
			ESP_LOGI(TAG, "Device disconnected.");
			delay(500); // give the bluetooth stack the chance to get things ready
			start_advertising();
			connection_state = DISCONNECTED;
		}
		break;
	}
}

void start_advertising()
{
	BLEDevice::startAdvertising();
	delay(100);
	BLEDevice::stopAdvertising();
	advertizingInterval.start(1000, AsyncDelay::MILLIS);
}