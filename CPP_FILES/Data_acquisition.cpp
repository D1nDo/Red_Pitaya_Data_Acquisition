#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <cstring>
#include <stdint.h>
#include <fstream>
#include <cmath>

#define DMA_BASE 0x40400000
#define MAP_SIZE 0x1000

#define S2MM_DMACR 0x30
#define S2MM_DMASR 0x34
#define S2MM_CURDESC 0x38
#define S2MM_TAILDESC 0x40

#define XAXIDMA_BD_CTRL_TXSOF_MASK 0x08000000
#define XAXIDMA_BD_CTRL_TXEOF_MASK 0x04000000

#define DDR_BASE 0x1FC00000
#define RX_BUFFER_BASE (DDR_BASE + 0x000000) 
#define S2MM_BD_BASE   (DDR_BASE + 0x200000) 

#define SAMPLES_PER_BLOCK 512
#define BLOCK_SIZE (SAMPLES_PER_BLOCK * 4) 

#define BD_SPACE_BYTES 64 
#define BD_SPACE_WORDS (BD_SPACE_BYTES / 4)

/*
 * =====================================================================================
 * AXI DMA Scatter-Gather (S2MM) Data Acquisition
 * * Description:
 * This program performs high-speed data acquisition from a custom hardware IP to 
 * the DDR memory using the Xilinx AXI DMA engine in Scatter-Gather mode. 
 * * Flow:
 * 1. Takes the user's requested number of samples and rounds them up to fit 
 * 512-sample (2048 bytes) blocks.
 * 2. Maps the physical memory of the DMA registers, RX Data Buffer, and Buffer 
 * Descriptors (BD) into the user space via /dev/mem.
 * 3. Builds a linear chain of Buffer Descriptors in the mapped memory.
 * 4. Resets the DMA and triggers the S2MM (Stream-to-Memory-Mapped) transfer by 
 * setting the CURDESC and TAILDESC registers.
 * 5. Polls the 'Complete' bit of the last Buffer Descriptor to detect the end 
 * of the transfer (with a 5-second timeout safeguard).
 * 6. Flushes the acquired binary data directly into a file (/tmp/external_data.bin).
 * =====================================================================================
 */

int main()
{
    long requested_samples = 0;
    std::cout << "How many samples do you want to acquire? ";
    std::cin >> requested_samples;

    if (requested_samples <= 0) {
        std::cout << "Invalid number!\n";
        return -1;
    }

    // Calcolo dinamico dei blocchi necessari
    int target_blocks = std::ceil((double)requested_samples / SAMPLES_PER_BLOCK);
    long actual_samples = target_blocks * SAMPLES_PER_BLOCK;

    std::cout << "-> Rounded to " << target_blocks << " blocks (Total: " << actual_samples << " samples).\n\n";

    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) return -1;

    volatile uint32_t *dma = (uint32_t*) mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, DMA_BASE);
    
    // Mappiamo esattamente lo spazio RAM per gli N blocchi
    size_t data_mapping_size = target_blocks * BLOCK_SIZE;
    volatile uint32_t *rx_data = (uint32_t*) mmap(NULL, data_mapping_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, RX_BUFFER_BASE);
    
    // Mappiamo esattamente lo spazio per gli N descrittori
    size_t bd_mapping_size = target_blocks * BD_SPACE_BYTES;
    volatile uint32_t *s2mm_bd = (uint32_t*) mmap(NULL, bd_mapping_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, S2MM_BD_BASE);

    // Pulizia RAM
    memset((void*)rx_data, 0, data_mapping_size);
    memset((void*)s2mm_bd, 0, bd_mapping_size);

    std::ofstream outfile("/tmp/external_data.bin", std::ios::binary);
    if(!outfile.is_open()) return -1;

    std::cout << "Configuring chain of " << target_blocks << " descriptors...\n";

    // Creiamo la catena lineare
    for(int i = 0; i < target_blocks; i++) {
        uint32_t next_addr_rx = (i == target_blocks - 1) ? 0 : (S2MM_BD_BASE + ((i + 1) * BD_SPACE_BYTES));

        s2mm_bd[i * BD_SPACE_WORDS + 0] = next_addr_rx;
        s2mm_bd[i * BD_SPACE_WORDS + 2] = RX_BUFFER_BASE + (i * BLOCK_SIZE);
        s2mm_bd[i * BD_SPACE_WORDS + 6] = BLOCK_SIZE | XAXIDMA_BD_CTRL_TXSOF_MASK | XAXIDMA_BD_CTRL_TXEOF_MASK;
    }

    std::cout << "DMA Reset...\n";
    dma[S2MM_DMACR/4] = 4; 
    
    int reset_timeout = 0;
    while ((dma[S2MM_DMACR/4] & 4) && reset_timeout < 10000) {
        usleep(10);
        reset_timeout++;
    }
    usleep(10000); 

    // Avvio classico del DMA Scatter-Gather
    dma[S2MM_CURDESC/4] = S2MM_BD_BASE;
    dma[S2MM_DMACR/4] = 0x1; 

    std::cout << "Launching acquisition...\n";
    
    // Carichiamo il TAILDESC per dare il via
    dma[S2MM_TAILDESC/4] = S2MM_BD_BASE + ((target_blocks - 1) * BD_SPACE_BYTES);

    std::cout << "Waiting for DMA completion...\n";
    
    int last_bd_idx = (target_blocks - 1) * BD_SPACE_WORDS;
    
    // Polling sull'ultimo descrittore con timeout di sicurezza di 5 secondi
    int wait_timeout = 0;
    while (!(s2mm_bd[last_bd_idx + 7] & 0x80000000)) {
        usleep(1000); 
        wait_timeout++;
        
        if (wait_timeout > 5000) {
            std::cout << "Timeout! The hardware didn't send enough data or TLAST was missing.\n";
            break;
        }
    }

    std::cout << "DMA finished! Saving to file...\n";

    // Usiamo il doppio cast per risolvere il problema del compilatore sul puntatore volatile
    outfile.write(reinterpret_cast<const char*>(const_cast<const uint32_t*>(rx_data)), data_mapping_size);

    std::cout << "Done! File saved in /tmp/external_data.bin (" << (target_blocks * BLOCK_SIZE) / 1024 << " KB)\n";
    outfile.close();

    munmap((void*)dma, MAP_SIZE); munmap((void*)rx_data, data_mapping_size);
    munmap((void*)s2mm_bd, bd_mapping_size); close(fd);
    
    return 0;
}
