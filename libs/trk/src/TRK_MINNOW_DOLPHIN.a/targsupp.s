.include "macros.inc"
.file "targsupp.s"

.text
# US, PAL, the June US demo and both September demos: the TRK file stubs start on a 16-byte boundary;
# JP and the JP demo (BUILD_VERSION 2, 1; the order of include/version.h) keep 4.
.if BUILD_VERSION != 1 && BUILD_VERSION != 2
.balign 16
.else
.balign 4
.endif

.fn TRKAccessFile, global
	twui r0, 0x0
	blr
.endfn TRKAccessFile

.fn TRKOpenFile, global
	twui r0, 0x0
	blr
.endfn TRKOpenFile

.fn TRKCloseFile, global
	twui r0, 0x0
	blr
.endfn TRKCloseFile

.fn TRKPositionFile, global
	twui r0, 0x0
	blr
.endfn TRKPositionFile
