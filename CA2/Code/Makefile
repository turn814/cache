default:
	g++ -Wall -Wextra -static-libstdc++ *.cpp -o memory_driver
clean:
	rm -f memory_driver
mini_debug: default
	./memory_driver ../Trace/mini_debug.txt
L2-test: default
	./memory_driver ../Trace/L2-test.txt