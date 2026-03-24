# CPU Instruction TODO

This document tracks the current instruction and addressing-mode backlog that was previously maintained in `README.md`.


## Arithmetic / Logic
- [ ] ADC / SBC – Add/Subtract with Carry (sets C, V, Z, N)
- [ ] AND / ORA / EOR – Bitwise operations (set Z, N)
- [ ] CMP / CPX / CPY – Compare register with operand (set C, Z, N)

## Shifts / Rotates
- [ ] ASL / LSR – Shift left/right (affect C, Z, N)
- [ ] ROL / ROR – Rotate through carry (affect C, Z, N)

## Increments / Decrements
- [ ] INC / DEC (memory)
- [ ] INX / DEX / INY / DEY (registers)

## Branches & Jumps
- [ ] JMP (Absolute / Indirect)
- [ ] JSR / RTS – Subroutine call / return
- [ ] Branches: BPL, BMI, BNE, BEQ, BVC, BVS, BCC, BCS (relative addressing)

## Stack & Status
- [ ] PHA / PHP – Push A / Processor Status
- [ ] PLA / PLP – Pull A / Processor Status
- [ ] BRK / RTI – Force interrupt / Return from interrupt

## Other / Misc
- [ ] Flag setting/clearing: CLC, SEC, CLI, SEI, CLV, CLD, SED

## Backlog notes
- Additional instructions and addressing modes should continue to be added incrementally with accompanying tests.
- Implemented instructions are tracked separately in `docs/cpu-instructions.md`.
