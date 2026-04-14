# BACnet Virtual Devices Server Example C++
A basic BACnet Virtual Device Server Example written in C++ using the [CAS BACnet Stack](https://store.chipkin.com/services/stacks/bacnet-stack). Includes various sample BACnet virtual devices. For documentation, refer to CASBACnetStack - Virtual Devices.pdf in the docs folder.

## Releases

Build versions of this example can be downloaded from the [Releases](https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP/releases) page.

## Installation

Download the latest release zip on the [Releases](https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP/releases) page.

## Usage

Run the executable included in the zip file.

Each virtual device contains Analog_Input 1 and Network Port objects. Pre-configured with the following example BACnet device and virtual devices:
- **Device**: 389999 (Virtual Devices Container)
  - Virtual Device: 100000  (Virtual Device Bronze)
  - Virtual Device: 100001  (Virtual Device Chartreuse)
  - Virtual Device: 100002  (Virtual Device Diamond)
  - Virtual Device: 100003  (Virtual Device Emerald)
  - Virtual Device: 100004  (Virtual Device Fuchsia)
  - Virtual Device: 100005  (Virtual Device Gold)
  - Virtual Device: 100006  (Virtual Device Hot Pink)
  - Virtual Device: 100007  (Virtual Device Indigo)
  - Virtual Device: 100008  (Virtual Device Kiwi))
  - Virtual Device: 100009  (Virtual Device Lilac)
  - Virtual Device: 200000  (Virtual Device Magenta)
  - Virtual Device: 200001  (Virtual Device Nickel)
  - Virtual Device: 200002  (Virtual Device Onyx)
  - Virtual Device: 200003  (Virtual Device Purple)
  - Virtual Device: 200004  (Virtual Device Quartz)
  - Virtual Device: 200005  (Virtual Device Red)
  - Virtual Device: 200006  (Virtual Device Silver)
  - Virtual Device: 200007  (Virtual Device Turquoise)
  - Virtual Device: 200008  (Virtual Device Umber)
  - Virtual Device: 200009  (Virtual Device Vermillion)
  - Virtual Device: 300000  (Virtual Device White)
  - Virtual Device: 300001  (Virtual Device Xanadu)
  - Virtual Device: 300002  (Virtual Device Yellow)
  - Virtual Device: 300003  (Virtual Device Zebra White)
  - Virtual Device: 300004  (Virtual Device Apricot)
  - Virtual Device: 300005  (Virtual Device Blueberry)
  - Virtual Device: 300006  (Virtual Device Carrot)
  - Virtual Device: 300007  (Virtual Device Date)
  - Virtual Device: 300008  (Virtual Device Eggplant)
  - Virtual Device: 300009  (Virtual Device Fig)

The following keyboard commands can be issued in the server window:
* **h**: Display help menu
* **q**: Quit and exit the server

## Build

A [Visual studio 2019](https://visualstudio.microsoft.com/downloads/) project is included with this project. This project also auto built using [Gitlab CI](https://docs.gitlab.com/ee/ci/) on every commit.

The CAS BACnet Stack submodule is required for compilation.

## Example Output

```
CAS BACnet Stack Virtual Devices Server Example v0.0.6.0
https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP

FYI: Loading CAS BACnet Stack functions... OK
FYI: CAS BACnet Stack version: 4.6.0.0
FYI: Connecting UDP Resource to port=[47808]... OK, Connected to port
FYI: Registering the callback Functions with the CAS BACnet Stack
Setting up main server device. device.instance=[389999]
Created Device.
Enabling IAm... OK
Enabling ReadPropertyMultiple... OK
Enabling WriteProperty... OK
Enabling ReinitializeDevice... OK
Added NetworkPort. networkPort.instance=[1]... OK
Adding Virtual Devices and Objects...
Adding Virtual Device. device.instance=[100000] to network=[1000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[100000], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[100001] to network=[1000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[100001], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[100002] to network=[1000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[100002], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[200000] to network=[2000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[200000], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[200001] to network=[2000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[200001], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[200002] to network=[2000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[200002], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[300000] to network=[3000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[300000], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[300001] to network=[3000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[300001], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[300002] to network=[3000]...OK
Enabling IAm... OK
OK
Adding Analog Input to Virtual Device. device.instance=[300002], analogInput.instance=[1]...OK
FYI: Sending I-AM broadcast

FYI: Sending message to [192.168.2.255:47808] length [25]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [31]

FYI: Sending message to [192.168.2.255:47808] length [17]
FYI: Entering main loop...

FYI: Received message from [192.168.2.99:47808], length [25]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [31]

FYI: Received message from [192.168.2.99:47808], length [17]
::CASBACnetStack::BACnetNetworkLayer::ProcessNetworkLayerMessage() in file: X:\Work\Repos\BACnetVirtualDevicesServerExampleCPP\submodules\cas-bacnet-stack\source\BACnetNetworkLayer.cpp(339) - FYI: Received IAmRouterToNetwork, current implementation does not handle this type of network message, ignore
```