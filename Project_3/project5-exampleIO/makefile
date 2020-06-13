NUM?=5
numbers=50 54 55 56 58 59 61 62 64
tests=project5-exampleIO

run:
	for number in $(numbers); \
	do \
		./lab$(NUM) $(tests)/in$$number.txt > $(tests)/out.txt ; \
		diff $(tests)/out.txt $(tests)/out$$number.txt || (echo "diff failed on test $$number") \
	done
	rm $(tests)/out.txt

compile:
	g++ -Wall -Werror -std=c++17 -g *.cpp -o lab$(NUM)
