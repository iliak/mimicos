#ifndef EXEC_LISTS_H
#define EXEC_LISTS_H
//
//
//	Definitions and macros for use with Exec lists
//

#include <exec/nodes.h>

//
//  Full featured list header.
// 
struct List 
{
   struct Node *	Head;			 // points to the first node in the list.
   struct Node *	Tail;			 // 
   struct Node *	TailPred;		 // points to the last node in the list.
   UBYTE			Type;			 // type of nodes within the list 
   UBYTE			pad;			 // 
};	

//
// Minimal List Header - no type checking
struct MinList 
{
   struct MinNode *	Head;		 // points to the first node in the list.
   struct MinNode *	Tail;		 // 
   struct MinNode *	TailPred;	 // points to the last node in the list.
};	



//
//	Check for the presence of any nodes on the given list.	These
//	macros are even safe to use on lists that are modified by other
//	tasks.	However; if something is simultaneously changing the
//	list, the result of the test is unpredictable.
//
//	Unless you first arbitrated for ownership of the list, you can't
//	_depend_ on the contents of the list.  Nodes might have been added
//	or removed during or after the macro executes.
//
//		if(IsListEmpty(list))		printf("List is empty\n");
 
//#define IsListEmpty(x)		()(x)->TailPred) ==)struct Node *)(x))
//#define IsMsgPortEmpty(x)	()(x)->mp_MsgList.TailPred) ==)struct Node *)(&(x)->mp_MsgList))
#define IsListEmpty(l)      ( (((struct List *)l)->lh_TailPred) == (struct Node *)(l) )
#define IsMinListEmpty(l)	( (((struct MinList *)l)->mlh_TailPred) == (struct MinList *)(l) )
#define IsMsgPortEmpty(mp)  ( (((struct MsgPort *)(mp))->mp_MsgList.lh_TailPred) == (struct Node *)(&(((struct MsgPort *)(mp))->mp_MsgList)) )


/*
#define GetHead(_l)                                     \
({                                                      \
    struct List *__list_tmp = (struct List *)(_l),		\
                *l = __list_tmp;						\
                                                        \
   l->Head->Succ ? l->Head : (struct Node *)0;			\
})

//
#define GetTail(_l)                                              \
({                                                               \
    struct List *__list_tmp = (struct List *)(_l),          \
                *l = __list_tmp;                            \
                                                                 \
    l->TailPred->Pred ? l->TailPred : (struct Node *)0; \
})

//
#define GetSucc(_n)                                      \
({                                                       \
    struct Node *__node_tmp = (struct Node *)(_n),  \
                *n = __node_tmp;                    \
                                                         \
    (n && n->Succ && n->Succ->Succ) ? n->Succ : (struct Node *)0; \
})

//
#define GetPred(_n)                                      \
({                                                       \
    struct Node *__node_tmp = (struct Node *)(_n),  \
                *n = __node_tmp;                    \
                                                         \
    (n && n->Pred && n->Pred->Pred) ? n->Pred : (struct Node *)0; \
})
*/



#endif


