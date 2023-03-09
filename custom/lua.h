#define addr_FrameScript__Execute 0x00819210

typedef void (__cdecl* T_FrameScript__Execute)(const char* luastring, const char *filename, int unk0);

static T_FrameScript__Execute FrameScript__Execute = (T_FrameScript__Execute)addr_FrameScript__Execute;
