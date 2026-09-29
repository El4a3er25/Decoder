#include "riscv_decoder.h"
#include <iostream>
#include <iomanip>

using namespace std;

RiscVDecoder::RiscVDecoder(const vector<uint8_t>& data, uint64_t addr) 
    : instructionData(data), baseAddress(addr) {}

void RiscVDecoder::decode() {
    cout << "Decoding .text section..." << endl;
    
    // We assume 32-bit instructions for now.
    for (size_t i = 0; i + 3 < instructionData.size(); i += 4) {
        uint32_t inst = (instructionData[i+3] << 24) | 
                        (instructionData[i+2] << 16) | 
                        (instructionData[i+1] << 8) | 
                        instructionData[i];
                        
        uint64_t currentAddress = baseAddress + i;
        decodeInstruction(inst, currentAddress);
    }
}

void RiscVDecoder::decodeInstruction(uint32_t inst, uint64_t address) {
    uint32_t opcode = inst & 0x7F;
    uint32_t rd = (inst >> 7) & 0x1F;
    uint32_t funct3 = (inst >> 12) & 0x7;
    uint32_t rs1 = (inst >> 15) & 0x1F;
    uint32_t rs2 = (inst >> 20) & 0x1F;
    uint32_t funct7 = (inst >> 25) & 0x7F;
    
    cout << "0x" << hex << address << dec << ": ";
    
    switch (opcode) {
        case 0x33: { // R-type (ADD, SUB, AND, OR, XOR, SLL, SRL, SRA)
            string inst_name = "UNKNOWN";
            if (funct3 == 0x0) {
                if (funct7 == 0x00) inst_name = "ADD";
                else if (funct7 == 0x20) inst_name = "SUB";
            }
            else if (funct3 == 0x1) inst_name = "SLL";
            else if (funct3 == 0x2) inst_name = "SLT";
            else if (funct3 == 0x3) inst_name = "SLTU";
            else if (funct3 == 0x4) inst_name = "XOR";
            else if (funct3 == 0x5) {
                if (funct7 == 0x00) inst_name = "SRL";
                else if (funct7 == 0x20) inst_name = "SRA";
            }
            else if (funct3 == 0x6) inst_name = "OR";
            else if (funct3 == 0x7) inst_name = "AND";
            
            cout << inst_name << " x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
            break;
        }
        case 0x13: { // I-type (ADDI, ANDI, ORI, XORI, etc.)
            string inst_name = "UNKNOWN";
            int32_t imm = ((int32_t)inst) >> 20; // sign extension
            
            if (funct3 == 0x0) inst_name = "ADDI";
            else if (funct3 == 0x2) inst_name = "SLTI";
            else if (funct3 == 0x3) inst_name = "SLTIU";
            else if (funct3 == 0x4) inst_name = "XORI";
            else if (funct3 == 0x6) inst_name = "ORI";
            else if (funct3 == 0x7) inst_name = "ANDI";
            else if (funct3 == 0x1) {
                inst_name = "SLLI";
                imm = imm & 0x3F; // RV64 shift amount is 6 bits
            }
            else if (funct3 == 0x5) {
                if ((imm >> 6) == 0) inst_name = "SRLI";
                else inst_name = "SRAI";
                imm = imm & 0x3F; 
            }
            
            cout << inst_name << " x" << rd << ", x" << rs1 << ", " << imm << endl;
            break;
        }
        case 0x63: { // B-type (Branch: BEQ, BNE, BLT, BGE, BLTU, BGEU)
            string inst_name = "UNKNOWN";
            if (funct3 == 0x0) inst_name = "BEQ";
            else if (funct3 == 0x1) inst_name = "BNE";
            else if (funct3 == 0x4) inst_name = "BLT";
            else if (funct3 == 0x5) inst_name = "BGE";
            else if (funct3 == 0x6) inst_name = "BLTU";
            else if (funct3 == 0x7) inst_name = "BGEU";
            
            // Extract branch immediate
            int32_t imm = ((inst >> 31) << 12) |
                          (((inst >> 7) & 0x1) << 11) |
                          (((inst >> 25) & 0x3F) << 5) |
                          (((inst >> 8) & 0xF) << 1);
            imm = (imm << 19) >> 19; // Sign extend 13 bits to 32
            
            cout << inst_name << " x" << rs1 << ", x" << rs2 << ", " << imm << endl;
            break;
        }
        case 0x6F: { // J-type (JAL)
            int32_t imm = ((inst >> 31) << 20) |
                          (((inst >> 12) & 0xFF) << 12) |
                          (((inst >> 20) & 0x1) << 11) |
                          (((inst >> 21) & 0x3FF) << 1);
            imm = (imm << 11) >> 11; // Sign extend 21 bits
            cout << "JAL x" << rd << ", " << imm << endl;
            break;
        }
        case 0x3B: { // R-type 64-bit (ADDW, SUBW, etc)
            string inst_name = "UNKNOWN";
            if (funct3 == 0x0) {
                if (funct7 == 0x00) inst_name = "ADDW";
                else if (funct7 == 0x20) inst_name = "SUBW";
            }
            else if (funct3 == 0x1) inst_name = "SLLW";
            else if (funct3 == 0x5) {
                if (funct7 == 0x00) inst_name = "SRLW";
                else if (funct7 == 0x20) inst_name = "SRAW";
            }
            cout << inst_name << " x" << rd << ", x" << rs1 << ", x" << rs2 << endl;
            break;
        }
        case 0x1B: { // I-type 64-bit (ADDIW, etc)
            string inst_name = "UNKNOWN";
            int32_t imm = ((int32_t)inst) >> 20; 
            if (funct3 == 0x0) inst_name = "ADDIW";
            else if (funct3 == 0x1) {
                inst_name = "SLLIW";
                imm = imm & 0x1F;
            }
            else if (funct3 == 0x5) {
                if ((imm >> 5) == 0) inst_name = "SRLIW";
                else inst_name = "SRAIW";
                imm = imm & 0x1F;
            }
            cout << inst_name << " x" << rd << ", x" << rs1 << ", " << imm << endl;
            break;
        }
        case 0x03: { // Load instructions
            string inst_name = "UNKNOWN";
            int32_t imm = ((int32_t)inst) >> 20;
            if (funct3 == 0x0) inst_name = "LB";
            else if (funct3 == 0x1) inst_name = "LH";
            else if (funct3 == 0x2) inst_name = "LW";
            else if (funct3 == 0x3) inst_name = "LD";
            else if (funct3 == 0x4) inst_name = "LBU";
            else if (funct3 == 0x5) inst_name = "LHU";
            else if (funct3 == 0x6) inst_name = "LWU";
            cout << inst_name << " x" << rd << ", " << imm << "(x" << rs1 << ")" << endl;
            break;
        }
        case 0x23: { // Store instructions
            string inst_name = "UNKNOWN";
            int32_t imm = (((inst >> 25) & 0x7F) << 5) | ((inst >> 7) & 0x1F);
            imm = (imm << 20) >> 20; // Sign extend 12 bits
            
            if (funct3 == 0x0) inst_name = "SB";
            else if (funct3 == 0x1) inst_name = "SH";
            else if (funct3 == 0x2) inst_name = "SW";
            else if (funct3 == 0x3) inst_name = "SD";
            cout << inst_name << " x" << rs2 << ", " << imm << "(x" << rs1 << ")" << endl;
            break;
        }
        case 0x67: { // JALR
            int32_t imm = ((int32_t)inst) >> 20;
            cout << "JALR x" << rd << ", " << imm << "(x" << rs1 << ")" << endl;
            break;
        }
        case 0x37: { // LUI
            int32_t imm = inst & 0xFFFFF000;
            cout << "LUI x" << rd << ", 0x" << hex << imm << dec << endl;
            break;
        }
        case 0x17: { // AUIPC
            int32_t imm = inst & 0xFFFFF000;
            cout << "AUIPC x" << rd << ", 0x" << hex << imm << dec << endl;
            break;
        }
        default:
            cout << "UNKNOWN (opcode: 0x" << hex << opcode << dec << ")" << endl;
            break;
    }
}
