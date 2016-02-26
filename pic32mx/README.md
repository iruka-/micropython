# SUPPORT CHIP

* PIC32MX250 (128k Flash 32k SRAM w/USB IE)

* PIC32MX270 (256k Flash 64k SRAM w/USB IE)

* PIC32MX150 (128k Flash 32k SRAM )

* PIC32MX170 (256k Flash 64k SRAM )

# SCHEMATIC DIAGRAM


                   3.3V
                    |
                    *------10Ω--------------+
                   10k                       |
                    |       ___    ___       | 0.1u
      ラ   -->  ----*-MCLR [1  |__| 28] AVDD-*-||---GND
      イ   -->  --PGD3/RA0 [2       27] AVSS--------GND  LED
      ター -->  --PGC3/RA1 [3       26] RB15--1kΩ-------|＞|--GND
             ---HSYNC--RB0 [4       25] RB14
                     --RB1 [5       24] RB13--240Ω------------>Video OUT<---560Ω----RB0(HSYNC/VSYNC)
                     --RB2 [6       23] Vusb3v3--------3.3V
                  SCL2/RB3 [7       22] usb D-
       Xtal     GND----Vss [8       21] usb D+   +10uF
    +-----------------OSC1 [9       20] Vcap------||---GND
    *--|□|--*--------OSC2 [10      19] Vss------------GND
    |  8MHz  |    U1TX/RB4 [11      18] RB9
    22pF    22pF  U1RX/RA4 [12      17] RB8
    |        |   3.3v--Vdd [13      16] RB7
    |        |         RB5 [14      15] Vbus-----------USB Vbus(5V)
    GND    GND              ~~~~~~~~~~
                         PIC32MX250F128B


* Oneboard MicroComputer Board [ORANGE pico](http://www.picosoft.co.jp/orange/) based.


# ToDo

* UART1(U1TX/U1RX) Support.

* NTSC VideoOut Support.

* PS/2 Keyboard Input Support.

* and more...

# Build Instructions for PIC32MX

[INSTALL Pinguino-11 p32-gcc Toolchain](http://www.pinguino.cc/download.php)


    $ make
       .
       .
       .
    
    CC ../py/../extmod/vfs_fat_file.c
    CC ../py/../extmod/moduos_dupterm.c
    CC main.c
    CC syscalls.c
    p32-gcc -I../lib/netutils -I. -I.. -I/usr/share/pinguino-11/p32/include/non-free/ -Ibuild -march=m4k -EL -msoft-float  -c crt0.S -o build/crt0.o
    LINK build/firmware.elf
       text	   data	    bss	    dec	    hex	filename
      84300	   3656	   1492	  89448	  15d68	build/firmware.elf
    p32-objdump -d build/firmware.elf >main32.lss


* Enjoy!
