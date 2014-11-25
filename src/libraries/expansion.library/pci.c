

#include "pci.h"
//#include "../../../expansion.library/pci_db.h"


#define PCI_CONFIG_ADDRESS	0x0CF8
#define PCI_CONFIG_DATA		0x0CFC


UWORD pci_getVendorID(UWORD bus, UWORD device, UWORD function);
UWORD pci_getDeviceID(UWORD bus, UWORD device, UWORD function);
UWORD pci_getClassId(UWORD bus, UWORD device, UWORD function);
UWORD pci_getSubClassId(UWORD bus, UWORD device, UWORD function);
STRPTR pci_getStringOfClass(UBYTE c);
UWORD pci_config_read(UWORD bus, UWORD slot, UWORD func, UWORD offset);
STRPTR pci_class_to_string(UBYTE baseclass, UBYTE subclass);

//
//
//
void pci_scan(void)
{
	kprintf("PCI", "scanning bus...\n");

	ULONG count = 0;
	for (int bus = 0; bus < 256; bus++)
	{
		for (int slot = 0; slot < 32; slot++)
		{
			for (int function = 0; function < 8; function++)
			{
				UWORD vendor = pci_getVendorID(bus, slot, function);
				if (vendor == 0xFFFF)
					continue;

				UWORD device = pci_getDeviceID(bus, slot, function);
				UWORD baseclass = pci_getClassId(bus, slot, function);
				UWORD sub = pci_getSubClassId(bus, slot, function);
				//STRPTR desc = pci_getStringOfClass(class);
				STRPTR desc = pci_class_to_string(baseclass, sub);

				kprintf("PCI", "[%02X:%02X.%i] Vendor: [%04X:%04X]  Class: [%02X:%02X] \"%s\"\n", bus, slot, function, vendor, device, baseclass, sub, desc);
				console_printf("[%02X:%02X.%i] Vendor: [%04X:%04X]  Class: [%02X:%02X] \"%s\"\n", bus, slot, function, vendor, device, baseclass, sub, desc);

				count++;
			}
		}
	}

	kprintf("PCI", "%i pci device(s) found !\n");
	console_printf("%i pci device(s) found !\n");
}


//
//
//
UWORD pci_config_read(UWORD bus, UWORD slot, UWORD func, UWORD offset)
{
	ULONG lbus = (ULONG)bus;
	ULONG lslot = (ULONG)slot;
	ULONG lfunc = (ULONG)func;

	ULONG address = (ULONG)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((ULONG)0x80000000));
	outportw(PCI_CONFIG_ADDRESS, address);

	return (UWORD)((inportw(PCI_CONFIG_DATA) >> ((offset & 2) * 8)) & 0xffff);
}



//
//
//
UWORD pci_getVendorID(UWORD bus, UWORD device, UWORD function)
{
	return pci_config_read(bus, device, function, 0);
}


//
//
//
UWORD pci_getDeviceID(UWORD bus, UWORD device, UWORD function)
{
	return pci_config_read(bus, device, function, 2);
}


//
//
//
UWORD pci_getClassId(UWORD bus, UWORD device, UWORD function)
{
	int r = pci_config_read(bus, device, function, 0xA);
	return (r & ~0x00FF) >> 8;
}


//
//
//
UWORD pci_getSubClassId(UWORD bus, UWORD device, UWORD function)
{
	int r = pci_config_read(bus, device, function, 0xA);
	return (r & ~0xFF00);
}


