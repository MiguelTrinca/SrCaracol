#/bin/sh
N_SCC=`shuf -i 1000-10000 -n1`
N_V=`shuf -i 10000-1000000 -n1`
N_E=$((`shuf -i 2-10 -n1` * $N_V))
#./gerador $N_V $N_E $N_SCC 1 1000 $SEED

# Constante de Proporcionalidade
difficulty=1223
pontos=$(($difficulty*$difficulty))
rotas=$difficulty
subredes=$difficulty/10
min=1
max=$difficulty
seed=1
TMP_FILE=out.massif
#./gerador $pontos $rotas $subredes $min $max $seed

test_speed(){
  pontos=$difficulty
  rotas=$(($difficulty*$difficulty))
  subredes=$difficulty/5
  min=1
  max=$difficulty
  seed=1
  \time --format="%U" ./gerador $pontos $rotas $subredes $min $max $seed | ./a.out > /dev/null
}

test_mem(){
  pontos=$(($difficulty*$difficulty))
  rotas=$difficulty
  subredes=$difficulty/10
  min=1
  max=$difficulty
  seed=1
  TMP_FILE=`mktemp`
  ./gerador $pontos $rotas $subredes $min $max $seed | valgrind --tool=massif --massif-out-file="$TMP_FILE" ./asa.out  >/dev/null 2>&1
  grep mem_heap_B "$TMP_FILE" | sed -e 's/mem_heap_B=\(.*\)/\1/' | sort -g | tail -n 1
}

difficulty=10
while [ $difficulty -le 3000 ]
do
  #echo -ne "$difficulty,"
  #test
  test_speed
  difficulty=$(( $difficulty + 100 ))
done
