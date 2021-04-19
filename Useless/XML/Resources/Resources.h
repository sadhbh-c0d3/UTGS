#ifndef __INCLUDED_USELESS_RESOURCE_H__
#define __INCLUDED_USELESS_RESOURCE_H__

#pragma warning(disable:4786) // debug name turncation
#include <map>
#include <string>
#include <list>
#include "Useless/UselessConfig.h"
#include "Useless/TemplateConfig.h"
#include "Useless/Util/TemplateDefines.h"
#include "Useless/Util/Singleton.h"
#include "Useless/Util/str_conv.h"
#include "Useless/Error/Error.h"
#include "Useless/Error/ErrorMessage.h"

namespace Useless {

//! Base class for type detection while convertion
struct USELESS_RESOURCES_API Converter
{
    virtual ~Converter() {}
};

template< class _Y >
struct DynamicConverterTo : virtual Converter 
{
    struct Tag {};
    virtual _Y* Apply( void *, Tag ) = 0;

    template< class _T > _T& UnrawPtr( void *ptr )
    {
        return (*(_T*)(ptr));
    }
};

/*! Apply dynamic cast operator
 */
template< class _X, class _Y >
struct DynamicConverter : virtual DynamicConverterTo< _Y >
{
    _Y* Apply( void *ptr, DynamicConverterTo< _Y >::Tag )
    {
        return Apply( UnrawPtr< _X *>( ptr ));
        //return Apply( *(_X**)(ptr) );
    }
    
    _Y* Apply( _X *x )
    {
        return dynamic_cast< _Y *>(x);
    }
};

template< class _T > struct Converters : virtual Converter {};

/*! Base resource teleport class
 */
struct USELESS_RESOURCES_API Resource
{
    virtual ~Resource() {}
    virtual const char * GetTypeName() = 0;
    virtual const type_info & GetTypeInfo() = 0;
        
    virtual Converter * GetConverter() const = 0;
    virtual void * GetRawPtr() = 0;

    template< class T > T * DynaCast();
};

/*! Specialized resource teleport class
 */
template< class T >
struct AnyResource : public Resource
{
    USELESS_EMPTY_SPECIALIZATION AnyResource() {}

#define __RESOURCE_CTOR_MACRO(n) \
    template< _TPL_##n##_ > \
        AnyResource( _TPL_ARGS_##n ): _x(_TPL_PASS_ARGS_##n)  {}

    __RESOURCE_CTOR_MACRO(1)
    __RESOURCE_CTOR_MACRO(2)
    __RESOURCE_CTOR_MACRO(3)
    __RESOURCE_CTOR_MACRO(4)
    __RESOURCE_CTOR_MACRO(5)
    __RESOURCE_CTOR_MACRO(6)
    __RESOURCE_CTOR_MACRO(7)
    __RESOURCE_CTOR_MACRO(8)
    __RESOURCE_CTOR_MACRO(9)
    __RESOURCE_CTOR_MACRO(10)

#undef __RESOURCE_CTOR_MACRO


    T* Get() { return &_x; }
    virtual const char * GetTypeName() { return typeid(T).name(); }
    virtual const type_info & GetTypeInfo() { return typeid(T); }
    virtual void * GetRawPtr() { return &_x; }
    virtual Converter * GetConverter() const
    {
        static Converters< T > _converter;
        return &_converter;
    }

private:
    T _x;
};

template< class _T >
struct PointerResourceConvertion
{
    static _T * Apply( Resource *res )
    {
        if ( res->GetTypeInfo() == typeid( _T *))
        {
            return (*static_cast< AnyResource< _T *> *>( res )->Get());
        }
        else
        {
            Converter *conv = res->GetConverter();
            typedef DynamicConverterTo< _T > DConv;
            typedef typename DConv::Tag Tag;
            if ( DConv *dconv = dynamic_cast< DConv *>( conv ))
            {
                return dconv->Apply( res->GetRawPtr(), Tag() );
            }
            else
            {
                return 0;
            }
        }
    }
};

template< class T > T *
Resource::DynaCast()
{
    AnyResource< T > *pRT = dynamic_cast< AnyResource< T > *>( this );
    return (!pRT ? 0 : pRT->Get());
}

struct ResourceNotFoundError : Error
{
    ResourceNotFoundError( const std::string &category, const std::string &name )
        :   Error("Resources: Resource '%s' was not found in category <%s>."
                , name.c_str(), category.c_str() )
    {
    }
};

struct BadResourceTypeError : Error
{
    BadResourceTypeError( const std::string &category, const std::string &name, Resource *p, const std::type_info &expected_type )
        :   Error("Resources: Resource '%s' was found in category <%s> with type '%s', but type '%s' was expected."
                , name.c_str(), category.c_str(), p->GetTypeName(), expected_type.name() )
        {
        }
};

struct BadResourceIndexError : Error
{
    BadResourceIndexError( const std::string &category, const std::string &name, int index )
        :   Error("Resources: Requested index [%i] exceds the number of '%s' resources in category <%s>."
                , index , category.c_str() , name.c_str() )
        {
        }
};

class USELESS_RESOURCES_API __Resources
{
public:
    // pair( category, id)
    typedef std::pair<std::string, std::string> Identifier;
    // map of resources pair( category, id) => resource_ptr
    typedef std::multimap< Identifier, Resource* > ResourceMap;
    
