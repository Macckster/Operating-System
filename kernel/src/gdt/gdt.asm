[bits 64]
LoadGDT:
	lgdt [rdi]
	mov ax, 0x10 ;
	mov ds, ax	 ;
	mov es, ax   ; ---- Move kernel data selector into a bunch of stuff
	mov fs, ax	 ;
	mov gs, ax	 ;
	mov ss, ax	 ;

	pop rdi ; pop return address
	mov rax, 0x08 ; move kernel data selector to rax
	push rax ; push kernel code selector to stack
	push rdi ; push return address
	retfq ; far 64 bit return

GLOBAL LoadGDT