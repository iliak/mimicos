#ifndef PROTO_EXPANSION_H
#define PROTO_EXPANSION_H



#include <libraries/expansionbase.h>


extern struct ExpansionBase ExpansionBase;






VOID expansion_init(VOID);


//VOID AddConfigDev(struct ConfigDev *configDev);
//BOOL AddBootNode(LONG bootPri, ULONG flags, struct DeviceNode *deviceNode, struct ConfigDev *configDev);
//VOID AllocBoardMem(ULONG slotSpec);
//struct ConfigDev *AllocConfigDev(VOID);
//APTR AllocExpansionMem(ULONG numSlots, ULONG slotAlign);
//VOID ConfigBoard(APTR board, struct ConfigDev *configDev);
//VOID ConfigChain(APTR baseAddr);
struct ConfigDev *FindConfigDev(CONST struct ConfigDev *oldConfigDev, LONG manufacturer, LONG product);
//VOID FreeBoardMem(ULONG startSlot, ULONG slotSpec);
//VOID FreeConfigDev(struct ConfigDev *configDev);
//VOID FreeExpansionMem(ULONG startSlot, ULONG numSlots);
//UBYTE ReadExpansionByte(CONST APTR board, ULONG offset);
//VOID ReadExpansionRom(CONST APTR board, struct ConfigDev *configDev);
//VOID RemConfigDev(struct ConfigDev *configDev);
//VOID WriteExpansionByte(APTR board, ULONG offset, ULONG byte);
//VOID ObtainConfigBinding(VOID);
//VOID ReleaseConfigBinding(VOID);
//VOID SetCurrentBinding(struct CurrentBinding *currentBinding, ULONG bindingSize);
//ULONG GetCurrentBinding(CONST struct CurrentBinding *currentBinding, ULONG bindingSize);
//struct DeviceNode *MakeDosNode(CONST APTR parmPacket);
//BOOL AddDosNode(LONG bootPri, ULONG flags, struct DeviceNode *deviceNode);









#endif 