    /*! Parse expr like my_name[5] -> pair( my_name, 5)
     */
    struct NameIndex : public std::pair<std::string, int>
    {
        NameIndex() {}
        NameIndex( const std::string &expr )
        {
            int rb = expr.rfind("]",-1);
            int lb = expr.rfind("[",rb);
            
            if ( lb!=-1 && rb!=-1 )
            {
                second = to_integer( expr.substr(lb+1, lb-rb-2)  );
            }
            else if ( lb==-1 && rb==-1 )
            {
                second = 0;
            }
            else
            {
                throw Error("Resource-Index: Syntax error in index expresion '%s'.", expr.c_str());
            }
            first = expr.substr( 0, lb);
        }
    };

    /*! If there is more than one resource of the same id
     *  then id[n] will determine n-th resource
     */
    int Insert( const std::string &category,
                const std::string &id,
                Resource *resource )
    {
        int n = _resources.count( Identifier( category, id));
        _resources.insert( std::make_pair( Identifier(category, id), resource ) );
        C_message( __Resources, 0, ("Resource added:", category.c_str(), id.c_str(), resource->GetTypeName() ));
        return n;
    }

    /*! Delete resource
     */
    bool Remove( const std::string &category,
                 const std::string &id_expr, 
                 bool removeChilds=false)
    {
        NameIndex name_id( id_expr);
        ResourceMap::iterator i= Find( category ,name_id);

        if ( i!=_resources.end())
        {
            ResourceMap::iterator it = i++;
            delete (*it).second;
            _resources.erase(it);

            if( removeChilds )
            {
                std::string name(name_id.first+".");
                int namelen = name.size();
                while( i!=_resources.end() && 
                       (*i).first.first == category &&
                       name == std::string( (*i).first.second, 0, namelen ) )
                {
                        it = i++;
                        delete (*it).second;
                        _resources.erase(it);
                }
            }

            C_message( __Resources, 1, ("Resource removed:", category.c_str(), id_expr.c_str() )); 
            return true;
        }
        else
        {
            C_message( __Resources, 2, ("Resource was NOT removed:", category.c_str(), id_expr.c_str() )); 
            return false;
        }
    }

    /*! Get exact interface of named resource.
     *  If interface is of wrong type then error occurs.
     */
    template< class T >
    void Query( const std::string &category, const std::string &id_expr, T *&ptr )
    {
        NameIndex name_id( id_expr);
        ResourceMap::iterator i = Find( category, name_id);

        if ( i!=_resources.end() )
        {
            if ( (*i).first != Identifier(category, name_id.first))
            {
                throw BadResourceIndexError( category, id_expr, name_id.second );
            }

            Resource *p_res = (*i).second;
            AnyResource<T> *p = dynamic_cast< AnyResource<T>* >( p_res );
            if (!p)
            {
                throw BadResourceTypeError( category, id_expr, p_res, typeid(T) );
            }
            ptr = p->Get();
        } 
        else
        {
            throw ResourceNotFoundError( category, id_expr );
        }       
    }

