# Raspberry Pi 3 Model B and B+ - Bare Metal

Contains examples about how to boot, configure the mailbox, uart port and
EL3 exceptions.

## Raspberry Pi 3

Firmware can be found [here](https://github.com/raspberrypi/firmware/tree/master/boot)
or you can get it from the directory `firmware` in this repo.

## How to configure the cross compiler

In order to use the GCC compiler from linaro, first we download it
and then install it under `/opt`.

```shell
mkdir /opt/arm/linaro/gcc
sudo wget http://releases.linaro.org/components/toolchain/binaries/7.2-2017.11/aarch64-elf/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf.tar.xz
tar -xvf gcc-linaro-7.2.1-2017.11-x86_64_aarch64-elf.tar.xz
```

# How to connect to RPi 3 using the UART

Minicom can be installed in linux to use the UART.
It is necessary to find in which device the RPi is enable
in Linux. You can find it by checking `/var/log/syslog` after
connecting the RPi.

```
minicom -b 115200 -D /dev/ttyUSB0
```

In Mac, you can use `screen`. The serial ports devices usually have
names like `/dev/cu.serial`, so check what name was associated to
the serial device.

```
screen /dev/cu.usbserial-14620 115200
```

