cd apps/q4
make clean
cd ../..
mainframer.sh 'cd os && make'
mainframer.sh 'cd apps/q4 && make'
cd apps/q4
rm out.txt
make run > out.txt
