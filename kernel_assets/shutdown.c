typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

static inline void outb(uint16_t port, uint8_t val)  { __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port)); }
static inline void outw(uint16_t port, uint16_t val)  { __asm__ volatile ("outw %0, %1" : : "a"(val), "Nd"(port)); }
static inline uint16_t inw(uint16_t port)           { uint16_t r; __asm__ volatile ("inw %1, %0" : "=a"(r) : "Nd"(port)); return r; }

int memcmp(const char *s1, const char *s2, int n) {
    for (int i = 0; i < n; i++) if (s1[i] != s2[i]) return 1;
    return 0;
}

uint16_t find_s5_type(uint32_t dsdt_addr) {
    char *ptr = (char*)dsdt_addr;
    uint32_t len = *(uint32_t*)(dsdt_addr + 4);
    
    for (uint32_t i = 0; i < len - 4; i++) {
        if (memcmp(&ptr[i], "_S5_", 4) == 0) {
            ptr += i + 4;
            ptr += 4; 
            if (*ptr == 0x0A) ptr++;
            return (uint16_t)*ptr;
        }
    }
    return 5;
}

void shutdown() {
    uint32_t pm1a_cnt = 0;
    uint16_t s5_type = 5;

    for (uint32_t addr = 0xE0000; addr < 0x100000; addr += 16) {
        if (memcmp((char*)addr, "RSD PTR ", 8) == 0) {
            uint32_t rsdt_addr = *(uint32_t*)(addr + 16);
            uint32_t *rsdt = (uint32_t*)rsdt_addr;
            int entries = (rsdt[1] - 36) / 4;

            for (int i = 0; i < entries; i++) {
                uint32_t entry = rsdt[9 + i];
                if (memcmp((char*)entry, "FACP", 4) == 0) {
                    uint32_t smi_cmd = *(uint32_t*)(entry + 48);
                    uint8_t acpi_en = *(uint8_t*)(entry + 52);
                    pm1a_cnt = *(uint32_t*)(entry + 64);
                    uint32_t dsdt = *(uint32_t*)(entry + 40);
                    s5_type = find_s5_type(dsdt);
                    if (smi_cmd != 0 && (inw(pm1a_cnt) & 1) == 0) {
                        outb(smi_cmd, acpi_en);
                        for(volatile int w=0; w<100000; w++); 
                    }
                    break;
                }
            }
            break;
        }
    }

    if (pm1a_cnt != 0) {
        uint16_t slp_en = (1 << 13);
        uint16_t cmd = (s5_type << 10) | slp_en;
        
        outw(pm1a_cnt, cmd);
    }

    while(1) { __asm__("hlt"); }
}
