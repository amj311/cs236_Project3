NUM?=3
numbers=30 33 35 36 37 41 42 43 44 61 62
tests=exampleIO

run:
	for number in $(numbers); \
	do \
		./lab$(NUM) $(tests)/in$$number.txt > $(tests)/out.txt ; \
		diff $(tests)/out.txt $(tests)/out$$number.txt || (echo "diff failed on test $$number") \
	done
	rm $(tests)/out.txt

compile:
	g++ -Wall -Werror -std=c++17 -g *.cpp -o lab$(NUM)
