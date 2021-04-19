#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"
#include "Useless/XML/Resources/CreateBinders.h"

#include "Useless/System/KeyCodes.h"
#include "Useless/GUI/Misc/Layout.h"
#include "Useless/GUI/Relations/FillRelation.h"
#include "Useless/GUI/Relations/PlaceRelation.h"
#include "Useless/GUI/Widgets/FormWidget.h"
#include "Useless/GUI/Widgets/TableSystemWidget.h"
#include "Useless/GUI/Widgets/TemplateWidget.h"
#include "Useless/GUI/Widgets/WidgetTableWidget.h"

namespace Useless {

Resource* CreateFrameWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    const char *p_name = "create frame widget";
    BeginParentWidget( env, p_name, ch_env );

    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env );

    HubWidget * ptr;

    int form = 0;
    try_attribute_i( attributes, "form", &form);

    std::string skin, layout;
    if ( (*i)._name == "frame" )
    {
        skin = get_attribute( attributes, "skin" );
        int left=-1, right=-1, top=-1, bottom=-1;

        try_attribute_i( attributes, "left", &left);
        try_attribute_i( attributes, "right", &right);
        try_attribute_i( attributes, "top", &top);
        try_attribute_i( attributes, "bottom", &bottom);

        FrameStyle style( 100, 100, QuerySkin(skin), left, top, right, bottom );
        if( form == 1 )
            ptr = new TemplateWidget<FrameWidget,FormWidget>( style );
        else
            ptr = new FrameWidget( style );
    }
    else
    {
        if( form == 1 )
            ptr = new TemplateWidget<HubWidget,FormWidget>( 100, 100 );
        else
        {
            C_assert( !try_attribute( attributes, "skin", &skin )
            && "MISTAKE WARNING: Hub cannot have skin" );

            ptr = new HubWidget( 100, 100 );
        }
    }
    
    LayoutBase * p_layout=0;

    // use layout
    if ( try_attribute( attributes, "layout", &layout) )
    {
        if ( layout == "horizontal" )
        {
            p_layout = new HLayout( ptr );
        }
        else // vertical
        {
            p_layout = new VLayout( ptr );
        }
        ptr->Posses( p_layout );
        ch_env["parent_layout"] = p_layout;
        ch_env["parent_hub"] = ptr;
        Resources::Instance().Insert("layouts", p_name, new AnyResource<LayoutBase*>(p_layout) );
    }
    // regular frame
    else
    {
        ch_env["parent_hub"] = ptr;
    }

    // create children elements
    ChildsFromXML( i, &ch_env );

    InsertChildWidget( ptr, attributes, env);

    return new AnyResource<HubWidget*>( ptr );
}



