#ifndef  EXEC_PROTOS_H
#define  EXEC_PROTOS_H


#ifdef __cplusplus
extern "C" {
#endif

#include <exec/types.h>
#include <exec/devices.h>
#include <exec/memory.h>
#include <exec/interrupts.h>
/*
#include <exec/tasks.h>
#include <exec/ports.h>
#include <exec/io.h>
#include <exec/semaphores.h>
*/

//------ misc ---------------------------------------------------------
//ULONG Supervisor( ULONG (*CONST userFunction)() );
//------ special patchable hooks to internal exec activity ------------

//------ module creation ----------------------------------------------
//VOID InitCode(ULONG startClass, ULONG version );
//VOID InitStruct( CONST APTR initTable, APTR memory, ULONG size );
struct Library * MakeLibrary(const char *funcInit, const char *structInit, ULONG(*CONST libInit)(), ULONG dataSize, ULONG segList);
//Library * MakeLibrary(ULONG, void *, ULONG, ULONG, ULONG);
//VOID MakeFunctions( APTR target, CONST APTR functionArray, ULONG funcDispBase );
//Resident *FindResident(const char *name );
//APTR InitResident( CONST struct Resident *resident, ULONG segList );

//------ diagnostics --------------------------------------------------
VOID Alert(ULONG alert, STRPTR fmt, ...);
VOID Debug(ULONG flags);
//------ interrupts ---------------------------------------------------

void Disable();
void Enable();
void Permit();
void Forbid();
//APTR SuperState( VOID );
//VOID UserState( APTR sysStack );
void SetIntVector(ULONG number, InterruptHandler handler);
//void AddIntServer(ULONG number, void(_cdecl far &vect)())
//void RemIntServer(ULONG number, Interrupt * handler);
void Cause(LONG id);
void Halt();

//------ memory allocation --------------------------------------------
APTR AllocMem(ULONG size, ULONG attributes);
void FreeMem(APTR memoryBlock, ULONG size);

APTR Allocate(struct MemHeader *freeList, ULONG byteSize);
void Deallocate(struct MemHeader * freeList, APTR memoryBlock, ULONG byteSize);
APTR AllocAbs(ULONG byteSize, APTR location);

//MemList * AllocEntry(MemList * entry);
//void FreeEntry(MemList * entry);
//void AddMemList(MemoryRegion *region, STRPTR name);
void AddMemList(ULONG start, ULONG size, ULONG type, STRPTR name);


ULONG AvailMem(ULONG attributes);
ULONG TypeOfMem(void *address);

void CopyMem(APTR source, APTR dest, ULONG size);
void CopyMemQuick(APTR source, APTR dest, ULONG size);

//------ lists --------------------------------------------------------
void	Insert(struct List *list, struct Node *node, struct Node *pred);
void	AddHead(struct List *list, struct Node *node);
void	AddTail(struct List *list, struct Node *node);
void	Remove(struct Node *node);
struct Node *	RemHead(struct List *list);
struct Node *	RemTail(struct List *list);
void	Enqueue(struct List *list, struct Node *node);
struct Node *	FindName(struct List *list, STRPTR name);
struct Node *	GetHead(struct List *list);
void	NewList(struct List *list);
//void	NewList(struct MinList *list);

/*
//------ tasks --------------------------------------------------------
APTR AddTask( struct Task *task, CONST APTR initPC, CONST APTR finalPC );
VOID RemTask( struct Task *task );
struct Task *FindTask( CONST_STRPTR name );
BYTE SetTaskPri( struct Task *task, LONG priority );
ULONG SetSignal( ULONG newSignals, ULONG signalSet );
ULONG SetExcept( ULONG newSignals, ULONG signalSet );
*/
ULONG Wait(ULONG signalSet);
/*
VOID Signal( struct Task *task, ULONG signalSet );
BYTE AllocSignal( LONG signalNum );
VOID FreeSignal( LONG signalNum );
LONG AllocTrap( LONG trapNum );
VOID FreeTrap( LONG trapNum );
//------ messages -----------------------------------------------------
VOID AddPort( struct MsgPort *port );
VOID RemPort( struct MsgPort *port );
VOID PutMsg( struct MsgPort *port, struct Message *message );
struct Message *GetMsg( struct MsgPort *port );
VOID ReplyMsg( struct Message *message );
struct Message *WaitPort( struct MsgPort *port );
struct MsgPort *FindPort( CONST_STRPTR name );
*/
//------ libraries ----------------------------------------------------
void AddLibrary(struct Library *);
void RemLibrary(struct Library *);
struct Library * OpenLibrary(const char *, ULONG);
void CloseLibrary(struct Library *);
ULONG SetFunction(struct Library *, ULONG, ULONG);
void SumLibrary(struct Library *);



//------ devices ------------------------------------------------------
void AddDevice(struct Device *device);
VOID RemDevice(struct Device *device);
BYTE OpenDevice(STRPTR devName, ULONG unit, struct IORequest *ioRequest, ULONG flags);
VOID CloseDevice(struct IORequest *ioRequest);
BYTE DoIO(struct IORequest *ioRequest);
VOID SendIO(struct IORequest *ioRequest);
VOID BeginIO(struct IORequest *io);
//struct IORequest *CheckIO(struct IORequest *ioRequest);
BOOL CheckIO(struct IORequest *ioRequest);
BYTE WaitIO(struct IORequest *ioRequest);
VOID AbortIO(struct IORequest *ioRequest);
/*
//------ resources ----------------------------------------------------
VOID AddResource( APTR resource );
VOID RemResource( APTR resource );
APTR OpenResource( CONST_STRPTR resName );
//------ private diagnostic support -----------------------------------
//------ misc ---------------------------------------------------------
APTR RawDoFmt( CONST_STRPTR formatString, CONST APTR dataStream, VOID (*CONST putChProc)(), APTR putChData );
ULONG GetCC( VOID );
ULONG TypeOfMem( CONST APTR address );
ULONG Procure( struct SignalSemaphore *sigSem, struct SemaphoreMessage *bidMsg );
VOID Vacate( struct SignalSemaphore *sigSem, struct SemaphoreMessage *bidMsg );
struct Library *OpenLibrary( CONST_STRPTR libName, ULONG version );
//--- functions in V33 or higher (Release 1.2) ---
//------ signal semaphores (note funny registers)----------------------
VOID InitSemaphore( struct SignalSemaphore *sigSem );
VOID ObtainSemaphore( struct SignalSemaphore *sigSem );
VOID ReleaseSemaphore( struct SignalSemaphore *sigSem );
ULONG AttemptSemaphore( struct SignalSemaphore *sigSem );
VOID ObtainSemaphoreList( struct List *sigSem );
VOID ReleaseSemaphoreList( struct List *sigSem );
struct SignalSemaphore *FindSemaphore( STRPTR name );
VOID AddSemaphore( struct SignalSemaphore *sigSem );
VOID RemSemaphore( struct SignalSemaphore *sigSem );
//------ kickmem support ----------------------------------------------
ULONG SumKickData( VOID );
//------ more memory support ------------------------------------------
VOID AddMemList( ULONG size, ULONG attributes, LONG pri, APTR base, CONST_STRPTR name );
VOID CopyMem( CONST APTR source, APTR dest, ULONG size );
VOID CopyMemQuick( CONST APTR source, APTR dest, ULONG size );
//------ cache --------------------------------------------------------
//--- functions in V36 or higher (Release 2.0) ---
VOID CacheClearU( VOID );
VOID CacheClearE( APTR address, ULONG length, ULONG caches );
ULONG CacheControl( ULONG cacheBits, ULONG cacheMask );
*/
//------ misc ---------------------------------------------------------
struct IORequest * CreateIORequest(struct MsgPort *port, ULONG size);
VOID DeleteIORequest(struct IORequest * iorequest);
struct MsgPort * CreateMsgPort(VOID);
VOID DeleteMsgPort(struct MsgPort *port);
/*
VOID ObtainSemaphoreShared( struct SignalSemaphore *sigSem );
//------ even more memory support -------------------------------------
APTR AllocVec( ULONG byteSize, ULONG requirements );
VOID FreeVec( APTR memoryBlock );
//------ V39 Pool LVOs...
APTR CreatePool( ULONG requirements, ULONG puddleSize, ULONG threshSize );
VOID DeletePool( APTR poolHeader );
APTR AllocPooled( APTR poolHeader, ULONG memSize );
VOID FreePooled( APTR poolHeader, APTR memory, ULONG memSize );
//------ misc ---------------------------------------------------------
ULONG AttemptSemaphoreShared( struct SignalSemaphore *sigSem );
VOID ColdReboot( VOID );
VOID StackSwap( struct StackSwapStruct *newStack );
//------ task trees ---------------------------------------------------
VOID ChildFree( APTR tid );
VOID ChildOrphan( APTR tid );
VOID ChildStatus( APTR tid );
VOID ChildWait( APTR tid );
//------ future expansion ---------------------------------------------
APTR CachePreDMA( CONST APTR address, ULONG *length, ULONG flags );
VOID CachePostDMA( CONST APTR address, ULONG *length, ULONG flags );
//------ New, for V39
//--- functions in V39 or higher (Release 3) ---
//------ Low memory handler functions
VOID AddMemHandler( struct Interrupt *memhand );
VOID RemMemHandler( struct Interrupt *memhand );
//------ Function to attempt to obtain a Quick Interrupt Vector...
ULONG ObtainQuickVector( APTR interruptCode );
*/

#ifdef __cplusplus
}
#endif

#endif
