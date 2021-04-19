#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

#include <math.h>

namespace Useless {

Resource* CreateGraphWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string color = get_attribute( attributes, "color" );
    NormalPixel ncolor( to_hex( color ));

    int dotted = 0;
    try_attribute_i( attributes, "dotted", &dotted );

    GraphWidget *ptr = new GraphWidget( Dim2i( 100, 100 ), ncolor.c, 
                                        ( dotted ? GraphWidget::GT_POINTS : GraphWidget::GT_LINES ) 
                                      );
    
    std::string bounds;
    if ( try_attribute( attributes, "bounds", &bounds ))
    {
        std::vector< int > b = to_integer_vector( bounds );
        ptr->SetValueBounds( b[0], b[1], b[2], b[3] );
    }
    
    Useless::GraphWidget::tpe_input_vector initData( 100 );
    initData.resize(0);
    for ( double f=0.0; f <= 6.28; f += 3.14/100.0 )
    {
        double y = sin( f );
        initData.push_back( y );
    }
    ptr->SetInputData( initData );

    InsertChildWidget( ptr, attributes, env);    
    return new AnyResource<GraphWidget*>( ptr );
}

}; // namespace Useless
