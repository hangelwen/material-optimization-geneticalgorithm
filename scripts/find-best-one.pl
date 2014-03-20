#!/usr/bin/perl -w

use strict;
use File::Find;

my @files=();

sub filter{
if($_=~/\.txt$/i){
    push (@files,$_);
}
}

find(\&filter, ".");
my % result=();
for(my $i=1;$i<=10000;$i++)
{
    $result{$i}=0;
}
foreach (@files)
{
    open (OPEN, $_);
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
	if($fitness<=1.28)
	{
	    $result{$generation}=$result{$generation}+1;
	}
    }
}

for my $key(keys %result)
{
    my $value = $result{$key};
    print "$key $value\n";
}


