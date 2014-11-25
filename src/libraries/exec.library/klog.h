#ifndef KLOG_H
#define	KLOG_H


#define KERNEL_PRINT_TAG_LENGTH		6					// Length of the tag for kprintf() 



VOID kputc(UBYTE c);
VOID kputs(STRPTR str);



#endif // !KLOG_H
