/*
 ** $Id: AdvTableSystemWidget.cpp,v 1.9 2004/01/09 20:05:05 koolas Exp $
 **
 ** NAME 
 **    AdvTableSystemWidget
 **
 ** PURPOSE 
 **    AdvTableSystemWidget
 **
 ** AUTHOR(S)
 **      Sebastian Szymanski (szyman@iavision.com )
 */

#include "UselessPch.h"

#include "Useless/GUI/Widgets/AdvTableSystemWidget.h"
#include "Useless/GUI/Skins/NullSkin.h"
#include "Useless/GUI/Widgets/SizerWidget.h"
#include "Useless/GUI/Widgets/TableWidget.h"
#include "Useless/GUI/Widgets/DataTableWidget.h"
#include "Useless/GUI/Widgets/EnumWidget.h"
#include "Useless/GUI/Misc/DataTypes.h"
#include "Useless/GUI/Relations/FillRelation.h"

namespace {
    using namespace Useless;
    Widget * _MakeScrollbar( const ScrollbarStyle *style )
    {
        if ( const AdvScrollbarStyle *advStyle =
                dynamic_cast< const AdvScrollbarStyle *>( style ))
        {
            return new
                AdvScrollbarWidget( *advStyle );
        }
        else
        {
            return new
                ScrollbarWidget( *style );
        }
    }

    typedef TemplateWidget< ContentWidget, ButtonLogic > HeaderButton;

};//unnamed

namespace Useless
{
#pragma warning(disable:4355)

    AdvTableSystemWidget::AdvTableSystemWidget

        ( const SkinMan             * content_button_skins,
          const FontSet             * content_button_fonts,
          const ScrollbarStyle      * VScrollStyle,
          const ScrollbarStyle      * HScrollStyle,
          const FrameStyle          * frameStyle )

        : TableSystemWidget( 100,100 )
        , _content_button_skins( this, *content_button_skins )
        , _content_button_fonts( *content_button_fonts )

        {
            if( VScrollStyle != NULL )
            {
                TableSystemWidget::Insert( _MakeScrollbar( VScrollStyle ));
            }
            
            if( HScrollStyle != NULL )
            {
                TableSystemWidget::Insert( _MakeScrollbar( HScrollStyle ));
            }

            if( frameStyle )
            {
                TableSystemWidget::Insert( new Useless::FrameWidget( *frameStyle ));
            }
        }


    AdvTableSystemWidget::~AdvTableSystemWidget()
    {
    }

    void AdvTableSystemWidget::Repaint( const WidgetPainter &painter )
    {
        // We want sizer to be painted in state synchronized with table.
        // Since Sync() is called OnTableUpdate, we must call UpdateTable first.
        // Normaly UpdateTable() is called in Table().Repaint(),
        // so since Sizer() is painted before Table(),
        // Sync() was called to late (about one frame difference).
        Table().UpdateTable();
        TableSystemWidget::Repaint( painter );
    }

    void AdvTableSystemWidget::Insert( Widget *widget )
    {
        TableSystemWidget::Insert( widget );

        if (  IWTable *pi_tb = dynamic_cast< IWTable *>( widget ))
        {
            DataTableWidget &bodyTable = dynamic_cast< DataTableWidget &>( Table() );
            Tie2Signal( bodyTable.OnDataConnect, this, &AdvTableSystemWidget::DataConnected );
            
            SizerWidget *headSizer = new SizerWidget( NullSkin() );
            headSizer->SetupCells( bodyTable.GetColumns() );
            
            headSizer->Resize
                ( bodyTable.GetWidth(),
                  _content_button_skins.GetHeight() );
            
            TableSystemWidget::Insert( headSizer );
            
            StupidVector< Useless::Belt > columnBelts = Table().GetColumns();
            for( int col = 0; col < columnBelts.Size(); ++col )
            {
                _columns_properties.push_back
                    ( ColumnProperty
                      ( columnBelts[col]._size,
                        columnBelts[col]._fixed,
                        true )
                      );
            }
            DataConnected();

            bodyTable.OnTableUpdate.TieUnary( headSizer, &SizerWidget::Sync, &bodyTable );
        }
    }

    void AdvTableSystemWidget::ShowColumn( int column )
    {
        if( !_columns_properties[ column ]._visible )
        {
            _columns_properties[ column ]._visible = true;
            _columnFilter->_columnHidden.ClearBit( column );
            UpdateTableHeaders();
        }
    }


    void AdvTableSystemWidget::HideColumn( int column )
    {
        if( _columns_properties[ column ]._visible )
        {
            _columns_properties[ column ]._visible = false;
            _columnFilter->_columnHidden.SetBit( column );
            UpdateTableHeaders();
        }
    }

