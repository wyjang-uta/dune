#!/bin/bash
NRUNS=100
for (( a=1; a <= NRUNS; a++ ))
do
  echo -e "#!/bin/bash\nWORKDIR=/cluster/home/wyjang/dune/sim/g4/DumpSim/build\n"'$WORKDIR'"/exampleB1 "'$WORKDIR'"/POT1000.mac $a" > "run${a}.sh"
done
echo "Finished!"
