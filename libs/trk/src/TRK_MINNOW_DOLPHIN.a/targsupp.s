.include "macros.inc"
.file "targsupp.s"

.text
# US and PAL (BUILD_VERSION 1, 2): the TRK file stubs start on a 16-byte boundary; JP keeps 4.
.if BUILD_VERSION == 1 || BUILD_VERSION == 2
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
