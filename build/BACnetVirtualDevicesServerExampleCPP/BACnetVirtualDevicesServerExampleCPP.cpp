/*
 * BACnet Virtual Devices Server Example C++
 * ----------------------------------------------------------------------------
 * BACnetVirtualDevicesServerExampleCPP.cpp
 *
 * In this CAS BACnet Stack example, we create a BACnet IP server with various
 * virtual devices.
 *
 * More information https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP
 *
 * This file contains the 'main' function. Program execution begins and ends there.
 *
 * Created by: Steven Smethurst
 */

#include "CASBACnetStackAdapter.h"
#include "CASBACnetStackExampleConstants.h"
#include "CASBACnetStackExampleDatabase.h"
#include "CIBuildVersion.h"

// Helpers
#include "SimpleUDP.h"
#include "ChipkinEndianness.h"
#include "ChipkinConvert.h"
#include "ChipkinUtilities.h"

#include <iostream>
#ifndef __GNUC__ // Windows
#include <conio.h> // _kbhit
#else // Linux 
#include <sys/ioctl.h>
#include <termios.h>
bool _kbhit() {
	termios term;
	tcgetattr(0, &term);
	termios term2 = term;
	term2.c_lflag &= ~ICANON;
	tcsetattr(0, TCSANOW, &term2);
	int byteswaiting;
	ioctl(0, FIONREAD, &byteswaiting);
	tcsetattr(0, TCSANOW, &term);
	return byteswaiting > 0;
}
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
void Sleep(int milliseconds) {
	usleep(milliseconds * 1000);
}

#endif // __GNUC__

// Globals
// =======================================
CSimpleUDP g_udp; // UDP resource
ExampleDatabase g_database; // The example database that stores current values.

// Constants
// =======================================
const std::string APPLICATION_VERSION = "0.0.2";  // See CHANGELOG.md for a full list of changes.
const uint32_t MAX_XML_RENDER_BUFFER_LENGTH = 1024 * 20;

// Callback Functions to Register to the DLL
// Message Functions
uint16_t CallbackReceiveMessage(uint8_t* message, const uint16_t maxMessageLength, uint8_t* receivedConnectionString, const uint8_t maxConnectionStringLength, uint8_t* receivedConnectionStringLength, uint8_t* networkType);
uint16_t CallbackSendMessage(const uint8_t* message, const uint16_t messageLength, const uint8_t* connectionString, const uint8_t connectionStringLength, const uint8_t networkType, bool broadcast);

// System Functions
time_t CallbackGetSystemTime();

// Get Property Functions
bool CallbackGetPropertyCharString(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount, uint8_t* encodingType, const bool useArrayIndex, const uint32_t propertyArrayIndex);
bool CallbackGetPropertyEnum(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, uint32_t* value, const bool useArrayIndex, const uint32_t propertyArrayIndex);
bool CallbackGetPropertyOctetString(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, uint8_t* value, uint32_t* valueElementCount, const uint32_t maxElementCount, const bool useArrayIndex, const uint32_t propertyArrayIndex);
bool CallbackGetPropertyReal(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, float* value, const bool useArrayIndex, const uint32_t propertyArrayIndex);
bool CallbackGetPropertyUInt(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, uint32_t* value, const bool useArrayIndex, const uint32_t propertyArrayIndex);

// Helper functions 
bool DoUserInput();
bool GetObjectName(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount);
bool GetDeviceDescription(const uint32_t deviceInstance, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount);

