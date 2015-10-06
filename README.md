![avm-trigger-daemon](docs/assets/logo-project.png)

This project targets the tracking of a human motion to trigger an action on an
AVM Smart Home actor. When a motion is detected, a light sensor can be checked
for the current ambient light level and if a configured threshold is passed,
the configured actor will be turn on for example.  The `avm-motion-triggerd`
daemon can be configured to send user defined actions, like turn an actor on,
off or just toggle it.

- [About the setup](#about-the-setup)
- [Requirements](#requirements)
- [Installation](#installation)
  - [From Source](#from-source)
  - [Arch Linux Packaging](#arch-linux-packaging)
- [Configuration](#configuration)
  - [Service](#service)
  - [Process management](#process-management)
- [Development](#development)

## About the setup

The project runs productive on a [Raspberry Pi Model B rev2][] with [Arch Linux
ARM][] as distribution. For detecting human motions the [PIR Motion Sensor from
Seeed Studio][] was used.  The [Light Sensor Brick from ITead Studio][] was
used to detect the ambient light level. This sensor was attached to an
[MCP3008, 10 Bit / 8 Channel ADC][] which is connected via the SPI bus to the
Raspberry Pi. For further details consider the [Platform and Wiring][] wiki
page.

## Requirements

The project needs the following libraries to build:

* [glibc](https://www.gnu.org/software/libc/) (>=2.20)
* [openssl](https://www.openssl.org/) (>=1.0.1.l)
* [libxml2](http://www.xmlsoft.org/) (>=2.9.2)
* [libcurl](http://curl.haxx.se/libcurl/) (>=7.40.0)
* [libconfig](http://www.hyperrealm.com/libconfig/) (>=1.5)
* [libbcm2835](http://www.airspayce.com/mikem/bcm2835/) (>=1.46)

The versions behind the libraries are describing the lowest tested versions. Is it
possible that lower and/or higher versions will run fine, too.

The project was successfully compiled with the [GCC](https://gcc.gnu.org/) (>=4.9.2).

## Installation

### From Source

For all non-Arch Linux systems you can install the software the old way by
building it from source (`make build`) and fire a `make install`. The project
is not configured by autotools, but the makefiles makes use of the [standard
directory variables][]. Forthermore you can specify a `DESTDIR` variable for
packaging.  (`make DESTDIR=/tmp/xyz install`) If you are interessested in
changing the default directory variables consider the [Makefile.shared
file](Makefile.shared). They are change able the same way as `DESTDIR`. The
`uninstall` target takes care of the same variables and will cleanup your
system if you don't want the software to be installed.

:warning: This process expects you to build the software on the same
architecture as it will run on. For cross compiling consider a related guide
for your target architecture.

### Arch Linux Packaging

If you are interessested in building an Arch Linux package, all you have to do
is to clone this repository, install/build all the dependencies of the project
and run `makepkg` inside the `dist/archlinux` directory. Afterwards you will
find a fresh package (`avm-motion-trigger-*-*.pkg.tar.xz`) in the same
directory. This process underlies the same cross compiling restrictions as the
[From Source](#from-source) installation.

## Configuration

### Service

The [avm-motion-triggerd][] daemon can be configured with a configuration file
which follows the format which is described by the [avm-motion-trigger.conf][]
man page.  On Arch Linux the system wide configuration can be found at
`/etc/avm-motion-trigger.conf`.

### Process management

The [avm-motion-triggerd][] daemon can be started in different flavors. It is
able to run in foreground by adding the `-f|--foreground` parameters or in
background (without additional parameters).  While the daemon is running in
foreground, all logging outputs will be printed to `stdout` and/or `stderr`.
The foreground logging is prefixed with the current date/time and the log level
for the message. While the daemon runs in background, all logging outputs will
be redirected to `syslog`.  For Arch Linux there is a systemd unit file shipped
for the daemon. The service can be enabled with:
`sudo systemctl enable avm-motion-triggerd.service`.

## Development

The project is written in C and comes with a bulk of predefined GNU Make
targets to speed up common development tasks. To get in touch just fork and
clone the project, install/build the required dependencies and fire a `make build`.

The most adorable targets are `clean build test` (in this order :)). While
editing the man pages you can easily review the latest state with the `docs-*`
targets. (currently there are:
`docs-avmctl.1 docs-avm-motion-trigger.conf.5 docs-avm-motion-triggerd.1`)

The project comes with a whole bunch of simple test binaries which covers
single aspects of the project en bloc.  All test cases can be easily accessed
with their corresponding `test-*` and `test-mem-*` targets. The `test-mem-*`
variants will be checked for memory leaks by valgrind. Here is a list of the
currently available GNU Make targets:

```
test-ambient-light-level test-build-url test-config test-list-switches
test-logger test-login test-mem-ambient-light-level test-mem-build-url
test-mem-config test-mem-list-switches test-mem-logger test-mem-login
test-mem-password-challenge test-mem-password-response test-mem-pidfile
test-mem-pir-motion-detection test-mem-switch-off test-mem-switch-on
test-mem-switch-toggle test-password-challenge test-password-response
test-pidfile test-pir-motion-detection test-switch-off test-switch-on
test-switch-toggle
```

Please mind the [Contribution Guide][] and the given [.editorconfig file][] if
you plan to work on the project.

[avm-motion-triggerd]: ../../wikis/man/avm-motion-triggerd.1.html
[avm-motion-trigger.conf]: ../../wikis/man/avm-motion-trigger.conf.5.html
[Platform and Wiring]: ../../wikis/platform-and-wiring
[Contribution Guide]: CONTRIBUTING.md
[.editorconfig file]: .editorconfig
[standard directory variables]: https://www.gnu.org/prep/standards/html_node/Directory-Variables.html
[Raspberry Pi Model B rev2]: https://www.raspberrypi.org
[Arch Linux ARM]: http://archlinuxarm.org/
[Light Sensor Brick from ITead Studio]: http://wiki.iteadstudio.com/Light_Sensor_Brick
[PIR Motion Sensor from Seeed Studio]: http://www.seeedstudio.com/wiki/PIR_Motion_sensor_module
[MCP3008, 10 Bit / 8 Channel ADC]: http://www.adafruit.com/products/856
