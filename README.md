# Linux clear-projectc
A repository containing latest stable kernel patched with various patches to make it more perfomant.
Also contains a configurator that can patch vanilla kernels and help you compile them.

## Description
This project started as a hobby project for me to mantain my kernels better.
Currently the project is planned to have a supported, pre-patched version of the kernel that can be compiled and is
ready to take advantage of all the patches applied to it. The kernel source is stored in `./linux` and can be compiled normally.
It is **very important** to run `make oldconfig` before configuring it. 

Alternatively you can use the configurator at `./configurator/configurator` to help you compile the kernel.


## Pre-requisits
Standard dependencies to compile the kernel (varies in each distribution):
* `libncurses` 
* `gawk`
* `flex`
* `bison`
* `openssl`
* `libssl`
* `dkms`
* `libelf`
* `bc`

All the dependencies should be available in the distribution of your choice.

## Using the configurator (Requires root access)

The configurator is still WIP and is very rough around the edges, but it does work. 

If your kernel saves a config at `/proc/config.gz` you can use the configurator to use the current kernel's config using 
`./configurator -e`
**Note:** This completely ignores all other arguments and saves the config to be used by the upstream kernel on the repository.

You can use `./configurator -g /some/other/config.gz` to use a custom gzipped configuration. 

The `-i` allows you to use existing configuration files. Example: 
`./configurator -i /boot/config-5.19.10-stella`

The configurator also provides a `-o` option to override the path `-i` and `-g` store their configurations at. Thi can be used to
help compiling custom kernels. 
`./configurator -i /boot/config-5.19.10-stella -o /usr/src/linux/.config` 

`./configurator -g /proc/config.gz -o /usr/src/linux/.config` 


Although you can use `-o` the recommended way to patch and compile custom kernels is: 
`./configurator -k /path/to/vanilla/kernel -g /proc/config.gz` 

If you do not have a `/proc/config.gz` or a existing kernel configuration you can use, you may want to use a configuration provided by a distribution. 
The configurator currently suppots: 
* Arch Linux kernel configs 
* Gentoo distribution kernel configs 

This can be used using the `-a` flag.
`./configurator -a` or `./configurator -ak /path/to/custom/kernel`

Help about arguments can be always viewed using the `-h` flag.

