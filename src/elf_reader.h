#ifndef ELF_READER_H
#define ELF_READER_H

#include <string>
#include <vector>
#include <cstdint>

// Basic ELF 64 structures
struct Elf64_Ehdr {
    uint8_t  e_ident[16];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Elf64_Shdr {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
};

class ElfReader {
private:
    std::string filename;
    std::vector<uint8_t> fileData;

    bool readFile();

public:
    ElfReader(std::string name);
    bool parse();
    
    // New function to get .text section
    bool getTextSection(std::vector<uint8_t>& textData, uint64_t& startAddress);
};

#endif
