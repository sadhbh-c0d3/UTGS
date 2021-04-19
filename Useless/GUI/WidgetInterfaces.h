#ifndef __INCLUDED_USELESS_WIDGET_INTERFACES_H__
#define __INCLUDED_USELESS_WIDGET_INTERFACES_H__

namespace Useless {


class Widget;
/*
-----------------------------------*/
struct IWidget
{
    virtual Widget& Atom() = 0;
            Widget* AtomPtr() { return &Atom(); }
};


   
class ActiveWidget;
/*
-----------------------------------*/
struct IWActive : virtual public IWidget
{
    virtual ActiveWidget& Control() = 0;
};


class EnumWidget;
/*
-----------------------------------*/
struct IWEnum : virtual public IWidget
{
    virtual EnumWidget& Enumerator() = 0;
};

class BridgeWidget;
/*
-----------------------------------*/
struct IWBridge : virtual public IWidget
{
    virtual BridgeWidget& Bridge() = 0;
};



class HubWidget;
/*
-----------------------------------*/
struct IWHub : virtual public IWidget
{
    virtual HubWidget& Hub() = 0;
};




class SizerWidget;
/*
-----------------------------------*/
struct IWSizer : virtual public IWidget
{
    virtual SizerWidget& Sizer() = 0;
};




class TableWidget;
/*
-----------------------------------*/
struct IWTable : virtual public IWidget
{
    virtual TableWidget& Table() = 0;
};



class CaptionWidget;
/*
-----------------------------------*/
struct IWCaption : virtual public IWidget
{
    virtual CaptionWidget& Caption() = 0;
};



class ContentWidget;
/*
-----------------------------------*/
struct IWContent : virtual public IWidget
{
    virtual ContentWidget& Content() = 0;
};



class ListWidget;
/*
-----------------------------------*/
struct IWList : virtual public IWEnum
{
    virtual ListWidget& Items() = 0;
};



/*
-----------------------------------*/
struct IWTableList : virtual public IWTable, virtual public IWEnum
{
};



class ScrollbarWidget;
/*
-----------------------------------*/
struct IWScrollbar : virtual public IWidget
{
    virtual ScrollbarWidget& Scroller() = 0;
};



class ScrolledWidget;
/*
-----------------------------------*/
struct IWScrollee : virtual public IWidget
{
    virtual ScrolledWidget& Scrollee() =0;
};



/*
-----------------------------------*/
struct IWViewport : virtual public IWScrollee
{
    virtual IWScrollbar& HScroll() = 0;
    virtual IWScrollbar& VScroll() = 0;
};


};//namespace Useless


#endif//__INCLUDED_USELESS_WIDGET_INTERFACES_H__
