#ifndef __FirebaseUDP_H__
#define __FirebaseUDP_H__

#include "Arduino.h"
#include "AIS_NB_BC95.h"

#define SERVER_IP "45.32.104.64"
#define SERVER_PORT 55567

class FirebaseUDP {
	public: 
		FirebaseUDP(AIS_NB_BC95 *aisObj) ;
		
		void begin(String token, unsigned long port = SERVER_PORT) ;
		void set(String path, String data) ;
	
	private:
		AIS_NB_BC95 *_aisObj;
		String _token;
		unsigned long _port;
		
		void array_to_string(byte array[], unsigned int len, char buffer[]) ;
		String FirebaseUDP_Protocol(byte command, String payload) ;
	
}
;

#endif