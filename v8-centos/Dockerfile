FROM centos:7
LABEL maintainer="Eugene Cheltsov <chill.icp@gmail.com>"

RUN yum update
# because of lbzip2
RUN yum install -y epel-release
RUN yum install -y git python make gcc-multilib g++-multilib clang curl jq glib2-devel lbzip2

WORKDIR /tmp

RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
ENV PATH="$PATH:/tmp/depot_tools"

# download sources to /tmp/v8
# to view commands use `fetch -n v8`
RUN fetch v8

# sync project
RUN gclient sync

# source folder
WORKDIR /tmp/v8

# Generate config files
RUN python tools/dev/v8gen.py x64.release

# RUN gn gen out.gn/x64.release --args='is_debug=false target_cpu="x64" is_component_build=true'
# Build
RUN ninja -C out.gn/x64.release d8

# Optional tests
# RUN python tools/run-tests.py --gn

# set alias to binary
RUN alias d8=/tmp/v8/out.gn/x64.release/d8
