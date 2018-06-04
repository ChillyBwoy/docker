FROM centos:7

RUN yum update -y &&\
  yum groupinstall -y "Development Tools" &&\
  yum install -y python git bzip2 &&\
  yum install -y gmp-devel mpfr-devel libmpc-devel &&\
  yum clean all

ARG V8_VERSION=HEAD
ARG V8_OUT=/opt/v8-embed.x86_64
ARG V8_INSTALL_PREFIX=/usr/local/v8
ARG GCC_VERSION=5.4.0

# Build and install gcc
RUN mkdir /src &&\
  cd /src &&\
  echo "\n-*- gcc version: $GCC_VERSION -*-\n" &&\
  curl -O https://ftp.gnu.org/gnu/gcc/gcc-$GCC_VERSION/gcc-$GCC_VERSION.tar.bz2 &&\
  tar xfj gcc-$GCC_VERSION.tar.bz2 &&\
  cd /opt &&\
  mkdir gcc-$GCC_VERSION &&\
  cd /opt/gcc-$GCC_VERSION &&\
  /src/gcc-$GCC_VERSION/configure --enable-languages=c,c++ --disable-multilib &&\
  make -j$(nproc) &&\
  make install &&\
  cd /opt &&\
  rm -rf /src/gcc-$GCC_VERSION.tar.bz2 &&\
  rm -rf /src/gcc-$GCC_VERSION &&\
  rm -rf /opt/gcc-$GCC_VERSION

ENV PATH="$PATH:/opt/depot_tools"
ENV LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
ENV LD_RUN_PATH="/usr/local/lib64:$LD_RUN_PATH"

# download build utils and V8 sources
RUN echo "\n-*- V8 version: $V8_VERSION -*-\n" &&\
  cd /opt &&\
  git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git &&\
  cd /src &&\
  fetch v8 &&\
  git checkout $V8_VERSION &&\
  cd /src/v8 && gn gen $V8_OUT --args='\
 is_component_build=false\
 is_debug=false\
 is_official_build=true\
 msan_track_origins=0\
 symbol_level=0\
 use_custom_libcxx=false\
 use_custom_libcxx_for_host=false\
 use_sysroot=false\
 v8_enable_gdbjit=false\
 v8_enable_i18n_support=false\
 v8_enable_test_features=false\
 v8_monolithic=true\
 v8_static_library=true\
 v8_untrusted_code_mitigations=false\
 v8_use_external_startup_data=false' && ninja -C $V8_OUT

RUN mkdir -p $V8_INSTALL_PREFIX/{include/libplatform,lib64/third_party/icu,src/inspector} &&\
  install -p -m0644 $V8_OUT/obj/*.a $V8_INSTALL_PREFIX/lib64 &&\
  install -p -m0644 $V8_OUT/obj/third_party/icu/*.a $V8_INSTALL_PREFIX/lib64/third_party/icu &&\
  install -p -m0644 $V8_OUT/obj/src/inspector/*.a $V8_INSTALL_PREFIX/src/inspector &&\
  install -p -m0644 /src/v8/include/*.h $V8_INSTALL_PREFIX/include &&\
  install -p -m0644 /src/v8/include/libplatform/* $V8_INSTALL_PREFIX/include/libplatform
