# Compile

g++ Blur.cpp Convolution.cpp CannyEdge.cpp Laplace.cpp SobelPrewitt.cpp 1712195_1712329.cpp -o main.o `pkg-config --cflags --libs opencv4`
# Sobel

./main.o lena.jpg -sobel
# Prewitt

./main.o lena.jpg -prewitt
# Laplace

./main.o lena.jpg -laplace
# Canny

./main.o lena.jpg -canny
# Help

./main.o
