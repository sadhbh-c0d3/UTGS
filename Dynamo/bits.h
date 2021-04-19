#ifndef __INCLUDED_DYNAMO_BITS_H__
#define __INCLUDED_DYNAMO_BITS_H__

#include <string>
#include <vector>
#include <assert.h>

// HyperObject is defined koolib/HyperObject.h
// Use KOOLIB_NON_CONTRIB to avoid Useless linkage.
struct HyperObject;

// ExecutionState is needed for KooLiXP binding.
namespace XMLProgram {
    struct ExecutionState;
    struct IFiniteStateMachine;
};

namespace Useless {
    namespace GVM {
        struct Context;
        struct Message;
    };
};

namespace Dynamo {

    typedef Useless::GVM::Context GVM_Context;
    typedef Useless::GVM::Message GVM_Message;

    struct Point2i
    {
        int m_x;
        int m_y;
        Point2i( int x, int y ): m_x(x), m_y(y) {}
    };

    struct Rect4i
    {
        int m_x;
        int m_y;
        int m_w;
        int m_h;
        Rect4i( int x, int y, int w, int h ): m_x(x), m_y(y), m_w(w), m_h(h) {}
    };

    struct Loc3f
    {
        float   m_u;
        float   m_v;
        float   m_w;

        Loc3f( float u, float v, float w ): m_u(u), m_v(v), m_w(w) {}
    };

    struct Look
    {
        std::vector< std::string > m_id;
        float   m_speed;
        bool    m_loop;

        Look(): m_speed(1.0), m_loop(true) {}
    };

    
    // Count References, Manage Memory
    struct IInterface
    {
        virtual ~IInterface() {}

        virtual int AddRef() = 0;
        virtual int RemRef() = 0;

        virtual bool SetData( const std::string &name, const HyperObject &in ) { return false; }
        virtual bool GetData( const std::string &name, HyperObject &out ) { return false; }
    };

    template< class IType >
        struct Hand
        {
            IType *m_px;

            Hand( IType *px=0 ): m_px(px)
            {
                if ( m_px != 0 ) { m_px->AddRef(); }
            }

            Hand( const Hand &other ): m_px(other.m_px)
            {
                if ( m_px != 0 ) { m_px->AddRef(); } 
            }

            Hand &operator = ( const Hand &other )
            {
                if ( m_px != 0 ) { m_px->RemRef(); }
                m_px = other.m_px;
                if ( m_px != 0 ) { m_px->AddRef(); } 
                return (*this);
            }

            ~Hand()
            {
                if ( m_px != 0 ) { m_px->RemRef(); }
            }

            IType * get() { return m_px; }
            IType * get_const() const { return m_px; }
            IType * operator -> () const { assert(m_px); return m_px; }
        };
};

#endif//__INCLUDED_DYNAMO_BITS_H__
