#!/usr/bin/perl -w

use strict;
use File::Find;

my @files=();

sub filter{
if($_=~/\.txt$/i){
    push (@files,$_);
}
}

    open (OPEN, "final33.txt");
    while(my $line=<OPEN>)
    {
	next if($line!~m/best in generation:/);
	my @gen=split(/:/,$line);
	my $generation = $gen[1];
	chomp $generation;
	my $line2 = <OPEN>;
	my @fl = split(/\s+/,$line2);
	my @w = split(/:/,$fl[0]);
	my @lo = split(/:/,$fl[1]);
	my $fitness = $w[1];
	my $load = $lo[1];
	my $line3 = <OPEN>;
	my $line4 = <OPEN>;
	my @orien = split(/{/,$line3);
	my @orient = split(/,/,$orien[1]); 
	my $orvalue="";
	for (my $i=0;$i<20;$i++)
	{
	    $orvalue=$orvalue." ".$orient[$i];
	}
	my @m = split(/{/,$line4);
	my @mt = split(/,/,$m[1]); 
	my $mtvalue="";
	for (my $i=0;$i<20;$i++)
	{
	    $mtvalue=$mtvalue." ".$mt[$i];
	}
        print "$generation $fitness $load $orvalue $mtvalue\n";
    }
