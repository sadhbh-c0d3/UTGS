#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateConnections.h"
#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateSignalEvents.h"
#include "Useless/System/Application.h"
#include "Useless/System/KeyCodes.h"
#include "Useless/GUI/Relations/MultiTabRelation.h"
#include "Useless/GUI/Relations/AdvScrollRelation.h"
#include "Useless/GUI/Workspace.h"
#include "Useless/XML/Resources/SignalMap.h"

namespace Useless {

// Temporary goto handler
//----------------------------
static Widget *_current_screen = NULL;

void SetCurrentScreen(Widget * target)
{
    if (_current_screen)
    {
        _current_screen->Hide();
    }
    
    _current_screen = target;

    if ( _current_screen )
    {
        _current_screen->Show();
    }
}

struct FuncGotoScreen : PackedFunc
{
    virtual void operator()()
    {
        SetCurrentScreen( _next);
    }

    FuncGotoScreen( Widget *next): _next(next) {}
    Widget *_next;
};

struct FuncGetWorkspace
{
    virtual Workspace * operator()() const
    {
        assert( _widget->GetEnvironment() );
        return &_widget->GetEnvironment()->GetWorkspace();
    }

    FuncGetWorkspace( Widget *widget ): _widget( widget ) {}
    Widget *_widget;
};




//----------------------------

namespace {
    typedef std::map< Widget *, SignalSwitch< int > > _SigMUX;
    
    typedef std::pair< Signal *, SignalSwitch< int > * > _SubSignal;

    _SubSignal
    _GetSubSignal( _SigMUX &sigmux, Widget *activator, Signal_1< int > &source )
    {
        if ( sigmux.find( activator ) == sigmux.end() )
        {
            Tie2Signal( source
                    , &sigmux[ activator ]
                    , &SignalSwitch< int >::Switch );
        }
        return _SubSignal( &source, &sigmux[ activator ] );
    }
    
    Signal & _SourceSignal( Signal &sig ) { return sig; }
    Signal & _SourceSignal( _SubSignal &sw ) { return *sw.first; }
    SignalSwitch< int > & _GetSwitch( _SubSignal &sw ) { return *sw.second; }

    void
    _Tie( Signal &sig, int subSignal, PkFunc pf )
        { sig.Tie( pf ); }

    void
    _Tie( _SubSignal &sw, int subSignal, PkFunc pf )
        { sw.second->Tie( subSignal, pf ); }



    template< class _Signal > void
    _XMLTieImpl( _Signal &signal, int subSignal, const std::string &baseid,
            const XMLAttributes &attr, XMLCreatorEnv *env )
    {
        std::string target;
        if ( try_attribute( attr, "invoke", &target))
        {
            assert( target=="Exit()");
           
            _Tie( signal, subSignal, PackFunctor( VoidCall(
                    Application::InstancePtr(),
                    &Application::Terminate )
                    ));
        }

        if ( try_attribute( attr, "goto", &target))
        {
            target = baseid + target;
            
            HubWidget *hub = QueryHubWidget( target);
            
            _Tie( signal, subSignal, PkFunc( new FuncGotoScreen(hub)));
        }
        
        if ( try_attribute( attr, "open", &target))
        {
            target = baseid + target;

            Widget *popup = QueryWidget( target );
            
            _Tie( signal, subSignal, PackFunctor
                    ( VoidCall
                      ( popup,
                        &Widget::Show )
                      )
                    );
        }

        if ( try_attribute( attr, "close", &target))
        {
            target = baseid + target;
            
            Widget *popup = QueryWidget( target );

            _Tie( signal, subSignal, PackFunctor
                    ( VoidCall
                      ( popup,
                        &Widget::Hide )
                      )
                    );
        }
        
        if ( try_attribute( attr, "show_at_pointer", &target))
        {
            target = baseid + target;

            Widget *popup = QueryWidget( target );
            
            _Tie( signal , subSignal , PackFunctor
                    ( UnaryCall
                      ( PackFunctor2Holder
                        ( FuncGetWorkspace( popup ),
                          (Workspace *)0 ),
                        &Workspace::ShowAtPointer ),
                      popup )
                    );
        }

        if ( try_attribute( attr, "event", &target ))
        {
            SPointer< SignalEvent > event = QuerySignalEvent( target );
            
            std::string argstr;
            try_attribute( attr, "string", &argstr );

			//TODO: Problem with pointer convertion
			// from 'const Useless::SignalEvent *__w64 '
			// to 'Useless::SignalEvent *const '
			assert(0);
            _Tie( signal, subSignal, PackFunctor
                    ( BinaryCall
                      ( event.get(),
                        &SignalEvent::Raise ),
                      &_SourceSignal( signal ),
                      argstr )
                );
        }
    }
    
