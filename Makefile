.PHONY: loader go header custom install

all: custom

custom: dll.zig extra.zig deps/minhook/src/buffer.c deps/minhook/src/trampoline.c deps/minhook/src/hde/hde32.c deps/minhook/src/hook.c custom/patch.h custom/hooks.h custom/skip.h custom/zig.h custom/go.h custom/wowtypes.h custom/dx_patch.h custom/dll.c custom/patch.cpp custom/dx_patch.cpp custom/skip.c custom/globals.cpp custom/globals.h custom/safequeue.hpp custom/lua.h lib/libwow.a lib/libwow.h
	@zig build

header:
	@zig build-lib extra.zig -target x86-windows-gnu -mcpu i686 -lc -dynamic -femit-h -fno-emit-bin -fno-emit-implib

go: lib/libwow.a

lib/libwow.a lib/libwow.h: cmd/wow-lib/main.go cmd/wow-lib/lib.go
	@env GOARCH=386 GOOS=windows CGO_ENABLED=1 CC="zig cc -target x86-windows-gnu" go build -v -buildmode=c-archive -o lib/libwow.a ./cmd/wow-lib/

loader/DINPUT8.exp: loader/DINPUT8.def
	@/usr/i686-w64-mingw32/bin/dlltool -d loader/DINPUT8.def --output-exp loader/DINPUT8.exp

loader: loader/DINPUT8.exp loader/proxy.c
	@i686-w64-mingw32-gcc loader/proxy.c -m32 -mconsole -O2 -g -shared -o DINPUT8.dll -Wl,--tsaware -Wl,--nxcompat -Wl,--dynamicbase -static -static-libgcc -Wl,loader/DINPUT8.exp -Wl,--enable-stdcall-fixup -lkernel32

clean:
	@rm -rf zig-cache zig-out extra.h loader/DINPUT8.exp DINPUT8.dll custom.dll lib/libwow.a lib/libwow.h

install:
	@cp DINPUT8.dll /home/trevor/wk/wow/client/3.3.5/DINPUT8.dll
	@cp zig-out/lib/custom.dll /home/trevor/wk/wow/client/3.3.5/custom.dll
