%ifndef _COMMON_INC_
%define _COMMON_INC_

; where the kernel is to be loaded to in protected mode
%define IMAGE_PMODE_BASE 0xC0000000

; where the kernel is to be loaded to in real mode
%define IMAGE_RMODE_BASE 0x3000

; kernel name
ImageName     db "KERNEL  EXE"

ImageSize     db 0

%endif
