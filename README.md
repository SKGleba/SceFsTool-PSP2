# SceFsTool-PSP2
A "manager" for PS Vita's EMMC/GameCard/MemoryCard

Heavily based on VitaNDP (https://github.com/SKGleba/Vita-NDP) and YARM-FS-TOOLS

![ref0](https://github.com/SKGleba/SceFsTool-PSP2/raw/master/screens/cmdscefs.png)

# Current functions
- Mount/Unmount partitions as drives
- Extract partitions from the target
- Inject partitions into the target

# Usage

"sudo ./psp2scefstool mode devpath endpath opt1 opt2"

- modes:
  - "-m": mount all partitions in *devpath* to *endpath* OR mount the *opt1* partition in *devpath* to *endpath*
  - "-u": unmount all partitions in *devpath* from *endpath* OR unmount the *opt1* partition in *devpath* from *endpath*
  - "-x": extract all partitions from *devpath* to *endpath* OR extract the *opt1* partition from *devpath* to *endpath*
  - "-i": inject all partitions to *devpath* from *endpath* OR inject the *opt1* partition to *devpath* from *endpath*
  - "-r": display some info about *devpath*
  
- The *devpath* can be:
  - PS Vita's EMMC
  - Game Card for PS Vita
  - Memory Card for PS Vita
  - Any device with SceMBR similar to the one used on PS Vita.
  - Or a 1:1 image file of any of the mentioned devices
  
- *endpath* is the output directory name

- *opt1* is the target partition name [optional]

- Use/set *opt2* ONLY if the partition is active, if *opt2* is not set the job will be applied to the inactive partition

# Examples
- "sudo ./psp2scefstool -m /dev/mmcblk0 kappa"
  - All exFAT/FAT16 partitions from the device located in /dev/mmcblk0 will be mounted to the kappa/partition_name directory
- "sudo ./psp2scefstool -u mc_dump.img ligma ux0"
  - The ux0 partition mounted to the ligma/ux0-0 directory will be unmounted
- "sudo ./psp2scefstool -x emmc.bin sogma slb2 active"
  - The active slb2 will be extracted from emmc.bin to sogma/slb2-1
- "sudo ./psp2scefstool -i gc_raw.x updog"
  - All partitions in updog/ will be injected into gc_raw.x
  
# Screens

![ref1](https://github.com/SKGleba/SceFsTool-PSP2/raw/master/screens/scefsfmgr.png)
![ref2](https://github.com/SKGleba/SceFsTool-PSP2/raw/master/screens/scefsplain.png)
![ref3](https://github.com/SKGleba/SceFsTool-PSP2/raw/master/screens/scefsux.png)

# Notes
- It is still in beta stage, linux only for now.
- Use with caution.
- Sony uses TexFat so for exFat partitions you need exfat-nofuse
- The devices are mounted with RW perms
- Big thanks to:
  - Zecoxao
  - Team Molecule
  - Mathieulh
- For any help/serious issue DM me on twitter (twitter.com/skgleba).
- If you want to help me by donating: paypal.me/skgleba (^_^ thx).
