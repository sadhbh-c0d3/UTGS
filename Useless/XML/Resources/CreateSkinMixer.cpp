#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateSkinMixer( XMLIterator i, XMLCreatorEnv *env)
{
    SPointer<SkinMan> sp_skin_man;
    SkinMixer *p_mixer = new SkinMixer;

    const XMLNode &node = *i;

    std::map< std::string, int > skin_IDs;

    std::string map_name = get_attribute( node._attributes, "mapping");
    
    std::vector<int> *p_map;
    Resources::Instance().Query( "mappings", map_name, p_map);

    int ni = 0;
    
    for ( i.StepInto(); !!i; ++i, ++ni)
    {
        int oid = (*p_map)[ni];
        
        if ( (*i)._name == "state" )
        {
            XMLIterator j = i;
            
            for ( j.StepInto(); !!j; ++j)
            {
                if ( (*j)._name == "mix")
                {
                    int iid;
                    std::string id = get_attribute( (*j)._attributes, "id");
                    if ( skin_IDs.find( id) == skin_IDs.end())
                    {
                        Skin &skin = QuerySkin( id);
                        iid = p_mixer->Insert( skin);
                        skin_IDs[id] = iid;
                    }
                    else {
                        iid = skin_IDs[id];
                    }
                    
                    int alignment = -1;
                    Pos offset(0,0);
                    std::string tmp;
                    if ( try_attribute( (*j)._attributes, "halign", &tmp ))
                    {
                        if ( tmp == "left" ) { alignment = Rect::HALIGNLEFT; } else
                        if ( tmp == "center" ) { alignment = Rect::HALIGNCENTER; } else
                        if ( tmp == "right" ) { alignment = Rect::HALIGNRIGHT; } else
                        if ( tmp == "fill" ) { alignment = -1; }
                    }
                    if ( try_attribute( (*j)._attributes, "valign", &tmp ))
                    {
                        if ( alignment == -1 ) { alignment = 0; }
                        if ( tmp == "top" ) { alignment |= Rect::VALIGNTOP; } else
                        if ( tmp == "center" ) { alignment |= Rect::VALIGNCENTER; } else
                        if ( tmp == "bottom" ) { alignment |= Rect::VALIGNBOTTOM; } else
                        if ( tmp == "fill" ) { alignment = -1; }
                    }
                    
                    p_mixer->Mix( oid, iid, alignment, offset);
                    
                }
                else {
                    C_verify( !"CreateSkinMixer: Tag <mix /> was excpected.");
                }
            }
        }
    }

    return new AnyResource< SPointer<SkinMan> >( p_mixer);
}


};//namespace Useless
