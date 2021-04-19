#include "UselessPch.h"

#include "Useless/Graphic/Planes/OGraphics.h"
#include "Useless/Graphic/Device/Surface.h"

namespace Useless {


    void OGraphics::SetConstantAlpha( int alpha )
    {
        Surface *s = GetSurface();
        if (!!s)
        {
            s->SetConstantAlpha( alpha );
        }
    }

    int  OGraphics::GetConstantAlpha() const
    {
        const Surface *s = GetSurface();
        return !s ? 0 : s->GetConstantAlpha();
    }


}; //Useless
