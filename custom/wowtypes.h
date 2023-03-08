#include <stdint.h>

typedef struct SOVERLAPPED {
	// unk
} SOVERLAPPED;

typedef struct TASYNCPARAMBLOCK {
	// unk
} TASYNCPARAMBLOCK;

typedef struct SFile {
	uint32_t unk0; // 0x0 (maybe isFromArchive?)
	uint32_t unk1; // 0x4
	uint32_t unk2; // 0x8
	const char* m_filename; // 0xc
	uint32_t unk3; // 0x10
	uint32_t maybe_size;  // 0x14
	uint32_t unk4; // 0x18 (maybe fileHandle)
	uint8_t padding[1024]; // ???
} SFile;
