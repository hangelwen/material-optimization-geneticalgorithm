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

my $popsize =50;
my $numofile = @files;

my % result=();
for (my $i=0;$i<=100;$i=$i+10)
{
	$result{$i}=0;
}
foreach (@files)
{
    open (OPEN, $_);
    while(my $line=<OPEN>)
    {
	next if($line!~m/crossover probability\(/);
	my @sp = split(/:/,$line);
	my $size = $sp[1];
	chomp $size;
	#print "pop size:$size";
	
	while(my $line2 = <OPEN>)
	{
	    next if($line2 !~m/best/);
	    my @gen = split(/:/,$line2);
	    my $generation = $gen[1];
	    chomp $generation;
	   # print "gen:$generation\n";
	    $result{$size}=$result{$size}+$generation;
	    last;
	}
    }
}

for my $key(keys %result)
{
    my $value = $result{$key};
    my $outvalue = ($value-$numofile )*28;
    $outvalue = $outvalue/$numofile;
    $outvalue = $outvalue + 40;		
    print "$key $outvalue\n";
}