    Resource * QueryResource( const std::string &category, const std::string &id_expr )
    {
        NameIndex name_id( id_expr );
        ResourceMap::iterator i = Find( category, name_id );
        if ( i != _resources.end() )
        {
            if ( (*i).first != Identifier(category, name_id.first))
                { return 0; }
            else
                { return (*i).second; }
        }
        return 0;
    }

    /*! Tell if resource exists
     */
    bool Exists( const std::string &category, const std::string &id_expr )
    {
        NameIndex name_id( id_expr);
        return ( Find( category, name_id) != _resources.end());
    }

    /*! Get iterator to resource
     */
    ResourceMap::iterator Find( const std::string &category, const NameIndex &name_id)
    {
        Identifier identifier( category, name_id.first);     //< Create resource index
        ResourceMap::iterator i=_resources.find( identifier);//< Find corresponding resource
        for ( int j=0; j!=name_id.second && i!=_resources.end(); ++j, ++i ) {} //< Advance to n-th resource with this name
        if ( i!=_resources.end() && (*i).first.second != name_id.first )
        {
            i = _resources.end();
        }
        return i;
    }

    ResourceMap::iterator Begin()
    {
        return _resources.begin();
    }
    
    ResourceMap::iterator End()
    {
        return _resources.end();
    }
    
    ResourceMap::iterator Erase( ResourceMap::iterator it )
    {
        ResourceMap::iterator curr( it++ );
        delete ((*curr).second);
        _resources.erase( curr );
        return it;
    }

    bool IsChild( ResourceMap::iterator i, const NameIndex &name_id, std::string &childName )
    {
        if ( i == _resources.end() )
            { return false; }

        int pos = (*i).first.second.find( name_id.first );
        if ( 0 == pos )
        {
            childName = (*i).first.second.substr( name_id.first.size()+1, -1 );
            return true;
        }
        else
           { return false; }    
    }

    /*! Delete all resources
    */
    ~__Resources()
    {
        ResourceMap::iterator i=_resources.begin(), i2=_resources.end();
        for ( ; i!=i2; ++i )
        {
            try
                { delete (*i).second; }
            catch(...)
                { C_message( __Resources, 4,
                        ("Failed to release resource:", (*i).first.first.c_str(),
                                                        (*i).first.second.c_str() )); }
        }
    }

private:
    ResourceMap _resources;
};

DECLARE_SINGLETON(__Resources, Resources, USELESS_RESOURCES_API);


template< class _Type >
struct ResourceQuery
{
    typedef _Type Type;

    ResourceQuery
        ( const std::string &category,
          const std::string &id_expr,
          __Resources *instance = &Resources::Instance() )
        :   _ptr(0), _error(0)
    {
        Resource *r = instance->QueryResource( category, id_expr );
        
        if ( 0 != r )
        {
            _Type *p = r->DynaCast< _Type >();
            
            if ( 0 != p )
            {
                _ptr = new (_storage) _Type( *p );
            }
            else
            {
                _error = new (_storage) BadResourceTypeError( category, id_expr, r, typeid(_Type) );
            }
        }
        else
        {
            _error = new (_storage) ResourceNotFoundError( category, id_expr );
        }
    }

    ResourceQuery( Resource *r )
        :   _ptr(0), _error(0)
    {
        if ( 0 != r )
        {
            _Type *p = r->DynaCast< _Type >();
            
            if ( 0 != p )
            {
                _ptr = new (_storage) _Type( *p );
            }
            else
            {
                _error = new (_storage) BadResourceTypeError("?", "?", r, typeid(_Type));
            }
        }
        else
        {
            _error = new (_storage) ResourceNotFoundError("?", "?");
        }
    }


    ~ResourceQuery()
    {
        if ( _ptr )
            { _ptr->~_Type(); }
        else
            { _error->~Error(); }
    }

    bool IsValid() const { return (0 != _ptr); }
    const Error & GetError() const { return *_error; }

    _Type * operator ->() const
    {
        if ( _error ) { throw GetError(); }
        return _ptr;
    }

    _Type & operator * () const
    {
        if ( _error ) { throw GetError(); }
        return *_ptr;
    }

private:
    char  _storage[ sizeof(_Type) > sizeof( Error ) ? sizeof(_Type) : sizeof(Error) ];
    _Type *_ptr;
    Error *_error;
};

};//namespace Useless
#endif __INCLUDED_USELESS_RESOURCE_H__
