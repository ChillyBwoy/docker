FROM centos:7

RUN yum update
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

# Build
RUN ninja -C out.gn/x64.release