void InsertChildWidget( Widget *ptr, const XMLAttributes &attributes, XMLCreatorEnv *env )
{
    LayoutBase  *p_parent_layout=0;
    HubWidget   *p_parent_hub=0;
    SizerWidget *p_parent_sizer=0;
    WidgetTableWidget *p_parent_table=0;
    TableSystemWidget *p_parent_table_system=0;

    int ignore_layout = false;
    try_attribute_i( attributes, "ignore_layout", &ignore_layout );

    if ( !ignore_layout &&
            (*env).Query("parent_layout", p_parent_layout) )
    {
        (*p_parent_layout) << ptr;
        int x;
        bool horizontal_layout = (0 != dynamic_cast< HLayout * >( p_parent_layout ));
        bool vertical_layout = !horizontal_layout;

        if ( try_attribute_i( attributes, "weight", &x) )
        {
            Weight_ w(x); (*p_parent_layout) << w;
        }

        if ( try_attribute_i( attributes, "overlap", &x) )
        {
            Overlap_ w(x); (*p_parent_layout) << w;
        }

        if ( try_attribute_i( attributes, "padstart", &x) )
        {
            PadStart_ w(x); (*p_parent_layout) << w;
        }
        if ( try_attribute_i( attributes, "padend", &x) )
        {
            PadEnd_ w(x); (*p_parent_layout) << w;
        }
        
        if ( try_attribute_i( attributes, "alignsize", &x) )
        {
            AlignSize_ a(x); (*p_parent_layout) << a;
        }
        if ( try_attribute_i( attributes, "alignsizepad", &x) )
        {
            AlignSizePad_ a(x); (*p_parent_layout) << a;
        }
        
        if ( try_attribute_i( attributes, "fixedsize", &x) )
        {
            MinSize_ w1(x); (*p_parent_layout) << w1;
            MaxSize_ w2(x); (*p_parent_layout) << w2;
        }
        else
        {
            if ( try_attribute_i( attributes, "minsize", &x) )
            {
                MinSize_ w(x); (*p_parent_layout) << w;
            }
            if ( try_attribute_i( attributes, "maxsize", &x) )
            {
                MaxSize_ w(x); (*p_parent_layout) << w;
            }
        }

        if ( try_attribute_i( attributes, "orthofixedsize", &x) )
        {
            OrthoMinSize_ a1(x); (*p_parent_layout) << a1;
            OrthoMaxSize_ a2(x); (*p_parent_layout) << a2;
        }
        else
        {
            if ( try_attribute_i( attributes, "orthominsize", &x) )
            {
                OrthoMinSize_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "orthomaxsize", &x) )
            {
                OrthoMaxSize_ a(x); (*p_parent_layout) << a;
            }
        }
        if ( try_attribute_i( attributes, "orthoalignsize", &x) )
        {
            OrthoAlignSize_ a(x); (*p_parent_layout) << a;
        }
        if ( try_attribute_i( attributes, "orthoalignsizepad", &x) )
        {
            OrthoAlignSizePad_ a(x); (*p_parent_layout) << a;
        }
        if ( try_attribute_i( attributes, "orthoplacement", &x) )
        {
            OrthoPlacement_ a(x); (*p_parent_layout) << a;
        }
        
        if ( horizontal_layout )
        {
            if ( try_attribute_i( attributes, "fixedwidth", &x ))
            {
                MinSize_ w1(x); (*p_parent_layout) << w1;
                MaxSize_ w2(x); (*p_parent_layout) << w2;
            }
            else
            {
                if ( try_attribute_i( attributes, "minwidth", &x) )
                {
                    MinSize_ w(x); (*p_parent_layout) << w;
                }
                if ( try_attribute_i( attributes, "maxwidth", &x) )
                {
                    MaxSize_ w(x); (*p_parent_layout) << w;
                }
            }
            if ( try_attribute_i( attributes, "alignwidth", &x) )
            {
                AlignSize_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "alignwidthpad", &x) )
            {
                AlignSizePad_ a(x); (*p_parent_layout) << a;
            }

            if ( try_attribute_i( attributes, "fixedheight", &x) )
            {
                OrthoMinSize_ a1(x); (*p_parent_layout) << a1;
                OrthoMaxSize_ a2(x); (*p_parent_layout) << a2;
            }
            else
            {
                if ( try_attribute_i( attributes, "minheight", &x) )
                {
                    OrthoMinSize_ a(x); (*p_parent_layout) << a;
                }
                if ( try_attribute_i( attributes, "maxheight", &x) )
                {
                    OrthoMaxSize_ a(x); (*p_parent_layout) << a;
                }
            }
            if ( try_attribute_i( attributes, "alignheight", &x) )
            {
                OrthoAlignSize_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "alignheightpad", &x) )
            {
                OrthoAlignSizePad_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "vplacement", &x) )
            {
                OrthoPlacement_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "vpadstart", &x) )
            {
                PadStart_ w(x); (*p_parent_layout) << w;
            }
            if ( try_attribute_i( attributes, "vpadend", &x) )
            {
                PadEnd_ w(x); (*p_parent_layout) << w;
            }
        }
        
        if ( vertical_layout )
        {
            if ( try_attribute_i( attributes, "fixedheight", &x ))
            {
                MinSize_ w1(x); (*p_parent_layout) << w1;
                MaxSize_ w2(x); (*p_parent_layout) << w2;
            }
            else
            {
                if ( try_attribute_i( attributes, "minheight", &x) )
                {
                    MinSize_ w(x); (*p_parent_layout) << w;
                }
                if ( try_attribute_i( attributes, "maxheight", &x) )
                {
                    MaxSize_ w(x); (*p_parent_layout) << w;
                }
            }
            if ( try_attribute_i( attributes, "alignheight", &x) )
            {
                AlignSize_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "alignheightpad", &x) )
            {
                AlignSizePad_ a(x); (*p_parent_layout) << a;
            }


            if ( try_attribute_i( attributes, "fixedwidth", &x) )
            {
                OrthoMinSize_ a1(x); (*p_parent_layout) << a1;
                OrthoMaxSize_ a2(x); (*p_parent_layout) << a2;
            }
            else
            {
                if ( try_attribute_i( attributes, "minwidth", &x) )
                {
                    OrthoMinSize_ a(x); (*p_parent_layout) << a;
                }
                if ( try_attribute_i( attributes, "maxwidth", &x) )
                {
                    OrthoMaxSize_ a(x); (*p_parent_layout) << a;
                }
            }
            if ( try_attribute_i( attributes, "alignwidth", &x) )
            {
                OrthoAlignSize_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "alignwidthpad", &x) )
            {
                OrthoAlignSizePad_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "hplacement", &x) )
            {
                OrthoPlacement_ a(x); (*p_parent_layout) << a;
            }
            if ( try_attribute_i( attributes, "hpadstart", &x) )
            {
                PadStart_ w(x); (*p_parent_layout) << w;
            }
            if ( try_attribute_i( attributes, "hpadend", &x) )
            {
                PadEnd_ w(x); (*p_parent_layout) << w;
            }
        }
    }
    else if ( (*env).Query("parent_hub", p_parent_hub) )
    {
        (*p_parent_hub).Insert( ptr );
        ApplyWidgetSize( ptr, attributes);
    }
    else if ( (*env).Query("parent_sizer", p_parent_sizer) )
    {
        int idx = (*p_parent_sizer).GetNumCells();
        int size;
        bool fixed;
        int align = H_CENTER;
        int valign = V_CENTER;
        std::string sizes="100";
        try_attribute( attributes, "size", &sizes );
        fixed = (sizes.find("px",0) != -1)? 1:0;
        size = to_integer(sizes);

        std::string align_str;
        if ( try_attribute( attributes, "valign", &align_str ) )
        {
            if (align_str=="center") { valign = V_CENTER;} else
            if (align_str=="top")    { valign = V_TOP;   } else
            if (align_str=="bottom") { valign = V_BOTTOM;}
        }
        if ( try_attribute( attributes, "align", &align_str ) )
        {
            if (align_str=="center") { align = H_CENTER;} else
            if (align_str=="left")   { align = H_LEFT;  } else
            if (align_str=="right")  { align = H_RIGHT; }
        }

        (*p_parent_sizer).AddCell( size, fixed );
        (*p_parent_sizer).Insert( idx, ptr, align + valign );
        ApplyWidgetSize( ptr, attributes);
    }
    else if ( (*env).Query("parent_table", p_parent_table) )
    {
        int align = H_CENTER;
        int valign = V_CENTER;

        std::string align_str;
        if ( try_attribute( attributes, "valign", &align_str ) )
        {
            if (align_str=="center") { valign = V_CENTER;} else
            if (align_str=="top")    { valign = V_TOP;   } else
            if (align_str=="bottom") { valign = V_BOTTOM;}
        }
        if ( try_attribute( attributes, "halign", &align_str ) )
        {
            if (align_str=="center") { align = H_CENTER;} else
            if (align_str=="left")   { align = H_LEFT;  } else
            if (align_str=="right")  { align = H_RIGHT; }
        }

        int row;
        (*env).Query("row_num", row);
        
        int column;
        (*env).Query("column_num", column);

        if ( p_parent_table->Settled( column, row ))
        {
            p_parent_table->Remove( column, row );
        }
               
        p_parent_table->Insert( column, row, ptr, align | valign );        
        ApplyWidgetSize( ptr, attributes);
    }
    else if ( (*env).Query("parent_tablesystem", p_parent_table_system ))
    {
        ApplyWidgetSize( ptr, attributes);
        p_parent_table_system->Insert( ptr );
    }

    ApplyWidgetAttr( ptr, attributes );
}

