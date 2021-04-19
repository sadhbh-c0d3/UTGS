#ifndef __INCLUDED_USELESS_IO_STRUCT_H__
#define __INCLUDED_USELESS_IO_STRUCT_H__

#include "Useless/TemplateConfig.h"
#include "Useless/File/IFile.h"
#include "Useless/File/OFile.h"
#include "Useless/Util/SPointer.h"

namespace Useless {

/*------------------------------------------------------------------------
                  Generic I/O Types
------------------------------------------------------------------------*/

//! AnyType<T> - creates type wrapper
template< typename T >
struct AnyType
{
    typedef T Type;

    AnyType() {}
    AnyType( const T &x): _x(x) {}
    operator T& () { return _x; }
    operator const T& () const { return _x; }
    int Size() const { return sizeof(T); }

    void     Set( const T &x ) { _x=x; }
    const T& Get() const { return _x; }

    T _x;
};

//! IOTypes - used to load and save data into files
//@{
struct IOByte : AnyType< Types::BYTE >
{
    void Load( IFile &f ) { _x = f.ReadByte(); }
    void Save( OFile &f ) { f.WriteByte(_x); }
};

struct IOWordLE : AnyType< Types::WORD >
{
    void Load( IFile &f ) { _x = f.ReadWordLE(); }
    void Save( OFile &f ) { f.WriteWordLE(_x); }
};

struct IOWordHE : AnyType< Types::WORD >
{
    void Load( IFile &f ) { _x = f.ReadWordHE(); }
    void Save( OFile &f ) { f.WriteWordHE(_x); }
};

struct IOLongLE : AnyType< Types::LONG >
{
    void Load( IFile &f ) { _x = f.ReadLongLE(); }
    void Save( OFile &f ) { f.WriteLongLE(_x); }
};

struct IOLongHE : AnyType< Types::LONG >
{
    void Load( IFile &f ) { _x = f.ReadLongHE(); }
    void Save( OFile &f ) { f.WriteLongHE(_x); }
};

struct IOQuadLE : AnyType< Types::QUAD >
{
    void Load( IFile &f ) { _x = f.ReadQuadLE(); }
    void Save( OFile &f ) { f.WriteQuadLE(_x); }
};

struct IOQuadHE : AnyType< Types::QUAD >
{
    void Load( IFile &f ) { _x = f.ReadQuadHE(); }
    void Save( OFile &f ) { f.WriteQuadHE(_x); }
};

struct IOString : AnyType< std::string >
{
    IOString() {}
    IOString( const char *s ): AnyType< std::string >(s) {}
    IOString( const std::string &s ): AnyType< std::string >(s) {}
    void Load( IFile &f )
    {
        _x = std::string( f.ReadWordLE(), 0x0 );
        f.Read( (void*)_x.c_str(), _x.size() );
    }

    void Save( OFile &f )
    {
        f.WriteWordLE( _x.size() );
        f.Write( (void*)_x.c_str(), _x.size() );
    }
    size_t Size() {  return sizeof(IOWordLE) + _x.size(); }
};

template< int I >
struct IOChars
{
    void Load( IFile &f ) { f.Read( _x, I); }
    void Save( OFile &f ) { f.Write( _x, I); }
    int Size() { return I; }

    char _x[I];
};

//@}

/*------------------------------------------------------------------------
                  TypeLists and Strucutres
------------------------------------------------------------------------*/

//! NullType - not defined type
struct NullType;

//! TypeList<Head, Tail> - binary node Head * Tail
template< class H, class T >
struct TypeList
{
    typedef H Head;
    typedef T Tail;
};
template< int I > struct ArgAt {};

//! InList<L>::At<I> - get type in list L at position I
//-------------------------------------------------
#ifdef WIN32// MSVC version
namespace TypeList_imp {
    template< int I> struct TypeAt
    {
        template< class L > struct InList
        {
            typedef typename TypeAt<I-1>::InList< L::Tail >::Result Result;
        };

        template<> struct InList< NullType >
        {
            typedef NullType Result;
        };
    };

    template<> struct TypeAt<0>
    {
        template< class L > struct InList
        {
            typedef typename L::Head Result;
        };
    };
};//namespace
    template< class L, int I > struct InListAt
    {
        typedef typename TypeList_imp::TypeAt<I>::InList<L>::Result Result;
    };
//-------------------------------------------------
#else// GCC version
    template< class L, int I > struct InListAt
    {
        typedef typename InListAt< typename L::Tail, I-1 >::Result Result;
    };

    template< class L> struct InListAt<L,0>
    {
        typedef typename L::Head Result;
    };

    template< int I> struct InListAt< NullType, I>
    {
        typedef NullType Result;
    };
#endif

//! Struct<L> - create instance of TypeList L
template< class L >
struct Struct : public Struct< USELESS_TYPENAME L::Tail >
{
    typedef USELESS_TYPENAME L::Head Arg;
    typedef Struct< USELESS_TYPENAME L::Tail > Next;
    typedef L List;
    Struct() {}
    Arg _a;
};

USELESS_SPECIALIZATION
struct Struct<NullType>
{
    typedef NullType Arg;
    typedef Struct<NullType> Next;
    typedef NullType List;
    Struct() {}
};

//! InStruct<S>::At<I> - retrive I-th ancestor Struct type
#ifdef WIN32// MSVC version
namespace Struct_imp {
    template< const int I > struct TypeAt
    {
        template< class S > struct In
        {
            typedef USELESS_TYPENAME TypeAt<I-1>::In< S::Next >::Result Result;
            typedef USELESS_TYPENAME TypeAt<I-1>::In< S::Next >::Arg Arg;
        };
    };
    
