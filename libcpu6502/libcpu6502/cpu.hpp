#ifndef CPU_H
#define CPU_H

#include "mem.hpp"

class CPU {
public:
    void Reset() const;
    void Execute(U32 Cycles) const;
};

#endif // CPU_H
