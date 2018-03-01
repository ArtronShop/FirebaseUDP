#ifndef __FirebaseUDP_CPP__
#define __FirebaseUDP_CPP__

#include "FirebaseUDP.h"

FirebaseUDP::FirebaseUDP(AIS_NB_BC95 *aisObj) {
	_aisObj = aisObj;
}

void FirebaseUDP::begin(String token, unsigned long port) {
	_token = token;
	_port = port;

	_aisObj->setupDevice(String(_port + 2));
	
	// Auth
	_aisObj->sendUDPmsg(SERVER_IP, String(_port), FirebaseUDP_Protocol(0x01, String("\"" + token + "\"")));
}

void FirebaseUDP::set(String path, String data) {
	String payload = "{\"path\":\"" + path + "\",\"data\":\"" + data + "\"}";
	_aisObj->sendUDPmsg(SERVER_IP, String(_port), FirebaseUDP_Protocol(0x03, payload));
}

void FirebaseUDP::array_to_string(byte array[], unsigned int len, char buffer[]) {
	for (unsigned int i = 0; i < len; i++) {
		byte nib1 = (array[i] >> 4) & 0x0F;
		byte nib2 = (array[i] >> 0) & 0x0F;
		buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
		buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
	}
	buffer[len * 2] = '\0';
}

String FirebaseUDP::FirebaseUDP_Protocol(byte command, String payload) {
  int dataSize = payload.length() + 2 + 1; // Command bit & Fix bit & Chack sum
  
  byte data[255];
  data[0] = (dataSize >> 8) & 0xFF; // MSB
  data[1] = (dataSize) & 0xFF;  // LSB
  data[2] = command;
  data[3] = 0x1F;
  for (int i = 4; i < payload.length() + 4; i++) {
    data[i] = (byte)payload.charAt(i - 4);
  }
  data[payload.length() + 4] = 0x1B;

  char bufferString[dataSize * 2 + 8];
  array_to_string(data, dataSize + 2, bufferString);
  return String(bufferString);
}

#endif