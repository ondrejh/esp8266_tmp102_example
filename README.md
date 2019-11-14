# esp8266_tmp102_example
The set of examples of web-enabled thermometer (esp8266, tmp102)

You can find there:
* how to get data from TMP102 i2c sensor
* how to set ESP8266 as an wifi access point
* how to make simple HTTP server running on ESP8266
* how to send json data
* how to send "big" file
* how to use standalone jquery on ESP8266

## tmp102_example
Test connection to TMP102. The output on UART. Very basics.

Resource: http://www.esp8266learning.com/tmp102-digital-sensor-and-esp8266-example.php

Schematic:

![the schematic from the resource page](/esp8266_TMP102.jpg)

## ap_http_example
The combination of AP (access point) firmware and simple HTTP server. Static IP, no password (open network).

## ap_http_tmp102
The example creates AP with static address "192.168.42.1", http server open.
The root address "/" returns the web page using jquery showing TMP102 temperature and the device up-time. Data are delivered on "/data" request in json form. The page also needs the jquery.min.js for its function. It is delivered on "/jquery.min.js" reques. The content of "jquery.min.js" file is provided by "jq.h" file, which is generated by bin2c.py program.

## my_bin2c
The simple program converting binary file into c code, which can be included into program and send by ESP8266WebServer.sendContent_P method.
To generate "jq.h" run "bin2c.py jquery.min.js jq.h".

# TODO
* Improve ap_http_tmp102 html styling
* Make a picture
* Provide more links
* Blink on sensor read (to show its doing something)
