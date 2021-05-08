g++ main.cpp -pg -o main.out
./main.out < tools/in/0000.txt > tools/out/0.txt
gprof ./main.out gmon.out -b > gmon.log