    std::string _GetBaseId( XMLCreatorEnv *env )
    {
        std::string id, base;

        const char *pc_base;
        if ( env->Query("base", pc_base))
        {
            base = pc_base;
            id = base + ".";
        }
        return id;
    }
    
}//unnamed

Resource* CreateConnectionBase( XMLIterator i, XMLCreatorEnv *env)
{
    std::string baseid = _GetBaseId( env );
    XMLCreatorEnv ch_env = *env;
    std::string base = baseid + get_attribute( (*i)._attributes, "id");
    ch_env.Set("base", base.c_str());
    
    ChildsFromXML( i, &ch_env);
    return NULL;
}

Resource *
XMLTieAction ( XMLIterator i, XMLCreatorEnv *env )
{
    std::string baseid( _GetBaseId( env ));

    std::string id = baseid + get_attribute( (*i)._attributes, "id");

    ActiveWidget *activator = QueryActiveWidget( id );
    Signal *signal = 0;

    if ( (*i)._name == "onclick" )
    {
        int button_no = 0;
        try_attribute_i( (*i)._attributes, "button", &button_no );
            
        _XMLTieImpl( activator->OnClick[ button_no ]
                , 0
                , baseid
                , (*i)._attributes
                , env);
    }
    
    if ( (*i)._name == "onkey" )
    {
        static _SigMUX sigmux;
        int keyCode = Keys::GetCode( get_attribute( (*i)._attributes, "key_code" ));
        
        _XMLTieImpl( _GetSubSignal( sigmux, activator, activator->OnKeyPress )
                , keyCode
                , baseid
                , (*i)._attributes
                , env);        
        activator->EnableKey( keyCode, true );
    }

    if ( (*i)._name == "handler" )
    {
        std::string signalName = get_attribute( (*i)._attributes, "signal" );
        Signal &s = SignalMap_ActiveWidget::Instance().GetSignalByName( signalName, activator );
        
        _XMLTieImpl( s
                , 0
                , baseid
                , (*i)._attributes
                , env);
    }

    return NULL;
}

Resource *
CreateMultiTab ( XMLIterator i, XMLCreatorEnv *env )
{
    std::string baseid = _GetBaseId( env );
    std::string tabsName = baseid + get_attribute( (*i)._attributes, "tabs" );
    std::string groupsName = baseid + get_attribute( (*i)._attributes, "groups" );
    std::string def;
    try_attribute( (*i)._attributes, "default", &def );
    
    __Resources::NameIndex nameId( tabsName );
    __Resources::ResourceMap::iterator it = Resources::Instance().Find( "widgets", nameId );
    assert( it != Resources::Instance().End() );

    HubWidget *tabGroup = PointerResourceConvertion< HubWidget >::Apply( (*it).second );
    if ( !tabGroup )
    {
        throw Error("MultiTab expects 'tabs' pointing to HubWidget\n"
                    "tabs: %s\n"
                    "type: %s"
                    , tabsName.c_str()
                    , (*it).second->GetTypeName()
                    );
    }
    
    MultiTabRelation *relation = new MultiTabRelation( *tabGroup );
    tabGroup->Posses( relation );

    std::string childPart;
    while ( ++it, Resources::Instance().IsChild( it, nameId, childPart ))
    {
        EnumWidget *tab = PointerResourceConvertion< EnumWidget >::Apply( (*it).second );
        if ( 0 != tab )
        {
            std::string hubName = groupsName + "." + childPart;
            Resource *res = Resources::Instance().QueryResource( "widgets", hubName );
            if ( res )
            {
                HubWidget *hub = PointerResourceConvertion< HubWidget >::Apply( res );
                if ( hub )
                {
                    bool isDefault = ( childPart == def );
                    relation->Tie( tab, hub, isDefault );
                }
                else
                {
                    C_message( __Resources, 100
                            , ( "Multitab warning, target is not HubWidget:"
                                , hubName.c_str() ));
                }
            }
            else
            {
                C_message( __Resources, 100
                        , ( "Multitab warning, target not found:"
                            , hubName.c_str() ));
            }
        }
        else
        {
            C_message( __Resources, 100
                    , ( "Multitab warning, 'tabs' child is not EnumWidget:"
                        , (tabsName +"."+ childPart).c_str() ));
        }
    }
    
    return NULL;
};

Resource *
CreateScrollRelation ( XMLIterator i, XMLCreatorEnv *env)
{
    std::string baseid = _GetBaseId( env );
    std::string scrolledName = baseid + get_attribute( (*i)._attributes, "scrolled" );
    std::string scrollbarName = baseid + get_attribute( (*i)._attributes, "scrollbar" );
    int use_mouse_wheel = false;
    try_attribute_i( (*i)._attributes, "use_mouse_wheel", &use_mouse_wheel );
    
    ScrolledWidget *scrolled = QueryScrolledWidget( scrolledName );
    IWScrollbar *scrollbar = 0;
    
    scrollbar = QueryScrollbar( scrollbarName );
    
    scrolled->Posses( new BaseScrollRelation< ActivateWidgetAction >
            ( *scrolled, *scrollbar, use_mouse_wheel ));
    
    return NULL;
}

Resource *
CreateShowColumns ( XMLIterator i, XMLCreatorEnv *env)
{
    std::string baseid = _GetBaseId( env );
    std::string menuName = baseid + get_attribute( (*i)._attributes, "menu" );
    std::string tableName= baseid + get_attribute( (*i)._attributes, "tableview" );

    AdvTableSystemWidget *tableview =
        WidgetResource< AdvTableSystemWidget >::Query( tableName );

    __Resources::NameIndex nameId( menuName );
    __Resources::ResourceMap::iterator it = Resources::Instance().Find( "widgets", nameId );
    assert( it != Resources::Instance().End() );

    HubWidget *menu = PointerResourceConvertion< HubWidget >::Apply( (*it).second );
    if ( !menu )
    {
        throw Error("ShowColumns expects 'menu' pointing to HubWidget\n"
                    "menu: %s\n"
                    "type: %s"
                    , menuName.c_str()
                    , (*it).second->GetTypeName()
                    );
    }
   
    static _SigMUX sigmux;
    int column=0;
    std::string childPart;
    while ( ++it, Resources::Instance().IsChild( it, nameId, childPart ))
    {
        EnumWidget *mark = PointerResourceConvertion< EnumWidget >::Apply( (*it).second );
        if ( 0 != mark )
        {
            _SubSignal &subsignal = _GetSubSignal( sigmux, mark, mark->OnChange );

            _Tie( subsignal, 1, PackFunctor
                    ( UnaryCall
                      ( tableview,
                        &AdvTableSystemWidget::ShowColumn ),
                      column )
                    );
            
            _Tie( subsignal, 0, PackFunctor
                    ( UnaryCall
                      ( tableview,
                        &AdvTableSystemWidget::HideColumn ),
                      column )
                    );

            mark->SetSelected( 1 );

            ++column;
        }
        else
        {
            C_message( __Resources, 100
                    , ( "ShowColumns, 'menu' child is not EnumWidget:"
                        , (menuName +"."+ childPart).c_str() ));
        }
    }

    return 0;
}


};//namespace Useless
