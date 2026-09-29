#include <iostream>
#include <string>
#include <vector>
#include "elf_reader.h"
#include "riscv_decoder.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <elf_file>" << endl;
        return 1;
    }

    string filename = argv[1];
    ElfReader reader(filename);

    if (!reader.parse()) {
        cout << "Failed to parse ELF file." << endl;
        return 1;
    }

    vector<uint8_t> textSection;
    uint64_t startAddress = 0;
    
    if (reader.getTextSection(textSection, startAddress)) {
        RiscVDecoder decoder(textSection, startAddress);
        decoder.decode();
    } else {
        cout << "Nothing to decode." << endl;
    }
    
    return 0;
}
