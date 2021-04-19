#ifndef __INCLUDED_USELESS_SKIN_SET_H__
#define __INCLUDED_USELESS_SKIN_SET_H__

#include "Useless/GUI/Skins/SkinMan.h"
#include "Useless/GUI/Skins/Skin.h"
#include "Useless/Util/Storage/AutoVector.h"

namespace Useless {

/*! \ingroup Skins
 *  Manager holding set of skins.
 */
class USELESS_API SkinSet : public SkinMan
{
public:
    SkinSet();
    SkinSet( CopyOf<Skin> skin_copy, int id=0 );
    virtual ~SkinSet();
    SkinSet( const SkinSet&);

    void Insert( int id, CopyOf<Skin> skin_copy );
     int Insert(         CopyOf<Skin> skin_copy ); //< Auto associate ID

    void Remove( int id );

            Skin& GetSkin( int id )            { return **( (*_sp_skins).Find(id) ); }
      const Skin& GetSkin( int id )      const { return **( (*_sp_skins).Find(id) ); }
    virtual   int  Size     ()           const { return (*_sp_skins).Size(); }
    virtual  bool  Empty    ()           const { return (*_sp_skins).Empty(); }
    virtual  bool  Exists   ( int id )   const { return (*_sp_skins).Find(id); }
    
    virtual  void  Paint    ( int id, const Painter& painter, const Pos& = Pos() );

    virtual   int  GetWidth    ( int id )             const { return (*_sp_skins)[id]->GetWidth();  }
    virtual   int  GetHeight   ( int id )             const { return (*_sp_skins)[id]->GetHeight(); }
    virtual   Pos  GetDirection( int id )             const { return (*_sp_skins)[id]->GetDirection(); }
    virtual  void  SetSize     ( int id, const Dim2i& d)    { (*_sp_skins)[id]->SetSize(d); }
    virtual   Pos  GetDirMask  ( int id )             const { return (*_sp_skins)[id]->GetDirMask(); }
    virtual  void  SetDirMask  ( int id, const Pos &m)      { (*_sp_skins)[id]->SetDirMask(m); }
    virtual  void  SetSizeAll  ( const Dim2i& d);
    virtual Dim2i  GetMaxSize  ()                     const;
    virtual  bool  SafeResize  ( int id, const Dim2i& d);

    //! Some skins may have specific dimentions i.e. cell padding
    virtual  int   GetDimension( int id, int dimID ) const;

    //! Set cooperation between widget and set of skins
    virtual void Cooperate( int id, Widget *client);

private:
    typedef AssocVector<int, Skin*> SkinMap;
    typedef AutoVector<Skin*> __Skins;
    SPointer< __Skins > _sp_skins;
};

};//namespace Useless

REGISTER_COPIER(Useless,SkinMan,SkinSet);

#endif//__INCLUDED_USELESS_SKIN_SET_H__
