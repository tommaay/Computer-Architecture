#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu)
{
  char data[DATA_LEN] = {
      // From print8.ls8
      0b10000010, // LDI R0,8
      0b00000000,
      0b00001000,
      0b01000111, // PRN R0
      0b00000000,
      0b00000001 // HLT
  };

  int address = 0;

  for (int i = 0; i < DATA_LEN; i++)
  {
    cpu->ram[address++] = data[i];
  }

  // TODO: Replace this with something less hard-coded
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char pc)
{
  return cpu->ram[pc];
}

void cpu_ram_write(struct cpu *cpu, unsigned char mdr, unsigned char pc)
{
  cpu->ram[pc] = mdr;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    // TODO
    break;

    // TODO: implement more ALU ops
  }
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
  int running = 1; // True until we get a HLT instruction

  while (running)
  {
    // TODO
    // 1. Get the value of the current instruction (in address PC).
    int IR = cpu_ram_read(cpu, cpu->pc); // instruction register

    // 2. Figure out how many operands this next instruction requires
    int numOps = IR >> 6;

    // 3. Get the appropriate value(s) of the operands following this instruction
    int ops[4];
    for (int i = 0; i < numOps; i++)
    {
      ops[i] = cpu_ram_read(cpu, cpu->pc + 1 + i);
    }

    // 4. switch() over it to decide on a course of action.
    // 5. Do whatever the instruction should do according to the spec.
    switch (IR)
    {
    case LDI:
      cpu->registers[ops[0]] = ops[1];
      break;

    case PRN:
      printf("%d\n", cpu->registers[ops[0]]);
      break;

    case HLT:
      running = 0;
      break;

    default:
      printf("Unknown instruction at %d: %d\n", cpu->pc, IR);
      exit(1);
    }

    // 6. Move the PC to the next instruction.
    cpu->pc += 1 + numOps;
  }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
  // TODO: Initialize the PC and other special registers
  cpu->pc = 0;
  memset(cpu->registers, 0, 8 * (sizeof(char)));
  memset(cpu->ram, 0, 128 * sizeof(char));
}
