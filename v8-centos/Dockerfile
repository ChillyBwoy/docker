FROM centos:7

# epel-release for lbzip2
RUN yum update && \
  yum install -y epel-release && \
  yum install -y git python make gcc-multilib g++-multilib clang curl jq glib2-devel lbzip2 && \
  yum clean all

WORKDIR /tmp

RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
ENV PATH="$PATH:/tmp/depot_tools"

# download sources to /tmp/v8 and prepare to build
# to view commands use `fetch -n v8`
RUN fetch v8 && gclient sync

# source folder
WORKDIR /tmp/v8

# Generate config files, build and set alias to binary
RUN python tools/dev/v8gen.py x64.release && \
  ninja -C out.gn/x64.release d8 && \
  echo "export PATH=\$PATH:/tmp/v8/out.gn/x64.release/" >> ~/.bashrc && \
  echo "alias v8=/tmp/v8/out.gn/x64.release/d8" >> ~/.bashrc

# Optional tests
# RUN python tools/run-tests.py --gn
