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
  - [ArchLinux Packaging](#archlinux-packaging)
- [Configuration](#configuration)
  - [Service](#service)
  - [Process management](#process-management)
- [Development](#development)

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

The project is written in C and comes with a bulk of predefined GNU Make
targets to speed up common development tasks. To get in touch just fork and
clone the project, install/build the required dependencies and fire a `make
build`.

The most adorable targets are `clean build test` (in this order :)). While
editing the man pages you can easily review the latest state with the `docs-*`
targets. (currently there are: `docs-avmctl.1 docs-avm-motion-trigger.conf.5
docs-avm-motion-triggerd.1`)

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
[Contribution Guide]: CONTRIBUTING.md
[.editorconfig file]: .editorconfig
