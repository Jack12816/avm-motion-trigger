## 0.4.0

* The `avm-motion-triggerd` now does not die on failed CUrl requests anymore (See #13)
* The PIR motion sensor does not remember motions while timeouts anymore (See #12)
* All timeouts are now configurable (See #11)
  * Added a new configuration parameter: `too_bright_timeout`
  * Added a new configuration parameter: `backup_action_timeout`
  * Added a new configuration parameter: `failed_backup_action_timeout`
* Improved the man pages

## 0.3.1

* Changed the log level to NOTICE of the message when a motion was detected
* Fixed some logging output typos

## 0.3.0

* Fixed the broken logic for the ambient light sensor check / threshold
* When setting the ambient light sensor threshold to zero, will now disable the ambient light sensor

## 0.2.0

* Implemented the ambient light sensor on the MCP3008 (SPI)
* Added new configuration parameter (`light_sensor_device`) to choose a Linux kernel SPI device

## 0.1.1

* Added buffer flushes to foreground logging, so systemd/journald should receive them before the process ends
* Reworked the ArchLinux build script (See #10)

## 0.1.0

* Add `avmctl` binary to interface the AVM Smart Home library
* Add `avm-motion-triggerd` daemon to watch for motions to handle
