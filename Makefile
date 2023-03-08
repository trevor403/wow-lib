.PHONY: loader header custom install

all: custom

custom: dll.zig extra.zig deps/minhook/src/buffer.c deps/minhook/src/trampoline.c deps/minhook/src/hde/hde32.c deps/minhook/src/hook.c custom/dll.c custom/patch.cpp custom/dx_patch.cpp
	@zig build

header:
	@zig build-lib extra.zig -target x86-windows-gnu -mcpu i686 -lc -dynamic -femit-h -fno-emit-bin -fno-emit-implib

loader/DINPUT8.exp: loader/DINPUT8.def
	@/usr/i686-w64-mingw32/bin/dlltool -d loader/DINPUT8.def --output-exp loader/DINPUT8.exp

loader: loader/DINPUT8.exp loader/proxy.c
	@i686-w64-mingw32-gcc loader/proxy.c -m32 -mconsole -O2 -g -shared -o DINPUT8.dll -Wl,--tsaware -Wl,--nxcompat -Wl,--dynamicbase -static -static-libgcc -Wl,loader/DINPUT8.exp -Wl,--enable-stdcall-fixup -lkernel32

clean:
	@rm -rf zig-cache zig-out extra.h loader/DINPUT8.exp DINPUT8.dll custom.dll

install:
	@cp DINPUT8.dll /home/trevor/wk/wow/client/3.3.5/DINPUT8.dll
	@cp zig-out/lib/custom.dll /home/trevor/wk/wow/client/3.3.5/custom.dll
