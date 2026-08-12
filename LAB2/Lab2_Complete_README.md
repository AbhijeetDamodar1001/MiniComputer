# CS527 Lab 2 — Complete Function & Workflow Guide

> **Purpose:** Ye document Lab 2 ke compiler, processor, memory aur complete simulator workflow ko ek jagah combine karta hai. Iska main purpose **exam/viva revision + future README reference** hai.

---

## 1. Complete Simulator Workflow

```text
program.txt
    |
    v
compiler.c
    |
    v
program.byte
    |
    v
memory.initialize()
    |
    +----> Instruction[]
    |
    v
processor
fetch() -> decode() -> execute()
    |
    v
Data[] changes
    |
    v
memory.finalize()
    |
    v
data.byte
```

Overall flow:

```c
compile();
initialize();
reset();

while(!end_of_simulation)
{
    fetch();
    decode();
    execute();
}

finalize();
```

### Har stage ka kaam

| Stage | Kaam |
|---|---|
| `compile()` | `program.txt` ko machine bytecode me convert karta hai |
| `initialize()` | `program.byte` aur `data.byte` ko memory arrays me load karta hai |
| `reset()` | Registers, PC aur flags ko initial state deta hai |
| `fetch()` | Current instruction ke 4 bytes read karta hai |
| `decode()` | Current implementation me empty hai |
| `execute()` | Instruction actually perform karta hai |
| `finalize()` | Final `Data[]` ko `data.byte` me save karta hai |

Assignment specification me processor ke registers, PC, flags, fetch/decode/execute aur memory module ka ye overall structure diya gaya hai.

---

# 2. Compiler — `compiler.c`

Compiler ka kaam:

```text
Human-readable program
        ↓
program.txt
        ↓
compiler
        ↓
program.byte
```

Current compiler two-pass approach use karta hai:

```text
Pass 1 → labels ke addresses find karo
Pass 2 → actual instructions generate karo
```

---

## 2.1 `writeInstruction()`

### Kaam

Ek instruction ko 4-byte format me output file me likhta hai.

Conceptually:

```text
opcode destination operand1 operand2
```

Example:

```text
01 03 01 02
```

### Kyun chahiye?

Bytecode writing ka format ek hi function me rakhne ke liye.

### Exam me kya change ho sakta hai?

- output format change
- new instruction field
- byte formatting

---

## 2.2 `removeComment()`

### Kaam

Source line me `%` aate hi uske baad ka text remove karta hai.

Example:

```text
x3 = x1 + x2 % this is a comment
```

Compiler effectively dekhega:

```text
x3 = x1 + x2
```

### Kyun?

Comments compiler ko instruction nahi banane chahiye.

### Exam task

Professor comment syntax change karwa sakta hai.

---

## 2.3 `addLabel()`

### Kaam

Label ka naam aur uska address store karta hai.

Example:

```text
.loop
```

Agar current address `4` hai:

```text
labelName[0] = ".loop"
labelAddress[0] = 4
```

### Kyun?

Baad me branch instruction ko target address chahiye.

---

## 2.4 `findLabel()`

### Kaam

Label table me search karta hai.

Example:

```text
findLabel(".loop")
```

Agar `.loop` address 4 par hai:

```text
return 4;
```

Agar label nahi mila:

```text
return -1;
```

### Exam task

- missing label handling
- duplicate labels
- lookup behavior

---

## 2.5 `branchCode()`

Branch mnemonic ko numeric code me convert karta hai.

```text
BEQ → 0
BNE → 1
BCS → 2
BCC → 3
BMI → 4
BPL → 5
BVS → 6
BVC → 7
BHI → 8
BLS → 9
BGE → 10
BLT → 11
BGT → 12
BLE → 13
BAL → 14
```

### Important

Branch opcode:

```text
opcode = 0x10 + branchCode
```

Example:

```text
BLT
↓
branchCode = 11
↓
opcode = 0x10 + 11
↓
0x1B
```

