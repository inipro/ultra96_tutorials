FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://user.cfg \
${@bb.utils.contains('ULTRA96_VERSION', '2', 'file://0001-Patch-the-kernel-to-manipulate-the-wifi-part-s-pins.patch', '', d)} \
file://0002-ov5640-revert-to-2019.2.patch \
file://0003-Add-support-Infineon-IR38060-IR38062-IR38063.patch \
"