int main()
{
	// Print the application version information 
	std::cout << "CAS BACnet Stack Virtual Devices Server Example v" << APPLICATION_VERSION << "." << CIBUILDNUMBER << std::endl;
	std::cout << "https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP" << std::endl << std::endl;

	// 1. Load the CAS BACnet stack functions
	// ---------------------------------------------------------------------------
	std::cout << "FYI: Loading CAS BACnet Stack functions... ";
	if (!LoadBACnetFunctions()) {
		std::cerr << "Failed to load the functions from the DLL" << std::endl;
		return 0;
	}
	std::cout << "OK" << std::endl;
	std::cout << "FYI: CAS BACnet Stack version: " << fpGetAPIMajorVersion() << "." << fpGetAPIMinorVersion() << "." << fpGetAPIPatchVersion() << "." << fpGetAPIBuildVersion() << std::endl;

	// 2. Connect the UDP resource to the BACnet Port
	// ---------------------------------------------------------------------------
	std::cout << "FYI: Connecting UDP Resource to port=[" << g_database.networkPort.BACnetIPUDPPort << "]... ";
	if (!g_udp.Connect(g_database.networkPort.BACnetIPUDPPort)) {
		std::cerr << "Failed to connect to UDP Resource" << std::endl;
		std::cerr << "Press any key to exit the application..." << std::endl;
		(void)getchar();
		return -1;
	}
	std::cout << "OK, Connected to port" << std::endl;

	// 3. Setup the callbacks
	// ---------------------------------------------------------------------------
	std::cout << "FYI: Registering the callback Functions with the CAS BACnet Stack" << std::endl;

	// Message Callback Functions
	fpRegisterCallbackReceiveMessage(CallbackReceiveMessage);
	fpRegisterCallbackSendMessage(CallbackSendMessage);

	// System Time Callback Functions
	fpRegisterCallbackGetSystemTime(CallbackGetSystemTime);

	// Get Property Callback Functions
	fpRegisterCallbackGetPropertyCharacterString(CallbackGetPropertyCharString);
	fpRegisterCallbackGetPropertyEnumerated(CallbackGetPropertyEnum);
	fpRegisterCallbackGetPropertyOctetString(CallbackGetPropertyOctetString);
	fpRegisterCallbackGetPropertyReal(CallbackGetPropertyReal);
	fpRegisterCallbackGetPropertyUnsignedInteger(CallbackGetPropertyUInt);

	// 4. Setup the BACnet device
	// ---------------------------------------------------------------------------

	std::cout << "Setting up main server device. device.instance=[" << g_database.mainDevice.instance << "]" << std::endl;

	// Create the Main Device
	if (!fpAddDevice(g_database.mainDevice.instance)) {
		std::cerr << "Failed to add Device." << std::endl;
		return false;
	}
	std::cout << "Created Device." << std::endl;

	// Enable the services that this device supports
	// Some services are mandatory for BACnet devices and are already enabled.
	// These are: Read Property, Who Is, Who Has
	//
	// Any other services need to be enabled as below.

	std::cout << "Enabling ReadPropertyMultiple... ";
	if (!fpSetServiceEnabled(g_database.mainDevice.instance, CASBACnetStackExampleConstants::SERVICE_READ_PROPERTY_MULTIPLE, true)) {
		std::cerr << "Failed to enable the ReadPropertyMultiple" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;
	
	// Enable Optional Device Properties
	if (!fpSetPropertyEnabled(g_database.mainDevice.instance, CASBACnetStackExampleConstants::OBJECT_TYPE_DEVICE, g_database.mainDevice.instance, CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_DESCRIPTION, true)) {
		std::cerr << "Failed to enable the description property for the Main Device" << std::endl;
		return false;
	}

	// Add Main Device Objects
	// ---------------------------------------

	// Add the Network Port Object
	std::cout << "Added NetworkPort. networkPort.instance=[" << g_database.networkPort.instance << "]... ";
	if (!fpAddNetworkPortObject(g_database.mainDevice.instance, g_database.networkPort.instance, CASBACnetStackExampleConstants::NETWORK_TYPE_IPV4, CASBACnetStackExampleConstants::PROTOCOL_LEVEL_BACNET_APPLICATION, CASBACnetStackExampleConstants::NETWORK_PORT_LOWEST_PROTOCOL_LAYER)) {
		std::cerr << "Failed to add NetworkPort" << std::endl;
		return -1;
	}
	std::cout << "OK" << std::endl;

	// Add Virtual Devices and Objects
	std::cout << "Adding Virtual Devices and Objects..." << std::endl;
	std::map<uint16_t, std::vector<ExampleDatabaseDevice> >::iterator it;
	for (it = g_database.virtualDevices.begin(); it != g_database.virtualDevices.end(); ++it) {
		// Add the Virtual network
		if (!fpAddVirtualNetwork(g_database.mainDevice.instance, it->first, it->first * 10)) {
			std::cerr << "Failed to add virtual network " << it->first << std::endl;
			return -1;
		}

		std::vector<ExampleDatabaseDevice>::iterator devIt;
		for (devIt = it->second.begin(); devIt != it->second.end(); ++devIt) {
			// Add the Virtual Device
			std::cout << "Adding Virtual Device. device.instance=[" << devIt->instance << "] to network=[" << it->first << "]...";
			if (!fpAddDeviceToVirtualNetwork(devIt->instance, it->first)) {
				std::cerr << "Failed to add Virtual Device" << std::endl;
				return -1;
			}
			std::cout << "OK" << std::endl;

			// Enable Read Property Multiple
			if (!fpSetServiceEnabled(devIt->instance, CASBACnetStackExampleConstants::SERVICE_READ_PROPERTY_MULTIPLE, true)) {
				std::cerr << "Failed to enable the ReadPropertyMultiple" << std::endl;
				return -1;
			}
			std::cout << "OK" << std::endl;

			// Add the Analog Input to the Virtual Device
			std::cout << "Adding Analog Input to Virtual Device. device.instance=[" << devIt->instance << "], analogInput.instance=[" << g_database.analogInputs[devIt->instance].instance << "]...";
			if (!fpAddObject(devIt->instance, CASBACnetStackExampleConstants::OBJECT_TYPE_ANALOG_INPUT, g_database.analogInputs[devIt->instance].instance)) {
				std::cerr << "Failed to add AnalogInput" << std::endl;
				return -1;
			}
			std::cout << "OK" << std::endl;

			// Enable Reliability property 
			fpSetPropertyByObjectTypeEnabled(devIt->instance, CASBACnetStackExampleConstants::OBJECT_TYPE_ANALOG_INPUT, CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_RELIABILITY, true);
		}
	}

	// 5. Send I-Am of this device
	// ---------------------------------------------------------------------------
	// To be a good citizen on a BACnet network. We should announce  ourselves when we start up. 
	std::cout << "FYI: Sending I-AM broadcast" << std::endl;
	uint8_t connectionString[6]; //= { 0xC0, 0xA8, 0x01, 0xFF, 0xBA, 0xC0 };
	memcpy(connectionString, g_database.networkPort.BroadcastIPAddress, 4);
	connectionString[4] = g_database.networkPort.BACnetIPUDPPort / 256;
	connectionString[5] = g_database.networkPort.BACnetIPUDPPort % 256;

	// Send IAm for the Main Device
	if (!fpSendIAm(g_database.mainDevice.instance, connectionString, 6, CASBACnetStackExampleConstants::NETWORK_TYPE_IP, true, 65535, NULL, 0)) {
		std::cerr << "Unable to send IAm broadcast for mainDevice.instance=[" << g_database.mainDevice.instance << "]" << std::endl;
		return false;
	}

	// Send IAm for each virtual device
	for (it = g_database.virtualDevices.begin(); it != g_database.virtualDevices.end(); ++it) {
		std::vector<ExampleDatabaseDevice>::iterator devIt;
		for (devIt = it->second.begin(); devIt != it->second.end(); ++devIt) {
			if (!fpSendIAm(devIt->instance, connectionString, 6, CASBACnetStackExampleConstants::NETWORK_TYPE_IP, true, 65535, NULL, 0)) {
				std::cerr << "Unable to send IAm broadcast for virtualDevice.instance=[" << devIt->instance << "]" << std::endl;
				return false;
			}
		}
	}

	// Send IAmRouterToNetwork
	if (!fpSendIAmRouterToNetwork(connectionString, 6, CASBACnetStackExampleConstants::NETWORK_TYPE_IP, true, 65535, NULL, 0)) {
		std::cerr << "Unable to send IAmRouterToNetwork broadcast" << std::endl;
		return false;
	}

	// 6. Start the main loop
	// ---------------------------------------------------------------------------
	std::cout << "FYI: Entering main loop..." << std::endl;
	for (;;) {
		// Call the DLLs loop function which checks for messages and processes them.
		fpLoop();

		// Handle any user input.
		// Note: User input in this example is used for the following:
		//		h - Display options
		//		q - Quit
		if (!DoUserInput()) {
			// User press 'q' to quit the example application.
			break;
		}

		// Update values in the example database
		g_database.Loop();

		// Call Sleep to give some time back to the system
		Sleep(0); // Windows 
	}

	// All done. 
	return 0;
}

// Helper Functions

// Handle any user input.
// Note: User input in this example is used for the following:
//		h - Display options
//		q - Quit
bool DoUserInput()
{
	// Check to see if the user hit any key
	if (!_kbhit()) {
		// No keys have been hit
		return true;
	}

	// Extract the letter that the user hit and convert it to lower case
	char action = tolower(getchar());

	// Handle the action 
	switch (action) {
		// Quit
	case 'q': {
		return false;
	}
	case 'h':
	default: {
		// Print the Help
		std::cout << std::endl << std::endl;
		// Print the application version information 
		std::cout << "CAS BACnet Stack Virtual Devices Server Example v" << APPLICATION_VERSION << "." << CIBUILDNUMBER << std::endl;
		std::cout << "https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP" << std::endl << std::endl;

		std::cout << "Help:" << std::endl;
		std::cout << "h - (h)elp" << std::endl;
		std::cout << "q - (q)uit" << std::endl;
		std::cout << std::endl;
		break;
	}
	}

	return true;
}

// Callback used by the BACnet Stack to check if there is a message to process
uint16_t CallbackReceiveMessage(uint8_t* message, const uint16_t maxMessageLength, uint8_t* receivedConnectionString, const uint8_t maxConnectionStringLength, uint8_t* receivedConnectionStringLength, uint8_t* networkType)
{
	// Check parameters
	if (message == NULL || maxMessageLength == 0) {
		std::cerr << "Invalid input buffer" << std::endl;
		return 0;
	}
	if (receivedConnectionString == NULL || maxConnectionStringLength == 0) {
		std::cerr << "Invalid connection string buffer" << std::endl;
		return 0;
	}
	if (maxConnectionStringLength < 6) {
		std::cerr << "Not enough space for a UDP connection string" << std::endl;
		return 0;
	}

	char ipAddress[32];
	uint16_t port = 0;

	// Attempt to read bytes
	int bytesRead = g_udp.GetMessage(message, maxMessageLength, ipAddress, &port);
	if (bytesRead > 0) {
		ChipkinCommon::CEndianness::ToBigEndian(&port, sizeof(uint16_t));
		std::cout << std::endl << "FYI: Received message from [" << ipAddress << ":" << port << "], length [" << bytesRead << "]" << std::endl;

		// Convert the IP Address to the connection string
		if (!ChipkinCommon::ChipkinConvert::IPAddressToBytes(ipAddress, receivedConnectionString, maxConnectionStringLength)) {
			std::cerr << "Failed to convert the ip address into a connectionString" << std::endl;
			return 0;
		}
		receivedConnectionString[4] = port / 256;
		receivedConnectionString[5] = port % 256;

		*receivedConnectionStringLength = 6;
		*networkType = CASBACnetStackExampleConstants::NETWORK_TYPE_IP;

		// Process the message as XML
		static char xmlRenderBuffer[MAX_XML_RENDER_BUFFER_LENGTH];
		if (fpDecodeAsXML((char*)message, bytesRead, xmlRenderBuffer, MAX_XML_RENDER_BUFFER_LENGTH) > 0) {
			std::cout << xmlRenderBuffer << std::endl;
			memset(xmlRenderBuffer, 0, MAX_XML_RENDER_BUFFER_LENGTH);
		}
	}

	return bytesRead;
}

// Callback used by the BACnet Stack to send a BACnet message
uint16_t CallbackSendMessage(const uint8_t* message, const uint16_t messageLength, const uint8_t* connectionString, const uint8_t connectionStringLength, const uint8_t networkType, bool broadcast)
{
	if (message == NULL || messageLength == 0) {
		std::cout << "Nothing to send" << std::endl;
		return 0;
	}
	if (connectionString == NULL || connectionStringLength == 0) {
		std::cout << "No connection string" << std::endl;
		return 0;
	}

	// Verify Network Type
	if (networkType != CASBACnetStackExampleConstants::NETWORK_TYPE_IP) {
		std::cout << "Message for different network" << std::endl;
		return 0;
	}

	// Prepare the IP Address
	char ipAddress[32];
	if (broadcast) {
		snprintf(ipAddress, 32, "%u.%u.%u.%u",
			(connectionString[0] & g_database.networkPort.IPSubnetMask[0]) == 0 ? 255 : connectionString[0],
			(connectionString[1] & g_database.networkPort.IPSubnetMask[1]) == 0 ? 255 : connectionString[1],
			(connectionString[2] & g_database.networkPort.IPSubnetMask[2]) == 0 ? 255 : connectionString[2],
			(connectionString[3] & g_database.networkPort.IPSubnetMask[3]) == 0 ? 255 : connectionString[3]);
	}
	else {
		snprintf(ipAddress, 32, "%u.%u.%u.%u", connectionString[0], connectionString[1], connectionString[2], connectionString[3]);
	}

	// Get the port
	uint16_t port = 0;
	port += connectionString[4] * 256;
	port += connectionString[5];

	std::cout << std::endl << "FYI: Sending message to [" << ipAddress << ":" << port << "] length [" << messageLength << "]" << std::endl;

	// Send the message
	if (!g_udp.SendMessage(ipAddress, port, (unsigned char*)message, messageLength)) {
		std::cout << "Failed to send message" << std::endl;
		return 0;
	}

	// Get the XML rendered version of the just sent message
	static char xmlRenderBuffer[MAX_XML_RENDER_BUFFER_LENGTH];
	if (fpDecodeAsXML((char*)message, messageLength, xmlRenderBuffer, MAX_XML_RENDER_BUFFER_LENGTH) > 0) {
		std::cout << xmlRenderBuffer << std::endl;
		memset(xmlRenderBuffer, 0, MAX_XML_RENDER_BUFFER_LENGTH);
	}

	return messageLength;
}

// Callback used by the BACnet Stack to get the current time
time_t CallbackGetSystemTime()
{
	return time(0);
}

// Callback used by the BACnet Stack to get Character String property values from the user
bool CallbackGetPropertyCharString(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount, uint8_t* encodingType, const bool useArrayIndex, const uint32_t propertyArrayIndex)
{
	// Example of Object Name property
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_OBJECT_NAME) {
		return GetObjectName(deviceInstance, objectType, objectInstance, value, valueElementCount, maxElementCount);
	}
	// Example of Device Description
	else if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_DESCRIPTION && objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_DEVICE) {
		return GetDeviceDescription(deviceInstance, value, valueElementCount, maxElementCount);
	}
	return false;
}

