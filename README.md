![avm-trigger-daemon](docs/assets/logo-project.png)

This project targets the tracking of a human motion to trigger an action on an
AVM Smart Home actor. When a motion is detected, a light sensor can be checked
for the current ambient light level and if a configured threshold is passed,
the configured actor will be turn on for example.  The `avm-motion-triggerd`
daemon can be configured to send user defined actions, like turn an actor on,
off or just toggle it.

- [About the project](#about-the-project)
- [About the setup](#about-the-setup)
- [Requirements](#requirements)
- [Installation](#installation)
  - [From Source](#from-source)
  - [ArchLinux Packaging](#archlinux-packaging)
- [Configuration](#configuration)
  - [Service](#service)
  - [Process management](#process-management)
- [Development](#development)

## About the project
## About the setup
## Requirements
## Installation
### From Source
### ArchLinux Packaging
## Configuration
### Service

The [avm-motion-triggerd][] daemon can be configured with a configuration file
which follows the format which is described by the [avm-motion-trigger.conf][]
man page.  On ArchLinux the system wide configuration can be found at
`/etc/avm-motion-trigger.conf`.

### Process management

The [avm-motion-triggerd][] daemon can be started in different flavors. It is
able to run in foreground by adding the `-f|--foreground` parameters or in
background (without additional parameters).  While the daemon is running in
foreground, all logging outputs will be printed to `stdout` and/or `stderr`.
The foreground logging is prefixed with the current date/time and the log level
for the message. While the daemon runs in background, all logging outputs will
be redirected to `syslog`.  For ArchLinux there is a systemd unit file shipped
for the daemon. The service can be enabled with: `sudo systemctl enable
avm-motion-triggerd.service`.

## Development

[avm-motion-triggerd]: ../../wikis/man/avm-motion-triggerd.1.html
[avm-motion-trigger.conf]: ../../wikis/man/avm-motion-trigger.conf.5.html
