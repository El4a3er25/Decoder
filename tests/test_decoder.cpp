#include <gtest/gtest.h>
#include "../src/riscv_decoder.h"
#include <vector>
#include <string>

using namespace std;

TEST(DecoderTest, DecodeADDI) {
    vector<uint8_t> dummy;
    RiscVDecoder decoder(dummy, 0);
    
    testing::internal::CaptureStdout();
    decoder.decodeInstruction(0x00a00093, 0x10000); // ADDI x1, x0, 10
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "0x10000: ADDI x1, x0, 10\n");
}

TEST(DecoderTest, DecodeADD) {
    vector<uint8_t> dummy;
    RiscVDecoder decoder(dummy, 0);
    
    testing::internal::CaptureStdout();
    decoder.decodeInstruction(0x00108133, 0x10004); // ADD x2, x1, x1
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "0x10004: ADD x2, x1, x1\n");
}

TEST(DecoderTest, DecodeBEQ) {
    vector<uint8_t> dummy;
    RiscVDecoder decoder(dummy, 0);
    
    testing::internal::CaptureStdout();
    decoder.decodeInstruction(0x00010463, 0x10008); // BEQ x2, x0, 8
    string output = testing::internal::GetCapturedStdout();
    
    EXPECT_EQ(output, "0x10008: BEQ x2, x0, 8\n");
}
