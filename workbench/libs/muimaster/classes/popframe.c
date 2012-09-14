/*
    Copyright � 2002-2006, The AROS Development Team. All rights reserved.
    $Id$
*/

#define MUIMASTER_YES_INLINE_STDARG

#include <clib/alib_protos.h>
#include <proto/exec.h>
#include <proto/graphics.h>
#include <proto/utility.h>
#include <proto/intuition.h>
#include <proto/muimaster.h>

/*  #define MYDEBUG 1 */
#include "debug.h"
#include "mui.h"
#include "muimaster_intern.h"
#include "support.h"
#include "support_classes.h"
#include "popframe_private.h"

extern struct Library *MUIMasterBase;

IPTR Popframe__OM_NEW(struct IClass *cl, Object *obj, struct opSet *msg)
{
    struct Popframe_DATA *data;
    struct TagItem *tag, *tags;

    obj = (Object *) DoSuperNewTags(
        cl, obj, NULL,
        ButtonFrame,
        InnerSpacing(4, 4),
        MUIA_Background, MUII_ButtonBack,
        MUIA_InputMode, MUIV_InputMode_RelVerify,
        MUIA_Draggable, TRUE,
        TAG_MORE, (IPTR) msg->ops_AttrList);

    if (!obj)
        return FALSE;

    data = INST_DATA(cl, obj);
    data->wintitle = NULL;

    /* parse initial taglist */
    for (tags = msg->ops_AttrList; (tag = NextTagItem(&tags));)
    {
        switch (tag->ti_Tag)
        {
        case MUIA_Window_Title:
            data->wintitle = (CONST_STRPTR) tag->ti_Data;
            break;
        }
    }

    DoMethod(obj, MUIM_Notify, MUIA_Pressed, FALSE, (IPTR) obj, 1,
        MUIM_Popframe_OpenWindow);

    return (IPTR) obj;
}

IPTR Popframe__OM_DISPOSE(struct IClass *cl, Object *obj, Msg msg)
{
    struct Popframe_DATA *data = INST_DATA(cl, obj);

    if (data->wnd)
    {
        D(bug("Popframe_Dispose(%p) : MUI_DisposeObject(%p)\n", obj,
                data->wnd));
        MUI_DisposeObject(data->wnd);
        data->wnd = NULL;
    }
    return DoSuperMethodA(cl, obj, (Msg) msg);
}

IPTR Popframe__MUIM_Hide(struct IClass *cl, Object *obj,
    struct opGet *msg)
{
#if 0
    struct Popframe_DATA *data = INST_DATA(cl, obj);
    if (data->wnd)
    {
        D(bug("Popframe_Hide(%p) : closing window %p\n", obj, data->wnd));
        set(data->wnd, MUIA_Window_Open, FALSE);
        D(bug("Popframe_Hide(%p) : app REMMEMBER win (%p)\n", obj,
                data->wnd));
        DoMethod(_app(obj), OM_REMMEMBER, (IPTR) data->wnd);
        D(bug("Popframe_Hide(%p) : MUI_DisposeObject(%p)\n", obj,
                data->wnd));
        MUI_DisposeObject(data->wnd);
        data->wnd = NULL;
    }
#endif
    return DoSuperMethodA(cl, obj, (Msg) msg);
}

