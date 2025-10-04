cd apps/q5
make clean
cd ../..
mainframer.sh 'cd os && make'
mainframer.sh 'cd apps/q5 && make'
cd apps/q5
rm out.txt
make run > out.txt
