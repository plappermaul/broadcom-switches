# 
# $Id: swap.pl,v 1.3 Broadcom SDK $
#
# $Copyright: (c) 2016 Broadcom.
# Broadcom Proprietary and Confidential. All rights reserved.$
#
# swap.pl
# SWAP the 32 bit words in the file. 1234 -> 4321
#

    ($#ARGV == 1) ||
    die "Usage: $0 <unswapped-file> <swapped-file>\n" .
        "\tThis utility swaps the 32-bit words in <unswapped-file>\n" .
        "\tand writes them into <swapped-file>.\n";

    my $in_file=shift;
    my $out_file=shift;

    print STDOUT "Converting ", $in_file, " to  ", $out_file, " ...";

    $RECSIZE = 1; # size of record, in bytes
    $recno   = 0;  # which record to update
    open(FH, "<$in_file") || die "can't read $in_file : $!";
    open(FHO, ">$out_file") || die "can't write $out_file : $!";

    binmode FH;
    binmode FHO;

    while ( ! eof(FH) ) {
        if (read(FH, $byte0, $RECSIZE) != $RECSIZE) {
            $byte0 = "\000";
        }
        if (read(FH, $byte1, $RECSIZE) != $RECSIZE) {
            $byte1 = "\000";
        }
        if (read(FH, $byte2, $RECSIZE) != $RECSIZE) {
            $byte2 = "\000";
        }
        if (read(FH, $byte3, $RECSIZE) != $RECSIZE) {
            $byte3 = "\000";
        }
        print FHO $byte3, $byte2 ,$byte1 ,$byte0 ;
    }
    close FH;  
    close FHO;  

    print STDOUT "  done\n";
