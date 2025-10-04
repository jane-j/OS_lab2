cd apps/q3
make clean
cd ../..
mainframer.sh 'cd os && make'
mainframer.sh 'cd apps/q3 && make'
cd apps/q3
rm out.txt
make run > out.txt
