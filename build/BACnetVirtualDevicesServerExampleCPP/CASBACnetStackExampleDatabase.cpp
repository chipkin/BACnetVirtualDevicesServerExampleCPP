/*
 * BACnet Virtual Devices Server Example C++
 * ----------------------------------------------------------------------------
 * CASBACnetStackExampleDatabase.cpp
 *
 * Sets up virtual devices and in the database.
 *
 * Created by: Steven Smethurst
*/

#include "CASBACnetStackExampleDatabase.h"

#include <time.h> // time()
#ifdef _WIN32 
#include <winsock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#endif // _WIN32 

ExampleDatabase::ExampleDatabase() {
	this->Setup();
}

ExampleDatabase::~ExampleDatabase() {
	this->Setup();
}

const std::string ExampleDatabase::GetColorName() {
	static uint16_t offset = 0;
	static const std::vector<std::string> colors = {
	"Amber", "Bronze", "Chartreuse", "Diamond", "Emerald", "Fuchsia", "Gold", "Hot Pink", "Indigo",
	"Kiwi", "Lilac", "Magenta", "Nickel", "Onyx", "Purple", "Quartz", "Red", "Silver", "Turquoise",
	"Umber", "Vermilion", "White", "Xanadu", "Yellow", "Zebra White", "Apricot", "Blueberry", "Carrot", 
	"Date", "Eggplant", "Fig", "Grapefruit", "Honeydew"  };

	++offset;
	return colors.at(offset % colors.size());
}

void ExampleDatabase::Setup() {
	this->mainDevice.instance = 389999;
	this->mainDevice.objectName = "Virtual Devices Container";
	this->mainDevice.description = "Chipkin test BACnet IP Virtual Devices Server device";
	this->mainDevice.systemStatus = 0;	// operational (0), non-operational (4)

	for (size_t networkIndex = 0; networkIndex < NUMBER_OF_VIRTUAL_NETWORKS; networkIndex++) {
		for (size_t deviceIndex = 0; deviceIndex < NUMBER_OF_DEVICES_PER_NETWORK; deviceIndex++) {
			ExampleDatabaseDevice device;
			device.instance = STARTING_DEVICE_INSTANCE + (networkIndex * STARTING_DEVICE_INSTANCE) + deviceIndex;
			std::string color = ExampleDatabase::GetColorName();
			device.objectName = "Virtual Device " + color;
			device.description = "Example virtual device";
			device.systemStatus = 0;	// operational (0), non-operational (4)

			// Create the object
			ExampleDatabaseAnalogInput analogInput;
			analogInput.instance = 1;
			analogInput.presentValue = (networkIndex * 100) + deviceIndex + 1;
			analogInput.objectName = "Analog Input " + color;
			analogInput.reliability = 0;  // no-fault-detected (0), unreliable-other (7)
			this->analogInputs[device.instance] = analogInput;

			// Add the device
			uint16_t network = STARTING_VIRTUAL_NETWORK + (networkIndex * VIRTUAL_NETWORK_OFFSET);
			this->virtualDevices[network].push_back(device);
		}
	}

	this->networkPort.instance = 1;
	this->networkPort.objectName = "Network Port for Ipv4";
	this->LoadNetworkPortProperties();
}

void ExampleDatabase::LoadNetworkPortProperties() {

	// This function loads the Network port property values needed.
	// It uses system functions to get values like the IP Address and stores them
	// in the example database

	this->networkPort.BACnetIPUDPPort = 47808;


#ifdef _WIN32 
	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX | GAA_FLAG_INCLUDE_GATEWAYS;

	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	ULONG outBufLen = 15000;
	DWORD dwRetVal = 0;
	pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
	if (pAdapterInfo == NULL) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
		return;
	}

	// Make an initial call to GetAdaptersInfo to get
	// the necessary size into the ulOutBufLen variable
	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
		if (pAdapterInfo == NULL) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
			return;
		}
	}

	std::string selectedAdapterName;

	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			// If this is the Ethernet port, then extract the parameters
			if (pAdapter->Type == MIB_IF_TYPE_ETHERNET) {
				// Ethernet adapter
				// Extract the ethernet parameters needed for the Network Port Object
				// IP Address
				this->networkPort.IPAddressLength = sscanf_s(pAdapter->IpAddressList.IpAddress.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPAddress[0], &this->networkPort.IPAddress[1], &this->networkPort.IPAddress[2], &this->networkPort.IPAddress[3]);
				if (strcmp(pAdapter->IpAddressList.IpAddress.String, "0.0.0.0") == 0) {
					pAdapter = pAdapter->Next;
					continue;
				}
				// Default Gateway
				this->networkPort.IPDefaultGatewayLength = sscanf_s(pAdapter->GatewayList.IpAddress.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPDefaultGateway[0], &this->networkPort.IPDefaultGateway[1], &this->networkPort.IPDefaultGateway[2], &this->networkPort.IPDefaultGateway[3]);

				// Subnet Mask
				this->networkPort.IPSubnetMaskLength = sscanf_s(pAdapter->IpAddressList.IpMask.String, "%hhd.%hhd.%hhd.%hhd", &this->networkPort.IPSubnetMask[0], &this->networkPort.IPSubnetMask[1], &this->networkPort.IPSubnetMask[2], &this->networkPort.IPSubnetMask[3]);

				// Interface Name
				selectedAdapterName = std::string(pAdapter->AdapterName);

				// Prepare the broadcast address
				for (size_t i = 0; i < 4; i++) {
					this->networkPort.BroadcastIPAddress[i] = this->networkPort.IPAddress[i] | ~this->networkPort.IPSubnetMask[i];
				}

				break;
			}
			pAdapter = pAdapter->Next;
		}
	}

	if (pAdapterInfo) {
		FREE(pAdapterInfo);
	}

	// Get the address info, this is to get the DNS info
	pAddresses = (IP_ADAPTER_ADDRESSES*)MALLOC(outBufLen);
	if (pAddresses == NULL) {
		printf("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
		return;
	}

	if (GetAdaptersAddresses(AF_INET, flags, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW) {
		FREE(pAddresses);
		pAddresses = NULL;
		return;
	}

	PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
	IP_ADAPTER_DNS_SERVER_ADDRESS* pDnServer = NULL;
	while (pCurrAddresses && this->networkPort.IPDNSServers.size() <= 0) {
		std::string adapterName = std::string(pCurrAddresses->AdapterName);
		if (adapterName.compare(selectedAdapterName) == 0) {

			// Extract the DNS server details
			pDnServer = pCurrAddresses->FirstDnsServerAddress;
			while (pDnServer) {
				SOCKADDR* sockaddr = pDnServer->Address.lpSockaddr;
				if (sockaddr != NULL && sockaddr->sa_family == AF_INET) {
					SOCKADDR_IN* temp = (SOCKADDR_IN*)sockaddr;
					uint8_t* dns = new uint8_t[4];

					sscanf_s(inet_ntoa(temp->sin_addr), "%hhd.%hhd.%hhd.%hhd", &dns[0], &dns[1], &dns[2], &dns[3]);
					this->networkPort.IPDNSServers.push_back(dns);
					this->networkPort.IPDNSServerLength = 4;
				}
				pDnServer = pDnServer->Next;
			}

			break;
		}

		pCurrAddresses = pCurrAddresses->Next;
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

#endif // _WIN32 
}

void ExampleDatabase::Loop() {
}