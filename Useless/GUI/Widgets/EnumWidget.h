#ifndef __INCLUDED_USELESS_ENUM_WIDGET_H__
#define __INCLUDED_USELESS_ENUM_WIDGET_H__

#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Util/VariableUpdater.h"

namespace Useless {

    /*! \ingroup Widgets
     *  Widget with state information.
     */
    class USELESS_API EnumWidget : virtual public Widget, virtual public IWEnum
    {
        public:
            EnumWidget():
                OnChange(varSelector.OnUpdate),
            varSelector(-1)
            {
                Tie2Signal( OnChange, this, &EnumWidget::CheckSync ); 
            }
            virtual ~EnumWidget() {}

            virtual EnumWidget& Enumerator() { return *this; }

            /* Signal Refeernce            | Name            |      Args
             *------------------------------------------------------------------*/
            mutable Signal_1<int>           &OnChange;       // selected item no
            mutable Signal_1<int>            OnConfirm;      // selected item no


            virtual  int GetSelected() const = 0;
            virtual void SetSelected(int s) = 0;

            void Sync() { SetSelected( (int)varSelector); }

            VariableUpdater<int> varSelector;

        private:
            void CheckSync(int n) { varSelector.QuietUpdate(n); }
    };

};//namespace Useless


#endif//__INCLUDED_USELESS_ENUM_WIDGET_H__
