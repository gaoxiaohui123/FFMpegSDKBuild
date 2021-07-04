#!/bin/sh

#/usr/bin/ruby -e "$(curl -fsSL https://cdn.jsdelivr.net/gh/ineo6/homebrew-install/install)"
#git clone https://github.com/cisco/openh264.git
#git clone http://git.videolan.org/git/x264.git
#git clone git://source.ffmpeg.org/ffmpeg.git

#brew info sdl2
#brew install sdl2
#brew install sdl2_image
#brew install sdl2_ttf

#sudo ln -s /use/bin/yasm /use/bin/nasm
ROOT_PATH=`pwd`
FFMPEG_ROOT=`pwd`

echo 'input a number'
read Num
###############################
case $Num in
1)
  #nasm: fatal: unrecognised output format `macho64'
  #nasm -v
  #for openh264,workable version 2.10.06 or above
  #
  cd ./openh264-1.5.0
  make clean
  #make OS=linux ARCH=x86_64 SHARED= CFLAGS="-fPIC"
  #make OS=linux ARCH=x86_64 CFLAGS="-fPIC"
  #make SHARED= CFLAGS="-fPIC -fvisibility=hidden"
  make ARCH=x86_64 CFLAGS="-fPIC -fvisibility=hidden"
  #make ARCH=x86_32 CFLAGS="-fPIC -fvisibility=hidden"

  cp *.a ../mylib-mac/
  make clean
  #ldd libopenh264.so.6
  #根目录下openh264.def添加自定义函数
  #wels_enc_export.def
  cd ../
  ################################
;;
2)
  cd ./x264
  CFLAGS="-fPIC -fvisibility=hidden" ./configure --enable-pic --enable-static --disable-cli --disable-opencl
  make clean
  make -j 4
  cp *.a ../mylib-mac/
  make clean
  cd ../
  ###############################
;;
3)
  #brew reinstall libtool
  cd ./libaacplus-2.0.2-mac
  make clean
  #frontend/au_channel.h #define inline
  #注意：文件存储位置会影响以下编译命令的结果（外挂硬盘上可能会编译错误）
  #libtool on OS X is quite different from the gnu lib tool, which is called glib tool on OS X
  #注意：以下命令只能执行一次（autogen.sh被改写一次，否则改写导致错误内容）
  #sed -i '.bck' -e 's/libtool/glibtool/' autogen.sh
  ./autogen.sh --enable-static --disable-shared
  CFLAGS="-fPIC" ./configure --enable-static --disable-shared
  make
  cp src/.libs/*.a ../mylib-mac/
  make clean
  cd ../
;;
4)
  #mac下将文件/rtmpdump-2.4/librtmp/makefile中的-soname 改为 -dylib_install_name；
  cd ./rtmpdump-2.4
  make SHARED= CRYPTO= XDEF=-DNO_SSL CFLAGS="-fPIC -DRTMPDUMP_VERSION=2.4 -I$ROOT_PATH/openssl-1.0.2d/include"
  cp ./librtmp/*.a ../mylib-mac/
  make clean
  cd ../
;;
5)
  #buid yasm to replace nasm
  
  cd ffmpeg-4.3.2

  CFLAGS="-fPIC" ./configure --enable-pic --enable-postproc \
  --prefix=/usr/local \
  --target-os=darwin \
  --arch=x86_64 \
  --cpu=generic \
  --disable-doc \
  --disable-debug \
  --enable-asm \
  --enable-videotoolbox \
  --enable-hwaccel=h264_vda \
  --enable-gpl --enable-nonfree \
  --enable-libaacplus \
  --enable-librtmp \
  --enable-static \
  --enable-pthreads --enable-zlib \
  --enable-encoder=libx264 --enable-libx264 \
  --enable-encoder=libopenh264 --enable-libopenh264 --enable-muxer=h264 \
  --extra-cflags="-fpic -I$FFMPEG_ROOT/rtmpdump-2.4 -I$FFMPEG_ROOT/libaacplus-2.0.2-mac/include -I$FFMPEG_ROOT/x264 -I$FFMPEG_ROOT/openh264-1.5.0/include -I$FFMPEG_ROOT/zlib-1.2.8 -I$FFMPEG_ROOT/openssl-1.0.2d/include -I/usr/local/include" \
  --extra-ldflags="-L$FFMPEG_ROOT/mylib-mac -L/usr/local/lib" \
  --extra-libs="-laacplus -lx264 -lopenh264 -lcommon -lprocessing -lconsole_common -lz -ldl -lrtmp -lpthread -lstdc++ -lm"
  make clean
  make -j 4
  cp */*.a ../mylib-mac/
  make clean
  cd ../
;;
6)
  cd ../cJSON
  mkdir build
  cd build
  make clean
  #cmake .. -DENABLE_CJSON_UTILS=Off -DENABLE_CJSON_TEST=On -DCMAKE_INSTALL_PREFIX=/usr #（生成bin+lib）
  cmake .. -DENABLE_CJSON_UTILS=Off -DENABLE_CJSON_TEST=On -DCMAKE_INSTALL_PREFIX=/usr -DBUILD_SHARED_LIBS=Off #（生成bin）
  make 
  cp *.a ../../ffmpeg-linux/mylib-mac/
  make clean
  cd ../../ffmpeg-linux/
;;
7)
  cd ../openfec_v1.4.2
  rm -r build
  mkdir build
  cd build
  cmake .. -DDEBUG:STRING=OFF
  #make
  #cp ../bin/Release/*.a ../../ffmpeg-linux/mylib-mac/
  #make clean
  cd ../../ffmpeg-linux/
;;
8)
  #buid yasm to replace nasm
  
  cd ffmpeg-4.3.2
  #make clean
  make -j 4
  cp */*.a ../mylib-mac/
  #make clean
  cd ../
;;
esac

