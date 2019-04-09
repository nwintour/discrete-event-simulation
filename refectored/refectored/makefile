 all: DiscreteEventSimulation
 
 DiscreteEventSimulation: main.cpp
	clang++ -o DiscreteEvent main.cpp

test:
	./test.sh

run:
	python data.py

clean:
	rm -f DiscreteEvent
	rm -f test_data.txt