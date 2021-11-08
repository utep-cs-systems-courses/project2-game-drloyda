	.arch msp430g2553
	.p2align 1,0

	.extern secondCount
	.extern ld_g
	.extern ld_r
LED_RED: 

	.text
	.global greenBeat


greenBeat:
	cmp #63, &secondCount
	jl less_eq_62
	jmp less_eq_125
	
less_eq_62:
	xor #ld_r, &P1OUT
less_eq_125:
	xor #ld_g, &P1OUT
out:
	pop r0

	
