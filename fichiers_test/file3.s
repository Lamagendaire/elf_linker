	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 6
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"file3.c"
	.section	.rodata
	.align	2
.LC0:
	.ascii	"%d\012\000"
	.text
	.align	2
	.global	afficher
	.type	afficher, %function
afficher:
	@ args = 0, pretend = 0, frame = 16
	@ frame_needed = 1, uses_anonymous_args = 0
	stmfd	sp!, {fp, lr}
	add	fp, sp, #4
	sub	sp, sp, #16
	mov	r3, #100
	str	r3, [fp, #-8]
	mov	r3, #50
	str	r3, [fp, #-12]
	ldr	r3, [fp, #-12]
	mov	r2, r3, asl #1
	ldr	r3, [fp, #-8]
	add	r3, r2, r3
	str	r3, [fp, #-16]
	ldr	r0, .L2
	ldr	r1, [fp, #-16]
	bl	printf
	sub	sp, fp, #4
	@ sp needed
	ldmfd	sp!, {fp, pc}
.L3:
	.align	2
.L2:
	.word	.LC0
	.size	afficher, .-afficher
	.ident	"GCC: (15:4.9.3+svn231177-1) 4.9.3 20150529 (prerelease)"
