#pragma once
#include <stdio.h>

namespace SpriteStudio6 {
namespace Platform {
class FileAbstract
{
public:
virtual void* DataLoad(void** handleHeap, const char* path, size_t alignment) = 0;

virtual void DataRelease(void* handleHeap) = 0;

virtual void* TextLoad(void** handleHeap, const char* path, size_t alignment) = 0;
};
}
}
