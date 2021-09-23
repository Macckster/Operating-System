kernel/bin/CustomOS.img: gnu-efi\x86_64\bootloader\main.efi kernel/bin/kernel.elf
	$(MAKE) -C kernel buildimg
	
gnu-efi\x86_64\bootloader\main.efi:
	$(MAKE) -C gnu-efi bootloader
	
kernel/bin/kernel.elf:
	$(MAKE) -C kernel kernel