#include "elf_reader.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

ElfReader::ElfReader(string name) : filename(name) {}

bool ElfReader::readFile() {
    ifstream file(filename, ios::binary | ios::ate);
    if (!file.is_open()) {
        cout << "Error opening file: " << filename << endl;
        return false;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    fileData.resize(size);
    if (!file.read((char*)fileData.data(), size)) {
        return false;
    }

    return true;
}

bool ElfReader::parse() {
    if (!readFile()) {
        return false;
    }

    if (fileData.size() < sizeof(Elf64_Ehdr)) {
        cout << "File is too small to be an ELF64" << endl;
        return false;
    }

    Elf64_Ehdr* header = (Elf64_Ehdr*)fileData.data();

    // check ELF magic numbers
    if (header->e_ident[0] != 0x7f || header->e_ident[1] != 'E' || 
        header->e_ident[2] != 'L' || header->e_ident[3] != 'F') {
        cout << "Not a valid ELF file" << endl;
        return false;
    }
    
    // check if it's 64-bit (class 2)
    if (header->e_ident[4] != 2) {
        cout << "Not a 64-bit ELF file" << endl;
        return false;
    }
    
    // check if it's RISC-V (machine 0xF3)
    if (header->e_machine != 0xF3) {
        cout << "Warning: Not a RISC-V architecture file" << endl;
    }

    cout << "Valid RISC-V ELF file found." << endl;
    
    return true;
}

bool ElfReader::getTextSection(vector<uint8_t>& textData, uint64_t& startAddress) {
    Elf64_Ehdr* header = (Elf64_Ehdr*)fileData.data();
    
    Elf64_Shdr* sections = (Elf64_Shdr*)(fileData.data() + header->e_shoff);
    Elf64_Shdr* strTableSection = &sections[header->e_shstrndx];
    char* strTable = (char*)(fileData.data() + strTableSection->sh_offset);
    
    for (int i = 0; i < header->e_shnum; i++) {
        char* name = strTable + sections[i].sh_name;
        if (strcmp(name, ".text") == 0) {
            uint64_t offset = sections[i].sh_offset;
            uint64_t size = sections[i].sh_size;
            
            textData.clear();
            for(uint64_t j = 0; j < size; j++) {
                textData.push_back(fileData[offset + j]);
            }
            startAddress = sections[i].sh_addr;
            return true;
        }
    }
    
    cout << "Could not find .text section" << endl;
    return false;
}
