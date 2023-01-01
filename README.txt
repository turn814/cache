Project Description
Overview
In computer assignment 2, we want to design a cache/memory controller. You are responsible for designing the memory controller. 

The input to your memory is a 32-bit address. Your code is responsible for loading the data from the memory (if it is an LW) or storing the value at the correct address if it is an SW. A skeleton code (memory_driver.cpp) is provided to you for initializing the variables, creating the cache object and main memory array, reading a trace, and passing the relevant information to your memory hierarchy. You are free to change this skeleton as you see fit!
Your memory hierarchy has two levels of cache and the main memory (i.e., the same array/vector you used in Project 1). 

Your L1 cache should be a direct-mapped cache with 1 block per line with a total of 16 lines/sets. Each block in our cache should be 4 bytes (i.e., an int). It is your responsibility to correctly calculate the index, block offset, and tag in your code. 

Your L2 cache should be an 8-way set-associative cache. Each block should be still 4 bytes and there should be 1 block per line (same as L1). There should be 16 sets for L2. Same as above, your code should correctly compute tag, index, and block offset.  Your main memory has 4096 lines each one 4 bytes. 

Since L2 is using the SA cache, you should use the LRU replacement policy (L1 is DM, so no replacement policy is needed).

Your cache design should be exclusive/write-no-allocate/write-through. You should assume that your cache is initially randomly initialized with all valid bits equal to zero.
Trace
Each line in a trace has four values (MemR, MemW, adr, data). The first two are either zero or one indicating whether this is a LOAD instruction or STORE (always only one of them will be equal to one). The address is a number between 0 and 4095 (you can safely assume this), and the data is a signed integer value. The code already read the trace line by line and stores the values in an array called myTrace. 

In the main loop of the driver code, each entry is read one-by-one and should be handled by your memory hierarchy. 

As mentioned before, you can assume that your cache and memory are initially empty (with random values) and we first store things before reading anything. 

We have provided one simple trace file, but we will add more soon. Same as CA1, your code will be tested with multiple different traces on Gradescope. We strongly recommend creating your own traces and testing your code using those.  
Design
The entry for your code is your memory controller (controller) which is called in a loop in the main function of memory_driver.cpp. For each item in the trace, this controller function should be called, and appropriate actions should be taken. Apart from these actions, you should also update stats in this controller. We strongly recommend adding more functions to your cache object for each action (e.g., search, update, evict, etc.)

The controller should first check whether this request is load or store and then follow an algorithm for each. For load, the controller should first search L1, then L2. If data is not found in either, data should be brought from memory (remember that there is no miss in main memory and the address provided in the trace is the index for the main memory array). 

Once data is found, you should make sure to (1) Update the stats for each cache, and (2) Update data, tag, and LRU positions if needed. 

If data is in L1, you just need to update the stats. If it is in L2, you should bring data to L1 (and remove it from L2) and update LRU, data, and tag. If it is in neither, data should be brought from memory and installed in L1 (update tag and data), the evicted data from L1 should be placed in L2 (should be put as most recently used), and evicted data from L2 should be removed. 

If it is a store, you should do similar things, but remember that we use a write-no-allocate write-through strategy (i.e., if data is in both cache and memory, both should be updated. If it is not in the cache, only memory should be updated, but update the stats for each level.) Remember that you need to search both caches for the store the same as the load, and you have to update the stats in both cases. 
Cache Driver Code and Benchmarks: 
The entry point of your project is Òmemory_driver.cppÓ. The program should run like this: 
      Ò./memory_driver <inputfile.txt>Ó, 

Your program should print the miss-rate for L1 and L2, as well as the average access time (AAT) of the system in the terminal. For AAT, you can assume that the hit time for L1 is 1 cycle, for L2 is 8 cycles, and for main memory is 100 cycles. You are free to add more stats in your code if needed. 

We will provide an additional trace file (or maybe two) with the ground truth. 

Same as the previous project, it is your choice how you want to structure your code (e.g., whether you want to have separate objects for each class, or you want to instantiate an object within another class, or even not use any class at all and utilize functions and structs, etc.). 


What to submit.
You need to submit the following files on Gradescope. If you are submitting the bonus part, there is a separate submission link for it. 
1. Your well-commented code (all the files Ð DO NOT put it under a folder). Note that we will use different traces to test your codeÕs correctness. Your code should be compiled with the following command: Òg++ *.cpp -o memory_driverÓ. If the code fails to compile, you will lose points. Your code should produce the results exactly as hard-coded in your skeleton code. Failing to create that format will result in losing points.

ECE M116C - CS M151B Computer Architecture Systems - UCLA
