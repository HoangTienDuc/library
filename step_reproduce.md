# Install FFMPEG
```
git clone https://github.com/HoangTienDuc/FFmpeg.git
cd FFmpeg
./configure --enable-nonfree --enable-pic --enable-shared
make -j 
make install
```

# Install DSL
```
git clone https://github.com/HoangTienDuc/library
cd library
make -j
make install
```

# Add LD path
```
export LD_LIBRARY_PATH=/usr/local/lib:/usr/local/lib64/:/usr/local/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
```
