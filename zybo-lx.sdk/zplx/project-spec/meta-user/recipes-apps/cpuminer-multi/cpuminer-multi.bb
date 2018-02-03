#
# This is the GPIO-DEMO apllication recipe
#
#

SUMMARY = "cpuminer-multi autoconf application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://cpuminer-multi \
        "
S = "${WORKDIR}/cpuminer-multi"
CFLAGS_prepend = "-I ${S}/include" 
EXTRA_OECONF="--with-crypto --with-curl"
inherit autotools
