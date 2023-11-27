#!/usr/bin/env perl

use strict;
use warnings;
use utf8;
use Data::Dumper;
use Test::More tests => 1 + 5*4;

my %trapezoid_epsilons = (
	12     => 1e-02,
	192    => 1e-05,
	3072   => 1e-07,
	49152  => 1e-10,
	786432 => 1e-11,
);

my %simpsons3_epsilons = (
	12     => 1e-06,
	192    => 1e-14,
	3072   => 1e-13,
	49152  => 1e-11,
	786432 => 1e-10,
);

my %simpsons38_epsilons = (
	12     => 1e-06,
	192    => 1e-13,
	3072   => 1e-13,
	49152  => 1e-13,
	786432 => 1e-12,
);

my %boole_epsilons = (
	12     => 1e-06,
	192    => 1e-13,
	3072   => 1e-13,
	49152  => 1e-12,
	786432 => 1e-10,
);

my $re = qr/
	(\d+)
	\s+(?:[+-])?(nan|inf|\d+(?:\.\d+)(?:e[+-]\d+)?)
	\s+(?:[+-])?(nan|inf|\d+(?:\.\d+)(?:e[+-]\d+)?)
	\s+(?:[+-])?(nan|inf|\d+(?:\.\d+)(?:e[+-]\d+)?)
	\s+(?:[+-])?(nan|inf|\d+(?:\.\d+)(?:e[+-]\d+)?)
/x;

my %trapezoid_errors;
my %simpsons3_errors;
my %simpsons38_errors;
my %boole_errors;

open (my $fh, "-|", "./pi") or die "$!\n";
while (<$fh>) {
	if (/$re/) {
		my ($n, $trapezoid, $simpsons3, $simpsons38, $boole) = ($1, $2, $3, $4, $5);
		$trapezoid_errors{$n} = $trapezoid;
		$simpsons3_errors{$n} = $simpsons3;
		$simpsons38_errors{$n} = $simpsons38;
		$boole_errors{$n} = $boole;
	}
}
close $fh;
ok(!$?, "pi didn't crash");

my @integrators = (
	["trapezoid", \%trapezoid_epsilons, \%trapezoid_errors],
	["simpsons3", \%simpsons3_epsilons, \%simpsons3_errors],
	["simpsons38", \%simpsons38_epsilons, \%simpsons38_errors],
	["boole", \%boole_epsilons, \%boole_errors],
);

for my $i (0 .. $#integrators) {
	my ($integrator, $epsilons, $errors) = @{$integrators[$i]};

	for (my $n = 12; $n < 800000; $n *= 16) {
		my $epsilon = $epsilons->{$n};
		my $error = $errors->{$n};

		if (!defined($error)) {
			fail("$integrator(n=$n) error < $epsilon - did we even get a test at n=$n?)");
			next;
		}

		cmp_ok($error, "<", $epsilon, "$integrator(n=$n) error < $epsilon");
	}
}
