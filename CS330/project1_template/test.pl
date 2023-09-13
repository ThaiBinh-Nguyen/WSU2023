#!/usr/bin/env perl

use strict;
use warnings;
use utf8;

my $quiet = @ARGV > 0 && $ARGV[0] eq "q";

die "no myexp program exists!\n" unless -e "myexp";

my $n = 100;
my $epsilon = 1e-6;

my $good = 0;

my ($x0, $x1) = (-10, 10);
my $dx = ($x1 - $x0)/($n - 1);
my $x = $x0;
for (my $i = 0; $i < $n; $i++, $x += $dx) {
    my $truth = exp $x;
    my $result = `./myexp $x`;
    chomp($result);
    my $delta = abs($truth - $result);
    my $rerr = $delta/$truth;
    my $success = $rerr < $epsilon;
    $good++ if $success;
    unless ($quiet) {
	printf("exp(%0.7e)=%0.8e : %0.8e (rerror=%0.8e)", 
	       $x, $truth, $result, $rerr);;
	if ($success) {
	    print "\n";
	} else {
	    print "  : FAIL!\n";
	}
    }
}

print "PASSED $good / $n\n";
exit $n - $good; 
