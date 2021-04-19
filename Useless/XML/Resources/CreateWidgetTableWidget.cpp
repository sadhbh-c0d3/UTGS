#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"
#include "Useless/XML/Resources/CreateStyle.h"
#include "Useless/XML/Resources/CreateSkins.h"

namespace Useless {

Resource* CreateWidgetTableWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;
    
    const char *p_name = "create table widget";
    BeginParentWidget( env, p_name, ch_env );

    CascadeStyles( i, env, attributes, branch_style, ch_env );

    std::string body_skin = get_attribute( attributes, "skin" );

    WidgetTableWidget *ptr;
    TableList *ptrD;
    Share< DataConnector > *dataCon = 0;
    
    std::string dataTable;
    if ( try_attribute( attributes, "data", &dataTable ))
    {
        Resource *res = Resources::Instance().QueryResource( "database", dataTable );
        if ( !res )
        {
            throw Error("CreateTableList error, data not exist\n"
                        "data: %s"
                        , dataTable.c_str()
                        );
        }
        dataCon = res->DynaCast< Share< DataConnector > >();
        if ( !dataCon )
        {
            throw Error("CreateTableList error, data wrong type\n"
                        "data: %s\n"
                        "type: %s"
                        , dataTable.c_str()
                        , res->GetTypeName()
                    );
        }
    }

    std::string hilite_skin;
    if ( try_attribute( attributes, "hilite_skin", &hilite_skin ) )
    {
        if ( (*i)._name == "tablelist" )
        {
            ptr = ptrD = new TableList(
                    QueryArraySkin( body_skin ),
                    QuerySkinMan( hilite_skin ));

            if ( dataCon )
                ptrD->SetDataConnector( *dataCon );
        }
        else
        {
            ptr = new TableListedWidgets(
                    QueryArraySkin( body_skin ),
                    QuerySkinMan( hilite_skin ));
        }
    }
    else
    {
        if ( (*i)._name == "tablelist" )
        {
            ptr = ptrD = new TableList(
                    QueryArraySkin( body_skin ));
            
            if ( dataCon )
                ptrD->SetDataConnector( *dataCon );
        }
        else{
            ptr = new WidgetTableWidget(
                    QueryArraySkin( body_skin ));
        }
    }

    ch_env.Set( "row_num", 0 );
    ch_env.Set( "column_num", 0 );
    ch_env.Set( "parent_table", ptr );

    ChildsFromXML( i, &ch_env );

    InsertChildWidget( ptr, attributes, env);    
	SetHint(ptr, attributes);

    return new AnyResource<WidgetTableWidget*>( ptr );
}

Resource* CreateTableCells( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env;
    XMLAttributes attributes;
    XMLStyle      branch_style;

    const char *p_name = "create table cell";
    BeginParentWidget( env, p_name, ch_env );

    CascadeStyles( i, env, attributes, branch_style, ch_env );  

    TableListedWidgets *table;

    if ( (*env).Query("parent_table", table) )
    {       
        int fixed = 0;
        try_attribute_i( attributes, "fixed", &fixed );
        
        int size = 100;
        try_attribute_i( attributes, "size", &size );

        int row_num;
        if ( try_attribute_i( attributes, "row", &row_num ))
        {
            (*env).Set( "row_num", row_num );
        }
        else
        {
            (*env).Query( "row_num", row_num );
        }
            
        int column_num;
        if ( try_attribute_i( attributes, "col", &column_num ))
        {
            (*env).Set( "column_num", column_num );
        }
        else
        {
            (*env).Query( "column_num", column_num );
        }
            
        if ( (*i)._name == "row" )
        {
            if ( row_num == table->GetNumRows() )
            {
                table->AddRow( size, fixed );
            }
            else
            {
                int size = table->GetRowHeight( row_num );
                try_attribute_i( attributes, "size", &size );
               
                table->ResizeRow( row_num, size, fixed );
            }
            
            env->Set( "row_num", row_num + 1 );
            column_num = 0;
        }
        else
        {
            if ( column_num == table->GetNumColumns() )
            {
                table->AddColumn( size, fixed );
            }
            else
            {
                int size = table->GetColumnWidth( column_num );
                try_attribute_i( attributes, "size", &size );
               
                table->ResizeColumn( column_num, size, fixed );
            }

            env->Set( "column_num", column_num + 1 );
        }
        ch_env.Set( "parent_table", table );
        ch_env.Set( "column_num", column_num );
        ch_env.Set( "row_num", row_num );
    }

    ChildsFromXML( i, &ch_env );

    return NULL;
}


}; // namespace Useless
