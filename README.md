# LeakDetector

This is a basic C++ class for leak detection, a follow up to a question at https://codereview.stackexchange.com/questions/233625/leak-detection-simple-class

I recommend to use other well tested tools like valgrind and memory leak detection, but if for some reason you can't use these tools, you can use this code.

# Release mode
In release mode define "REL_MODE" in the header file, or just comment the code so the macros are void

# Note
This is not a well tested solution or a replacement for the already existent tools, this is just for me to track leaks while developing so I don't have to use valgrind every time I compile, because it slows things for me.
