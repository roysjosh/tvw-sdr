#!/bin/sh

SUBCMD='printf "/{ REG_/ s/0x%04x, 0x2000,/TVW_\1, 0x2000,/" "0x\2"'
sed -re "s|.*'mm([[:alnum:]_]*)', ([0-9A-F]*)h?, 0.*|$SUBCMD|e" tvwreg.txt | grep -v rdata > tvwreg.sed
SUBCMD='printf "#define %-47s 0x%04x" "TVW_\1" "0x\2"'
sed -re "s|.*'mm([[:alnum:]_]*)', ([0-9A-F]*)h?, 0.*|$SUBCMD|e" tvwreg.txt | grep -v rdata > tvwreg.h