// Callback used by the BACnet Stack to get Enumerated property values from the user
bool CallbackGetPropertyEnum(uint32_t deviceInstance, uint16_t objectType, uint32_t objectInstance, uint32_t propertyIdentifier, uint32_t* value, bool useArrayIndex, uint32_t propertyArrayIndex)
{
	// Example of Analog Inputs Reliability Property
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_RELIABILITY) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_ANALOG_INPUT) {
			if (g_database.analogInputs.count(deviceInstance) > 0 && g_database.analogInputs[deviceInstance].instance == objectInstance) {
				*value = g_database.analogInputs[deviceInstance].reliability;
				return true;
			}
			return false;
		}
	}

	// Example of System Status
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_SYSTEM_STATUS &&
		objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_DEVICE)
	{
		if (objectInstance == g_database.mainDevice.instance) {
			*value = g_database.mainDevice.systemStatus;
			return true;
		}
		else {
			std::map<uint16_t, std::vector<ExampleDatabaseDevice> >::iterator it;
			for (it = g_database.virtualDevices.begin(); it != g_database.virtualDevices.end(); ++it) {
				std::vector<ExampleDatabaseDevice>::iterator devIt;
				for (devIt = it->second.begin(); devIt != it->second.end(); ++devIt) {

					if (objectType == devIt->instance) {
						*value = devIt->systemStatus;
						return true;
					}
				}
			}
			return false;
		}
	}

	// We could not answer this request. 
	return false;
}

