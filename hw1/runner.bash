for (( i=0 ; $i<10 ; i=(($i+1)) ))
do
  gcc -o gfg -fopenmp parallel_block.c
  ./gfg
done