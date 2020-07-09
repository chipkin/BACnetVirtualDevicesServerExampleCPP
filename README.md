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
CAS BACnet Stack Virtual Devices Server Example v0.0.2.0
https://github.com/chipkin/BACnetVirtualDevicesServerExampleCPP

FYI: Loading CAS BACnet Stack functions... OK
FYI: CAS BACnet Stack version: 3.17.0.0
FYI: Connecting UDP Resource to port=[47808]... OK, Connected to port
FYI: Registering the callback Functions with the CAS BACnet Stack
Setting up main server device. device.instance=[389999]
Created Device.
Enabling ReadPropertyMultiple... OK
Added NetworkPort. networkPort.instance=[1]... OK
Adding Virtual Devices and Objects...
Adding Virtual Device. device.instance=[100000] to network=[1000]...OK
OK
Adding Analog Input to Virtual Device. device.instance=[100000], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[100001] to network=[1000]...OK
OK
Adding Analog Input to Virtual Device. device.instance=[100001], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[100002] to network=[1000]...OK
OK
Adding Analog Input to Virtual Device. device.instance=[100002], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[100003] to network=[1000]...OK
OK
...
Adding Analog Input to Virtual Device. device.instance=[300008], analogInput.instance=[1]...OK
Adding Virtual Device. device.instance=[300009] to network=[3000]...OK
OK
Adding Analog Input to Virtual Device. device.instance=[300009], analogInput.instance=[1]...OK
FYI: Sending I-AM broadcast

FYI: Sending message to [192.168.1.255:47808] length [25]
<!-- CAS BACnet Stack v3.17.0.0 --><BACnetPacket networkType='IP'><BVLL function='originalBroadcastNPDU' /><NPDU control='0x20' version='1'><DestinationNetwork>65535</DestinationNetwork><DestinationAddress length='0' /><HopCount>255</HopCount></NPDU><UnconfirmedRequestPDU serviceChoice='iAm'><IAmRequest><IAmDeviceIdentifier datatype='12' objectInstance='389999' objectType='8'>device, 389999</IAmDeviceIdentifier><MaxAPDULengthAccepted datatype='2' value='1476'>1476</MaxAPDULengthAccepted><SegmentationSupported datatype='9' value='3'>noSegmentation</SegmentationSupported><VendorId datatype='2' value='389'>389</VendorId></IAmRequest></UnconfirmedRequestPDU></BACnetPacket>

FYI: Sending message to [192.168.1.255:47808] length [31]
<!-- CAS BACnet Stack v3.17.0.0 --><BACnetPacket networkType='IP'><BVLL function='originalBroadcastNPDU' /><NPDU control='0x28' version='1'><DestinationNetwork>65535</DestinationNetwork><DestinationAddress length='0' /><HopCount>254</HopCount><SourceNetwork>1000</SourceNetwork><SourceAddress length='3'>0x0186A0</SourceAddress></NPDU><UnconfirmedRequestPDU serviceChoice='iAm'><IAmRequest><IAmDeviceIdentifier datatype='12' objectInstance='100000' objectType='8'>device, 100000</IAmDeviceIdentifier><MaxAPDULengthAccepted datatype='2' value='1476'>1476</MaxAPDULengthAccepted><SegmentationSupported datatype='9' value='3'>noSegmentation</SegmentationSupported><VendorId datatype='2' value='389'>389</VendorId></IAmRequest></UnconfirmedRequestPDU></BACnetPacket>
```