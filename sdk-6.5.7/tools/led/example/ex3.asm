; $Id: ex3.asm,v 1.2 Broadcom SDK $
;
; $Copyright: (c) 2016 Broadcom.
; Broadcom Proprietary and Confidential. All rights reserved.$
; 
;
;Here is an example program for programming the LEDs on the 5605.
;It is like example #2, but the scan out order is different.
;
;The situation is this:
;
;    26 ports (port 0-25) each have three LEDs.
;          led 0: RX
;          led 1: TX
;          led 2: link enabled
;
;    The scan out order is RX for ports 0 to 25, then TX for
;    ports 0 to 25, then link enable for ports 0 to 25.
;
;
;-------------------------- start of program --------------------------

begin:
	ld	B, RX
	call	portsub

	ld	B, TX
	call	portsub

	ld	B, COLL
	call	portsub

	send	78		; send 26*3 LED pulses and halt


portsub:
	ld	A,0		; start with port 0
	ld	(portnum),A

portloop:
	port	A		; specify which port we're dealing with
	pushst	B		; extract given status bit
	pack			; send to output buffer
	inc	(portnum)
	ld	A,(portnum)
	cmp	A,26
	jnz	portloop

	ret			; return from subroutine


; data storage
portnum		equ	0x7F	; temp to hold which port we're working on


; symbolic labels
; this gives symbolic names to the various bits of the port status fields

RX		equ	0x0	; received packet
TX		equ	0x1	; transmitted packet
COLL		equ	0x2	; collision indicator
SPEED_C		equ	0x3	;  100 Mbps
SPEED_M		equ	0x4	; 1000 Mbps
DUPLEX		equ	0x5	; half/full duplex
FLOW		equ	0x6	; flow control capable
LINKUP		equ	0x7	; link down/up status
LINKEN		equ	0x8	; link disabled/enabled status

;-------------------------- end of program --------------------------
;
;This program is 30 bytes in length and is only slightly more complicated
;than Example #2.
;
