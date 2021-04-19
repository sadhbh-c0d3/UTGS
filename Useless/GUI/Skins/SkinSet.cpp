#include "UselessPch.h"

#include "Useless/GUI/Skins/SkinSet.h"
#include "Useless/ErrorConfig.h"

#include "Useless/GUI/Widgets/Widget.h"

namespace Useless {

SkinSet::SkinSet( const SkinSet &skinset)
{
    _sp_skins = new __Skins;

    SkinMap::ConstIterator iter = skinset._sp_skins->ConstBegin();
    for( ; !!iter; ++iter )
    {
        Skin *pskin = iter.Value();
        Skin *pnew_skin  = &( CopyOf<Skin>( *pskin ) );
        _sp_skins->GetMap().Insert( iter.Key(), pnew_skin);
    }
}


SkinSet::~SkinSet()
{
    if ( (_sp_skins.GetCounter()==1) && !_sp_skins->Empty() )
    {
        SkinMap::Iterator iter = _sp_skins->Begin();
        for( ; !!iter; ++iter )
        { 
            delete iter.Value();
        }
    }
}

SkinSet::SkinSet()
    : _sp_skins( new __Skins )
{
}

SkinSet::SkinSet( CopyOf<Skin> skin_copy, int id )
    : _sp_skins( new __Skins )
{
    _sp_skins->GetMap().Insert(id, &skin_copy );
}

void SkinSet::Insert( int id, CopyOf<Skin> skin_copy )
{
    _sp_skins->GetMap().Insert(id, &skin_copy );
}

int SkinSet::Insert( CopyOf<Skin> skin_copy )
{
    return _sp_skins->Insert( &skin_copy );
}

void SkinSet::Remove(int id)
{
    _sp_skins->Remove(id);
}

void SkinSet::Paint( int id, const Painter& painter, const Pos& p)
{
    Skin **pps = _sp_skins->Find(id);
    if (pps)
    {
        assert(*pps && "SkinSet::Paint()");
        (**pps).Paint(painter, p);
    }
}

void  SkinSet::SetSizeAll( const Dim2i& d )
{
    SkinMap::Iterator iter = _sp_skins->Begin();
    for( ; !!iter; ++iter )
    { 
        if (! iter.Value()->SafeResize(d) )
        {
            APP_WARNING(Error("Cannot resize %i skin", iter.Key() ),"SkinSet"); 
        }
    }
}

bool SkinSet::SafeResize( int id, const Dim2i& d)
{
    Skin **pps = _sp_skins->Find(id);
    if ( pps )
    {
        return (*pps)->SafeResize(d);
    }
    else { APP_WARNING(Error("Cannot resize skin %i (no such skin)",id),"SkinSet"); }

    return false;
}

Dim2i SkinSet::GetMaxSize() const
{
    SkinMap::ConstIterator iter = _sp_skins->ConstBegin();
    Dim2i d;

    for( ; !!iter; ++iter )
    { 
        Dim2i c( iter.Value()->GetWidth(), iter.Value()->GetHeight() );
        if ( d.x<c.x ) { d.x = c.x; }
        if ( d.y<c.y ) { d.y = c.y; }
    }
    return d;
}

int SkinSet::GetDimension( int id, int dimID ) const
{
    Skin **pps = _sp_skins->Find(id);
    if ( pps )
    {
        return (*pps)->GetDimension( dimID );
    }
    return 0;
}

void SkinSet::Cooperate( int id, Widget *client)
{
    Skin &skin = GetSkin(id);
    client->BeAlwaysDirty( skin.GetFrameRate() > 0 );
}

};//namespace Useless
