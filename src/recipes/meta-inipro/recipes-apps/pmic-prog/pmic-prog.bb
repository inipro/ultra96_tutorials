#
# This is the pmic-prog application recipe
#
#

SUMMARY = "pmic-prog application"
SECTION = "PETALINUX/apps"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = "i2c-tools"

COMPATIBLE_MACHINE = "ultra96-zynqmp"

SRC_URI = "git://github.com/Avnet/BSP-rootfs-sources.git;protocol=https;branch=${SRCBRANCH};subpath=${SUBPATH};"

SRC_URI_append_ultra96-zynqmp = " file://pmic-configs/"

#SRCREV = "${AUTOREV}"
#SRCREV = "ce8fcca99540a07ed60400fc9b218d843d0de8b7"
SRCREV = "${@ "ce8fcca99540a07ed60400fc9b218d843d0de8b7" if bb.utils.to_boolean(d.getVar('BB_NO_NETWORK')) else d.getVar('AUTOREV') }"



SRCBRANCH ?= "master"
SUBPATH = "pmic-prog"
S = "${WORKDIR}/${SUBPATH}"

inherit pkgconfig cmake

FILES_${PN} += "${ROOT_HOME}/${SUBPATH}/*"

do_install() {
        install -d ${D}${ROOT_HOME}/${SUBPATH}
        install -m 0755 ${B}/pmic_prog ${D}${ROOT_HOME}/${SUBPATH}/
}

do_install_append_ultra96-zynqmp() {
        cp -r ${WORKDIR}/pmic-configs ${D}${ROOT_HOME}/${SUBPATH}/
}