// Callback used by the BACnet Stack to get OctetString property values from the user
bool CallbackGetPropertyOctetString(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, const uint32_t propertyIdentifier, uint8_t* value, uint32_t* valueElementCount, const uint32_t maxElementCount, const bool useArrayIndex, const uint32_t propertyArrayIndex)
{
	// Example of Network Port Object IP Address property
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_IP_ADDRESS) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			memcpy(value, g_database.networkPort.IPAddress, g_database.networkPort.IPAddressLength);
			*valueElementCount = g_database.networkPort.IPAddressLength;
			return true;
		}
	}
	// Example of Network Port Object IP Default Gateway property
	else if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_IP_DEFAULT_GATEWAY) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			memcpy(value, g_database.networkPort.IPDefaultGateway, g_database.networkPort.IPDefaultGatewayLength);
			*valueElementCount = g_database.networkPort.IPDefaultGatewayLength;
			return true;
		}
	}
	// Example of Network Port Object IP Subnet Mask property
	else if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_IP_SUBNET_MASK) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			memcpy(value, g_database.networkPort.IPSubnetMask, g_database.networkPort.IPSubnetMaskLength);
			*valueElementCount = g_database.networkPort.IPSubnetMaskLength;
			return true;
		}
	}
	// Example of Network Port Object IP DNS Server property
	else if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_IP_DNS_SERVER) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			// The IP DNS Server property is an array of DNS Server addresses
			if (useArrayIndex) {
				if (propertyArrayIndex != 0 && propertyArrayIndex <= g_database.networkPort.IPDNSServers.size()) {
					memcpy(value, g_database.networkPort.IPDNSServers[propertyArrayIndex - 1], g_database.networkPort.IPDNSServerLength);
					*valueElementCount = g_database.networkPort.IPDNSServerLength;
					return true;
				}
			}
		}
	}
	return false;
}

