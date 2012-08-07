#!/usr/bin/perl

use strict;
use warnings;

use constant {
	STATE_SPECIFY => 0,
	STATE_REG_RW  => 1,
	STATE_UC_RW   => 2,
	STATE_UC_R1   => 3,
	STATE_UC_R2   => 4,
};

use constant {
	FLAG_READ => 15,
};

my $state = STATE_SPECIFY;

foreach(<ARGV>) {
	chomp;
	my ($rw, $bytes) = split(/ \| /);
	my @bytes = split(/:/, $bytes);

	if ($state == STATE_SPECIFY) {
		@bytes = map { hex } @bytes;
		if (scalar(@bytes) == 12) {
			my $addr = $bytes[0] | ($bytes[1] << 8);
			my $page = $bytes[2] | ($bytes[3] << 8);
			my $dlen = $bytes[4] | ($bytes[5] << 8);
			my $flag = $bytes[6] | ($bytes[7] << 8);
			if ($addr == 0x4400 && $dlen == 4) {
				$state = STATE_UC_RW;
			}
			else {
				printf "\t{ %10s 0x%04x, 0x%04x, 0x",
					($flag & (1 << FLAG_READ)) ? 'REG_READ, ' : 'REG_WRITE,',
					$addr, $page;
				$state = STATE_REG_RW;
			}
		}
		else {
			print "\t// UNK\n";
		}
	}
	elsif ($state == STATE_REG_RW) {
		print join('', reverse(@bytes)) . " },\n";
		$state = STATE_SPECIFY;
	}
	elsif ($state == STATE_UC_RW) {
		@bytes = map { hex } @bytes;
		my $addr = $bytes[1] | ($bytes[2] << 8);
		printf "\t{ %10s 0x%04x,      0,       0x",
		       ($bytes[3] & 0x40 ? 'UC_READ,  ' : 'UC_WRITE, '), $addr;
		if ($bytes[3] & 0x80) {
			printf "%02x },\n", $bytes[0];
			$state = STATE_SPECIFY;
		}
		else {
			$state = STATE_UC_R1;
		}
	}
	elsif ($state == STATE_UC_R1) {
		$state = STATE_UC_R2;
	}
	elsif ($state == STATE_UC_R2) {
		@bytes = map { hex } @bytes;
		printf "%02x },\n", $bytes[0];
		$state = STATE_SPECIFY;
	}
}
