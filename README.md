# zybo-lx
zybo linux 

This project is about creating a petalinux distribution that works from a vivado created hardware bitstream.

The create_project.tcl creates the vivado project. 

I assume it recreates the block design, and then you can generate a bitstream and export the hardware and open SDK.

the export is committed here in zybo-lx.sdk

cd into zybo-lx.sdk
create a petalinux project from the sdk directory (name it zybo-peta)

config peta linux
build petalinux