---

# 3. Pass 1 — `pass1()`

Pass 1 ka main kaam:

> **Labels ke addresses collect karna.**

Example:

```text
x1 = 10
.loop
x1 = x1 - 1
BLT .loop
```

Compiler source ko scan karta hai.

Instruction size:

```text
1 instruction = 4 bytes
```

So:

```text
address = 0
instruction → address 0
next       → address 4
next       → address 8
```

Label khud instruction nahi hai, isliye label par address increase nahi hota.

### Pass 1 ka mental model

```text
read line
   ↓
remove comment
   ↓
blank?
   ↓ no
label?
   ↓ yes
store label + current address
   ↓
continue

instruction?
   ↓
address += 4
```

### Why Pass 1?

Suppose:

```text
BLT .loop
...
...
.loop
```

`.loop` future me defined hai.

Pass 1 pehle complete file scan karke target address jaan leta hai.

---

# 4. Pass 2 — `pass2()`

Pass 2 actual machine instructions generate karta hai.

Concept:

```text
program.txt
    ↓
parse instruction
    ↓
identify opcode
    ↓
identify operands
    ↓
calculate branch offset if needed
    ↓
writeInstruction()
    ↓
program.byte
```

### Important

Most new source-language instructions professor de to **pass2()** sabse pehla place hai jahan dekhna hai.

---

# 5. Compiler Examples

## Example 1 — Register Arithmetic

Source:

```text
x3 = x1 + x2
```

Parsed:

```text
dest = 3
src1 = 1
src2 = 2
operation = +
```

Opcode:

```text
ADD = 01
```

Output:

```text
01 03 01 02
```

---

## Example 2 — Immediate Arithmetic

Source:

```text
x3 = x1 + 10
```

Conceptually:

```text
dest = 3
src1 = 1
immediate = 10
```

Output:

```text
09 03 01 0A
```

---

# 6. Branch Offset — Very Important

Suppose:

```text
.loop
...
BLT .loop
```

Each instruction = 4 bytes.

If:

```text
current address = 12
target address  = 4
```

Then:

```text
target - current
= 4 - 12
= -8 bytes
```

Since one instruction = 4 bytes:

```text
-8 / 4 = -2 instructions
```

So offset:

```text
-2
```

Meaning:

> Branch target current position se **2 instructions piche** hai.

---

# 7. Processor — `processor.c`

Processor ke important global variables:

```text
Register[256]
PC
opcode
dest
src1
src2
Z
N
C
V
end_of_simulation
```

### Meaning

| Variable | Meaning |
|---|---|
| `Register[256]` | 256 CPU registers |
| `PC` | Program Counter |
| `opcode` | Current instruction ka operation |
| `dest` | Destination |
| `src1` | First source |
| `src2` | Second source / immediate / offset |
| `Z` | Zero flag |
| `N` | Negative flag |
| `C` | Carry flag |
| `V` | Overflow flag |
| `end_of_simulation` | Simulation stop karne ka signal |

---

# 8. `readData()`

### Kaam

Memory se 4 bytes read karke ek 32-bit value banata hai.

```text
Data[address]
Data[address+1]
Data[address+2]
Data[address+3]
        ↓
    32-bit value
```

### Why?

Memory byte-addressable hai, lekin processor 32-bit data operations karta hai.

Example:

```text
Data[0] = byte 0
Data[1] = byte 1
Data[2] = byte 2
Data[3] = byte 3
```

Inhe combine karke ek 32-bit number banaya jata hai.

---

# 9. `writeData()`

`readData()` ka opposite.

```text
32-bit value
     ↓
4 bytes
     ↓
Data[]
```

Example concept:

```text
value = 0x12345678
```

Little-endian storage:

```text
Data[address]     = 78
Data[address + 1] = 56
Data[address + 2] = 34
Data[address + 3] = 12
```

---

