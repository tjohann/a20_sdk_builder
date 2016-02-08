SDK - Builder
===============

A simple tool to handle all toolchain, image and device related topics for an embedded development environment (see https://github.com/tjohann/a20_sdk.git as an example).


Basic concepts
--------------

The basic idea behind the software development kit is to provide the same user interfaces for different devices/targets. There'fore i split it into different parts. First the git repository which is something like a runtime environment and second, another directory with the big binarys, which is something like a working directory.

So what are the general basic steps to setup a development environment and a device/target? First you need the images for your devices. Normally you have a *.img file wich you dd to your sd-card. The problem with that approach is that you need at least the exat size of sd-card the builder used. If your sd-card is bigger you need to resize it by hand, if it's smaller you have a problem.

I personaly use a different way which looks a little bit comlicated, but to my opinion it's more flexible -> provide the content of /boot, / and /home as tarballs. So my sd-card layout consist of a /boot partition (fat32) which holds the bootloader (U-Boot) and the kernel, a root partition which holds / and finaly a home partition for /home (and also a small swap partition). The main problem with that approach is, that you have to manually setup the partition table, and you have to extract every tarball to the right directory which you have to mount bevor. So it would make sense to automate this -> this is one goal of this tool. Below the menu "sd-card" (or target/device menu) you find the different tasks as gui items. So you don't have to think about it. Another task is to download all tarballs, which can also be done by this tool -> below menu "sd-card" or below the target (Bananapi or ...) menu.

The next step would be to install a cross-toolchain. There're different approaches based on your host and/or target distribution. One would be to use the packages of your distribution. The problem is that you only have a compiler and no addtional libs like libconfig or others. So my approach is to build a cross-toolchain with crosstool-ng (LINK EINFÜGEN) and additional libs with buildroot (LINK EINFÜGEN). This is to my opinion the most flexible way. As additions i can build host tools like mkimage or gdb independend of the host distribution.
For libs i use the sysroot mechanism of gcc -> the libs build with buildroot are rsync within the sysroot dir of the toolchain. So you have two tarballs which carry the content based on crosstool-ng (toolchain_x86_64.tgz) and buildroot (host_x86_64.tgz). There's also a menu called "SDK" for download and unpack.

The bracket around toolchain and images is the git repository. There're all configs for kernel, U-Boot and others. Also additional help is located in the git repository. One main content is the environment file (armhf_env). After sourcing it will provide some basic environment variables and additions to $PATH.


The git repository (runtime dir)
--------------------------------

Here're all configuration, scripts, environment files and all other text based parts. The repository has a makefile to control all important parts like download toolchain or clone external repositorys (see below). As an example see my a20_sdk.git repository on github.

This repo resides in /var/lib/..._sdk (/var/lib/a20_sdk). If you use the SDK-Builder then you can clone it based on an configuration file. This files are installed during package build (see $PREFIX/sdk_builder/*.conf).



The blob directory (working dir)
--------------------------------

The main goal of the dir (/opt/*_sdk) is to split the binrary from the configuration files. It is also independent of the git repository where all configuration changes will be done. If you init the working dir over the menu SDK/init it will copy a makefile based dir/subdir structure to it.

After download of the toolchain you have there a directory named toolchain and host (see above for description). The tarballs are checked for corruption via sha256 checksum (runtime_dir/checksum.sha256).

Through the menu entry "additions" you can clone useful repository to working_dir/external. Some examples are U-Boot, Allwinners documentation repo or the Linux tree. It's also possible to use the makefile within the dir for a non-gui usage (see working_dir/externel/README). With that you will only get the basic repositories, with the gui you can select all possible external repositries.

With sourcing the environment file (armhf_env) you set the latest supported kernel version (RT-PREEMPT and non-RT). Throught the menu entry "additions" you can download all needed files to build your own kernel. It will be copied to working_dir/kernel. It's also possible to use the makefile within the dir for a non-gui usage.

You also have all target image tarballs below working_dir/images. It behaviours similiar to kernel and external.



Workflow repository/toolchain
-----------------------------

Here's an example workflow based on the usage of my a20_sdk which supports Allwinners A20 based devices like Bananapi or Cubietruck. The configuration file for it is $PREFIX/sdk_builder/a20_sdk_builder.conf.

You can start the gui in different ways:

    a20_sdk_builder

which starts sdk_builder with the configuration a20_sdk_builder.conf.


This is equivalent to

    sdk_builder -f a20_sdk_builder.conf


or you start the sdk_builder whithout a configuration:

    sdk_builder


If you use the latest approach then you have to open a configuration file via menu File and new/open.

Next step is to clone the repository (runtime_dir) via button clone. Now you should see a progessbar and some info in the textfiled. After finish you can close the window and click the download button. It will download all needed files for toolchain and host (is supported). The progress of each file will be shown in the progressbar.  Hint: it will show the progress on ONE file and starts with 0% after finishing the first. You will also get some info about it in the textfield.

Now you have a working toolchain in /opt/*_sdk/ ... Hint: you can't click on clone and download anymore. The reason is simple -> you have both on your disk, so need to clone or download again (see Update below).


Update repository/toolchain
---------------------------

If you have already cloned the repository you can't click on the clone button again. So you have to use the update button. The thread will do a fetch on the git repository. It also checks if available tarballs below /opt/*_sdk have the same checksum like the one in /var/lib/*_sdk/checksum.sha256. If there're not the same it will activate the download button and you can update the tarballs.


Workflow sd-card
----------------

tbd.


Workflow device/target
----------------------
