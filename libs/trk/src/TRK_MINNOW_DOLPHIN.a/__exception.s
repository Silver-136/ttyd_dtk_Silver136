.include "macros.inc"
.file "__exception.s"

.section .init, "ax"
.balign 4

.global gTRKInterruptVectorTable
gTRKInterruptVectorTable:
	.asciz "Metrowerks Target Resident Kernel for PowerPC"
	.skip 0xD2
.L_rfi_0:
	b __TRK_reset
	.skip 0xFC
.L_rfi_1:
	mtsprg 1, r2
	mfsrr0 r2
	icbi r0, r2
	mfdar r2
	dcbi r0, r2
	mfsprg r2, 1
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x200
	rfi
	.skip 0xB4
.L_rfi_2:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x300
	rfi
	.skip 0x78
.L_gTRKInterruptVectorTable_500:
	.skip 0x54
.L_rfi_3:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x400
	rfi
	.skip 0xCC
.L_rfi_4:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x500
	rfi
	.skip 0xCC
.L_rfi_5:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x600
	rfi
	.skip 0xCC
.L_rfi_6:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x700
	rfi
	.skip 0xCC
.L_rfi_7:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x800
	rfi
	.skip 0xCC
.L_rfi_8:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x900
	rfi
	.skip 0x2CC
.L_rfi_9:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0xc00
	rfi
	.skip 0xCC
.L_rfi_10:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0xd00
	rfi
	.skip 0xCC
.L_rfi_11:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0xe00
	rfi
	.skip 0xCC
.L_rfi_12:
	b .L_rfi_13
	.skip 0x1C
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0xf20
	rfi
.L_rfi_13:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0xf00
	rfi
	.skip 0x78
.L_rfi_14:
	mtsprg 1, r2
	mfcr r2
	mtsprg 2, r2
	mfmsr r2
	andis. r2, r2, 0x2
	beq .L_80004184
	mfmsr r2
	xoris r2, r2, 0x2
	sync
	mtmsr r2
	sync
	mtsprg 1, r2
.L_80004184:
	mfsprg r2, 2
	mtcrf 255, r2
	mfsprg r2, 1
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1000
	rfi
	.skip 0x90
.L_rfi_15:
	mtsprg 1, r2
	mfcr r2
	mtsprg 2, r2
	mfmsr r2
	andis. r2, r2, 0x2
	beq .L_80004284
	mfmsr r2
	xoris r2, r2, 0x2
	sync
	mtmsr r2
	sync
	mtsprg 1, r2
.L_80004284:
	mfsprg r2, 2
	mtcrf 255, r2
	mfsprg r2, 1
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1100
	rfi
	.skip 0x90
.L_rfi_16:
	mtsprg 1, r2
	mfcr r2
	mtsprg 2, r2
	mfmsr r2
	andis. r2, r2, 0x2
	beq .L_80004384
	mfmsr r2
	xoris r2, r2, 0x2
	sync
	mtmsr r2
	sync
	mtsprg 1, r2
.L_80004384:
	mfsprg r2, 2
	mtcrf 255, r2
	mfsprg r2, 1
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1200
	rfi
	.skip 0x90
.L_rfi_17:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1300
	rfi
	.skip 0xCC
.L_rfi_18:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1400
	rfi
	.skip 0x1CC
.L_rfi_19:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1600
	rfi
	.skip 0xCC
.L_rfi_20:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1700
	rfi
	.skip 0x4CC
.L_rfi_21:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1c00
	rfi
	.skip 0xCC
.L_rfi_22:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1d00
	rfi
	.skip 0xCC
.L_rfi_23:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1e00
	rfi
	.skip 0xCC
.L_rfi_24:
	mtsprg 1, r2
	mtsprg 2, r3
	mtsprg 3, r4
	mfsrr0 r2
	mfsrr1 r4
	mfmsr r3
	ori r3, r3, 0x30
	mtsrr1 r3
	lis r3, TRKInterruptHandler@h
	ori r3, r3, TRKInterruptHandler@l
	mtsrr0 r3
	li r3, 0x1f00
	rfi

.global gTRKInterruptVectorTableEnd
gTRKInterruptVectorTableEnd:
