# Comperling 

g++ Convolution.cpp EdgeDetector.cpp CannyEdgeDetector.cpp EdgeDetectorOpenCV.cpp main.cpp -o main.o `pkg-config --cflags --libs opencv4`

# Execute with sobel

./main.o image/lena.png 1 3 3

# Execute with prewitt

./main.o image/lena.png 2 3 3

# Execute with laplace

./main.o image/lena.png 3 3 3

# Execute with canny

./main.o image/lena.png 4 3 3