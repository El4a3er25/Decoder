import struct

instructions = [0x00a00093, 0x00108133, 0x00010463, 0xff5ff06f]
text_data = b''.join(struct.pack('<I', inst) for inst in instructions)

shstrtab = b'\x00.text\x00.shstrtab\x00'

# Align to 8 bytes
def align8(data):
    rem = len(data) % 8
    if rem != 0:
        return data + b'\x00' * (8 - rem)
    return data

text_data = align8(text_data)
shstrtab = align8(shstrtab)

elf_header = bytearray(64)
elf_header[0:4] = b'\x7fELF'
elf_header[4] = 2 # 64-bit
elf_header[5] = 1 # little endian
elf_header[6] = 1 # version
elf_header[16:18] = struct.pack('<H', 2) # e_type = EXEC
elf_header[18:20] = struct.pack('<H', 243) # e_machine = RISC-V
elf_header[20:24] = struct.pack('<I', 1) # e_version
elf_header[24:32] = struct.pack('<Q', 0x10000) # e_entry
elf_header[32:40] = struct.pack('<Q', 0) # e_phoff

text_offset = 64
shstrtab_offset = text_offset + len(text_data)
shdrs_offset = shstrtab_offset + len(shstrtab)

elf_header[40:48] = struct.pack('<Q', shdrs_offset) # e_shoff
elf_header[52:54] = struct.pack('<H', 64) # e_ehsize
elf_header[54:56] = struct.pack('<H', 0) # e_phentsize
elf_header[56:58] = struct.pack('<H', 0) # e_phnum
elf_header[58:60] = struct.pack('<H', 64) # e_shentsize
elf_header[60:62] = struct.pack('<H', 3) # e_shnum
elf_header[62:64] = struct.pack('<H', 2) # e_shstrndx

shdrs = bytearray(3 * 64)

# shdr 1: .text
shdrs[64:68] = struct.pack('<I', 1) # sh_name
shdrs[68:72] = struct.pack('<I', 1) # sh_type
shdrs[72:80] = struct.pack('<Q', 6) # sh_flags
shdrs[80:88] = struct.pack('<Q', 0x10000) # sh_addr
shdrs[88:96] = struct.pack('<Q', text_offset) # sh_offset
shdrs[96:104] = struct.pack('<Q', len(text_data)) # sh_size

# shdr 2: .shstrtab
shdrs[128:132] = struct.pack('<I', 7) # sh_name
shdrs[132:136] = struct.pack('<I', 3) # sh_type
shdrs[144:152] = struct.pack('<Q', 0) # sh_addr
shdrs[152:160] = struct.pack('<Q', shstrtab_offset) # sh_offset
shdrs[160:168] = struct.pack('<Q', len(shstrtab)) # sh_size

with open('test_prog.elf', 'wb') as f:
    f.write(elf_header)
    f.write(text_data)
    f.write(shstrtab)
    f.write(shdrs)
