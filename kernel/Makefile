
OSNAME = CustomOS

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kernel.ld
CC = gcc
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -DCOMPILE_TIME=`date '+%s'`
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)          
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
DIRS = $(wildcard $(SRCDIR)/*)

kernel: $(OBJS) $(BUILDDIR)/kernel.elf

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@ echo ==== COMPILING $^ ====
	@ mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@
	
$(BUILDDIR)/kernel.elf: $(OBJS)
	@ echo ==== LINKING ====
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kernel.elf $(OBJS)

buildimg: $(BUILDDIR)/$(OSNAME).img

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

$(BUILDDIR)/$(OSNAME).img: kernel
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img -f 2880 ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kernel.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light16.psf ::

clean:
	rm -rf $(BUILDDIR)/kernel.elf
	rm -rf $(OBJDIR)/*.o
	rm -rf $(OBJDIR)/gfx/*.o
	rm -rf $(OBJDIR)/memory/*.o
	rm -rf $(OBJDIR)/util/*.o
	rm -rf $(BUILDDIR)/$(OSNAME).img
