#ifdef __cplusplus
#include <string>

#include "safequeue.hpp"

extern SafeQueue<std::string> lua_queue;
extern std::string login_script;
#endif

#ifdef __cplusplus
extern "C" {
#endif

void setup_globals();

#ifdef __cplusplus
}
#endif

