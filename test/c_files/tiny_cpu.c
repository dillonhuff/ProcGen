#include <stdint.h>

#define OPCODE_OR 0
#define OPCODE_AND 1

#define INSTR_CODE_NO_OP 0
#define INSTR_CODE_LOAD_IMM 1
#define INSTR_CODE_LOAD_MEM 2
#define INSTR_CODE_STORE 3
#define INSTR_CODE_JUMP 4
#define INSTR_CODE_ALU 5

#define REG_MASK 0b11111

uint32_t main_memory[64];
uint32_t register_file[32];
uint32_t PC;

void reset() {
  PC = 0;
}

void instr_no_op(uint32_t instr) {
  
}

void instr_store_memory(uint32_t instr) {
  uint8_t data_register = (instr >> 17) & REG_MASK;
  uint8_t mem_location_register = (instr >> 12) & REG_MASK;
  main_memory[mem_location_register] = register_file[data_register];
}

void instr_load_immediate(uint32_t instr) {
  uint32_t immediate = (instr >> 6) & 0b1111111111;
  uint8_t reg = (instr >> 1) & REG_MASK;
  register_file[reg] = immediate;
}

void instr_load_memory(uint32_t instr) {
  uint8_t mem_location_register = (instr >> 17) & REG_MASK;
  uint8_t dest_register = (instr >> 12) & REG_MASK;
  register_file[dest_register] = main_memory[register_file[mem_location_register]];
}

void instr_jump(uint32_t instr) {
  uint8_t condition_register = (instr >> 17) & REG_MASK;
  uint8_t mem_register = (instr >> 12) & REG_MASK;

  if (register_file[condition_register]) {
    PC = register_file[mem_register];
  }
}

void instr_alu(uint32_t instr) {
  uint8_t operand0 = (instr >> 17) & REG_MASK;
  uint8_t operand1 = (instr >> 12) & REG_MASK;
  uint8_t result = (instr >> 7) & REG_MASK;

  uint8_t opcode = (instr >> 22) & REG_MASK;

  switch (opcode) {
  case OPCODE_OR:
    register_file[result] = register_file[operand0] | register_file[operand1];
    return;
  case OPCODE_AND:
    register_file[result] = register_file[operand0] & register_file[operand1];
    return;
  default:
    register_file[result] = 0;
    return;
  }
}

void dispatch(const uint32_t instr) {
  uint8_t opcode = (instr >> 27) & 0b11111;
  switch (opcode) {
  case INSTR_CODE_ALU:
    instr_alu(instr);
    break;
  case INSTR_CODE_NO_OP:
    instr_no_op(instr);
    break;
  case INSTR_CODE_LOAD_MEM:
    instr_load_memory(instr);
    break;
  case INSTR_CODE_STORE:
    instr_store_memory(instr);
    break;
  case INSTR_CODE_LOAD_IMM:
    instr_load_immediate(instr);
    break;
  case INSTR_CODE_JUMP:
    instr_jump(instr);
    break;
  default:
    instr_no_op(instr);
    break;
  }
}

