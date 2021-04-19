#ifndef __INCLUDED_USELESS_LAYOUT_H__
#define __INCLUDED_USELESS_LAYOUT_H__

#pragma warning(disable:4786)       // disable 'indentifier was truncated...'
#include "Useless/Util/Storage/StupidVector.h"
#include "Useless/Functional/Signal.h"

namespace Useless {

//USES
    class Widget;

/*! Gap_ is accepted by LayerBase.
 *  Absolute value is gap size.
 *  If negative then size is in pixels, else its placement weight.
 */
struct Gap_ {   Gap_( int s=0 ): _s(s) {}   int _s; };


/*! Base for Layouts 
 */
class USELESS_API LayoutBase
{
public:
    virtual ~LayoutBase() {}

    class Attrib 
    {
    public:
        virtual ~Attrib() {}
        virtual void Accept( Attrib &visitor ) const = 0;
    };

    virtual void Fill() = 0; //<! Applies layout

    virtual SPointer<LayoutBase> Copy() const = 0;

    virtual LayoutBase& operator << ( Widget *p_widget ) = 0;
    virtual LayoutBase& operator << ( const Attrib &attrib ) = 0;
    virtual LayoutBase& operator << ( const LayoutBase &layout ) = 0;
    virtual LayoutBase& operator << ( const Gap_ &gap ) = 0;

            LayoutBase& operator ,( Widget *p_widget )         { return operator <<(p_widget); }
            LayoutBase& operator ,( const Attrib &attrib )     { return operator <<(attrib); }
            LayoutBase& operator ,( const LayoutBase &layout ) { return operator <<(layout); }
            LayoutBase& operator ,( Gap_ gap )                 { return operator <<(gap); }


    template< typename T, int I > class X_ : virtual public Attrib
    {
    public:
        X_() : x(0) {}
        X_(int x): x(x) {}
        virtual ~X_() {}

        virtual void Accept( Attrib &visitor ) const
        {
            if ( X_* x_ = dynamic_cast< X_* >(&visitor) )
            {
                x_->Visit(*this);
            }
        }
        void Visit( const X_ &a )
        {
            x = a.x;
        }
        X_& operator += (const X_ &a)
        {
            x += a.x; 
            return *this;
        }
        X_& Max(const X_ &a)
        {
            x = (a.x > x)? a.x : x;
        }
        X_& Min(const X_ &a)
        {
            x = (a.x < x)? a.x : x;
        }
        operator       T& ()       { return x; }
        operator const T& () const { return x; }



        T x;
    };
};

/* Some generic attributes
-----------------------------------------------------------------*/
typedef LayoutBase::X_<int,0> Weight_;
typedef LayoutBase::X_<int,1> MaxSize_;
typedef LayoutBase::X_<int,2> MinSize_;
typedef LayoutBase::X_<int,3> AlignSize_;
typedef LayoutBase::X_<int,4> AlignSizePad_;
typedef LayoutBase::X_<int,5> Overlap_;
typedef LayoutBase::X_<int,20> OrthoPlacement_;
typedef LayoutBase::X_<int,21> OrthoMaxSize_;
typedef LayoutBase::X_<int,22> OrthoMinSize_;
typedef LayoutBase::X_<int,23> OrthoAlignSize_;
typedef LayoutBase::X_<int,24> OrthoAlignSizePad_;
typedef LayoutBase::X_<int,31> PadStart_;
typedef LayoutBase::X_<int,32> PadEnd_;

//USES
    class HubWidget;

/* Exp Layout
-----------------------------------------------------------------*/
class USELESS_API Layout : public LayoutBase
{
public:
    Layout();
    Layout( HubWidget *p_root );
    virtual ~Layout();

    virtual void Fill();

    Layout( const Layout &layout );
    Layout& operator =( const Layout &layout );

    virtual LayoutBase& operator << ( Widget *p_widget );
    virtual LayoutBase& operator << ( const Attrib &attrib );
    virtual LayoutBase& operator << ( const LayoutBase &layout );
    virtual LayoutBase& operator << ( const Gap_ &gap );

            HubWidget* Get() const { return _p_root; }
            HubWidget* Release() { _root_owner=false; return _p_root; }

    class Attrib_
        : public Weight_
        , public MaxSize_
        , public MinSize_
        , public AlignSize_
        , public AlignSizePad_
        , public Overlap_
        , public OrthoMaxSize_
        , public OrthoMinSize_
        , public OrthoAlignSize_
        , public OrthoAlignSizePad_
        , public OrthoPlacement_
        , public PadStart_
        , public PadEnd_
    {
    public:
        Attrib_()
            : Weight_(0), MaxSize_(0), MinSize_(0), AlignSize_(0), AlignSizePad_(0), Overlap_(0)
            , OrthoMaxSize_(0), OrthoMinSize_(0), OrthoAlignSize_(0), OrthoAlignSizePad_(0), OrthoPlacement_(0) {}
        virtual ~Attrib_() {}

        virtual void Accept( Attrib &visitor ) const
        {
            Weight_ ::Accept(visitor);
            MaxSize_::Accept(visitor);
            MinSize_::Accept(visitor);
            AlignSize_::Accept(visitor);
            AlignSizePad_::Accept(visitor);
            Overlap_::Accept(visitor);
            OrthoMaxSize_::Accept(visitor);
            OrthoMinSize_::Accept(visitor);
            OrthoAlignSize_::Accept(visitor);
            OrthoAlignSizePad_::Accept(visitor);
            OrthoPlacement_::Accept(visitor);
            PadStart_::Accept(visitor);
            PadEnd_::Accept(visitor);
        }
    };

protected:
    typedef std::pair< Widget*, Attrib_>  WidgetAttrib;
    typedef StupidVector< WidgetAttrib >  LayoutMap;
    typedef AssocIterator< Widget*, Attrib_, LayoutMap::Iterator > Iterator;

    LayoutMap  _layout;
    Iterator   _current;

    virtual void SetupWidgets( const StupidVector<int> &sizes ) = 0;
    virtual int  GetSize() = 0;

private:
    HubWidget      *_p_root;
    Signal::FuncID  _on_resize_root;
    bool            _root_owner;
    
    int             _fill_requests;
    friend struct FillLock;
    struct FillLock
    {
        Layout *_p_layout;
        FillLock( Layout *p_layout): _p_layout( p_layout ) { ++p_layout->_fill_requests; }
        ~FillLock() { --_p_layout->_fill_requests; }
    };

    StupidVector< SPointer<LayoutBase> > _children_layouts;
    StupidVector< Signal::FuncID > _on_child_resize;
};


class USELESS_API HLayout : public Layout
{
public:
    HLayout() {}
    HLayout( HubWidget *p_root ) : Layout(p_root) {}
    virtual ~HLayout() {}
    
    virtual SPointer<LayoutBase> Copy() const { return new HLayout(*this); }

protected:
    virtual void SetupWidgets( const StupidVector<int> &sizes );
    virtual int  GetSize() ;
};

class USELESS_API VLayout : public Layout
{
public:
    VLayout() {}
    VLayout( HubWidget *p_root ) : Layout(p_root) {}
    virtual ~VLayout() {}

    virtual SPointer<LayoutBase> Copy() const { return new VLayout(*this); }
    
protected:
    virtual void SetupWidgets( const StupidVector<int> &sizes );
    virtual int  GetSize() ;
};


};//namespace Useless
#endif//__INCLUDED_USELESS_LAYOUT_H__
