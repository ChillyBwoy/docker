all: hello process

hello:
	g++ -I. -Iinclude src/hello.cpp -o build/hello -Wl,--start-group \
/opt/v8-embed.x86_64/obj/{libv8_{base,libbase,external_snapshot,libplatform,libsampler},src/inspector/libinspector}.a \
-Wl,--end-group -lrt -ldl -pthread -std=c++0x

process:
	g++ -I. -Iinclude src/process.cpp -o build/process -Wl,--start-group \
/opt/v8-embed.x86_64/obj/{libv8_{base,libbase,external_snapshot,libplatform,libsampler},src/inspector/libinspector}.a \
-Wl,--end-group -lrt -ldl -pthread -std=c++0x
