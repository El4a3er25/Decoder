#ifndef RISCV_DECODER_H
#define RISCV_DECODER_H

#include <vector>
#include <cstdint>

class RiscVDecoder {
private:
    std::vector<uint8_t> instructionData;
    uint64_t baseAddress;

    void decodeInstruction(uint32_t inst, uint64_t address);

public:
    RiscVDecoder(const std::vector<uint8_t>& data, uint64_t addr);
    void decode();
    // Made public for testing purposes
    void decodeInstruction(uint32_t inst, uint64_t address);
};

#endif
