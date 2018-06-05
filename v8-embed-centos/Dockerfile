FROM centos:7

ARG V8_VERSION=HEAD
ARG V8_OUT=/opt/v8-embed.x86_64
ARG V8_INSTALL_PREFIX=/usr/local/v8
ARG GCC_VERSION=5.4.0

RUN yum update -y &&\
  yum groupinstall -y "Development Tools" &&\
  yum install -y epel-release &&\
  yum install -y centos-release-scl-rh &&\
  yum install -y install devtoolset-7-gcc* &&\
  yum install -y https://centos7.iuscommunity.org/ius-release.rpm && \
  yum install -y git bzip2 &&\
  yum install -y gmp-devel mpfr-devel libmpc-devel libatomic glib2 glib2-devel &&\
  yum install -y cmake3 &&\
  yum install -y python36u python36u-pip python36u-devel &&\
  yum clean all &&\
  ln -s /usr/bin/cmake3 /usr/bin/cmake

# Build and install gcc
RUN mkdir /src &&\
  cd /src &&\
  echo -e "\n-*- gcc version: $GCC_VERSION -*-\n" &&\
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

ENV LD_LIBRARY_PATH="/usr/local/lib64:$LD_LIBRARY_PATH"
ENV LD_RUN_PATH="/usr/local/lib64:$LD_RUN_PATH"
ENV PATH="$PATH:/opt/depot_tools"

# download build utils and V8 sources
RUN echo -e "\n-*- V8 version: $V8_VERSION -*-\n" &&\
  cd /opt &&\
  git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git &&\
  cd /src &&\
  fetch v8 &&\
  cd /src/v8 &&\
  git checkout $V8_VERSION

RUN cd /src/v8 && gn gen $V8_OUT --args='\
 is_component_build=false\
 is_debug=false\
 symbol_level=0\
 use_custom_libcxx=false\
 use_custom_libcxx_for_host=false\
 v8_enable_gdbjit=false\
 v8_enable_i18n_support=false\
 v8_static_library=true\
 v8_use_external_startup_data=false' && ninja -C $V8_OUT

# make install
RUN mkdir -p $V8_INSTALL_PREFIX/{include/libplatform,lib64,src/inspector} &&\
  install -p -m0644 $V8_OUT/obj/*.a $V8_INSTALL_PREFIX/lib64 &&\
  install -p -m0644 $V8_OUT/obj/src/inspector/*.a $V8_INSTALL_PREFIX/src/inspector &&\
  install -p -m0644 /src/v8/include/*.h $V8_INSTALL_PREFIX/include &&\
  install -p -m0644 /src/v8/include/libplatform/* $V8_INSTALL_PREFIX/include/libplatform

ENTRYPOINT ["scl", "enable", "devtoolset-7"]
CMD ["bash"]
