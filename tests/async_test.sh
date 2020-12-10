#!/bin/bash
DATA="./../tests/files"
prog="./a.out"

if [[ -x ${prog} ]]; then
    for ((p = 1; p <= 4; p++)) do 
    echo "########################### ${prog} p = $p ###########################"
    echo "=========================== a.txt   ==========================="
    timeout 1m ${prog} 4 $p 3 4 0 "${DATA}/a.txt" 
    echo "=========================== a20.txt ==========================="
    timeout 1m ${prog} 4 $p 3 4 0 "${DATA}/a20.txt" 
    echo "=========================== b.txt  ==========================="
    timeout 1m ${prog} 4 $p 3 4 0 "${DATA}/b.txt" 
    echo "=========================== c.txt  ==========================="
    timeout 1m ${prog} 6 $p 3 6 0 "${DATA}/c.txt" 
    echo "=========================== d.txt  ==========================="
    timeout 1m ${prog} 6 $p 3 6 0 "${DATA}/d.txt" 
    echo "=========================== e.txt  ==========================="
    timeout 1m ${prog} 6 $p 3 6 0 "${DATA}/e.txt" 
    echo "=========================== f.txt  ==========================="
    timeout 1m ${prog} 6 $p 3 6 0 "${DATA}/f.txt"
    for ((s = 1; s <= 4; s++));
        do for ((n = 3; n <= 30; n++))
            do for ((m = 3; m <= $n; m += 3))
                do echo "=========================== n = $n, m = $m, s = $s  ===========================" 
                   ${prog} $n $p $m 0 $s
                done
            done
        done   
    done
fi