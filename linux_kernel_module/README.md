# Implementing a basic Linux Kernel Module Driver
Wrote a tutorial documenting how I did this on [Medium](https://medium.com/@hershjoshi/writing-your-first-linux-kernel-module-and-dealing-with-secure-boot-137194174aba)

## Notes
* To see where the Linux headers are installed look in `/usr/lib/modules/$(uname -r)/build/include/linux/`

## Resources
* [Medium - How to Write a Device Driver for Linux](https://jvgd.medium.com/how-to-write-a-device-driver-76d0584a4be3)
* [Stackoverflow Post on Having the build/bin files be generated in a seperate directory](https://stackoverflow.com/questions/12244979/build-kernel-module-into-a-specific-directory)
* Dealing with "insmod: ERROR: could not insert module myDriver.ko: Key was rejected by service" error [here](https://stackoverflow.com/questions/58546126/insmod-error-could-not-insert-module-helloworld-ko-operation-not-permitted) and [here](https://askubuntu.com/questions/762254/why-do-i-get-required-key-not-available-when-install-3rd-party-kernel-modules).
