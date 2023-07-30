# RISC-MC8 Instruction Set Architecture

Version 4.5

---

## Instructions

| Mnemonic |    Format    | Description |  
|:--------:|:------------:|:------------|  
|  `ANDI`  | `00000[reg]` | Bitwise AND between `ireg` and `reg`, storing the result in `reg`.  
|  `NAND`  | `00001[reg]` | Bitwise NAND between `ireg` and reg, storing the result in `reg`.  
|  `ADDI`  | `00010[reg]` | Add `ireg` and `reg`, storing the result in `reg`.  
|  `SUBI`  | `00011[reg]` | Subtract `ireg` from `reg`, storing the result in `reg`.  
|  `IORI`  | `00100[reg]` | Bitwise OR between `ireg` and `reg`, storing the result in `reg`.  
|  `XORI`  | `00101[reg]` | Bitwise XOR between `ireg` and `reg`, storing the result in `reg`.  
|  `DUPI`  | `00110[reg]` | Duplicate the value in `ireg` to `reg`.  
|  `DUPR`  | `00111[reg]` | Duplicate the value in `reg` to `ireg`.  
|  `LOAD`  | `01000[reg]` | Set `reg` to a value in RAM, using the value in `ireg` as the address.  
|  `STOR`  | `01001[reg]` | Set a value in RAM to `reg`, using the value in `ireg` as the address.  
|  `SHIF`  | `01010[reg]` | Shift `reg` left by the signed amount in the lower 4 bits of `ireg` (the upper 4 bits are ignored). Negative values indicate a right shift, the sign is not extended. Both 0 and -8 indicate no shift.  
|  `SKIP`  | `01011[reg]` | Skip the next instruction if the values in `ireg` and `reg` are equal.  
|  `STLO`  | `0110[bits]` | Set the lower 4 bits of `ireg`, the upper 4 bits are unchanged.  
|  `STHI`  | `0111[bits]` | Set the upper 4 bits of `ireg`, the lower 4 bits are unchanged.  
|  `JUMP`  | `1[offset]`  | Jump by adding the signed 7-bit immediate to the current program counter value instead of incrementing by 1.  

---

## Registers

| Address | Short Name | Usage |  
|:-------:|:----------:|:------|  
|  `000`  |   `ireg`   | General purpose register. Implied argument register for instructions.  
|  `001`  |    `r1`    | General purpose register.  
|  `010`  |    `r2`    | General purpose register.  
|  `011`  |    `r3`    | General purpose register.  
|  `100`  |    `r4`    | General purpose register.  
|  `101`  |    `r5`    | General purpose register.  
|  `110`  |    `r6`    | General purpose register.  
|  `111`  |    `r7`    | General purpose register.  
|   N/A   |    `PC`    | Program counter. This register is not required to be 8-bits, larger sizes may be used to accomodate a larger instruction memory.  

---

## Input/Output  

To accomodate IO, the bytes at memory addresses 0 and 1 may optionally be reserved for this purpose.  
Hardware interrupts shall be used to communicate when a byte has been read by the IO device or the CPU.  