//
//
//
STRPTR pci_class_to_string(UBYTE baseclass, UBYTE subclass)
{
	switch (baseclass)
	{
		case 0x00: switch (subclass)
		{
			default: return "Unknown Device";
			case 0x00: return "Non-VGA unclassified device";
			case 0x01: return "VGA-Compatible Device";
		}
		case 0x01: switch (subclass)
		{
			default: return "Unknown Mass Storage Controller";
			case 0x00: return "SCSI Bus Controller";
			case 0x01: return "IDE Controller";
			case 0x02: return "Floppy Disk Controller";
			case 0x03: return "IPI Bus Controller";
			case 0x04: return "RAID Controller";
			case 0x05: return "ATA Controller";
			case 0x06: return "Serial ATA";
			case 0x07: return "Serial Attached SCSI";
			case 0x08: return "Non-Volatile memory controller";
			case 0x80: return "Mass storage controller";
		}
		case 0x02: switch (subclass)
		{
			default: return "Unknown Network Controller";
			case 0x00: return "Ethernet Controller";
			case 0x01: return "Token Ring Controller";
			case 0x02: return "FDDI Controller";
			case 0x03: return "ATM Controller";
			case 0x04: return "ISDN Controller";
			case 0x05: return "WorldFip Controller";
			case 0x06: return "PICMG 2.14 Multi Computing";
			case 0x80: return "Network controller";
		}
		case 0x03: switch (subclass)
		{
			default: return "Unknown Display Controller";
			case 0x00: return "VGA-Compatible Controller";
			case 0x01: return "XGA Controller";
			case 0x02: return "3D Controller";
			case 0x80: return "Display controller";
		}
		case 0x04: switch (subclass)
		{
			default: return "Unknown Multimedia Controller";
			case 0x00: return "Multimedia video controller";
			case 0x01: return "Multimedia audio controller";
			case 0x02: return "Computer Telephony Device";
		}
		case 0x05: switch (subclass)
		{
			default: return "Unknown Memory Controller";
			case 0x00: return "RAM memory";
			case 0x01: return "Flash Controller";
			case 0x80: return "Memory controller";
		}
		case 0x06: switch (subclass)
		{
			default: return "Unknown Bridge Device";
			case 0x00: return "Host Bridge";
			case 0x01: return "ISA Bridge";
			case 0x02: return "EISA Bridge";
			case 0x03: return "MCA Bridge";
			case 0x04: return "PCI-to-PCI Bridge";
			case 0x05: return "PCMCIA Bridge";
			case 0x06: return "NuBus Bridge";
			case 0x07: return "CardBus Bridge";
			case 0x08: return "RACEway Bridge";
			case 0x09: return "PCI-to-PCI Bridge";
			case 0x0A: return "InfiniBrand-to-PCI Host Bridge";
			case 0x80: return "Bridge";
		}
		case 0x07: switch (subclass)
		{
			default: return "Unknown Simple Communication Controller";
			case 0x00: return "Serial Controller";
			case 0x01: return "IEEE 1284 or Parallel Port";
			case 0x02: return "Multiport Serial Controller";
			case 0x03: return "Generic Modem";
			case 0x04: return "IEE 488.1/2 Controller";
			case 0x05: return "Smart Card";
		}
		case 0x08: switch (subclass)
		{
			default: return "Unknown Base System Peripheral";
			case 0x00: return "Programmable Interrupt Controller";
			case 0x01: return "DMA Controller";
			case 0x02: return "System Timer";
			case 0x03: return "RTC Controller";
			case 0x04: return "PCI Hot-Plug Controller";
			case 0x05: return "SD Host controller";
			case 0x06: return "IOMMU";
			case 0x80: return "System peripheral";
		}
		case 0x09: switch (subclass)
		{
			default: return "Unknown Input Device";
			case 0x00: return "Keyboard Controller";
			case 0x01: return "Digitzer";
			case 0x02: return "Mouse Controller";
			case 0x03: return "Scanner Controller";
			case 0x04: return "Gameport Controller";
		}
		case 0x0A: switch (subclass)
		{
			default: return "Unknown Docking Station";
			case 0x00: return "Generic docking station";
			case 0x80: return "Docking Station";
		}
		case 0x0B: switch (subclass)
		{
			default: return "Unknown Processor";
			case 0x00: return "386 Processor";
			case 0x01: return "486 Processor";
			case 0x02: return "Pentium Processor";
			case 0x30: return "MIPS Processor";
			case 0x40: return "Co-Processor";
		}
		case 0x0C: switch (subclass)
		{
			default: return "Unknown Serial Bus Controller";
			case 0x00: return "IEE 1394 Controller";
			case 0x01: return "ACCESS bus";
			case 0x02: return "SSA";
			case 0x03: return "USB Controller";
			case 0x04: return "Fibre Channel Controller";
			case 0x05: return "SMBus";
			case 0x06: return "InfiniBand";
			case 0x07: return "IPMI Interface";
			case 0x08: return "SERCOS Interface Standard";
			case 0x09: return "CANbus";
		}
		case 0x0D: switch (subclass)
		{
			default: return "Unknown Wireless Controller";
			case 0x00: return "IRDA Compatible Controller";
			case 0x01: return "Consumer IR Controller";
			case 0x10: return "RF Controller";
			case 0x11: return "Bluetooth Controller";
			case 0x12: return "Broadband Controller";
			case 0x20: return "Ethernet Controller (802.11a)";
			case 0x21: return "Ethernet Controller (802.11b)";
		}
		case 0x0E: switch (subclass)
		{
			default: return "Unknown Intelligent I/O Controller";
		}
		case 0x0F: switch (subclass)
		{
			default: return "Unknown Satellite Communication Controller";
			case 0x01: return "TV Controller";
			case 0x02: return "Audio Controller";
			case 0x03: return "Voice Controller";
			case 0x04: return "Data Controller";
		}
		case 0x10: switch (subclass)
		{
			default: return "Unknown Encryption/Decryption Controller";
			case 0x00: return "Network and Computing Encryption/Decryption";
			case 0x10: return "Entertainment Encryption/Decryption";
		}
		case 0x11: switch (subclass)
		{
			default: return "Unknown Data Acquistion or Signal Processing Controller";
			case 0x00: return "DPIO Modules";
			case 0x01: return "Performance Counters";
			case 0x10: return "Communication Syncrhonization Plus Time and Frequency Test/Measurement";
			case 0x20: return "Management Card";
		}
		default: return "Unknown Device";
	}
}

//
//
//
STRPTR pci_getStringOfClass(UBYTE c)
{
	switch (c)
	{
		default:
		return "Unknown";
		case 0x01:
		return "Mass Storage Controller";
		case 0x02:
		return "Network Controller";
		case 0x03:
		return "Display Controller";
		case 0x04:
		return "Multimedia Controller";
		case 0x05:
		return "Memory Controller";
		case 0x06:
		return "Bridge Device";
		case 0x07:
		return "Simple Communication Controller";
		case 0x08:
		return "Generic  System Peripherial";
		case 0x09:
		return "Input Device";
		case 0x0A:
		return "Docking Station";
		case 0x0B:
		return "Processor";
		case 0x0C:
		return "Serial Bus Controller";
		case 0x0D:
		return "Wireless Controller";
		case 0x0E:
		return "Intelligent I/O Controller";
		case 0x0F:
		return "Satellite Communication Controller";
		case 0x10:
		return "Encryption/Decryption Controller";
		case 0x11:
		return "Data Acquisition and Signal Processing Controller";
		case 0x12:
		return "Processing accelerators";
		case 0x13:
		return "Non-Essential Instrumentation";
		case 0xFF:
		return "Unassigned class";
	}
}
