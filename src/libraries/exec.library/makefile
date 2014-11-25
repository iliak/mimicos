PROGRAM 		= 	exec
OBJ_DIR 		= 	debug/
SRC_DIR 		=	$(shell) *.c $(shell) api/*.c


DEFINE 			= 	/D "ARCH_X86;_DEBUG"
INCLUDE 		= 	/I "../../Include" \
					/I "./"


########### Compiler
CC 				= 	cl
# http://msdn.microsoft.com/en-us/library/fwkeyyhe.aspx
CFLAGS 			= 	/c /X /MP /GS- /TC /analyze- /W4 /wd"4214" /wd"4206" /wd"4100" /Gm- /Od /Fd"$(OBJ_DIR)$(PROGRAM).pdb" \
				  	/FI"config.h" /fp:precise /Zp1 /errorReport:none /GF- /WX- /Zc:forScope /GR- /Gd /MD  \
				  	/FAsu /Fa"$(OBJ_DIR)" /Zl /Fo"$(OBJ_DIR)" $(INCLUDE) $(DEFINE)


########### Linker
LIBS			= 
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\input.device.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\keyboard.device.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\serial.device.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\timer.device.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\trackdisk.device.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\dos.library.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\expansion.library.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\graphics.library.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\hal.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\intuition.library.lib" \
#					"D:\dev\mimicprod\mimicos\SysCore\Debug\libc.lib" 				
LD				=	link
# http://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx
LDFLAGS			=	/OUT:$(OBJ_DIR)$(PROGRAM).exe /MANIFEST:NO /DRIVER /DYNAMICBASE /MAPINFO:EXPORTS /MAP:$(OBJ_DIR)$(PROGRAM).map \
					/FIXED:NO /BASE:"0x100000" /MACHINE:X86 /ENTRY:"kernel_entry" /OPT:REF /OPT:ICF /SAFESEH /INCREMENTAL:NO \
					/SUBSYSTEM:NATIVE /DEBUG /errorReport:none /NOLOGO /NODEFAULTLIB /ASSEMBLYDEBUG:DISABLE /TLBID:1 \
					$(LIBS)
#					/PGD:$(OBJ_DIR)kernel.pgd 


# {dbi\}.cpp{$(ODIR)}.obj::
#         $(CC) $(CFLAGS) $(YUDBI) $<


########### build a list of all .obj files
OBJS = $(SRC_DIR:.c=.obj)
# OBJS = $(OBJ_DIR)*.obj
# OBJS = $(shell) $(OBJ_DIR)*.obj

all: $(PROGRAM)

########### how to link .obj files to create an executable
$(PROGRAM): $(OBJS)
#	@echo $(OBJS)
#	pushd
#	@cd $(OBJ_DIR)
#	@$(LD) $(OBJS) $(LDFLAGS)
#	$(LD) $(OBJ_DIR) $(LDFLAGS)
	$(LD) $(shell) $(OBJ_DIR)*.obj $(LDFLAGS)
#	popd

.c.obj:
	@if not exist "$(OBJ_DIR)"	mkdir "$(OBJ_DIR)"
	$(CC) $(CFLAGS) /Fe$(OBJ_DIR)$(TARGET_NAME) $*.c

clean:
	@if exist $(OBJ_DIR)		rd /Q /S "$(OBJ_DIR)"
