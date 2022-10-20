Rishab Das 


Errors: The errors that can happen is when their is not a valid size of memory, when it cannot allocate the bytes if memory, when the pointer that is given has a value of NULL, when the pointer was not allocated, when it is freed to many times.
I did this by having defragment which goes through the linked list and checks if nodes are not in use, multiple nodes. Then the flag varible is used and one gets added to show an error occurred.  
test4: This test runs a while loop for 120 times and allocates memory for five bytes five times in a loop. Once five time happens then it dealloctes the array that was created

test5: This test goes through one time by allocatign space twice in two different ways then free the same varible right after to test if the varible is freed after. 