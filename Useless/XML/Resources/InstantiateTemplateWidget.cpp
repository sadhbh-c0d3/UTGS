#include "ResourcesPch.h"

#include "Useless/XML/Resources/CreateWidgets.h"

namespace Useless {

Resource* InstantiateTemplateWidget( XMLIterator i, XMLCreatorEnv *env )
{
    XMLCreatorEnv ch_env = *env;
    const char *p_name = "instantiate template widget";
    BeginParentWidget( env, p_name, ch_env );

    std::string class_name = get_attribute( (*i)._attributes, "class" );
    
    XMLParser::Nodes *p_nodes;
    Resources::Instance().Query( "classes", class_name, p_nodes );

    XMLParser::Nodes nodes = *p_nodes;
    //(*nodes.begin()).second._parent_node = 0; //I assert there is at least <template> node
    //XMLParser::Node root_node;
    //root_node._parent_node = -1;
    //nodes.insert( std::make_pair( 0, root_node) );
    p_nodes = &nodes; // Flip to new nodes

    int num_nodes = p_nodes->size(); //debug info
    XMLParser::Nodes::iterator it = p_nodes->begin(), iend=p_nodes->end();
    XMLParser::Node &node = (*it).second;

    typedef std::map< std::string, std::string > Assignments;
    Assignments assignments;

    int sub_tree_parent_id = 0;
    typedef std::map< std::string, int > SubTreeParentIdMap;
    SubTreeParentIdMap sub_tree_parent_id_map; 

    std::string var;
    // collect required attributes
    if ( try_attribute( node._attributes, "required", &var ) )
    {
        std::vector< std::string > required = to_tokens(var);
        std::vector< std::string >::iterator p = required.begin();
        for ( ; p!=required.end(); ++p )
        {
            assignments[*p] = get_attribute( (*i)._attributes, *p );
        }
    }
    // collect optional attributes
    if ( try_attribute( node._attributes, "optional", &var ) )
    {
        std::vector< std::string > optional = to_tokens(var);
        std::vector< std::string >::iterator p = optional.begin();
        for ( ; p!=optional.end(); ++p )
        {
            std::string x;
            if ( try_attribute( (*i)._attributes, *p, &x ) )
            {
                assignments[*p] = x;
            }
        }
    }
    // make substitutions for required & optional attributes
    for (; it!=iend; ++it ) // for each XML node
    {
        XMLParser::Node& node = (*it).second;
        Assignments::iterator ai = assignments.begin(),
                          ai_end = assignments.end();

        std::string at = "@";
        std::string sc = ";";

        for (; ai!=ai_end; ++ai ) // for each req. or opt. attribute
        {
            std::string var = (*ai).first;
            std::string val = (*ai).second;

            // node value: replace all occurences of @arg_name; by arg_value
            int p=0, q=0;
            while ( (q=node._value.find( make_wstring( at+var+sc ), p)) != -1 )
            {
                node._value.replace( q, var.length()+2, make_wstring( val ));
            }
            Assignments::iterator ati = node._attributes.begin(),
                              ati_end = node._attributes.end();

            // node attributes: replace all @arg_name by arg_value
            for (; ati != ati_end; ++ati )
            {
                if ( (*ati).second==at+var )
                {
                    (*ati).second = val;
                }
                else
                {
                    int p=0, q=0;
                    while ( (q=(*ati).second.find( at+var+sc, p)) != -1 )
                    {
                        (*ati).second.replace( q, var.length()+2, val );
                    }
                }
            }
        }

        int p = -1;
         // locate node subtrees
        while (( p = node._value.find( make_wstring("@subtree"), p+1 )) != -1 )
        {
            int q = node._value.find( make_wstring(";"), p);
            int r = node._value.find( make_wstring("."), p);
            
            if ( r > p && r < q )
            {
                std::wstring sub_name = node._value.substr( r+1, q-r-1 );                
                sub_tree_parent_id_map[ make_string( sub_name )] = (*it).first;
            }
            else
            {
                if ( sub_tree_parent_id != 0 )
                {
                    throw Error("Cannot generate more than one subtree."
                            "In %s instantiation of %s ", p_name, class_name.c_str() );
                }
                sub_tree_parent_id = (*it).first;
            }
        }
   }
    
    if ( sub_tree_parent_id!=0 )
    {
        if (! sub_tree_parent_id_map.empty() )
        {
            throw Error("Cannot use single unnamed subtree in connection with named subtrees");
        }

        XMLParser::Nodes nodes = i.SubTree();
        XMLParser::Nodes::iterator xi = nodes.begin(), xi_end = nodes.end();
        for ( ++xi; xi!=xi_end; ++xi )
        {   
            int id = (*xi).first;
            if ( (*xi).second._parent_node == i.Key() )
            {
                XMLParser::Node &node = (*xi).second;
                node._parent_node = sub_tree_parent_id;
            }
            p_nodes->insert( *xi );
        }
    }

    if (! sub_tree_parent_id_map.empty() )
    {
        for ( i.StepInto(); !!i; ++i )
        {
            std::string id = get_attribute( (*i)._attributes, "id" );
            sub_tree_parent_id = sub_tree_parent_id_map[ id ];
        
            XMLParser::Nodes nodes = i.SubTree();
            XMLParser::Nodes::iterator xi = nodes.begin(), xi_end = nodes.end();
            if ( ++xi == xi_end )
            {
                XMLParser::Iterator pni = p_nodes->find( sub_tree_parent_id );
                if ( pni != p_nodes->end() )
                {
                    std::wstring &res = const_cast< std::wstring & >( (*pni).second._value );
                    if ( -1 != res.find(L"@subtree") )
                    {
                        res.clear();
                    }
                    res += (*i)._value;
                }
                else
                {
                    //invalid subtree not found
                }
            }
            else
            {
                for ( xi; xi!=xi_end; ++xi )
                {   
                    int id = (*xi).first;
                    if ( (*xi).second._parent_node == i.Key() )
                    {
                        XMLParser::Node &node = (*xi).second;
                        node._parent_node = sub_tree_parent_id;
                    }
                    p_nodes->insert( *xi );
                }
            }
        }
    }

    XMLDocument xml( *p_nodes );
 
    ChildsFromXML( xml.Root(), &ch_env );

    return NULL;
}


};//namespace Useless