    void AdvTableSystemWidget::Sort( int column )
    {
        const int FLAG_SORT= DisplayState::USER;
        const int FLAG_ASC = DisplayState::USER << 1;
        bool ascending = false;

        int visibleColumns = 0;
        for( int realColumn=0, N=_columns_properties.size();
                realColumn < N; ++realColumn )
        {
            if ( _columns_properties[ realColumn ]._visible )
            {
                int col = visibleColumns++;

                // get n-th column header button (\sa UpdateTableHeaders)
                Widget *widget = Sizer().Settler( col );
                LogicWidget *p_header = dynamic_cast< LogicWidget *>( widget );            
                if ( !p_header) { break; }

                // get display flags for header button
                DisplayState dp = p_header->GetDP();

                if ( col == column )
                {
                    // update display flags for sort and sort-ascending
                    if ( !dp.Get( FLAG_SORT ) || !dp.Get( FLAG_ASC ))
                    {
                        dp.Set( FLAG_SORT );
                        dp.Set( FLAG_ASC );
                    }
                    else
                    {
                        dp.Clr( FLAG_ASC );
                    }

                    // get data connector
                    DataTableWidget *p_table = dynamic_cast< DataTableWidget* >( &Table() );
                    Share< DataConnector > datcon( p_table->GetDataConnector() );

                    // get currently selected row number and its data
                    int selRow = Enumerator().GetSelected();
                    bool visible = IsVisible();
                    DataStream selData;                
                    if ( -1 != selRow )
                    {
                        selData = datcon->GetData( selRow );
                    }

                    // sort data by column
                    datcon->SortData( realColumn, dp.Get( FLAG_ASC ));

                    // update selector 
                    if ( -1 != selRow )
                    {
                        // find row with correct data
                        int newRow = datcon->FindNext( selData );
                        Enumerator().SetSelected( newRow );
                        // for TableMenu SetSelected hides our widget
                        if ( visible ) { Show(); }
                    }
                }
                else
                {
                    // reset flags for button in other columns
                    dp.Clr( FLAG_SORT );
                    dp.Clr( FLAG_ASC );
                }
                // set updated flags
                p_header->SetState( dp );

            }
        }
    }

    void AdvTableSystemWidget::DataConnected()
    {
        _columnFilter = new DF_ShowColumns();
        DataTableWidget &   bodyTable = dynamic_cast< DataTableWidget &>( Table() );
        Share< DataConnector > datcon = bodyTable.GetDataConnector();
        datcon->SetFilter( _columnFilter );
        UpdateTableHeaders();
    }


    void AdvTableSystemWidget::UpdateTableHeaders()
    {
        SizerWidget &       headSizer = Sizer();
        DataTableWidget &   bodyTable = dynamic_cast< DataTableWidget &>( Table() );
        
        Share< DataConnector > datcon = bodyTable.GetDataConnector();
        DataStream headers = datcon->GetHeaders();
        
        int numColumns = headers.size();
        int numSizerCells = headSizer.GetNumCells();
        int numVisibleColumns;
        
        for ( int i = 0/*numColumns*/; i < numSizerCells; ++i )
        {
            headSizer.Remove( i );
        }
        
        Useless::StupidVector< Useless::Belt > columnBelts;
        
        ColumnProperties::iterator i_cp = _columns_properties.begin();
        
        for ( numVisibleColumns=0;
                i_cp != _columns_properties.end(); ++i_cp )
        {
            if ( (*i_cp)._visible )
            {
                columnBelts.push_back
                    ( Belt
                      ( (*i_cp)._width,
                        (*i_cp)._fixed )
                    );
                ++numVisibleColumns;
            }            
        }
        
        bodyTable.SetupColumns( columnBelts );
        bodyTable.UpdateTable();
        
        while( numVisibleColumns-- )
        {
            if ( numVisibleColumns < headers.size() )
            {
                HeaderButton *button = new HeaderButton
                    ( headers.Nolock()[ numVisibleColumns ]->CloneData(),
                     _content_button_skins.Get(),
                     _content_button_fonts.Get() );
                
                headSizer.Insert( numVisibleColumns, button );
                
                button->OnClick[0].TieUnary( this, &AdvTableSystemWidget::Sort, numVisibleColumns );

                button->Posses( new FillRelation( *button ));
            }
        }
            
        TableSystemWidget::AutoCover( true );            
        
        
        // get currently selected row number
        int selRow = Enumerator().GetSelected();
        if ( -1 != selRow )
        {
            bool visible = IsVisible();
            // update selector 
            Enumerator().SetSelected( -1 );
            Enumerator().SetSelected( selRow );
            // for TableMenu SetSelected hides our widget
            if ( visible ) { Show(); }
        }
    }


}; // end namespace Useless
