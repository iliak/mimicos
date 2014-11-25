#ifndef	INTUITION_PROTOS_H
#define	INTUITION_PROTOS_H


#ifdef __cplusplus
//extern "C" {
#endif 

#include <intuition/intuition.h>
//#include <intuition/classes.h>
//#include <intuition/cghooks.h>
//#include <intuition/classusr.h>


void intuition_init(void);

//VOID Intuition(InputEvent *iEvent);
//UWORD AddGadget(Window *window, Gadget *gadget, ULONG position);
//BOOL ClearDMRequest(Window *window);
//VOID ClearMenuStrip(Window *window);
//VOID ClearPointer(Window *window);
BOOL CloseScreen(struct Screen *screen);
//VOID CloseWindow(Window *window);
LONG CloseWorkbench(VOID);
//VOID CurrentTime(ULONG *seconds, ULONG *micros);
//BOOL DisplayAlert(ULONG alertNumber, STRPTR string, ULONG height);
//VOID DisplayBeep(Screen *screen);
//BOOL DoubleClick(ULONG sSeconds, ULONG sMicros, ULONG cSeconds, ULONG cMicros);
//VOID DrawBorder(RastPort *rp, CONST Border *border, LONG leftOffset, LONG topOffset);
//VOID DrawImage(RastPort *rp, Image *image, LONG leftOffset, LONG topOffset);
//VOID EndRequest(Requester *requester, Window *window);
//Preferences *GetDefPrefs(Preferences *preferences, LONG size);
//Preferences *GetPrefs(Preferences *preferences, LONG size);
//VOID InitRequester(Requester *requester);
//MenuItem *ItemAddress(CONST Menu *menuStrip, ULONG menuNumber);
//BOOL ModifyIDCMP(Window *window, ULONG flags);
//VOID ModifyProp(Gadget *gadget, Window *window, Requester *requester, ULONG flags, ULONG horizPot, ULONG vertPot, ULONG horizBody, ULONG vertBody);
//VOID MoveScreen(Screen *screen, LONG dx, LONG dy);
//VOID MoveWindow(Window *window, LONG dx, LONG dy);
//VOID OffGadget(Gadget *gadget, Window *window, Requester *requester);
//VOID OffMenu(Window *window, ULONG menuNumber);
//VOID OnGadget(Gadget *gadget, Window *window, Requester *requester);
//VOID OnMenu(Window *window, ULONG menuNumber);
struct Screen * OpenScreen(struct NewScreen *newScreen);
struct Window * OpenWindow(struct NewWindow *newWindow);
ULONG OpenWorkbench(VOID);
//VOID PrintIText(RastPort *rp, CONST IntuiText *iText, LONG left, LONG top);
//VOID RefreshGadgets(Gadget *gadgets, Window *window, Requester *requester);
//UWORD RemoveGadget(Window *window, Gadget *gadget);
//// The official calling sequence for ReportMouse is given below. 
//// Note the register order.  For the complete story, read the ReportMouse 
//// autodoc. 
//VOID ReportMouse(LONG flag, Window *window);
//VOID ReportMouse1(Window *window, LONG flag);
//BOOL Request(Requester *requester, Window *window);
//VOID ScreenToBack(Screen *screen);
VOID ScreenToFront(struct Screen *screen);
//BOOL SetDMRequest(Window *window, Requester *requester);
//BOOL SetMenuStrip(Window *window, Menu *menu);
//VOID SetPointer(Window *window, UWORD *pointer, LONG height, LONG width, LONG xOffset, LONG yOffset);
//VOID SetWindowTitles(Window *window, STRPTR windowTitle, STRPTR screenTitle);
//VOID ShowTitle(Screen *screen, LONG showIt);
//VOID SizeWindow(Window *window, LONG dx, LONG dy);
//View *ViewAddress(VOID);
//ViewPort *ViewPortAddress(CONST Window *window);
//VOID WindowToBack(Window *window);
//VOID WindowToFront(Window *window);
//BOOL WindowLimits(Window *window, LONG widthMin, LONG heightMin, ULONG widthMax, ULONG heightMax);
////--- start of next generation of names -------------------------------------
//Preferences *SetPrefs(CONST Preferences *preferences, LONG size, LONG inform);
////--- start of next next generation of names --------------------------------
//LONG IntuiTextLength(CONST IntuiText *iText);
//BOOL WBenchToBack(VOID);
//BOOL WBenchToFront(VOID);
////--- start of next next next generation of names ---------------------------
//BOOL AutoRequest(Window *window, CONST IntuiText *body, CONST IntuiText *posText, CONST IntuiText *negText, ULONG pFlag, ULONG nFlag, ULONG width, ULONG height);
//VOID BeginRefresh(Window *window);
//Window *BuildSysRequest(Window *window, CONST IntuiText *body, CONST IntuiText *posText, CONST IntuiText *negText, ULONG flags, ULONG width, ULONG height);
//VOID EndRefresh(Window *window, LONG complete);
//VOID FreeSysRequest(Window *window);
//// The return codes for MakeScreen(), RemakeDisplay(), and RethinkDisplay() 
//// are only valid under V39 and greater.  Do not examine them when running 
//// on pre-V39 systems! 
//LONG MakeScreen(Screen *screen);
//LONG RemakeDisplay(VOID);
//LONG RethinkDisplay(VOID);
////--- start of next next next next generation of names ----------------------
//APTR AllocRemember(Remember **rememberKey, ULONG size, ULONG flags);
//VOID FreeRemember(Remember **rememberKey, LONG reallyForget);
////--- start of 15 Nov 85 names ------------------------
//ULONG LockIBase(ULONG dontknow);
//VOID UnlockIBase(ULONG ibLock);
////--- functions in V33 or higher (Release 1.2) ---
//LONG GetScreenData(APTR buffer, ULONG size, ULONG type, CONST Screen *screen);
//VOID RefreshGList(Gadget *gadgets, Window *window, Requester *requester, LONG numGad);
//UWORD AddGList(Window *window, Gadget *gadget, ULONG position, LONG numGad, Requester *requester);
//UWORD RemoveGList(Window *remPtr, Gadget *gadget, LONG numGad);
//VOID ActivateWindow(Window *window);
//VOID RefreshWindowFrame(Window *window);
//BOOL ActivateGadget(Gadget *gadgets, Window *window, Requester *requester);
//VOID NewModifyProp(Gadget *gadget, Window *window, Requester *requester, ULONG flags, ULONG horizPot, ULONG vertPot, ULONG horizBody, ULONG vertBody, LONG numGad);
//LONG QueryOverscan(ULONG displayID, Rectangle *rect, LONG oScanType);
//VOID MoveWindowInFrontOf(Window *window, Window *behindWindow);
//VOID ChangeWindowBox(Window *window, LONG left, LONG top, LONG width, LONG height);
//Hook *SetEditHook(Hook *hook);
//LONG SetMouseQueue(Window *window, ULONG queueLength);
//VOID ZipWindow(Window *window);
//
////--- public screens ---
//Screen *LockPubScreen(STRPTR name);
//VOID UnlockPubScreen(STRPTR name, Screen *screen);
//List *LockPubScreenList(VOID);
//VOID UnlockPubScreenList(VOID);
//STRPTR NextPubScreen(CONST Screen *screen, STRPTR namebuf);
//VOID SetDefaultPubScreen(STRPTR name);
//UWORD SetPubScreenModes(ULONG modes);
//UWORD PubScreenStatus(Screen *screen, ULONG statusFlags);
//
//RastPort *ObtainGIRPort(GadgetInfo *gInfo);
//VOID ReleaseGIRPort(RastPort *rp);
//VOID GadgetMouse(Gadget *gadget, GadgetInfo *gInfo, WORD *mousePoint);
//VOID GetDefaultPubScreen(STRPTR nameBuffer);
//LONG EasyRequestArgs(Window *window, CONST Easy*easyStruct, ULONG *idcmpPtr, CONST APTR args);
//LONG EasyRequest(Window *window, CONST Easy*easyStruct, ULONG *idcmpPtr, ...);
//Window *BuildEasyRequestArgs(Window *window, CONST Easy*easyStruct, ULONG idcmp, CONST APTR args);
//Window *BuildEasyRequest(Window *window, CONST Easy*easyStruct, ULONG idcmp, ...);
//LONG SysReqHandler(Window *window, ULONG *idcmpPtr, LONG waitInput);
//Window *OpenWindowTagList(CONST NewWindow *newWindow, CONST TagItem *tagList);
//Window *OpenWindowTags(CONST NewWindow *newWindow, ULONG tag1Type, ...);
//Screen *OpenScreenTagList(CONST NewScreen *newScreen, CONST TagItem *tagList);
//Screen *OpenScreenTags(CONST NewScreen *newScreen, ULONG tag1Type, ...);
//
////	new Image functions 
//VOID DrawImageState(RastPort *rp, Image *image, LONG leftOffset, LONG topOffset, ULONG state, CONST DrawInfo *drawInfo);
//BOOL PointInImage(ULONG point, Image *image);
//VOID EraseImage(RastPort *rp, Image *image, LONG leftOffset, LONG topOffset);
//
//APTR NewObjectA(IClass *classPtr, STRPTR classID, CONST TagItem *tagList);
//APTR NewObject(IClass *classPtr, STRPTR classID, ULONG tag1, ...);
//
//VOID DisposeObject(APTR object);
//ULONG SetAttrsA(APTR object, CONST TagItem *tagList);
//ULONG SetAttrs(APTR object, ULONG tag1, ...);
//
//ULONG GetAttr(ULONG attrID, APTR object, ULONG *storagePtr);
//
//// 	special set attribute call for gadgets 
//ULONG SetGadgetAttrsA(Gadget *gadget, Window *window, Requester *requester, CONST TagItem *tagList);
//ULONG SetGadgetAttrs(Gadget *gadget, Window *window, Requester *requester, ULONG tag1, ...);

