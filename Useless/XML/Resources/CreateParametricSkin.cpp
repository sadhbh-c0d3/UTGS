#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateParametricSkin( XMLIterator i, XMLCreatorEnv *env )
{
    ParmList_19<
        CPT::border_type,
        CPT::border_width,
        CPT::h_divider_width,
        CPT::v_divider_width,
        CPT::h_divider_margin,
        CPT::v_divider_margin,
        CPT::cross_h_margin,
        CPT::cross_v_margin,
        CPT::use_inner_bevel,
        CPT::use_outer_bevel,
        CPT::use_h_dividers,
        CPT::use_v_dividers,
        CPT::use_body,
        CPT::border_color,
        CPT::highlight_color,
        CPT::shadow_color,
        CPT::body_color,
        CPT::h_divider_color,
        CPT::v_divider_color > parms;
        
    std::string temp_str;
    int temp_i;
    int border_type_i = ParametricSkin::BORDER_3D;
    
    const XMLParser::Node &node = *i;

    if ( try_attribute( node._attributes, "border_type", &temp_str) )
    {
        if ( temp_str == "3D" )
            border_type_i = ParametricSkin::BORDER_3D;
        else if ( temp_str == "BEVEL_IN" )
            border_type_i = ParametricSkin::BEVEL_IN;
        else if ( temp_str == "BEVEL_OUT" )
            border_type_i = ParametricSkin::BEVEL_OUT;
        else if ( temp_str == "NONE" )
            border_type_i = ParametricSkin::NO_BORDER;
        else
            throw Error("Wrong border type");
    }
    CTC::Assign( parms[ CTC::type<CPT::border_type>() ], border_type_i );

    if ( try_attribute_i( node._attributes, "border_width", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::border_width>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::border_width>() ], 1 );

    if ( try_attribute_i( node._attributes, "h_divider_width", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::h_divider_width>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::h_divider_width>() ], 1 );

    if ( try_attribute_i( node._attributes, "v_divider_width", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::v_divider_width>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::v_divider_width>() ], 1 );

    if ( try_attribute_i( node._attributes, "h_divider_margin", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::h_divider_margin>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::h_divider_margin>() ], 1 );

    if ( try_attribute_i( node._attributes, "v_divider_margin", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::v_divider_margin>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::v_divider_margin>() ], 1 );

    if ( try_attribute_i( node._attributes, "cross_h_margin", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::cross_h_margin>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::cross_h_margin>() ], 0 );

    if ( try_attribute_i( node._attributes, "cross_v_margin", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::cross_v_margin>() ], temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::cross_v_margin>() ], 0 );

    if ( try_attribute_i( node._attributes, "use_inner_bevel", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::use_inner_bevel>() ], (bool)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::use_inner_bevel>() ],
            border_type_i==ParametricSkin::BORDER_3D ||
            border_type_i==ParametricSkin::BEVEL_IN );

    if ( try_attribute_i( node._attributes, "use_outer_bevel", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::use_outer_bevel>() ], (bool)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::use_outer_bevel>() ],
            border_type_i==ParametricSkin::BORDER_3D ||
            border_type_i==ParametricSkin::BEVEL_OUT );

    if ( try_attribute_i( node._attributes, "use_h_dividers", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::use_h_dividers>() ], (bool)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::use_h_dividers>() ], true );

    if ( try_attribute_i( node._attributes, "use_v_dividers", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::use_v_dividers>() ], (bool)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::use_v_dividers>() ], true );

    if ( try_attribute_i( node._attributes, "use_body", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::use_body>() ], (bool)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::use_body>() ], true );

    if ( try_attribute_h( node._attributes, "border_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::border_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::border_color>() ], (NormalPixel)0xA0A0A0 );

    if ( try_attribute_h( node._attributes, "body_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::body_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::body_color>() ], (NormalPixel)0xD0D0D0 );

    if ( try_attribute_h( node._attributes, "highlight_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::highlight_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::highlight_color>() ], (NormalPixel)0xF0F0F0 );

    if ( try_attribute_h( node._attributes, "shadow_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::shadow_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::shadow_color>() ], (NormalPixel)0x000000 );

    if ( try_attribute_h( node._attributes, "h_divider_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::h_divider_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::h_divider_color>() ], (NormalPixel)0xC0C0C0 );

    if ( try_attribute_h( node._attributes, "v_divider_color", &temp_i) )
        CTC::Assign( parms[ CTC::type<CPT::v_divider_color>() ], (NormalPixel)temp_i );
    else
        CTC::Assign( parms[ CTC::type<CPT::v_divider_color>() ], (NormalPixel)0x808080 );

    ParametricSkin *p_skin = new ParametricSkin( parms );

    SetupSkinAttributes( *p_skin, node._attributes);

    int col=0, row=0;
    for ( i.StepInto(); !!i; ++i )
    {
        const XMLParser::Node &node = *i;
        int color;

        if ( node._name == "cell" )
        {
            color = to_hex( get_attribute( node._attributes, "color" ) );
            std::string pos = get_attribute( node._attributes, "pos" );
            std::stringstream itp; itp << pos; int x,y;
            itp >> x; itp >> y;
            p_skin->ColorCell( x,y, color );
        }
        else if ( try_attribute_h( node._attributes, "color", &color ) )
        {
            if ( node._name == "col" )
            {
                p_skin->ColorColumn( col++, color );
            }
            else if ( node._name == "row" )
            {
                p_skin->ColorRow( row++, color );
            }
        }
        else if ( node._name == "col" ) { ++col; }
        else if ( node._name == "row" ) { ++row; }
    }

    InsertChildSkin( *p_skin, env);

    return new AnyResource< SPointer<Skin> >( p_skin );
}

};//namespace Useless
