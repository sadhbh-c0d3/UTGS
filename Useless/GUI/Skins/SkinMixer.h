#ifndef __INCLUDED_USELESS_SKIN_MIXER_H__
#define __INCLUDED_USELESS_SKIN_MIXER_H__

#include "Useless/GUI/Skins/SkinSet.h"

namespace Useless {

/*! Creates set of skins, mixing several source skins together 
--------------------------------------------------------------*/   
class USELESS_API SkinMixer:  public SkinSet
{
public:    

    virtual void  Paint  ( int id, const Painter& painter, const Pos &p =Pos() );
    virtual void  SetSize( int id, const Dim2i& d);
    virtual void  SetSizeAll  ( const Dim2i& d );
    virtual Dim2i GetMaxSize  () const;

    virtual   int GetWidth ( int id ) const;
    virtual   int GetHeight( int id ) const;

    virtual   int Size     ()         const;
    virtual  bool Exists   ( int id ) const;
    virtual  void Cooperate( int id, Widget *client);

    virtual   Pos  GetDirection( int id ) const;
    virtual   Pos  GetDirMask  ( int id ) const;
    virtual  void  SetDirMask  ( int id, const Pos & m);
    virtual  bool  SafeResize  ( int id, const Dim2i& d);
    virtual   int  GetDimension( int id, int dimID ) const;
    
    struct MixProps;
            void  Mix( int output_id, const MixProps &mix_props );
            void  Mix( int output_id, int source_id, int alignment=0, const Pos &offset=Pos() );
            void  Mix( int output_id, const Skin &s, int alignment=0, const Pos &offset=Pos() );
    
    /*! Mixing properties
    -------------------------------------------------------------*/
    struct MixProps 
    {
        MixProps() {}
        MixProps( int sid, int algn=0, const Pos &offs=Pos() ):
                source_id(sid), alignment(algn), offset(offs) {}

        MixProps( const MixProps &mp, int sid ): 
                source_id(sid), alignment(mp.alignment),
                offset(mp.offset), size(mp.size) {}
            
        int    source_id;   //< ID of source skin to mix-in
        int    alignment;   //< See Rect::Alignment
        Pos    offset;      //< Additional offset from aligned position
        Dim2i  size;        //< Desired compound size
    };
    
private:
    typedef AssocVector< int, MixProps > MixMap;
    MixMap  _mix_map;
};

};//namespace Useless

#include "Useless/Util/CopyingFactory.h"
REGISTER_COPIER(Useless,SkinMan,SkinMixer);

#endif//__INCLUDED_USELESS_SKIN_MIXER_H__