void ApplyWidgetAttr( Widget *ptr, const XMLAttributes &attributes )
{
    int always_dirty;
    if ( try_attribute_i( attributes, "always_dirty", &always_dirty))
    {
        ptr->BeAlwaysDirty(always_dirty);
    }
    
    int hidden=0;
    if ( try_attribute_i( attributes, "hidden", &hidden ))
    {
        if ( hidden )
            ptr->Hide();
    }

    int phantom=0;
    if ( try_attribute_i( attributes, "phantom", &phantom ))
    {
        ptr->SetPhantom( phantom );
    }

    int opacity=255;
    if ( try_attribute_i( attributes, "opacity", &opacity ))
    {
        ptr->SetOpacity( opacity );
    }


    int zindex=50;
    if ( try_attribute_i( attributes, "zindex", &zindex ))
    {
        HubWidget *hub = dynamic_cast< HubWidget *>( ptr->GetParent());        
        assert( hub );
        hub->SetChildZIndex( zindex, ptr );
    }
    
    if ( ActiveWidget *pa = dynamic_cast<ActiveWidget*>(ptr))
    {
        int sticky_focus=false;
        int sloppy_focus=false;
        if ( try_attribute_i( attributes, "sticky_focus", &sticky_focus) && sticky_focus ||
                try_attribute_i( attributes, "sloppy_focus", &sloppy_focus) && sloppy_focus)
        {
            pa->OnMoveOver.TieVoid( pa, &Widget::FocusIntercept);
            if ( sloppy_focus)
            {
                pa->OnMoveOutside.TieVoid( pa, &Widget::FocusRelease);
            }
        }

        std::string key_name;
        if ( try_attribute( attributes, "accel_key", &key_name ))
        {
            pa->SetActivationKey( Keys::GetCode( key_name ));
        }

        if ( KeyButtonLogic *pkb = dynamic_cast< KeyButtonLogic *>( pa ))
        {
            if ( try_attribute( attributes, "select_key", &key_name ))
            {
                pkb->EnableFocussing( Keys::GetCode( key_name ));
            }
        }

        int disabled=0;
        if ( try_attribute_i( attributes, "disabled", &disabled ))
        {
            if ( !disabled )
                pa->Activate();
            else
                pa->Deactivate();
        }

        int focus_adaptive=0;
        if ( try_attribute_i( attributes, "focus_adaptive", &focus_adaptive ))
        {
            pa->SetFocusAdaptive( focus_adaptive );
        }

        std::string binders;
        if ( try_attribute( attributes, "binders", &binders ))
        {
            std::vector< std::string > binderNames = to_tokens( binders );
            for ( int i=0, I=binderNames.size(); i<I; ++i )
            {
                GUIBinderResource::Type binder = *GUIBinderResource("binders", binderNames[i] );
                binder->BindTo( pa );
            }
        }
    }

    if ( IWEnum *penum = dynamic_cast< IWEnum *>( ptr ))
    {
        if ( ListLogic *pll = dynamic_cast< ListLogic *>( penum ))
        {
            int alwaysSelected = 0;
            int selectorSelects = 0;
            int followMode = 0;
            if ( try_attribute_i( attributes, "always_selected", &alwaysSelected ))
            {
                pll->AllwaysSelected( alwaysSelected );
            }
            if ( try_attribute_i( attributes, "selector_selects", &selectorSelects ))
            {
                pll->SelectorSelects( selectorSelects );
            }
            if ( try_attribute_i( attributes, "follow_mode", &followMode ))
            {
				pll->SetFollowMode( Useless::ListLogic::Follow( followMode ));
            }
        }
    }   

    if ( Form *pf = dynamic_cast< Form *>( ptr ))
    {
        std::string submit_key, cancel_key;
        if ( try_attribute( attributes, "submit_key", &submit_key ))
        {
            pf->SetSubmitKey( Keys::GetCode( submit_key ));
        }
        if ( try_attribute( attributes, "cancel_key", &cancel_key ))
        {
            pf->SetCancelKey( Keys::GetCode( cancel_key ));
        }
    }
}