// Callback used by the BACnet Stack to get Real property values from the user
bool CallbackGetPropertyReal(uint32_t deviceInstance, uint16_t objectType, uint32_t objectInstance, uint32_t propertyIdentifier, float* value, bool useArrayIndex, uint32_t propertyArrayIndex)
{
	// Example of Analog Input / Value Object Present Value property
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_PRESENT_VALUE) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_ANALOG_INPUT) {
			if (g_database.analogInputs.count(deviceInstance) > 0 && g_database.analogInputs[deviceInstance].instance == objectInstance) {
				*value = g_database.analogInputs[deviceInstance].presentValue;
				return true;
			}
			return false;
		}
	}

	return false;
}

// Callback used by the BACnet Stack to get Unsigned Integer property values from the user
bool CallbackGetPropertyUInt(uint32_t deviceInstance, uint16_t objectType, uint32_t objectInstance, uint32_t propertyIdentifier, uint32_t* value, bool useArrayIndex, uint32_t propertyArrayIndex)
{
	// Example of Network Port Object BACnet IP UDP Port property
	if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_BACNET_IP_UDP_PORT) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			*value = g_database.networkPort.BACnetIPUDPPort;
			return true;
		}
	}
	// Example of Network Port Object IP DNS Server Array Size property
	// Any properties that are an array must have an entry here for the array size.
	// The array size is provided only if the useArrayIndex parameter is set to true and the propertyArrayIndex is zero.
	else if (propertyIdentifier == CASBACnetStackExampleConstants::PROPERTY_IDENTIFIER_IP_DNS_SERVER) {
		if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance) {
			if (useArrayIndex && propertyArrayIndex == 0) {
				*value = (uint32_t)g_database.networkPort.IPDNSServers.size();
				return true;
			}
		}
	}
	
	return false;
}