# 10. Flags — `Z N C V`

Arithmetic operation ke baad processor result ke saath flags bhi update karta hai.

```text
Z = Zero
N = Negative
C = Carry
V = Overflow
```

Ye flags baad me branch instructions use karti hain.

---

# 11. `setFlagsAdd()`

Function:

```c
void setFlagsAdd(int a, int b, int result)
```

Addition ke baad:

```text
Z
N
C
V
```

update karta hai.

---

## Z — Zero

```c
Z = (result == 0);
```

Example:

```text
5 + (-5) = 0
```

So:

```text
Z = 1
```

Agar result 0 nahi hai:

```text
Z = 0
```

---

## N — Negative

Result ka sign bit check hota hai.

32-bit integer me highest bit:

```text
bit 31
```

Sign bit:

```text
0 → non-negative
1 → negative
```

So:

```text
negative result → N = 1
positive result → N = 0
```

---

## C — Carry

Addition me unsigned carry/wraparound indication.

Simple intuition:

```text
maximum unsigned value
        +
       1
        ↓
wraparound + carry
```

Current implementation `ur < ua || ur < ub` expression use karti hai.

---

## V — Signed Overflow

Overflow tab hota hai jab signed result representable range se bahar chala jaye.

Important patterns:

```text
positive + positive → negative
```

or

```text
negative + negative → positive
```

Ye signed overflow indicate karta hai.

Example:

```text
INT_MAX + 1
```

Result signed 32-bit range se bahar hai.

So:

```text
V = 1
```

---

# 12. `setFlagsSub()`

Function:

```c
void setFlagsSub(int a, int b, int result)
```

Subtraction ke baad flags update karta hai.

```text
Z → result zero?
N → result negative?
C → current subtraction carry/borrow condition
V → signed overflow?
```

---

## Example

```text
5 - 5 = 0
```

Therefore:

```text
Z = 1
N = 0
```

Later:

```text
BEQ
```

`Z` ko check karke branch le sakta hai.

---

## Subtraction Overflow

Important pattern:

```text
positive - negative → negative
```

or

```text
negative - positive → positive
```

Ye signed overflow ka indication ho sakta hai.

Example:

```text
INT_MIN - 1
```

Mathematically range se bahar chala jata hai.

---

# 13. `reset()`

### Kaam

CPU ko clean initial state me laata hai.

```text
Register[] = 0
PC = 0
Z = 0
N = 0
C = 0
V = 0
end_of_simulation = 0
```

### Why?

Simulation predictable state se start honi chahiye.

---

# 14. `fetch()`

Current instruction ke 4 bytes read karta hai:

```c
opcode = Instruction[PC];
dest   = Instruction[PC + 1];
src1   = Instruction[PC + 2];
src2   = Instruction[PC + 3];
```

So:

```text
Instruction[PC]     → opcode
Instruction[PC + 1] → dest
Instruction[PC + 2] → src1
Instruction[PC + 3] → src2
```

### Example

Memory:

```text
01 03 01 02
```

Fetch ke baad:

```text
opcode = 01
dest   = 03
src1   = 01
src2   = 02
```

---

# 15. `decode()`

Current implementation me:

```c
void decode(void)
{
}
```

Ye currently empty hai.

Assignment me decode stage placeholder hai.

Exam me professor pooch sakta hai:

> Decode function empty kyun hai?

Answer:

> Current implementation me instruction fields `fetch()` already extract kar deta hai, so separate decode logic required nahi hai.

---

# 16. `execute()`

Ye processor ka **main function** hai.

Kaam:

```text
opcode dekho
   ↓
corresponding operation perform karo
   ↓
register/memory/flags update karo
   ↓
PC update karo
```

---

# 17. Important Opcode Groups

