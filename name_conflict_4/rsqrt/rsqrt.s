	.file	"rsqrt.c"
	.intel_syntax noprefix
# GNU C17 (Gentoo 14.2.1_p20250419 p8) version 14.2.1 20250419 (x86_64-pc-linux-gnu)
#	compiled by GNU C version 14.2.1 20250419, GMP version 6.3.0, MPFR version 4.2.2, MPC version 1.3.1, isl version isl-0.27-GMP

# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -march=skylake -masm=intel -O3 -foffload-options=-fno-stack-protector
	.text
	.p2align 4
	.globl	rsqrt
	.type	rsqrt, @function
rsqrt:
.LFB0:
	.cfi_startproc
# rsqrt.c:9:   return 1/x;
	vmovss	xmm1, DWORD PTR .LC0[rip]	# tmp103,
# rsqrt.c:4:   asm (".intel_syntax;"
#APP
# 4 "rsqrt.c" 1
	.intel_syntax;vsqrtss xmm0,xmm0,xmm0	# x, tmp104
# 0 "" 2
# rsqrt.c:9:   return 1/x;
#NO_APP
	vdivss	xmm0, xmm1, xmm0	# _3, tmp103, x
# rsqrt.c:14: }
	ret	
	.cfi_endproc
.LFE0:
	.size	rsqrt, .-rsqrt
	.p2align 4
	.globl	q_rsqrt
	.type	q_rsqrt, @function
q_rsqrt:
.LFB1:
	.cfi_startproc
# rsqrt.c:18:   asm (".intel_syntax;"
#APP
# 18 "rsqrt.c" 1
	.intel_syntax;vrsqrtss xmm0,xmm0,xmm0	# x, tmp102
# 0 "" 2
# rsqrt.c:24: }
#NO_APP
	ret	
	.cfi_endproc
.LFE1:
	.size	q_rsqrt, .-q_rsqrt
	.section	.rodata.cst4,"aM",@progbits,4
	.align 4
.LC0:
	.long	1065353216
	.ident	"GCC: (Gentoo 14.2.1_p20250419 p8) 14.2.1 20250419"
	.section	.note.GNU-stack,"",@progbits