IPTR Popframe__MUIM_Popframe_OpenWindow(struct IClass *cl, Object *obj,
    Msg msg)
{
    struct Popframe_DATA *data = INST_DATA(cl, obj);

    if (!data->wnd)
    {
        Object *ok_button, *cancel_button;
        struct MUI_Frame_Spec *frame_spec = NULL;
        ULONG x = 0, y = 0;

        get(_win(obj), MUIA_Window_LeftEdge, &x);
        get(_win(obj), MUIA_Window_TopEdge, &y);

        get(obj, MUIA_Framedisplay_Spec, &frame_spec);

        data->wnd = (Object *)(WindowObject,
            MUIA_Window_Title, (IPTR)data->wintitle,
            MUIA_Window_Activate, TRUE,
            MUIA_Window_IsSubWindow, TRUE,
            MUIA_Window_LeftEdge, _left(obj) + x,
            MUIA_Window_TopEdge, _bottom(obj) + y + 1,
            WindowContents, (IPTR)VGroup,
                Child, (IPTR)(data->frameadjust = MUI_NewObject(
                    MUIC_Frameadjust,
                    MUIA_CycleChain, 1,
                    MUIA_Frameadjust_Spec, (IPTR)frame_spec,
                    TAG_DONE)),
                Child, (IPTR)HGroup,
                    MUIA_Group_SameWidth, TRUE,
                    Child, (IPTR)(ok_button =
                        MUI_MakeObject(MUIO_Button, (IPTR)"_Ok")),
                    Child, (IPTR)HVSpace,
                    Child, (IPTR)HVSpace,
                    Child, (IPTR)(cancel_button =
                        MUI_MakeObject(MUIO_Button, (IPTR)"_Cancel")),
                    End,
                End,
            End);

        if (data->wnd)
        {
            set(ok_button, MUIA_CycleChain, 1);
            set(cancel_button, MUIA_CycleChain, 1);

            DoMethod(_app(obj), OM_ADDMEMBER, (IPTR) data->wnd);

            DoMethod(ok_button, MUIM_Notify, MUIA_Pressed, FALSE,
                (IPTR) _app(obj), 5, MUIM_Application_PushMethod,
                (IPTR) obj, 2, MUIM_Popframe_CloseWindow, TRUE);
            DoMethod(cancel_button, MUIM_Notify, MUIA_Pressed, FALSE,
                (IPTR) _app(obj), 5, MUIM_Application_PushMethod,
                (IPTR) obj, 2, MUIM_Popframe_CloseWindow, FALSE);
            DoMethod(data->wnd, MUIM_Notify, MUIA_Window_CloseRequest, TRUE,
                (IPTR) _app(obj), 5, MUIM_Application_PushMethod,
                (IPTR) obj, 2, MUIM_Popframe_CloseWindow, FALSE);
        }
    }

    if (data->wnd)
    {
        ULONG opened = FALSE;

        set(data->wnd, MUIA_Window_Open, TRUE);
        get(data->wnd, MUIA_Window_Open, &opened);
        if (!opened)
        {
            DoMethod(obj, MUIM_Popframe_CloseWindow, FALSE);
        }
    }

    return 1;
}

IPTR Popframe__MUIM_Popframe_CloseWindow(struct IClass *cl, Object *obj,
    struct MUIP_Popframe_CloseWindow *msg)
{
    struct Popframe_DATA *data = INST_DATA(cl, obj);
    int ok = msg->ok;

    set(data->wnd, MUIA_Window_Open, FALSE);

    if (ok)
    {
        STRPTR spec = NULL;
        get(data->frameadjust, MUIA_Frameadjust_Spec, &spec);
/*          D(bug("popframe: got %s\n", spec)); */
        set(obj, MUIA_Framedisplay_Spec, (IPTR) spec);
    }

    DoMethod(_app(obj), OM_REMMEMBER, (IPTR) data->wnd);
    MUI_DisposeObject(data->wnd);
    data->wnd = NULL;
    data->frameadjust = NULL;
    return 1;
}

IPTR Popframe__MUIM_DisconnectParent(struct IClass *cl, Object *obj,
    struct MUIP_DisconnectParent *msg)
{
    struct Popframe_DATA *data = INST_DATA(cl, obj);

    if (data->wnd)
        DoMethod(obj, MUIM_Popframe_CloseWindow, FALSE);
    return DoSuperMethodA(cl, obj, (Msg) msg);
}

#if ZUNE_BUILTIN_POPFRAME
BOOPSI_DISPATCHER(IPTR, Popframe_Dispatcher, cl, obj, msg)
{
    switch (msg->MethodID)
    {
    case OM_NEW:
        return Popframe__OM_NEW(cl, obj, (struct opSet *)msg);
    case OM_DISPOSE:
        return Popframe__OM_DISPOSE(cl, obj, msg);
    case MUIM_Hide:
        return Popframe__MUIM_Hide(cl, obj, (APTR) msg);
    case MUIM_Popframe_OpenWindow:
        return Popframe__MUIM_Popframe_OpenWindow(cl, obj, (APTR) msg);
    case MUIM_Popframe_CloseWindow:
        return Popframe__MUIM_Popframe_CloseWindow(cl, obj, (APTR) msg);
    case MUIM_DisconnectParent:
        return Popframe__MUIM_DisconnectParent(cl, obj, (APTR) msg);
    default:
        return DoSuperMethodA(cl, obj, msg);
    }
}
BOOPSI_DISPATCHER_END

const struct __MUIBuiltinClass _MUI_Popframe_desc =
{
    MUIC_Popframe,
    MUIC_Framedisplay,
    sizeof(struct Popframe_DATA),
    (void *) Popframe_Dispatcher
};
#endif /* ZUNE_BUILTIN_POPFRAME */
