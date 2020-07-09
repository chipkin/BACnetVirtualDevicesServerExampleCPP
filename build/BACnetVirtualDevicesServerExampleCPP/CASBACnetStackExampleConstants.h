/*
 * BACnet Virtual Devices Server Example C++
 * ----------------------------------------------------------------------------
 * CASBACnetStackExampleConstants.h
 *
 * This is a fully static class that contains all the constants used by this example.
 * This includes ObjectTypes, PropertyIdentifier, etc.
 * 
 * Created by: Steven Smethurst
*/

#ifndef __ExampleConstants_h__
#define __ExampleConstants_h__

#include "datatypes.h"

class CASBACnetStackExampleConstants {
public:

	// CAS BACnet Stack network type
	static const uint8_t NETWORK_TYPE_IP = 0;
	static const uint8_t NETWORK_TYPE_MSTP = 1;

	// General Constants
	static const uint32_t NETWORK_PORT_LOWEST_PROTOCOL_LAYER = 4194303;

	// Object Types
	static const uint16_t OBJECT_TYPE_ANALOG_INPUT = 0;
	static const uint16_t OBJECT_TYPE_DEVICE = 8;
	static const uint16_t OBJECT_TYPE_NETWORK_PORT = 56;
	
	// Property Identifiers
	static const uint32_t PROPERTY_IDENTIFIER_DESCRIPTION = 28;
	static const uint32_t PROPERTY_IDENTIFIER_OBJECT_NAME = 77;
	static const uint32_t PROPERTY_IDENTIFIER_PRESENT_VALUE = 85;
	static const uint32_t PROPERTY_IDENTIFIER_RELIABILITY = 103;
	static const uint32_t PROPERTY_IDENTIFIER_SYSTEM_STATUS = 112;

	// Network Port Property Identifiers
	static const uint32_t PROPERTY_IDENTIFIER_IP_ADDRESS = 400;
	static const uint32_t PROPERTY_IDENTIFIER_IP_DEFAULT_GATEWAY = 401;
	static const uint32_t PROPERTY_IDENTIFIER_IP_DNS_SERVER = 406;
	static const uint32_t PROPERTY_IDENTIFIER_IP_SUBNET_MASK = 411;
	static const uint32_t PROPERTY_IDENTIFIER_BACNET_IP_UDP_PORT = 412;
	static const uint32_t PROPERTY_IDENTIFIER_LINK_SPEED = 420;
	static const uint32_t PROPERTY_IDENTIFIER_MAC_ADDRESS = 423;

	// Services Supported
	static const uint8_t SERVICE_READ_PROPERTY_MULTIPLE = 14;
	
	// Network Type
	static const uint8_t NETWORK_TYPE_IPV4 = 5;

	// Protocol Level
	static const uint8_t PROTOCOL_LEVEL_BACNET_APPLICATION = 2;

	// Data types 
	static const uint32_t DATA_TYPE_NULL = 0;
	static const uint32_t DATA_TYPE_BOOLEAN = 1;
	static const uint32_t DATA_TYPE_UNSIGNED_INTEGER = 2;
	static const uint32_t DATA_TYPE_SIGNED_INTEGER = 3;
	static const uint32_t DATA_TYPE_REAL = 4;
	static const uint32_t DATA_TYPE_DOUBLE = 5;
	static const uint32_t DATA_TYPE_OCTET_STRING = 6;
	static const uint32_t DATA_TYPE_CHARACTER_STRING = 7;
	static const uint32_t DATA_TYPE_BIT_STRING = 8;
	static const uint32_t DATA_TYPE_ENUMERATED = 9;
	static const uint32_t DATA_TYPE_DATE = 10;
	static const uint32_t DATA_TYPE_TIME = 11;
	static const uint32_t DATA_TYPE_BACNET_OBJECT_IDENTIFIER = 12;
};

#endif // __ExampleConstants_h__
