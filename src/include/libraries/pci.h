#ifndef PCI_H
#define PCI_H

/*
 * The PCI interface treats multi-function devices as independent
 * devices.  The slot/function address of each device is encoded
 * in a single byte as follows:
 *
 *      7:3 = slot
 *      2:0 = function
 */
#define PCI_DEVFN(slot, func)   ((((slot) & 0x1f) << 3) | ((func) & 0x07))
#define PCI_SLOT(devfn)         (((devfn) >> 3) & 0x1f)
#define PCI_FUNC(devfn)         ((devfn) & 0x07)

//
// Represents a pci bus
//
struct pci_bus
{

};


//
// Represents a pci device
//
struct pci_dev 
{
};



//
// Represents a physical slot
//
struct pci_slot 
{
};


#endif // !PCI_H