// Gets the object name based on the provided parameters
bool GetObjectName(const uint32_t deviceInstance, const uint16_t objectType, const uint32_t objectInstance, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount)
{
	size_t stringSize = 0;
	if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_DEVICE && objectInstance == g_database.mainDevice.instance) {
		// Get the name of the main device
		stringSize = g_database.mainDevice.objectName.size();
		if (stringSize > maxElementCount) {
			std::cerr << "Error - not enough space to store full name of objectType=[" << objectType << "], objectInstance=[" << objectInstance << " ]" << std::endl;
			return false;
		}
		memcpy(value, g_database.mainDevice.objectName.c_str(), stringSize);
		*valueElementCount = (uint32_t)stringSize;
		return true;
	}
	else if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT && objectInstance == g_database.networkPort.instance && deviceInstance == g_database.mainDevice.instance) {
		// Get the name of the main ipv4 Network Port Object
		stringSize = g_database.networkPort.objectName.size();
		if (stringSize > maxElementCount) {
			std::cerr << "Error - not enough space to store full name of objectType=[" << objectType << "], objectInstance=[" << objectInstance << " ]" << std::endl;
			return false;
		}
		memcpy(value, g_database.networkPort.objectName.c_str(), stringSize);
		*valueElementCount = (uint32_t)stringSize;
		return true;
	}
	else if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_DEVICE) {
		// Get the name of a virtual device
		std::map<uint16_t, std::vector<ExampleDatabaseDevice> >::iterator it;
		for (it = g_database.virtualDevices.begin(); it != g_database.virtualDevices.end(); ++it) {
			std::vector<ExampleDatabaseDevice>::iterator devIt;
			for (devIt = it->second.begin(); devIt != it->second.end(); ++devIt) {
				if (objectInstance == devIt->instance) {
					stringSize = devIt->objectName.size();
					if (stringSize > maxElementCount) {
						std::cerr << "Error - not enough space to store full name of objectType=[" << objectType << "], objectInstance=[" << objectInstance << " ]" << std::endl;
						return false;
					}
					memcpy(value, devIt->objectName.c_str(), stringSize);
					*valueElementCount = (uint32_t)stringSize;
					return true;
				}
			}
		}
	}
	else if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_ANALOG_INPUT) {
		// Get the name of an analog input
		if (g_database.analogInputs.count(deviceInstance) > 0 && g_database.analogInputs[deviceInstance].instance == objectInstance) {
			stringSize = g_database.analogInputs[deviceInstance].objectName.size();
			if (stringSize > maxElementCount) {
				std::cerr << "Error - not enough space to store full name of objectType=[" << objectType << "], objectInstance=[" << objectInstance << " ]" << std::endl;
				return false;
			}
			memcpy(value, g_database.analogInputs[deviceInstance].objectName.c_str(), stringSize);
			*valueElementCount = (uint32_t)stringSize;
			return true;
		}
	}
	else if (objectType == CASBACnetStackExampleConstants::OBJECT_TYPE_NETWORK_PORT) {
		// Get the name of a virtual network port object
		if (deviceInstance == g_database.mainDevice.instance) {
			// Virtual network port object representing one of the virtual networks
			std::string name = "Network Port for virtual network " + ChipkinCommon::ChipkinConvert::ToString(objectInstance / 10);
			memcpy(value, name.c_str(), name.size());
			*valueElementCount = name.size();
			return true;
		}
		else {
			// Virtual network port object in the virtual device
			std::string name = "Network Port of virtual device " + ChipkinCommon::ChipkinConvert::ToString(deviceInstance);
			memcpy(value, name.c_str(), name.size());
			*valueElementCount = name.size();
			return true;
		}
	}

	return false;
}

