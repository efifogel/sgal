/* This is a big hack to work around a problem in libplatform/libplatform.h.
 * It contains the following include directive:
 *   #include "include/v8-platform.h"
 * while it should be
 *   #include <v8-platform.h>
 */
#include <v8-platform.h>
