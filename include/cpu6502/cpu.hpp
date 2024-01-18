#ifndef CPU_HPP
#define CPU_HPP

#include "mem.hpp"

class CPU {
public:
    void Reset() const;
    void Execute(U32 Cycles) const;
};

#endif // CPU_HPP
