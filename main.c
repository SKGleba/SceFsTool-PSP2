// Say hello to BuggyTool beta2
/*
    SceFsTool-PSP2 by SKGleba
    All Rights Reserved
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdint.h>

#define ARRAYSIZE(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#define BLOCK_SIZE 0x200

typedef struct {
	uint32_t off;
	uint32_t sz;
	uint8_t code;
	uint8_t type;
	uint8_t active;
	uint32_t flags;
	uint16_t unk;
} __attribute__((packed)) partition_t;

typedef struct {
	char magic[0x20];
	uint32_t version;
	uint32_t device_size;
	char unk1[0x28];
	partition_t partitions[0x10];
	char unk2[0x5e];
	char unk3[0x10 * 4];
	uint16_t sig;
} __attribute__((packed)) master_block_t;

const char *part_code(int code) {
	static char *codes[] = {
		"empty",
		"idstorage",
		"slb2",
		"os0",
		"vs0",
		"vd0",
		"tm0",
		"ur0",
		"ux0",
		"gro0",
		"grw0",
		"ud0",
		"sa0",
		"mediaid",
		"pd0",
		"unused"
	};
	return codes[code];
}

const char *part_type(int type) {
	if (type == 6)
		return "FAT16";
	else if (type == 7)
		return "exFAT";
	else if (type == 0xDA)
		return "raw";
	return "unknown";
}

int read(master_block_t *master, const char *src) {
 int nandump = 0;
 uint32_t idoff;
 static char devinfo[82];
 printf("\nMBR:\n %s\n version: %d\n device size: 0x%08x\n", master->magic, master->version, master->device_size * 0x200);
 printf("\nPartitions:\n");
	for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
		partition_t *p = &master->partitions[i];
		  if (p->code != 0) {
			printf("0x%X:\n name: %s active=%d\n offset=0x%08x size=0x%08x\n type=%s flags=0x%08x\n\n", p->code, part_code(p->code), p->active, p->off, p->sz, part_type(p->type), p->flags);
	      }
	      if (p->code == 1) {
	            nandump = 1;
	            idoff = p->off * 0x200;
	      }
	}
 if (nandump == 1) {
        	FILE *fp = fopen(src,"rb");
			fseek(fp,idoff + 0x14200,SEEK_SET);
			fread(devinfo,1,81,fp);
			fclose(fp);
			printf("Device info (idstorage):\n %s\n\n", devinfo);
 }
        
}

int pack(uint32_t off, uint32_t size, const char *src, const char *dst){
			unsigned char * buffer = (unsigned char *) malloc (size);
			FILE *fp = fopen(src,"rb");
			fread(buffer,1,size,fp);
			fclose(fp);
			FILE *fl = fopen(dst, "wb");
			fseek(fl,off,SEEK_SET);
			fwrite(buffer,1,size,fl);
			fclose(fl);
			free(buffer);
}

int packloop(master_block_t *master, const char *src, const char *dst){
 static char outfile[64];
	for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
		partition_t *p = &master->partitions[i];
		  if (p->code != 0) {
			printf("Packing partition %s active=%d offset 0x%08x size 0x%08x...\n", part_code(p->code), p->active, p->off, p->sz);
			sprintf(outfile, "%s/%s-%d", dst, part_code(p->code), p->active);
            pack(p->off * BLOCK_SIZE, p->sz * BLOCK_SIZE, outfile, src);
	      }
	}
}

int packone(master_block_t *master, const char *src, const char *dst, const char *name, int act){
 static char outfile[64];
	for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
		partition_t *p = &master->partitions[i];
		  if (p->code != 0 && strcmp(part_code(p->code),name)==0 && p->active == act) {
			printf("Packing partition %s active=%d offset 0x%08x size 0x%08x...\n", part_code(p->code), p->active, p->off, p->sz);
			sprintf(outfile, "%s/%s-%d", dst, part_code(p->code), p->active);
            pack(p->off * BLOCK_SIZE, p->sz * BLOCK_SIZE, outfile, src);
	      }
	}
}

int unpack(uint32_t off, uint32_t size, const char *src, const char *dst){
			unsigned char * buffer = (unsigned char *) malloc (size);
			FILE *fp = fopen(src,"rb");
			fseek(fp,off,SEEK_SET);
			fread(buffer,1,size,fp);
			fclose(fp);
			FILE *fl = fopen(dst, "wb");
			fwrite(buffer,1,size,fl);
			fclose(fl);
			free(buffer);
}

int unpackloop(master_block_t *master, const char *src, const char *dst){
 static char outfile[64];
	for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
		partition_t *p = &master->partitions[i];
		  if (p->code != 0) {
			printf("Unpacking partition %s active=%d offset 0x%08x size 0x%08x...\n", part_code(p->code), p->active, p->off, p->sz);
			sprintf(outfile, "%s/%s-%d", dst, part_code(p->code), p->active);
            unpack(p->off * BLOCK_SIZE, p->sz * BLOCK_SIZE, src, outfile);
	      }
	}
}

int unpackone(master_block_t *master, const char *src, const char *dst, const char *name, int act){
 static char outfile[64];
	for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
		partition_t *p = &master->partitions[i];
		  if (p->code != 0 && strcmp(part_code(p->code),name)==0 && p->active == act) {
			printf("Unpacking partition %s active=%d offset 0x%08x size 0x%08x...\n", part_code(p->code), p->active, p->off, p->sz);
			sprintf(outfile, "%s/%s-%d", dst, part_code(p->code), p->active);
            unpack(p->off * BLOCK_SIZE, p->sz * BLOCK_SIZE, src, outfile);
	      }
	}
}

int mount(uint32_t off, uint32_t size, const char *src, const char *dst, int mode){
 //Make sure that your exfat driver supports TexFat
            static char cmd[256];
            if (mode == 0) {
                sprintf(cmd, "mount -o loop,offset=0x%X,sizelimit=0x%X %s %s", off * 0x200, size * 0x200, src, dst);
                printf("Mounting 0x%08x to %s\n", off, dst);
            } else if (mode == 1) {
                sprintf(cmd, "umount %s", dst);
                printf("uMounting 0x%08x from %s\n", off, dst);
            }
            system(cmd);
	
}

int mountloop(master_block_t *master, const char *src, const char *dst, int mode){
 int c = 3;
 static char mkcmd[64];
 static char fullpath[32];
    while (c < 0xF) {
        if (c == 0xD) c = 0xE;
	        for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
	            partition_t *p = &master->partitions[i];
                if (p->code == c) {
                    if (mode == 0) {
                        sprintf(mkcmd, "mkdir %s/%s-%d", dst, part_code(c), p->active);
                        system(mkcmd);
                    }
                    sprintf(fullpath, "%s/%s-%d", dst, part_code(c), p->active);
                    mount(p->off, p->sz, src, fullpath, mode);
                    if (mode == 1) {
                        sprintf(mkcmd, "rmdir %s/%s-%d", dst, part_code(c), p->active);
                        system(mkcmd);
                    }
                }
	        }
	  c++;
	}
}

int mountone(master_block_t *master, const char *src, const char *dst, int mode, const char *name, int act){
 int c = 3;
 static char mkcmd[64];
 static char fullpath[32];
    while (c < 0xF) {
        if (c == 0xD) c = 0xE;
	        for (size_t i = 0; i < ARRAYSIZE(master->partitions); ++i) {
	            partition_t *p = &master->partitions[i];
                if (p->code == c && strcmp(part_code(p->code),name)==0 && p->active == act) {
                    if (mode == 0) {
                        sprintf(mkcmd, "mkdir %s/%s-%d", dst, part_code(c), p->active);

                        system(mkcmd);
                    }
                    sprintf(fullpath, "%s/%s-%d", dst, part_code(c), p->active);
                    mount(p->off, p->sz, src, fullpath, mode);
                    if (mode == 1) {
                        sprintf(mkcmd, "rmdir %s/%s-%d", dst, part_code(c), p->active);
                        system(mkcmd);
                    }
                }
	        }
	  c++;
	}
}

int main(int argc, char **argv){
	
	if(argc < 4){
		printf ("\nusage: psp2scefstool -mode devpath endpath opt1 opt2\nModes: [-m | -u | -x | -i | -r]\n-m: mount\n-u: unmount\n-x: extract/unpack\n-i: inject/pack\n-r: display device info\nex: psp2scefstool -m /dev/mmcblk0 /mnt/work\n\n");
		return -1;
	}

    int mode = 0;
    
    if (strcmp("-u", argv[1]) == 0){
         mode = 1;
    } else if (strcmp("-x", argv[1]) == 0){
         mode = 2;
    } else if (strcmp("-i", argv[1]) == 0){
         mode = 3;
    } else if (strcmp("-r", argv[1]) == 0){
         mode = 4;
    }


	static char mkkk[32];
	if (mode != 1 && mode != 3 && mode != 4) {
	sprintf(mkkk, "mkdir %s", argv[3]);
	system(mkkk);
	}
	
	FILE *fp = fopen(argv[2],"rb");
	
	static master_block_t master;
	fread(&master,1,sizeof(master),fp);
	fclose(fp);
	
    if (mode < 2) {
        if (argc == 4)  {
            mountloop(&master, argv[2], argv[3], mode);
        } else if (argc == 5) {
            mountone(&master, argv[2], argv[3], mode, argv[4], 0);
        } else if (argc == 6) {
            mountone(&master, argv[2], argv[3], mode, argv[4], 1);
        }
    } else if (mode == 2) {
        if (argc == 4)  {
            unpackloop(&master, argv[2], argv[3]);
        } else if (argc == 5) {
            unpackone(&master, argv[2], argv[3], argv[4], 0);
        } else if (argc == 6) {
            unpackone(&master, argv[2], argv[3], argv[4], 1);
        }
    } else if (mode == 3) {
        if (argc == 4)  {
            packloop(&master, argv[2], argv[3]);
        } else if (argc == 5) {
            packone(&master, argv[2], argv[3], argv[4], 0);
        } else if (argc == 6) {
            packone(&master, argv[2], argv[3], argv[4], 1);
        }
    } else if (mode == 4) {
        read(&master, argv[2]);
    }
    
    if (mode == 1) {
	sprintf(mkkk, "rmdir %s", argv[3]);
	system(mkkk);
	}
	
	return 0;
}
