# Automatically generated by configure - do not modify!
shared=
build_suffix=
prefix=/usr/local
libdir=${prefix}/lib
incdir=${prefix}/include
rpath=
source_path=.
LIBPREF=lib
LIBSUF=.a
extralibs_avutil="-pthread -lm -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices"
extralibs_avcodec="-liconv -lm -llzma -L/usr/local/lib -lz -framework AudioToolbox -L/usr/local/Cellar/x264/r2555/lib -lx264 -pthread -framework VideoToolbox -framework CoreFoundation -framework CoreMedia -framework CoreVideo -framework CoreServices"
extralibs_avformat="-lm -lbz2 -L/usr/local/lib -lz -Wl,-framework,CoreFoundation -Wl,-framework,Security"
extralibs_avdevice="-framework Foundation -lm -framework AVFoundation -framework CoreVideo -framework CoreMedia -pthread -framework CoreGraphics -L/usr/local/lib -lSDL2"
extralibs_avfilter="-framework OpenGL -framework OpenGL -pthread -lm -framework CoreImage -framework AppKit"
extralibs_avresample="-lm"
extralibs_postproc="-lm"
extralibs_swscale="-lm"
extralibs_swresample="-lm"
avdevice_deps="avfilter swscale postproc avformat avcodec swresample avutil"
avfilter_deps="swscale postproc avformat avcodec swresample avutil"
swscale_deps="avutil"
postproc_deps="avutil"
avformat_deps="avcodec swresample avutil"
avcodec_deps="swresample avutil"
swresample_deps="avutil"
avresample_deps="avutil"
avutil_deps=""
