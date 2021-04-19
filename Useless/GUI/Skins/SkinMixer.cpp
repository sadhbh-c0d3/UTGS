#include "UselessPch.h"

#include "Useless/GUI/Skins/SkinMixer.h"

#include "Useless/GUI/Widgets/Widget.h" // used for Cooperate

namespace Useless {

void SkinMixer::Paint( int id, const Painter& painter, const Pos &p )
{
    MixMap::Iterator it;

    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        int w = it->size.x;
        int h = it->size.y;
        Rect max_rect( 0, 0, w, h);

        do {
            int s_id = it->source_id;
            if ( it->alignment != -1 )
            {
                SkinSet::SafeResize( s_id, it->size);

                int i_w = SkinSet::GetWidth(s_id);
                int i_h = SkinSet::GetHeight(s_id);

                Rect skin_rect( 0, 0, i_w, i_h);

                skin_rect.Align( max_rect, Rect::Alignment(it->alignment) );

                Pos pos = skin_rect.GetPos() + it->offset;

                SkinSet::Paint( s_id, painter, p+pos);
            }
            else
            {
                SkinSet::SafeResize( s_id, Dim2i(w,h));
                SkinSet::Paint( s_id, painter, p);
            }
            
        } while( !!++it && it.Key()==id );
    }
}

void SkinMixer::Mix( int output_id, const MixProps &mix_props )
{

    if (!mix_props.size) 
    {
        MixProps m_p  = mix_props;
        m_p.size = Dim2i( SkinSet::GetWidth(m_p.source_id),
                               SkinSet::GetHeight(m_p.source_id) 
                              );
        _mix_map.Insert( output_id, m_p );
    }
    else
    {
        _mix_map.Insert( output_id, mix_props );
    }
}

void SkinMixer::Mix( int output_id, int source_id, int alignment, const Pos &offset )
{
    Mix( output_id, MixProps(source_id,alignment,offset) );
}

void SkinMixer::Mix( int output_id, const Skin &s, int alignment, const Pos &offset )
{
    Mix( output_id, MixProps( SkinSet::Insert(s) , alignment, offset) );
}


void SkinMixer::SetSize( int id, const Dim2i& d)
{
    /* Here should go all resizing code for other skin components ?
    ..............................................................*/
    MixMap::Iterator it;

    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    do {
        it->size = d;
            
    } while( !!++it && it.Key()==id );
}

int SkinMixer::GetWidth ( int id ) const
{
    MixProps *pp=_mix_map.Find(id); if (!!pp) { return pp->size.x; }
    return 0;
}

int SkinMixer::GetHeight( int id ) const
{
    MixProps *pp=_mix_map.Find(id); if (!!pp) { return pp->size.y; }
    return 0;
}

void SkinMixer::SetSizeAll( const Dim2i& d )
{
    MixMap::Iterator it = _mix_map.Begin();
    
    for (; !!it; )
    {
        it->size = d;
        int last_id = it.Key();
        while ( !!it && it.Key()==last_id ) { ++it; }
    }    
}


int SkinMixer::Size() const
{
    MixMap::ConstIterator it = _mix_map.ConstBegin();
    int cnt; 
    for ( cnt=0; !!it; )
    {
        ++cnt;
        int last_id = it.Key();
        while ( !!it && it.Key()==last_id ) { ++it; }
    }    
    return cnt;
}

bool SkinMixer::Exists( int id ) const
{
    return !!_mix_map.Find(id);
}

Dim2i SkinMixer::GetMaxSize() const
{
    MixMap::ConstIterator it = _mix_map.ConstBegin();
    Dim2i d;
    
    for (; !!it; )
    {
        if (d.x<it->size.x) { d.x = it->size.x; }
        if (d.y<it->size.y) { d.y = it->size.y; }

        int last_id = it.Key();
        while ( !!it && it.Key()==last_id ) { ++it; }
    }    

    return d;
}

void SkinMixer::Cooperate( int id, Widget *client)
{
    MixMap::Iterator it;
    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        bool is_always_dirty = false;
        do {
            int s_id = it->source_id;
            SkinSet::Cooperate( s_id, client);
            is_always_dirty |= client->IsAlwaysDirty();
            
        } while( !!++it && it.Key()==id );
        
        client->BeAlwaysDirty( is_always_dirty);
    }
}


Pos
SkinMixer::GetDirection ( int id ) const
{
    MixMap::Iterator it;
    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        int s_id = it->source_id;
        return SkinSet::GetDirection( s_id);
    }
    return Pos();
}

Pos
SkinMixer::GetDirMask   ( int id ) const
{
    MixMap::Iterator it;
    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        int s_id = it->source_id;
        return SkinSet::GetDirMask( s_id);
    }
    return Pos();
}

void
SkinMixer::SetDirMask   ( int id, const Pos & m)
{
    MixMap::Iterator it;
    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        int s_id = it->source_id;
        SkinSet::SetDirMask( s_id, m);
    }
}

bool
SkinMixer::SafeResize   ( int id, const Dim2i& d)
{
    SetSize( id, d);
    return true;
}

int
SkinMixer::GetDimension ( int id, int dimID ) const
{
    MixMap::Iterator it;
    if ( !!_mix_map.Find(id, (MixMap::ConstIterator*)&it) )
    {
        int s_id = it->source_id;
        return SkinSet::GetDimension( s_id, dimID);
    }
	return 0;
}

};//namespace Useless
