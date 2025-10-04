cd apps/q2
make clean
cd ../..
mainframer.sh 'cd os && make'
mainframer.sh 'cd apps/q2 && make'
cd apps/q2
rm out.txt
make run > out.txt
