# Comperling 

g++ Convolution.cpp EdgeDetector.cpp CannyEdgeDetector.cpp main.cpp -o main.o `pkg-config --cflags --libs opencv4`

# Execute with sobel

./main.o lena.png 1

# Execute with prewitt

./main.o lena.png 2

# Execute with laplace

./main.o lena.png 3

# Execute with canny

./main.o lena.png 4