void ApplyWidgetSize( Widget *ptr, const XMLAttributes &attributes )
{
    std::string temp_string;
    int auto_fill=0;
    try_attribute_i( attributes, "autofill", &auto_fill );
    if (auto_fill)
    {
        Rect padding;
        try_attribute_i( attributes, "hpadstart", &padding.x );
        try_attribute_i( attributes, "vpadstart", &padding.y );
        try_attribute_i( attributes, "hpadend", &padding.w );
        try_attribute_i( attributes, "vpadend", &padding.h );
        ptr->Posses( new FillRelation(*ptr, padding) );
    }
    else if ( try_attribute( attributes, "place", &temp_string))
    {
        std::vector<int> place = to_integer_vector( temp_string);
        std::vector<int> base;
        if ( try_attribute( attributes, "placer_base", &temp_string))
        {
            base = to_integer_vector( temp_string);
        }
        base.push_back(100);
        base.push_back(100);
        Pos b( base[0], base[1]);
        Rect alignSize;
        Pos  alignPlace;
        try_attribute_i( attributes, "alignwidthpad", &alignSize.x );
        try_attribute_i( attributes, "alignheightpad", &alignSize.y );
        try_attribute_i( attributes, "alignwidth", &alignSize.w );
        try_attribute_i( attributes, "alignheight", &alignSize.h );
        try_attribute_i( attributes, "hplacement", &alignPlace.x );
        try_attribute_i( attributes, "vplacement", &alignPlace.y );
        
        if ( place.size() == 2)
        {
            Pos p( place[0], place[1]);
            ptr->Posses( new PlaceRelation(*ptr, p, b, alignSize, alignPlace ));
        }
        else {
            C_assert( place.size() == 4);
            Rect r( place[0], place[1], place[2], place[3]);
            ptr->Posses( new PlaceRelation(*ptr, r, b, alignSize, alignPlace ));
        }
    }
    else
    {
        int w = ptr->GetWidth();
        int h = ptr->GetHeight();

        bool given_w = try_attribute_i( attributes, "width", &w );
        bool given_h = try_attribute_i( attributes, "height", &h );
        if ( given_w || given_h )
        {
            ptr->Resize(w,h);
        }

        std::string position;
        if ( try_attribute( attributes, "pos", &position ) )
        {
            std::vector<int> pos = to_integer_vector( position );
            if ( pos.size() != 2 ) { throw Error("pos attribute must have x and y coordinates"); }
            ptr->SetPosition( Pos(pos[0], pos[1] ));
        }
    }

    int hidden;
    if ( try_attribute_i( attributes, "hidden", &hidden))
    {
        ptr->SetVisible(!hidden);
    }
}

