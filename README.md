# Conection MQTT, Sensor, etc.

!!!! Important

MQTT_WIFI.ino that is code how to send in mqtt throught QoS 2 connection. So that library not pubsub clinet so this is some library that used in MQTT_WIFI.ino if you dont find that library in the arduino you might install it manually :

1. WiFi : a fundamental library that have been installed in the module so we don't need to install anymore
2. AsyncMqttClient : a library that use to organize communication asynchronous MQTT if in arduino library not that library you can install manually from github [AsyncMqttClient Library](https://github.com/marvinroger/async-mqtt-client)
3.  AsyncTCP : a library that dependency of AsyncMqttClient [AsyncTCP Library](https://github.com/me-no-dev/AsyncTCP)
4. FreeRTOS : a fundamental library that have been installed in the module so we don't need to install anymore

