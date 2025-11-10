#ifndef TYPES_H
#define TYPES_H

/* 使用标准头提供 size_t、uintptr_t、int32_t/uint64_t 等类型，避免重复定义 */
#include <stddef.h>
#include <stdint.h>

typedef unsigned int   uint;
typedef unsigned short ushort;
typedef unsigned char  uchar;

/* 使用标准固定宽度类型，名称保留为项目中已有简写 */
typedef uint64_t       uint64;
typedef int64_t        int64;
typedef int32_t        int32;

/* 注意：size_t 和 uintptr_t 等类型使用 <stddef.h>/<stdint.h> 中的定义，
   不在此处重新 typedef，以避免与系统头冲突。 */

#endif // TYPES_H