#include "smp.h"
#include <exec/kernel.h>


static UBYTE *mps_extp = NULL;
static UBYTE *mps_ext_endp = NULL;


UBYTE * find_sig(UBYTE *cp, int len, char *sig)
{
	long i;

	// Search for the "_MP_"  or "$HRT" signature
	for (i = 0; i < len; i += 16)
	{
		if (cp[0] == sig[0] && cp[1] == sig[1] &&
			cp[2] == sig[2] && cp[3] == sig[3])
			return(cp);
		cp += 16;
	}
	return(NULL);
}

static int checksum(UBYTE *cp, int len)
{
	int i;
	unsigned int cksum;

	for (i = cksum = 0; i < len; i++)
		cksum += (unsigned int)*cp++;

	return((int)(cksum & 0xFF));
}


void smp_init(void)
{
	UBYTE *extp;
	struct mps_fps_hdr *fpp = NULL;
	struct mps_ct_hdr *ctp;
	ULONG ebda_start, base_end;
	UWORD ebda_seg, base_size, ext_len, base_len, base_end_seg;

	base_size = *((UWORD *)(0x413));
	ebda_seg = *((UWORD *)(0x40e));
	ebda_start = ((ULONG)ebda_seg) << 4;
	if (ebda_seg != 0)
	{
		fpp = (struct mps_fps_hdr *)find_sig((UBYTE *)ebda_start, 1024, "_MP_");
	}
	if (fpp == NULL)
	{
		base_end_seg = (base_size > 512) ? 0x9FC0 : 0x7FC0;
		if (base_end_seg != ebda_seg)
		{
			base_end = ((ULONG)base_end_seg) << 4;
			fpp = (struct mps_fps_hdr *)find_sig((UBYTE *)base_end, 1024, "_MP_");
		}
	}
	if (fpp == NULL) {
		fpp = (struct mps_fps_hdr *)find_sig((UBYTE *)0xF0000, 0x10000, "_MP_");
	}

	if (fpp == NULL)
	{
		kprintf("SMP", "MP Spec table doesn't exist");
		return;
	}
	else
	{
		kprintf("SMP", "Found MP Floating Pointer Structure at %p\n", (void *)fpp);
	}

	if (checksum((UBYTE *)fpp, fpp->fps_len * 16) != 0)
	{
		kprintf("SMP", "MP Floating Pointer Structure checksum error");
		return;
	}

	ctp = (struct mps_ct_hdr *)(ULONG)fpp->fps_mpct_paddr;
	if (ctp->ct_sig != 0x504d4350)// check "PCMP" signature 
	{
		kprintf("SMP", "MP Configuration Table signature is wrong");
		return;
	}

	base_len = ctp->ct_len;
	if (checksum((UBYTE *)ctp, base_len) != 0)
	{
		kprintf("SMP", "MP Configuration Table checksum error");
		return;
	}
	if (ctp->ct_spec_rev != 4)// not MPSpec rev 1.4 
	{
		kprintf("SMP", "MP Spec 1.1 found - extended table doesn't exist");
		return;
	}
	if ((ext_len = ctp->ct_ext_tbl_len) == 0)
	{
		kprintf("SMP", "MP Spec 1.4 found - extended table doesn't exist");
		return;
	}
	extp = (UBYTE *)ctp + base_len;
	if (((checksum(extp, ext_len) + ctp->ct_ext_cksum) & 0xFF) != 0)
	{
		kprintf("SMP", "MP Extended Table checksum error");
		return;
	}
	mps_extp = extp;
	mps_ext_endp = mps_extp + ext_len;

	kprintf("SMP", "Done !\n");

}
