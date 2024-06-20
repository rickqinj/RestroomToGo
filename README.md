# RestroomToGo
## Introduction
There is only one washroom in Lincoln Appliance Service Centre office, and it is located in the middle of the building between the front shop and back shop. Although there is already a light hanging on the wall beside the washroom that is used to indicate the availability of the washroom, the light is not visible to all staff working in their own cubicles. Only some can see it directly. Now, via this project, all staff will be able to check the availability of the washroom from their computers and mobile phones if the devices have connected to the office's local network.

## Design
![Structure diagram](README.assets/44.png)

Notes: The "Reset button" in the above diagram is actually the button that is used to re-configure Wi-Fi connection.

## Hardware

1. Ai-Thinker NodeMCU-8266 v1.2 Development board x1
2. Micro push button x1

## Wiring Diagram

![20230921003825](README.assets/20230921003825.png)

GPIO-2 Built-in LED indicator (Mode: OUTPUT)

GPIO-4 Wi-Fi config button (Mode: PULLUP-INPUT)

GPIO-5 Washroom status sensor (Mode: PULLUP-INPUT)

## Application Information
mDNS: restroom
Access Link: http://restroom.local/
Wi-Fi cfg SSID: RestroomCfgAP

## Operation
### Configure Wi-Fi connection
1. When the gadget lost network connection, the built-in indicator will flash. Press the Wi-Fi config button will reset the network connection settings and activate the config AP "RestroomCfgAP". Use phone to connect to the AP and then go to the Wi-Fi connection configuration interface to config the Wi-Fi connection.
2. Or, just press directly the Wi-Fi config button, it will reset the current Wi-Fi settings and activate the "RestroomCfgAP" and give a chance to re-set the Wi-Fi connection.
### Check status
Open a browser on your device, and enter the link "http://resetroom.local/", you will see the current status. The webpage that shows you the status will refresh every 2 seconds.
