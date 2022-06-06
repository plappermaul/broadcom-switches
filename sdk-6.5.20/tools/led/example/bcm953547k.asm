;
; $Id: bcm953547k.asm, v 1.0 2017/08/17 15:30:14 leonlee Exp $
;
; This license is set out in https://raw.githubusercontent.com/Broadcom-Network-Switching-Software/OpenBCM/master/Legal/LICENSE file.
; 
; Copyright 2007-2020 Broadcom Inc. All rights reserved.
;
; This example is used for showing the serial EGPHY LEDs on
; BCM953547K.
;
; To start it, use the following commands from BCM:
;
;       led load bcm953547k.hex
;       led auto on
;       led start
;
; Each port needs to output 2 bits stream,
;   bit 0: LED_0
;   bit 1: LED_1
;
; Totally 24 ports need be outputed, i.e. 48 (= 24 * 2) bits.
; The output sequence for EGPHY will follow the user port sequence.
;
; The LED sequence is (User Port, Front Panel Order)
;   2, 3, ... 25
; Mapping onto physical port view, the sequence is:
;   13, 12, ..., 2, 18, 19, ..., 29
; The output order should be the inverted sequence of the physical
; mapping view
;   29, 28, ..., 18, 2, 3, ..., 13
;
; Link up/down info cannot be derived from LINKEN, as the LED
; processor does not always have access to link status.  This program
; assumes link status is kept current in bit 0 of RAM byte (0xA0 + portnum).
; Generally, a program running on the main CPU must update these
; locations on link change; see linkscan callback in
; $SDK/src/appl/diag/ledproc.c.
;

;
; Constants
;
NUM_PORTS   equ 24
;START_PORT_0 equ 13
;END_PORT_0 equ 2
;START_PORT_1 equ 18
;END_PORT_1 equ 29

START_PORT_0 equ 29
END_PORT_0 equ 18
START_PORT_1 equ 2
END_PORT_1 equ 13
;
; LED process
;

start_sec0:
    ld a, START_PORT_0
iter_sec0:
    port    a
    ld  (PORT_NUM), a
    call get_activity_hw

    ld  a, (PORT_NUM)
    port    a
    call get_link_hw

    ld  a, (PORT_NUM)
    dec a
    cmp a, END_PORT_0 - 1
    jnz iter_sec0

start_sec1:
    ld a, START_PORT_1
iter_sec1:
    port    a
    ld  (PORT_NUM), a
    call get_activity_hw

    ld  a, (PORT_NUM)
    port    a
    call get_link_hw

    ld  a, (PORT_NUM)
    inc a
    cmp a, END_PORT_1 + 1
    jnz iter_sec1

end:
    send    2*NUM_PORTS

;
; get_link_hw
;
;  This routine finds the link status LED for a port from HW.
;  Inputs: (PORT_NUM)
;  Outputs: Carry flag set if link is up, clear if link is down.
;  Destroys: a, b
get_link_hw:
    pushst LINKEN
    pop

    jc led_on
    jmp led_off

;
; get_activity_hw
;
;  This routine finds the link status LED for a port from HW.
;  Inputs: (PORT_NUM)
;  Outputs: Carry flag set if RX or TX is up, clear if link is down.
;  Destroys: a, b

get_activity_hw:
    pushst RX
    pushst TX
    tor
    pop

    jc led_on
    jmp led_off
;
; led_on
;
;  Outputs: Bits to the LED stream indicating ON
;
led_on:
    push 0
    pack
    ret

;
; led_off
;
;  Outputs: Bits to the LED stream indicating OFF
;
led_off:
    push 1
    pack
    ret

; Variables (SDK software initializes LED memory from 0xA0-0xff to 0)
PORTDATA    equ 0xA0
PORT_NUM    equ 0xE0

; Symbolic names for the bits of the port status fields
RX      equ 0x0 ; received packet
TX      equ 0x1 ; transmitted packet
COLL    equ 0x2 ; collision indicator
SPEED_C equ 0x3 ; 100 Mbps
SPEED_M equ 0x4 ; 1000 Mbps
DUPLEX  equ 0x5 ; half/full duplex
FLOW    equ 0x6 ; flow control capable
LINKUP  equ 0x7 ; link down/up status
LINKEN  equ 0x8 ; link disabled/enabled status
ZERO    equ 0xE ; always 0
ONE     equ 0xF ; always 1

