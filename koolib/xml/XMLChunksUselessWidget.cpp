#include "XMLChunksUseless.h"

#include <koolib/xml/XMLKompiler.h> //used for faster symbol access
#include <koolib/xml/XMLChunksUseless.h>
//#include <gl/GL.h> //@TODO: needed for PainterProxy::SubCanvasPaint
//#include "Useless/Graphic/Planes/MemSurface.h" //needed by QueryPixelColor
//#include "Useless/Graphic/Planes/ImageToFile.h" //needed by WriteImage

namespace XMLProgram {

    using namespace Useless;
    using namespace XMLFactory;
    using CXML::LazyGetChunkInScope;
    using CXML::LazyGetChunkMember;
    

    IChunk *make_value_chunk( const Useless::StupidVector< int > &vect )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        XMLProgram::add_methods( const_cast< Useless::StupidVector< int > *>( &vect ), block )
            .def("Empty", unconst_method_cast( &Useless::FontSkin::Letters::Empty ))
            .def("Size", unconst_method_cast( &Useless::FontSkin::Letters::Size ))
            .def_remap_1< int, int >("At", &Useless::StupidVector< int >::operator[], "index")
            ;
        return block;
    }

    IChunk *make_value_chunk( const Useless::NormalPixel &pixel )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        block->AddChunk(L"red", make_value_chunk( (int)pixel.c.r ));
        block->AddChunk(L"green", make_value_chunk( (int)pixel.c.g ));
        block->AddChunk(L"blue", make_value_chunk( (int)pixel.c.b ));
        block->AddChunk(L"alpha", make_value_chunk( (int)pixel.c.a ));
        return block;
    }
    
    IChunk *make_value_chunk( const Useless::FontPainter::Carrige &car )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        try {
            block->AddChunk(L"letterInText", make_value_chunk( car._letterInText ));
            block->AddChunk(L"letterInWord", make_value_chunk( car._letterInWord ));
            block->AddChunk(L"wordInText", make_value_chunk( car._wordInText ));
            block->AddChunk(L"wordInRow", make_value_chunk( car._wordInRow ));
            block->AddChunk(L"row", make_value_chunk( car._row ));
            return block;
        }
        catch(...) { delete block; throw; }
    }
    
    IChunk *make_value_chunk( const Useless::TextInfo::Word &word )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        try {
            block->AddChunk(L"begin", make_value_chunk( word.begin ));
            block->AddChunk(L"end", make_value_chunk( word.end ));
            block->AddChunk(L"width", make_value_chunk( word.width ));
            block->AddChunk(L"height", make_value_chunk( word.height ));
            block->AddChunk(L"separator_width", make_value_chunk( word.separator_width ));
            return block;
        }
        catch(...) { delete block; throw; }
    }
    
    IChunk *make_value_chunk( const Useless::TextInfo::Row &row )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        try {
            block->AddChunk(L"width", make_value_chunk( row.width ));
            block->AddChunk(L"height", make_value_chunk( row.height ));
            block->AddChunk(L"words", make_value_chunk( row.words ));
            return block;
        }
        catch(...) { delete block; throw; }
    }
    
    IChunk *make_value_chunk( const Useless::FontPainter::Status &s )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        XMLProgram::add_methods( const_cast< Useless::FontPainter::Status *>( &s ), block )
            .def("GetWordCount", unconst_method_cast( &FontPainter::Status::GetWordCount ))
            .def("GetLinesCount", unconst_method_cast( &FontPainter::Status::GetLinesCount ))
            .def("GetWidth", unconst_method_cast( &FontPainter::Status::GetWidth ))
            .def("GetHeight", unconst_method_cast( &FontPainter::Status::GetHeight ))
            ;
        return block;
    }
    
    IChunk *make_value_chunk( const Useless::FontPainter::Lines &s )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        XMLProgram::add_methods( const_cast< Useless::FontPainter::Lines *>( &s ), block )
            .def_remap_1< const_refrence_wrapper< Useless::TextInfo::Row >, int >
            ("GetRow", unconst_method_cast( &FontPainter::Lines::GetRow ), "row")
            .def("GetRowHeight", unconst_method_cast( &FontPainter::Lines::GetRowHeight ), "row")
            .def("GetNumRows", unconst_method_cast( &FontPainter::Lines::GetNumRows ))
            .def("GetWidth", unconst_method_cast( &FontPainter::Lines::GetWidth ))
            .def("GetHeight", unconst_method_cast( &FontPainter::Lines::GetHeight ))
            ;
        return block;
    }
    
    IChunk *make_value_chunk( const Useless::FontPainter::Words &s )
    {
        XMLCodeBlock *block = new XMLCodeBlock();
        XMLProgram::add_methods( const_cast< Useless::FontPainter::Words *>( &s ), block )
            .def_remap_1< const_refrence_wrapper< Useless::TextInfo::Word >, int >
            ("GetWord", unconst_method_cast( &FontPainter::Words::GetWord ), "word")
            .def("GetNumWords", unconst_method_cast( &FontPainter::Words::GetNumWords ))
            .def_remap_0< TextUtf8 >("GetSeparators", unconst_method_cast( &FontPainter::Words::GetSeparators ))
            ;
        return block;
    }


    /*******************************

      FontProxy

    */
    FontProxy::FontProxy( const Font &font ): m_font( font )
    {
        add_methods( &m_font, this )
            .def_remap_3< void, const Painter &, TextUtf8, const Pos & >("Paint", &Font::Paint, "__painter__", "text", "pos")
            .def("GetHeight", unconst_method_cast( &Font::GetHeight ))
            .def_remap_1< int, int >("GetCharWidth", unconst_method_cast( (int (Font::*)( UniCode ) const)&Font::GetWidth ), "index" )
            .def_remap_1< int, TextUtf8 >("GetTextWidth", unconst_method_cast( (int (Font::*)( const Text & ) const)&Font::GetWidth ), "text" )
            .def_remap_1< Font::Letters, TextUtf8 >("GetStats", unconst_method_cast( &Font::GetStats ), "text" )
            ;
    }
    
    
    
    
    /*******************************

      FontPainterProxy

    */
    FontPainterProxy::FontPainterProxy( const Useless::FontPainter &fp ): m_fontPaint( fp )
    {
        add_methods( &m_fontPaint, this )
            .def_remap_4< void, const Painter &, const Pos &, int, int >("Paint", &FontPainter::Paint, "__painter__", "pos", "linespace", "alignment" )
            .def_remap_1< void, TextUtf8 >("SetText", &FontPainter::SetText, "text")
            .def("SetFont", &FontPainter::SetFont, "font")
            .def("SetWidth", &FontPainter::SetWidth, "width")
            .def_remap_3< FontPainter::Carrige, const Pos &, int, int >("QueryPosition", &FontPainter::QueryPosition, "pos", "linespace", "alignment")
            .def_remap_0< FontPainter::Status >("GetStatus", unconst_method_cast( &FontPainter::GetStatus ))
            .def_remap_0< FontPainter::Lines >("GetLines", unconst_method_cast( &FontPainter::GetLines ))
            .def_remap_0< FontPainter::Words >("GetWords", unconst_method_cast( &FontPainter::GetWords ))
            .def_remap_0< Font >("GetFont", unconst_method_cast( &FontPainter::GetFont ))
            ;
    }

 

    /*******************************

      SkinProxy

    */
    SkinProxy::SkinProxy( const Skin &skin ): m_skin( skin )
    {
        add_methods( m_skin.get(), this )
            .def("Paint", &Skin::Paint, "__painter__", "pos")
            .def("GetWidth", unconst_method_cast( &Skin::GetWidth ))
            .def("GetHeight", unconst_method_cast( &Skin::GetHeight ))
            .def("SetSize", &Skin::SetSize, "size")
            .def("GetDirection", unconst_method_cast( &Skin::GetDirection ))
            .def("SafeResize", &Skin::SafeResize, "size" )
            .def("GetDirMask", unconst_method_cast( &Skin::GetDirMask ))
            .def("SetDirMask", &Skin::SetDirMask, "mask" )
            .def("GetSize", unconst_method_cast( &Skin::GetSize ))
            .def("GetLength", unconst_method_cast( &Skin::GetLength ))
            .def("SetLength", &Skin::SetLength, "length" )
            .def("SetClipRect", &Skin::SetClipRect, "rect")
            .def("GetClipRect", unconst_method_cast( &Skin::GetClipRect ))
            .def("IsSolid", unconst_method_cast( &Skin::IsSolid ))
            .def("IsColorKeyed", unconst_method_cast( &Skin::IsColorKeyed ))
            .def("GetColorKey", unconst_method_cast( &Skin::GetColorKey ))
            .def_remap_0< double >("GetFrameRate", unconst_method_cast( &Skin::GetFrameRate ))
            .def("GetDimension", unconst_method_cast( &Skin::GetDimension ), "index" )
            ;
    }
    



    /*******************************

      WidgetProxy

    */
    WidgetProxy::WidgetProxy( Widget *widget ): _widget( widget )
    {
        XMLProgram::add_methods( _widget, this )
            .def("SetPosition",     &Widget::SetPosition, "pos")
            .def("GetPosition",     unconst_method_cast( &Widget::GetPosition ))
            .def("SetBoundingRect", &Widget::SetBoundingRect, "bound")
            .def("GetBoundingRect", unconst_method_cast( &Widget::GetBoundingRect ))
            .def("Resize",      &Widget::Resize, "w", "h")
            .def("GetWidth",    unconst_method_cast( &Widget::GetWidth ))
            .def("GetHeight",   unconst_method_cast( &Widget::GetHeight ))
            .def("SetOpacity",  &Widget::SetOpacity, "opacity" )
            .def("GetOpacity",  unconst_method_cast( &Widget::GetOpacity ))
            .def("SetVisible",  &Widget::SetVisible, "visible")
            .def("IsVisible",   unconst_method_cast( &Widget::IsVisible ))
            .def("SetOnTop",    &Widget::SetOnTop, "ontop")
            .def("IsOnTop",     unconst_method_cast( &Widget::IsOnTop ))
            .def("SetPhantom",  &Widget::SetPhantom, "phantom" )
            .def("IsPhantom",   unconst_method_cast( &Widget::IsPhantom ))
            .def("FocusIntercept",  &Widget::FocusIntercept )
            .def("FocusRelease",    &Widget::FocusRelease )
            .def("HasFocus",    unconst_method_cast( &Widget::HasFocus ))
            .def("InputIsEager", unconst_method_cast( &Widget::InputIsEager ))
            .def("InputIncEager", (void (Widget::*)())&Widget::InputIncEager )
            .def("InputDecEager", (void (Widget::*)())&Widget::InputDecEager )
            .def("InputIntercept", (void (Widget::*)())&Widget::InputIntercept )
            .def("InputRelease", (void (Widget::*)())&Widget::InputRelease )
            .def("HasInput",    unconst_method_cast( &Widget::HasInput ))
            .def("DoInputReset", (void (Widget::*)())( &Widget::DoInputReset ))
            .def("SetDirty",    (void (Widget::*)())&Widget::SetDirty )
            .def("BeAlwaysDirty", &Widget::BeAlwaysDirty, "always")
            .def("IsAlwaysDirty", unconst_method_cast( &Widget::IsAlwaysDirty ))
            .def("GetAbsolutePos", unconst_method_cast( &Widget::GetAbsolutePos ), "relativePos")
            .def("GetRelativePos", unconst_method_cast( &Widget::GetRelativePos ), "absolutePos")
            .def("GetAbsoluteRect", unconst_method_cast( &Widget::GetAbsoluteRect ), "relativeRect")
            .def("GetRelativeRect", unconst_method_cast( &Widget::GetRelativeRect ), "absoluteRect")
            // Signals
            .add("OnVisible",   make_signal_1_chunk( widget->OnVisible ))
            .add("OnIdle",      make_signal_2_chunk( widget->OnIdle ))
            .add("OnResize",    make_signal_2_chunk( widget->OnResize ))
            .add("OnReposition",make_signal_1_chunk( widget->OnReposition ))
            .add("OnActorMove", make_signal_1_chunk( widget->OnActorMove ))
            .add("OnFocus",     make_signal_0_chunk( widget->OnFocus ))
            .add("OffFocus",    make_signal_0_chunk( widget->OffFocus ))
            ;

        if ( CompoundWidget *c = dynamic_cast< CompoundWidget *>( widget ))
        {
            XMLProgram::add_methods_block("Compound", c, this )
                .def_remap_1< void, const WidgetPainter & >("DoRepaint", &CompoundWidget::DoRepaint, "painter")
                ;
        
            XMLProgram::add_methods( _widget, this )
                .add("OnPaint", make_signal_1_chunk( c->OnPaint ))
                ;
        }
        
        if ( ActiveAreaWidget *c = dynamic_cast< ActiveAreaWidget *>( widget ))
        {
            XMLProgram::add_methods( _widget, this )
                .add("OnPaint", make_signal_1_chunk( c->OnPaint ))
                ;
        }

        if ( CreditsWidget *c = dynamic_cast< CreditsWidget *>( widget ))
        {
            XMLProgram::add_methods_block("Credits", c, this )
                .def("Finished", unconst_method_cast( &CreditsWidget::Finished ))
                .def("Restart", &CreditsWidget::Restart )
                ;
        }
        if ( IWActive *ic = dynamic_cast< IWActive *>( widget ))
        {
            ActiveWidget *c = &ic->Control();
            XMLProgram::add_methods_block( "Active", c, this )
                .def("SetActive", &ActiveWidget::SetActive, "active" )
                .def("IsActive", &ActiveWidget::IsActive )
                .def("Activate", &ActiveWidget::Activate )
                .def("Deactivate", &ActiveWidget::Deactivate )
                .def("SetDragRadius", &ActiveWidget::SetDragRadius, "radius" )
                .def("SetRepeatRate", &ActiveWidget::SetRepeatRate, "period", "delay" )
                .def("SetFocusAdaptive", &ActiveWidget::SetFocusAdaptive, "adaptive")
                .def("IsFocusAdaptive", unconst_method_cast( &ActiveWidget::IsFocusAdaptive ))
                .def("EnableKey", &ActiveWidget::EnableKey, "key", "enable")
                .def("EnableOnPathKey", &ActiveWidget::EnableOnPathKey, "key", "enable")
                .def("EnableCharInput", &ActiveWidget::EnableCharInput, "enable")
                .def("SetActivationKey", &ActiveWidget::SetActivationKey, "key" )
                .def("GetActivationKey", unconst_method_cast( &ActiveWidget::GetActivationKey ))
                .def("IsKeyPressed", unconst_method_cast( &ActiveWidget::IsKeyPressed ), "key")

                .add("OnPress",     make_signal_1_chunk( c->OnPress[0] ))
                .add("OnRelease",   make_signal_1_chunk( c->OnRelease[0] ))
                .add("OnClick",     make_signal_1_chunk( c->OnClick[0] ))
                .add("OnDoubleClick", make_signal_1_chunk( c->OnDoubleClick[0] ))
                .add("OnMoveIn",  make_signal_1_chunk( c->OnMoveIn[0] ))
                .add("OnMoveOut", make_signal_1_chunk( c->OnMoveOut[0] ))
                .add("OnCatch",   make_signal_1_chunk( c->OnCatch[0] ))
                .add("OnDrag",    make_signal_3_chunk( c->OnDrag[0] ))
                .add("OnDrop",    make_signal_2_chunk( c->OnDrop[0] ))
                .add("WhileHold", make_signal_1_chunk( c->WhileHold[0] ))
                .add("WhileDrag", make_signal_1_chunk( c->WhileDrag[0] ))
                .add("OffPress",  make_signal_1_chunk( c->OffPress[0] ))

                .add("OnPress1",     make_signal_1_chunk( c->OnPress[1] ))
                .add("OnRelease1",   make_signal_1_chunk( c->OnRelease[1] ))
                .add("OnClick1",     make_signal_1_chunk( c->OnClick[1] ))
                .add("OnDoubleClick1", make_signal_1_chunk( c->OnDoubleClick[1] ))
                .add("OnMoveIn1",     make_signal_1_chunk( c->OnMoveIn[1] ))
                .add("OnMoveOut1",    make_signal_1_chunk( c->OnMoveOut[1] ))
                .add("OnCatch1",   make_signal_1_chunk( c->OnCatch[1] ))
                .add("OnDrag1",    make_signal_3_chunk( c->OnDrag[1] ))
                .add("OnDrop1",    make_signal_2_chunk( c->OnDrop[1] ))
                .add("WhileHold1", make_signal_1_chunk( c->WhileHold[1] ))
                .add("WhileDrag1", make_signal_1_chunk( c->WhileDrag[1] ))
                .add("OffPress1",  make_signal_1_chunk( c->OffPress[1] ))

                .add("OnPress2",     make_signal_1_chunk( c->OnPress[2] ))
                .add("OnRelease2",   make_signal_1_chunk( c->OnRelease[2] ))
                .add("OnClick2",     make_signal_1_chunk( c->OnClick[2] ))
                .add("OnDoubleClick2", make_signal_1_chunk( c->OnDoubleClick[2] ))
                .add("OnMoveIn2",     make_signal_1_chunk( c->OnMoveIn[2] ))
                .add("OnMoveOut2",    make_signal_1_chunk( c->OnMoveOut[2] ))
                .add("OnCatch2",   make_signal_1_chunk( c->OnCatch[2] ))
                .add("OnDrag2",    make_signal_3_chunk( c->OnDrag[2] ))
                .add("OnDrop2",    make_signal_2_chunk( c->OnDrop[2] ))
                .add("WhileHold2", make_signal_1_chunk( c->WhileHold[2] ))
                .add("WhileDrag2", make_signal_1_chunk( c->WhileDrag[2] ))
                .add("OffPress2",  make_signal_1_chunk( c->OffPress[2] ))

                .add("OnMoveOver",  make_signal_1_chunk( c->OnMoveOver ))
                .add("OnMoveOutside",  make_signal_1_chunk( c->OnMoveOutside ))
                .add("WhileMoving",  make_signal_3_chunk( c->WhileMoving ))
                .add("OnActivate",  make_signal_1_chunk( c->OnActivate ))
                .add("OnWheel", make_signal_2_chunk( c->OnWheel ))
                .add("OffWheel", make_signal_2_chunk( c->OffWheel ))
                .add("OnChar", make_signal_1_chunk( c->OnChar ))
                .add("OnKeyPress", make_signal_1_chunk( c->OnKeyPress ))
                .add("OnKeyRelease", make_signal_1_chunk( c->OnKeyRelease ))
                .add("WhileKey", make_signal_1_chunk( c->WhileKey ))
                .add("WhileOffKey", make_signal_1_chunk( c->WhileOffKey ))
                .add("OffChar", make_signal_1_chunk( c->OffChar ))
                .add("OffKeyPress", make_signal_1_chunk( c->OffKeyPress ))
                ;
        }
        if ( IWEnum *ic = dynamic_cast< IWEnum *>( widget ))
        {
            EnumWidget *c = &ic->Enumerator();
            XMLProgram::add_methods_block("Enum", c, this )
                .def("GetSelected", unconst_method_cast( &EnumWidget::GetSelected ))
                .def("SetSelected", &EnumWidget::SetSelected, "no")
                .add("OnChange", make_signal_1_chunk( c->OnChange ))
                .add("OnConfirm", make_signal_1_chunk( c->OnConfirm ))
                ;
        }
        if ( IWList *ic = dynamic_cast< IWList *>( widget ))
        {
            ListWidget *c = &ic->Items();
            XMLProgram::add_methods_block( "List", c, this )
                .def
                ("GetNumEntries", unconst_method_cast( &ListWidget::GetNumEntries ))
                .def
                ("ClearEntries", &ListWidget::ClearEntries )
                .def_remap_1< int, TextUtf8 > 
                ("GetItemNo", unconst_method_cast( &ListWidget::GetItemNo ), "text")
                .def_remap_1< void, TextUtf8 >
                ("Insert", &ListWidget::Insert, "text")
                .def_remap_1< TextUtf8, int > 
                ("GetText", unconst_method_cast( &ListWidget::GetText ), "no")
                .def_remap_2< void, TextUtf8, TextUtf8 >
                ("SetText", (void (ListWidget::*)(const Text &, const Text &))&ListWidget::SetText, "find", "set")
                .def("SetUserFlagForItem", &ListLogic::SetUserFlagForItem, "no", "flagNo", "value")
                .def("GetUserFlagForItem", unconst_method_cast( &ListLogic::GetUserFlagForItem ), "no", "flagNo" )
                ;
        }
        if ( TextField *c = dynamic_cast< TextField *>( widget ))
        {
            XMLProgram::add_methods_block( "TextField", c, this )                
                .def_remap_0< TextUtf8 >
                ("GetText", unconst_method_cast( &TextField::GetText ))                
                .def_remap_1< void, TextUtf8 >
                ("SetText", &TextField::SetText, "text" )
                .add("OnCarMove", make_signal_1_chunk( c->OnCarMove ))
                .add("OnTextChange", make_signal_1_chunk( c->OnTextChange ))
                .add("OnConfirm", make_signal_1_chunk( c->OnConfirm ))
                ;
        }
        if ( BaseTextWidget *c = dynamic_cast< BaseTextWidget *>( widget ))
        {
            XMLProgram::add_methods_block( "Text", c, this )
                .def_remap_1< void, TextUtf8 >
				("SetText", &BaseTextWidget::SetText, "text")
                .def_remap_0< TextUtf8 >
				("GetText", unconst_method_cast( &BaseTextWidget::GetText ))
                .def("Clear", &BaseTextWidget::Clear )
                ;
        }
        if ( AdvTextBase *c = dynamic_cast< AdvTextBase *>( widget ))
        {
            XMLProgram::add_methods_block( "Text", c, this )
                .def_remap_1< void, TextUtf8 >
				("SetText", &AdvTextBase::SetText, "text")
                .def_remap_0< TextUtf8 >
				("GetText", unconst_method_cast( &AdvTextBase::GetText ))
                .def("Clear", &AdvTextBase::Clear )
                .def_remap_0< Font >("GetFont", unconst_method_cast( &AdvTextBase::GetFont ))
                .def_remap_1< void, const Font & >("SetFont", &AdvTextBase::SetFont, "font")
                ;
            XMLProgram::add_methods_block( "FontPainter", c, this )
                .def("QueryPosition", &AdvTextBase::QueryPosition, "pos")
                .add("Status", make_value_chunk( c->GetStatus() ))
                .add("Lines", make_value_chunk( c->GetLines() ))
                .add("Words", make_value_chunk( c->GetWords() ))
                ;
        }
        if ( IWCaption *c = dynamic_cast< IWCaption *>( widget ))
        {
            CaptionWidget &p = c->Caption();
            XMLProgram::add_methods_block( "Caption", &p, this )
                .def_remap_1< void, TextUtf8 >
                                ( "SetCaption", &CaptionWidget::SetCaption, "text")
                .def_remap_0< TextUtf8 >
                                ( "GetCaption", unconst_method_cast( &CaptionWidget::GetCaption ))
                ;
        }
        if ( ProgressWidget *c = dynamic_cast< ProgressWidget *>( widget ))
        {
            XMLProgram::add_methods_block( "Progress", c, this )
                .def("Advance", &ProgressWidget::Advance, "delta" )
                .def("SetAdvance", &ProgressWidget::SetAdvance, "advance" )
                .def("SetRange", &ProgressWidget::SetRange, "range" )
                .def("GetSize", unconst_method_cast( &ProgressWidget::GetSize ))
                ;
        }
        if ( ScoreRoller *c = dynamic_cast< ScoreRoller *>( widget ))
        {
            XMLProgram::add_methods_block( "ScoreRoller", c, this )
                .def
                ("SetScore", &ScoreRoller::SetScore, "score")
                .def
                ("GetScore", &ScoreRoller::GetScore )
                .def_remap_1< void, double >
                ("SetRollSpeed", &ScoreRoller::SetRollSpeed, "speed")
                .def_remap_0< double >
                ("GetRollSpeed", unconst_method_cast( &ScoreRoller::GetRollSpeed ))
                ;
        }
        if ( LoomWidget *c = dynamic_cast< LoomWidget *>( widget ))
        {
            XMLProgram::add_methods_block( "Loom", c, this )                
                .def("LoomUp", &LoomWidget::LoomUp )
                .def("Vanish", &LoomWidget::Vanish )
                .def("IsUp", unconst_method_cast( &LoomWidget::IsUp ))
                .def("LoomVanish", &LoomWidget::LoomVanish )
                .def("SetMinVisible", &LoomWidget::SetMinVisible, "numPixels" )
                .add("OnLoomStop", make_signal_0_chunk( c->OnLoomStop ))
                .add("OnVanishStop", make_signal_0_chunk( c->OnVanishStop ))
                ;
        }
        if ( ImageWidget *c = dynamic_cast< ImageWidget *>( widget ))
        {
            this->AddChunk(L"Image", new ImageWidgetProxy( c ));
        }
       
    }

    WidgetProxy::~WidgetProxy()
    {
    }

    // Helper - ImageWidget wrapper
    ImageWidgetProxy::ImageWidgetProxy( ImageWidget *w ): _imgWidget( w )
    {
        XMLProgram::add_methods( this )
            .def("SetImageByName", &ImageWidgetProxy::SetImageByName, "name")
            .def("SetSkinByName", &ImageWidgetProxy::SetSkinByName, "name")
            ;
    }

    ImageWidgetProxy::~ImageWidgetProxy()
    {
    }

    void ImageWidgetProxy::SetImageByName( std::string name )
    {
        _imgWidget->SetSkin( *MakeImage( name ));
    }

    void ImageWidgetProxy::SetSkinByName( std::string name )
    {
        _imgWidget->SetSkin( QuerySkin( name ));
    }


};//XMLProgram

