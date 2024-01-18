#ifndef MEM_H
#define MEM_H

using Byte = unsigned char;
using Word = unsigned short;
using U32 = unsigned int;

static constexpr U32 MAX_MEM = 1024 * 64;

class Memory {
public:
    Byte Data[MAX_MEM];
};

#endif // MEM_H
