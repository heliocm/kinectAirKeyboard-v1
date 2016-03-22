// kinect_linux


# sudo apt-get install libxtst-dev
# sudo apt-get install cmake-curses-gui
# sudo apt-get install cmake-gui
# sudo apt-get install libusb-dev
# sudo apt-get install libusb-1.0-0-dev
# dpkg -s libusb-dev

# freenetic depencencies
# sudo apt-get install git-core cmake freeglut3-dev pkg-config build-essential libxmu-dev libxi-dev libusb-1.0-0-dev

# compiling freenetic lib tips
# git clone git://github.com/OpenKinect/libfreenect.git
# cd /home/sbvb/Dropbox/guidance/2013/cpp/2013_2/KinectWorks/freenetic/libfreenect
# mkdir build
# cd build
# cmake ..
#### mv ../Makefile .
# make
# sudo make install





cd /home/sbvb/Dropbox/guidance/kinect_linux
# git clone git://github.com/OpenKinect/libfreenect.git
# cd libfreenect
# mkdir build
# cd build
# cmake ..
# make
# sudo make install


/home/sbvb/Dropbox/guidance/kinect_linux

// triangulo, cliente do kinectic
g++ Triangle.cpp -lglut -lGL -lGLU -o Opengltriangle

// kinect control (so version)
g++ 4buton.cpp -I ./libfreenect/include -lncurses -lfreenect -lX11 -lXtst -lglut -lGL -lGLU -lpthread -o kinectControl

// kinect control (static library version)
g++ 4buton.cpp -I /home/sbvb/Dropbox/guidance/2013/cpp/2013_2/KinectWorks/freenetic/libfreenect/include -lncurses /home/sbvb/Dropbox/guidance/2013/cpp/2013_2/KinectWorks/freenetic/libfreenect/lib/libfreenect.a -lX11 -lXtst -lglut -lGL -lGLU -lpthread -o kinectControl



sudo ldconfig /home/sbvb/Dropbox/guidance/2013/cpp/2013_2/KinectWorks/freenetic/libfreenect/lib
sudo ldconfig /home/sbvb/Dropbox/guidance/kinect_linux/libfreenect/build/lib



// somente executa com o kinect plugado
sudo ./Opengltriangle &
sudo ./kinectControl



/home/sbvb/Dropbox/guidance/2013/cpp/2013_2/KinectWorks/freenetic/libfreenect
/usr/local/include/libfreenect