//	for class implementors only 
//APTR NextObject(APTR objectPtrPtr);
//IClass *MakeClass(STRPTR classID, STRPTR superClassID, CONST IClass *superClassPtr, ULONG instanceSize, ULONG flags);
//VOID AddClass(IClass *classPtr);


//DrawInfo *GetScreenDrawInfo(Screen *screen);
//VOID FreeScreenDrawInfo(Screen *screen, DrawInfo *drawInfo);

//BOOL ResetMenuStrip(Window *window, Menu *menu);
//VOID RemoveClass(IClass *classPtr);
//BOOL FreeClass(IClass *classPtr);

//ScreenBuffer *AllocScreenBuffer(Screen *sc, BitMap *bm, ULONG flags);
//VOID FreeScreenBuffer(Screen *sc, ScreenBuffer *sb);
//ULONG ChangeScreenBuffer(Screen *sc, ScreenBuffer *sb);
//VOID ScreenDepth(Screen *screen, ULONG flags, APTR reserved);
//VOID ScreenPosition(Screen *screen, ULONG flags, LONG x1, LONG y1, LONG x2, LONG y2);
//VOID ScrollWindowRaster(Window *win, LONG dx, LONG dy, LONG xMin, LONG yMin, LONG xMax, LONG yMax);
//VOID LendMenus(Window *fromwindow, Window *towindow);
//ULONG DoGadgetMethodA(Gadget *gad, Window *win, Requester *req, Msg message);
//ULONG DoGadgetMethod(Gadget *gad, Window *win, Requester *req, ULONG methodID, ...);
//VOID SetWindowPointerA(Window *win, CONST TagItem *taglist);
//VOID SetWindowPointer(Window *win, ULONG tag1, ...);
//BOOL TimedDisplayAlert(ULONG alertNumber, STRPTR string, ULONG height, ULONG time);
//VOID HelpControl(Window *win, ULONG flags);

#ifdef __cplusplus
//}
#endif

#endif  
