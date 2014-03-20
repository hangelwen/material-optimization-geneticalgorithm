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

my $numofile = @files;

my % result=();
for (my $i=5;$i<=100;$i=$i+5)
{
	$result{$i}=0;
}
foreach (@files)
{
    open (OPEN, $_);
    while(my $line=<OPEN>)
    {
	next if($line!~m/selectpower:/);
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
    my $outvalue = $key *($value-1);
    $outvalue = $outvalue/$numofile;
    $outvalue = $outvalue + 40;		
    print "$key $value $outvalue\n";
}


