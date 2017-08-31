FROM centos:7

# epel-release for lbzip2
RUN yum update &&\
 yum install -y epel-release &&\
 yum install -y git python make gcc-multilib g++-multilib clang curl jq glib2-devel lbzip2 &&\
 yum clean all

WORKDIR /tmp

RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git
ENV PATH="$PATH:/tmp/depot_tools"

# Download sources to /tmp/v8 and prepare to build
# Generate config files, build and set alias to binary
# to view commands use `fetch -n v8`
RUN fetch v8 &&\
 gclient sync &&\
 cd /tmp/v8 &&\
 python tools/dev/v8gen.py x64.release &&\
 ninja -C out.gn/x64.release d8 &&\
 mkdir -p /usr/local/lib64/v8 &&\
 cp /tmp/v8/out.gen/x64.release/*.bin /usr/local/lib64/v8/ &&\
 cp /tmp/v8/out.gen/x64.release/d8 /usr/local/lib64/v8/ &&\
 rm -rf /tmp/v8/ &&\
 rm -rf /tmp/depot_tools &&\
 echo "alias d8=/usr/local/lib64/v8/d8" >> ~/.bashrc

CMD ["d8"]