    template<> struct TypeAt<0>
    {
        template< class S > struct In
        {
            typedef S Result;
            typedef USELESS_TYPENAME S::Arg Arg;
        };
    };
};//namespace Struct_imp

    //! InStructAt< S, I> = InStruct<S>::At<I>; defines Arg type
    template< class S, int I >
    struct InStructAt
    {
        typedef USELESS_TYPENAME Struct_imp::TypeAt<I>::In<S>::Result Result;
        typedef USELESS_TYPENAME Struct_imp::TypeAt<I>::In<S>::Arg Arg;
    };

#else// GCC version
    template< class S, int I > struct InStructAt
    {
        typedef USELESS_TYPENAME InStructAt< USELESS_TYPENAME S::Next, I-1 >::Result Result;
        typedef USELESS_TYPENAME InStructAt< USELESS_TYPENAME S::Next, I-1 >::Arg Arg;
    };

    template< class S > struct InStructAt< S, 0>
    {
        typedef S Result;
        typedef USELESS_TYPENAME  S::Arg Arg;
    };
#endif

//! Nth<I>::In( S &s ) - gain access to argument at position I (also const version)
template< int I >
struct Nth
{
    template< class S >
    static USELESS_TYPENAME InStructAt< USELESS_TYPENAME Struct<S>, I >::Arg& In( Struct<S> &s )
    {
        typedef USELESS_TYPENAME InStructAt< Struct<S>, I >::Result RType;
        RType &z = s;
        return z._a;
    }

    template< class S >
    static const USELESS_TYPENAME InStructAt< Struct<S>, I >::Arg& In( const Struct<S> &s )
    {
        typedef USELESS_TYPENAME InStructAt< Struct<S>, I >::Result RType;
        const RType &z = s;
        return z._a;
    }
};
/*------------------------------------------------------------------------
                  Loading and Saving strucutres
------------------------------------------------------------------------*/

//! LoadStruct<S> - structure S loader
template< class S >
struct LoadStruct
{
    static void Load( S &s, IFile &f )
    {
        s._a.Load(f);
        LoadStruct< USELESS_TYPENAME S::Next >::Load( s, f );
    }
};

USELESS_SPECIALIZATION
struct LoadStruct< Struct<NullType> >
{
    static void Load( Struct<NullType> &s, IFile &f ) {}
};

//! SaveStruct<S> - structure S saver
template< class S >
struct SaveStruct
{
    static void Save( S &s, OFile &f )
    {
        s._a.Save(f);
        SaveStruct< USELESS_TYPENAME S::Next >::Save( s, f );
    }
};

USELESS_SPECIALIZATION
struct SaveStruct< Struct<NullType> >
{
    static void Save( Struct<NullType> &s, OFile &f ) {}
};

//! SizeStruct<S> - structure S sizer
template< class S >
struct SizeStruct
{
    static size_t Size( S &s )
    {
        return s._a.Size() + SizeStruct< USELESS_TYPENAME S::Next >::Size( s );
    }
};

USELESS_SPECIALIZATION
struct SizeStruct< Struct<NullType> >
{
    static int Size( Struct<NullType> &s ) { return 0; }
};


//! Load( S &s, IFile &f ) - load data into structure 's' from IFile 'f'
template< class L > void Load( Struct<L> &s, IFile &f )
{
    LoadStruct< Struct<L> >::Load( s, f );
}

//! Save( S &s, OFile &f ) - save data into OFile 'f' from structure 's'
template< class L > void Save( Struct<L> &s, OFile &f )
{
    SaveStruct< Struct<L> >::Save( s, f );
}

//! Size( S &s ) - calculate size of structure 's'
template< class L > int Size( Struct<L> &s )
{
    return SizeStruct< Struct<L> >::Size( s );
}

//! IOStruct<L> - composite I/O structure
template< class L >
struct IOStruct : AnyType< Struct<L> >
{
    typedef L List;

    void Load( IFile &f )
    {
        Useless::Load( _x, f );
    }
    void Save( OFile &f )
    {
        Useless::Save( _x, f );
    }
    int Size()
    {
        return Useless::Size( _x );
    }

    template< int I >
    USELESS_TYPENAME InStructAt< Struct<L>, I >::Arg& operator[]( ArgAt<I> )
    {
        return Nth<I>::In(_x);
    }


    Struct<L>& Get() { return _x; }
    const Struct<L>& Get() const { return _x; }
};

//! A concept that needs more thinking ...
//@{
struct VirtualIOStruct
{
    virtual void Load( IFile &f ) = 0;
    virtual void Save( OFile &f ) = 0;
    virtual int  Size() = 0;
};

template< class IOType >
struct VirtualIOStructTpl : public VirtualIOStruct, public IOType
{
    virtual void Load( IFile &f ) { IOType::Load(f); }
    virtual void Save( OFile &f ) { IOType::Save(f); }
    virtual int Size() { return IOType::Size(); }
};

template< class Info >
struct AnyIOStruct
{
    AnyIOStruct( const SPointer< VirtualIOStruct > &p, const Info &i ): _p(p), _i(i) {}

    void Load( IFile &f ) { _i.Load(f); Setup(_i); _p->Load(f); }
    void Save( OFile &f ) { _i.Save(f); _p->Save(f); }
    int  Size() { return _p->Size(); }

    virtual void Setup( const Info &i ) = 0;

    Info _i;
    SPointer< VirtualIOStruct > _p;
};
//@}

};//namespace Useless
#endif//__INCLUDED_USELESS_IO_STRUCT_H__
