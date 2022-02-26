We changed the return type of function load_prog() in part2, where it calls the executable's entry point in part2, but now in part3, we return this entry point.
This enables the OS to get the entry point for 2 processes.

We didn't create any new file for part3. This is the only change we made to the load_prog() in part2.
