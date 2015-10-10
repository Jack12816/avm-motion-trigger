## 0.2.0

* Implemented the ambient light sensor on the MCP3008 (SPI)
* Added new configuration parameter (`light_sensor_device`) to choose a Linux kernel SPI device

## 0.1.1

* Added buffer flushes to foreground logging, so systemd/journald should receive them before the process ends
* Reworked the ArchLinux build script (See #10)

## 0.1.0

* Add `avmctl` binary to interface the AVM Smart Home library
* Add `avm-motion-triggerd` daemon to watch for motions to handle