| Opcode | Operation |
|---|---|
| `00` | End simulation |
| `01` | Register ADD |
| `02` | Register SUB |
| `03` | Register MUL |
| `04` | Register DIV |
| `05` | LOAD |
| `06` | STORE |
| `07` | Register copy |
| `09` | ADD immediate |
| `0A` | SUB immediate |
| `0B` | MUL immediate |
| `0C` | DIV immediate |
| `0D` | Load from constant address |
| `0E` | Store to constant address |
| `0F` | Move immediate |
| `10–1E` | Branches |

---

# 18. Register ADD

```text
Rd = Rs1 + Rs2
```

Example:

```text
R1 = 10
R2 = 20

ADD R3,R1,R2

R3 = 30
```

Then:

```text
setFlagsAdd()
```

call hota hai.

---

# 19. Register SUB

```text
Rd = Rs1 - Rs2
```

Example:

```text
R1 = 20
R2 = 5

R3 = R1 - R2

R3 = 15
```

Then:

```text
setFlagsSub()
```

call hota hai.

---

# 20. Immediate Operations

Difference:

### Register-register

```text
R3 = R1 + R2
```

### Immediate

```text
R3 = R1 + 10
```

Yahan `10` register nahi hai.

---

# 21. LOAD

Concept:

```text
address = Register[src1]
        ↓
memory[address]
        ↓
Register[dest]
```

So:

```text
Register[dest] = readData(Register[src1]);
```

---

# 22. STORE

Concept:

```text
Register[src1]
      ↓
memory address = Register[dest]
```

So:

```text
writeData(Register[dest], Register[src1]);
```

---

# 23. Branch Instructions

Branch opcodes:

```text
0x10 → BEQ
0x11 → BNE
0x12 → BCS
0x13 → BCC
0x14 → BMI
0x15 → BPL
0x16 → BVS
0x17 → BVC
0x18 → BHI
0x19 → BLS
0x1A → BGE
0x1B → BLT
0x1C → BGT
0x1D → BLE
0x1E → BAL
```

---

# 24. Branch Conditions

| Branch | Condition |
|---|---|
| BEQ | `Z` |
| BNE | `!Z` |
| BCS | `C` |
| BCC | `!C` |
| BMI | `N` |
| BPL | `!N` |
| BVS | `V` |
| BVC | `!V` |
| BHI | `C && !Z` |
| BLS | `!C || Z` |
| BGE | `N == V` |
| BLT | `N != V` |
| BGT | `!Z && N == V` |
| BLE | `Z || N != V` |
| BAL | Always |

---

# 25. BLT — Most Important Example

Suppose:

```text
BLT .loop
```

Compiler:

```text
BLT
 ↓
code = 11
 ↓
opcode = 0x10 + 11
 ↓
0x1B
```

Processor:

```text
if(code == 11)
    take = (N != V);
```

So:

```text
N != V
   |
   +---- true  → branch
   |
   +---- false → next instruction
```

### Why N and V?

Sirf `N` dekhna overflow ke case me misleading ho sakta hai.

`N != V` signed less-than condition ko implement karta hai.

---

# 26. Branch PC Update

Agar branch taken:

```c
PC = PC + ((int8_t)src2 * 4);
return;
```

Why `* 4`?

Because:

```text
1 instruction = 4 bytes
```

Example:

```text
src2 = -2
```

Then:

```text
PC = PC + (-2 × 4)
PC = PC - 8
```

Meaning:

```text
2 instructions backward
```

Agar branch taken nahi:

```text
PC = PC + 4
```

Meaning:

```text
next instruction
```

### `return` kyun?

Branch target set karne ke baad normal:

```text
PC = PC + 4
```

nahi chalna chahiye.

---

# 27. Memory Module

Assignment specification:

```text
Instruction[256]
Data[4096]
```

Memory byte-addressable hai.

Meaning:

```text
address 0 → one byte
address 1 → next byte
address 2 → next byte
...
```

One instruction:

```text
4 bytes
```

---

# 28. `initialize()`

Concept:

```text
program.byte
     ↓
Instruction[]
```

