#!/usr/bin/awk -f

BEGIN {
    last = -1;
}

{
    # match pairs of lines by field 1 (loop counter)

    if ($1 != last) {
	cat[$2]++;
	tot++;
    }
    last = $1;
}

    # Our input file may be long - periodically print a progress 
    # message with statistics so far
NR % 200000 == 0 { 
    print "Num children = ", NR / 2;
    for (k in cat) 
 	printf "%-6s %6d %6.2f%%\n", k, cat[k], cat[k] / tot * 100;
}

END {
    print "All done";
    for (k in cat) 
 	printf "%-6s %6d %6.2f%%\n", k, cat[k], cat[k] / tot * 100;
}