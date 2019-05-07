#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_load(struct cpu *cpu, char *file)
{
  FILE *fp;
  fp = fopen(file, "r");

  if (fp == NULL)
  {
    fprintf(stderr, "ls8: error opening:  %s\n", file);
    exit(2);
  }

  char line[1024];
  int address = 0;

  while (fgets(line, 1024, fp) != NULL)
  {
    char *endptr;

    // converts str to number
    unsigned char val = strtoul(line, &endptr, 2);

    // prevents unnecessary lines being stored on ram
    if (endptr == line)
    {
      continue;
    }

    cpu->ram[address++] = val;
  }

  fclose(fp);
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned int pc)
{
  return cpu->ram[pc];
}

void cpu_ram_write(struct cpu *cpu, unsigned int instruction, unsigned int pc)
{
  cpu->ram[pc] = instruction;
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
  switch (op)
  {
  case ALU_MUL:
    cpu->registers[regA] = cpu->registers[regA] * cpu->registers[regB];
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

    case MUL:
      alu(cpu, ALU_MUL, ops[0], ops[1]);
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