and:

```text
data.byte
     ↓
Data[]
```

Processor directly files ke saath kaam nahi karta.

Processor arrays ke saath kaam karta hai.

---

# 29. `finalize()`

Execution complete hone ke baad:

```text
Data[]
  ↓
data.byte
```

Final memory state save hoti hai.

---

# 30. Important Lab 1 vs Lab 2 Warning

Conversation me ek older Lab 1 `memory.c` implementation hai jo:

```text
Data[256]
```

use karta hai aur decimal file I/O karta hai.

Lab 2 specification:

```text
Data[4096]
```

aur hexadecimal byte representation describe karti hai.

Isliye exam/study ke time **current Lab 2 files + Lab 2 specification ko source of truth** rakho; purani Lab 1 implementation ko blindly mix mat karo.

---

# 31. Exam — Professor New Task De To Kahan Jaana Hai?

| Professor ka task | Sabse pehle kahan dekho |
|---|---|
| New source syntax | `compiler.c → pass2()` |
| New label behavior | `pass1()`, `addLabel()`, `findLabel()` |
| New branch mnemonic | `branchCode()`, `pass2()`, `processor.c → execute()` |
| New arithmetic opcode | `compiler.c` opcode generation + `processor.c → execute()` |
| New flag | `setFlagsAdd()` / `setFlagsSub()` |
| New memory instruction | `readData()` / `writeData()` / `execute()` |
| Change instruction format | `fetch()` + compiler byte writer |
| Change `.byte` file format | `memory.c` + compiler output |
| Change comments | `removeComment()` |

---

# 32. One-Page Viva Revision

## Compiler

```text
writeInstruction()
→ instruction ko bytecode me likhta hai

removeComment()
→ % ke baad ka text remove

addLabel()
→ label + address store

findLabel()
→ label ka address find

branchCode()
→ BEQ...BAL → 0...14

pass1()
→ labels collect

pass2()
→ instructions generate

compile()
→ pass1 + pass2
```

## Processor

```text
readData()
→ 4 bytes → 32-bit value

writeData()
→ 32-bit value → 4 bytes

setFlagsAdd()
→ ADD ke baad Z,N,C,V

setFlagsSub()
→ SUB ke baad Z,N,C,V

reset()
→ CPU initial state

fetch()
→ 4 instruction bytes read

decode()
→ currently empty

execute()
→ instruction perform
```

## Memory

```text
initialize()
→ files → memory arrays

finalize()
→ Data[] → data.byte
```

## Branch

```text
BLT → N != V

branch taken:
PC = PC + offset × 4

branch not taken:
PC = PC + 4
```

---

# 33. Final Mental Model

Agar poora assignment ek story ki tarah yaad karna ho:

```text
USER SOURCE
    ↓
compiler.c
    ↓
"Is line ka opcode kya hai?"
    ↓
program.byte
    ↓
memory.initialize()
    ↓
Instruction[]
    ↓
CPU fetch karta hai
    ↓
opcode + operands
    ↓
execute()
    ↓
register / memory / flags change
    ↓
branch?
   / \
 yes  no
  |    |
PC+   PC+4
offset
  |
next fetch
    ↓
END opcode
    ↓
finalize()
    ↓
data.byte
```

**Ye pura Lab 2 ka high-level mental map hai.**

---

## Source basis

This guide is based on the current Lab 2 compiler/processor files and the Lab specification available in the project materials. The compiler source defines the label table, helpers, two-pass compilation and bytecode writing. fileciteturn16file0L15-L30 fileciteturn16file0L32-L68 fileciteturn16file0L91-L122

The processor source defines the memory helpers, flags, reset/fetch/decode/execute and instruction handling. fileciteturn16file9L813-L851 fileciteturn16file9L853-L915

The Lab specification describes the processor state, four-byte fetch, memory arrays, byte-addressable memory and `.byte` representation. fileciteturn16file2L211-L240