Resource* ResolveParentWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    CascadeStyles( i, env, attributes, branch_style, ch_env);

    const char *p_name = "resolve parent widget";
    BeginParentWidget( env, p_name, ch_env );

    std::string id = get_attribute( (*i)._attributes, "id" );
    ch_env.Set( "branch", id.c_str() );

    if ( Resources::Instance().Exists("layouts", id) )
    {
        ch_env.Set("parent_layout", &QueryLayout(id) );
    }
    else
    {
        Useless::Resource *r = Useless::Resources::Instance().QueryResource("widgets", id );
        if ( !r )
        {
            throw Error("Resource %s not found", id.c_str());
        }
        else if( r->GetTypeInfo() == typeid( HubWidget *))
        {
            ch_env.Set("parent_hub"
                    , *static_cast< AnyResource< HubWidget *> *>(r)->Get());
        }
        else if ( r->GetTypeInfo() == typeid( WidgetTableWidget *))
        {
            ch_env.Set("parent_table"
                    , *static_cast< AnyResource< WidgetTableWidget *> *>(r)->Get());
        }
        else if ( r->GetTypeInfo() == typeid( SizerWidget *))
        {
            ch_env.Set("parent_sizer"
                    , *static_cast< AnyResource< SizerWidget *> *>(r)->Get());
        }
        else if ( r->GetTypeInfo() == typeid( TableSystemWidget *))
        {
            ch_env.Set("parent_tablesystem"
                    , *static_cast< AnyResource< TableSystemWidget *> *>(r)->Get());
        }
        else
        {
            throw Error("Resource %s found, but cannot be parent"
                    , id.c_str());
        }
    }

    ChildsFromXML( i, &ch_env );

    return NULL;
}

void BeginParentWidget(
      XMLCreatorEnv *env,
      const char*   &name,
      XMLCreatorEnv &ch_env )
{
    const char *p_cat;
    if ( !(*env).Query("category", p_cat) || strcmp( p_cat, "widgets" )!=0 )
    {
        throw Error("Cannot %s in %s context (must be widgets)", name);
    }

    ch_env.Set( "category", p_cat ); //not needed - we have copy of parent

    if ( (*env).Query("name", name ) )
    {
        ch_env.Set( "branch", name ); // override branch name
    }
}


};//namespace Useless
