#include "UselessPch.h"

#include "Useless/GUI/Relations/ResizeRelation.h"

#include "Useless/ErrorConfig.h"


namespace Useless {

ResizeRelation::ResizeRelation( Widget &copier, Widget &copied, bool copy_hsize, bool copy_vsize )
{
    if ( copy_hsize && copy_vsize )
    {
        Tie2Signal( copied.OnResize, &copier, &Widget::Resize );
    }
    else if ( copy_hsize )
    {
        copied.OnResize.TieBinary
            ( &copier, 
              &Widget::Resize,
               copied.OnResize.GetVar1(),
               PackFuncPtr2Holder( 
                     PackFunctorR( VoidRCall(&copier, &Widget::GetHeight, 0), 0)
                     )
            );
    }
    else if ( copy_vsize )
    {
        copied.OnResize.TieBinary
            ( &copier, 
              &Widget::Resize,
               PackFuncPtr2Holder( 
                     PackFunctorR( VoidRCall(&copier, &Widget::GetWidth, 0), 0)
                     ),
               copied.OnResize.GetVar2()
            );
    }
    else
    {
        APP_WARNING( Error("At least one direction of rsizizing should be set"), "ResizeRelation" );
    }

}

ResizeRelation::~ResizeRelation()
{
    _on_resize.Untie();
}


};//namespace Useless
