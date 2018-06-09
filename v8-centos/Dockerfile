FROM centos:7

# epel-release for lbzip2
RUN yum update &&\
 yum install -y epel-release &&\
 yum install -y git python make gcc-multilib g++-multilib clang curl jq glib2-devel lbzip2 &&\
 yum clean all

WORKDIR /opt

ENV PATH="$PATH:/opt/depot_tools"

# Download sources to /opt/v8 and prepare to build
# Generate config files, build and set alias to binary
# to view commands use `fetch -n v8`
RUN git clone https://chromium.googlesource.com/chromium/tools/depot_tools.git &&\
 fetch v8 &&\
 cd /opt/v8 &&\
 python tools/dev/v8gen.py x64.release &&\
 ninja -C out.gn/x64.release d8 &&\
 mkdir -p /usr/local/lib64/v8 &&\
 cp /opt/v8/out.gn/x64.release/*.bin /usr/local/lib64/v8/ &&\
 cp /opt/v8/out.gn/x64.release/d8 /usr/local/lib64/v8/ &&\
 rm -rf /opt/v8/ &&\
 rm -rf /opt/depot_tools &&\
 echo "alias d8=/usr/local/lib64/v8/d8" >> ~/.bashrc

# CMD ["/usr/local/lib64/v8/d8"]