bool GetDeviceDescription(const uint32_t deviceInstance, char* value, uint32_t* valueElementCount, const uint32_t maxElementCount) {
	size_t stringSize = 0;
	if (deviceInstance == g_database.mainDevice.instance) {
		stringSize = g_database.mainDevice.objectName.size();
		if (stringSize > maxElementCount) {
			std::cerr << "Error - not enough space to store full description for deviceInstance=[" << deviceInstance << " ]" << std::endl;
			return false;
		}
		memcpy(value, g_database.mainDevice.objectName.c_str(), stringSize);
		*valueElementCount = (uint32_t)stringSize;
		return true;
	}
	else {
		std::map<uint16_t, std::vector<ExampleDatabaseDevice> >::iterator it;
		for (it = g_database.virtualDevices.begin(); it != g_database.virtualDevices.end(); ++it) {
			std::vector<ExampleDatabaseDevice>::iterator devIt;
			for (devIt = it->second.begin(); devIt != it->second.end(); ++devIt) {
				if (deviceInstance == devIt->instance) {
					stringSize = devIt->objectName.size();
					if (stringSize > maxElementCount) {
						std::cerr << "Error - not enough space to store full description for deviceInstance=[" << deviceInstance << " ]" << std::endl;
						return false;
					}
					memcpy(value, devIt->objectName.c_str(), stringSize);
					*valueElementCount = (uint32_t)stringSize;
					return true;
				}
			}
		}
	}

	return false;
}
