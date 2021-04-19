#include "UselessPch.h"

#include "XMLParser.h"
#include <sstream>
#include <algorithm>

namespace Useless {

    XMLParser::XMLParser(): _parser_position(0), _last_id(0)
    {
    }

    XMLParser::XMLParser( const std::string &xml_string, const std::string &source )
        :_parser_position(0), _xml_string(xml_string), _last_id(0), _source(source)
        {
            Parse();
        }

    XMLParser::~XMLParser()
    {
    }

    XMLParser::XMLParser( const Nodes &nodes ): _nodes(nodes)
    {
        // nodes should be validated -- not implemented yet
    }


    std::string get_tag( const std::string &input, int *lt, int *rt, int start_from=0 )
    {
        *lt = input.find( "<", start_from );
        if ( *lt==-1 )
        { return std::string();
        }

        *rt = input.find( ">", *lt );
        if ( *rt==-1 )
        { return std::string();
        }

        return input.substr( (*lt)+1, (*rt)-(*lt)-1 );
    }

    bool is_at_end_of_string( const std::string &str, const std::string &ref )
    {
        if ( str.size() < ref.size() ) {
            return false;
        }
        else {
            return ( str.substr( str.size() - ref.size(), ref.size() ) == ref );
        }
    }

    XMLParser::XMLError::XMLError( const std::string &source,
            int lineNo,
            const char *text )
        :   Error("%s\n{ %i, %s }", text, lineNo, source.c_str() )
        {
        }

    void XMLParser::RaiseError( const Error &err )
    {
        int lineNo =
            std::count( _xml_string.begin(),
                    _xml_string.begin() + _parser_position, '\n') + 1;

        throw XMLError( _source, lineNo, err.GetError() );
    }

    void XMLParser::Parse()
    {
        if ( !_xml_string.empty() )
        { ParseImpl(0);}
    }

    void XMLParser::ParseImpl( int parent_id )
    {
        int left_tag, right_tag;
        int start_position = _parser_position;

        // get tag content, all between '<' and '>' characters
        std::string tag_string =
            chomp( get_tag( _xml_string, &left_tag, &right_tag, _parser_position ));
        _parser_position = right_tag;

        if ( tag_string.empty() ) { RaiseError("XML-Parser: Found empty tag."); }

        bool is_single_tag = ( *tag_string.rbegin() == '/' )? true : false;
        if ( is_single_tag )
        {
            tag_string.erase( tag_string.end()-1 );
        }

        // remove comments
        if ( tag_string.substr(0,3) == std::string("!--") )
        {
            _parser_position = _xml_string.find("-->", _parser_position-2);
            if ( _parser_position ==-1 ) {
                RaiseError( Error("XML-Parser: Found comment start, but comment end cannot be found."));
            }
            _parser_position += 2;
            return;
        }    
       
        // supply line number for errors
        int xmlLine = std::count( _xml_string.begin(), _xml_string.begin() + _parser_position, '\n') + 1;

        std::string &tagString = tag_string;
        int tagIter = 0;
        int tagSize = tagString.size();

        std::string tagName;
        // get tag name
        for (; tagIter < tagSize; ++tagIter )
        {
            char c = tagString[ tagIter ];
            if ( ( c >= 'a' && c <= 'z' ) ||
                    ( c >= 'A' && c <= 'Z' ) ||
                    ( c >= '0' && c <= '9' ) ||
                    ( c == '-' ) || ( c == '_' )
               ){
                tagName += c;
            }
            else if ( c == ' ' || c == '\n' || c == '\r' )
            {
                break;
            }
            else {
                RaiseError( Error("XML-Parser: Found bad %c (0x%0x) character in tag <%s>.", c, (int)c, tagName.c_str()));
            }
        }

        Attributes tagAttributes;
        // gather attributes
        while ( tagIter < tagSize )
        {   
            // skip white spaces
            for (; tagIter < tagSize; ++tagIter )
            {
                char c = tagString[ tagIter ];
                if ( c != ' ' && c != '\n' && c != '\r' ) { break; }
            }

            std::string attrName;
            // get attibute name
            for (; tagIter < tagSize; ++tagIter )
            {
                char c = tagString[ tagIter ];
                if ( ( c >= 'a' && c <= 'z' ) ||
                        ( c >= 'A' && c <= 'Z' ) ||
                        ( c >= '0' && c <= '9' ) ||
                        ( c == '-' ) || ( c == '_' )
                   ){
                    attrName += c;
                }
                else if ( c == ' ' || c == '\n' || c == '\r' )
                {
                    break;
                }
                else if ( c == '=' )
                {
                    break;
                }
                else {
                    RaiseError( Error("XML-Parser: Found bad %c (0x%0x) character in attribute '%s' of tag <%s>."
                                , c, (int)c, attrName.c_str(), tagName.c_str()));
                }
            }

            // skip white spaces before '=' character
            for (; tagIter < tagSize; ++tagIter )
            {
                char c = tagString[ tagIter ];
                if ( c != ' ' && c != '\n' && c != '\r' ) { break; }
            }

            // get attibute value
            std::string attrValue;
            // if attribute occured at end of tag without explicit value, use value of boolean true.
            if ( tagIter == tagSize || tagString[ tagIter ] != '=' )
            {
                attrValue = '1';
            }
            else
            {
                // skip '=' character
                ++tagIter; 
                // skip white spaces after '=' character
                for (; tagIter < tagSize; ++tagIter )
                {
                    char c = tagString[ tagIter ];
                    if ( c != ' ' && c != '\n' && c != '\r' ) { break; }
                } 
                // check wheather value is enclosed in ' or " quotes.
                bool isSingleQutedAttr = false;
                bool isDoubleQuotedAttr = false;
                if ( tagString[ tagIter ] == '"' ) {
                    isDoubleQuotedAttr = true;
                    ++tagIter;
                }
                else if ( tagString[ tagIter ] == '\'' ) {
                    isSingleQutedAttr = true;
                    ++tagIter;
                }
                else {
                    RaiseError( Error("XML-Parser: Attribute '%s' of tag <%s> must be enclosed in ' or \" quotes. Opening quote not found."
                        , attrName.c_str(), tagName.c_str() ));
                }
                // gather characters of attribute value
                for (; tagIter < tagSize; ++tagIter )
                {
                    char c = tagString[ tagIter ];
                    // handle escaped characters
                    if ( c == '\\' )
                    {
                        if ( ++tagIter == tagSize )
                        { RaiseError( Error("XML-Parser: Character expected after '\\' character in value of attribute '%s' of tag <%s>."
                                    , attrName.c_str(), tagName.c_str() ));
                        }
                        c = tagString[ tagIter ];
                        if ( c == '\'' || c == '"' || c == '\\' ) {
                            attrValue += c;
                        }
                        else {
                            std::stringstream ss;
                            for (; tagIter < tagSize; ++tagIter )
                            {
                                c = tagString[ tagIter ];
                                if ( c == ' ' || c == '\n' || c == '\r' ) { break; }
                                else {
                                    ss << tagString[ tagIter ];
                                }
                            }
                            int charNo;
                            ss >> charNo;
                            attrValue += (char)charNo;
                        }
                    }
                    else
                    {
                        if ( isDoubleQuotedAttr && c == '"' ) {
                            isDoubleQuotedAttr = false;
                            ++tagIter;
                            break;
                        }
                        else if ( isSingleQutedAttr && c == '\'' ) {
                            isSingleQutedAttr = false;
                            ++tagIter;
                            break;
                        }
                        else {
                            attrValue += c;
                        }
                    }
                }// gather characters of attribute value
                if ( isSingleQutedAttr || isDoubleQuotedAttr ) {
                    RaiseError( Error("XML-Parser: Attribute '%s' of tag <%s> must be enclosed in ' or \" quotes. Closing quote not found."
                        , attrName.c_str(), tagName.c_str() ));
                }
                // set locale
                if ( 0 == parent_id && "xml" == tagName && "encoding" == attrName )
                {
                    _locale = LangSelector::Instance()[ attrValue ];
                }
            }// if-else
            tagAttributes[ attrName ] = attrValue;

        }// gather attributes


        std::string &name = tagName;
        std::string value;
        Attributes &attributes = tagAttributes;

        int id = ++_last_id;
        Node *theNode = &_nodes[id];

        // deep scan recursive loop
        if ( !is_single_tag )
        {
            std::string tag;

            std::string end_tag = "/"; end_tag += name;
            int lt,rt;

            for( ;; )
            {
                tag = get_tag( _xml_string, &lt, &rt, _parser_position );            
                if ( tag.empty() ) {
                    break;
                }
                value += _xml_string.substr( _parser_position+1, lt-_parser_position-1 );
                std::stringstream interpreter; interpreter << tag; interpreter >> tag;
                if ( tag==end_tag ) {
                    break;
                }
                ParseImpl(id);
            }
            _parser_position = rt;
        }
        theNode->_name = name;

        {// chomp every word
            std::stringstream interpreter; interpreter << value;
            std::string word;
            std::string val;

            while ( interpreter >> word )
            {
                if ( !val.empty() ) {
                    if ( !is_at_end_of_string( val, "&sp;") &&
                            !is_at_end_of_string( val, "&nl;") &&
                            !is_at_end_of_string( val, "&rc;") &&
                            !is_at_end_of_string( val, "&ts;") ) {
                        val += "&sp;";
                    }
                }
                val += to_chars( word );
            }

            typedef std::map< std::string, std::string > IdMap;
            IdMap idmap;
            idmap["sp"] = " ";
            idmap["nl"] = "\n";
            idmap["rc"] = "\r";
            idmap["ts"] = "\t";
            idmap["amp"] = "&";
            idmap["quot"] = "\"";
            idmap["lt"] = "<";
            idmap["gt"] = ">";
            idmap["tag"] = "\020";

            int p = val.find('&');
            int q = val.find(';', p);
            while ( p > -1 && q > -1 )
            {
                std::string ids = val.substr( p+1, q-p-1 );
                IdMap::iterator idit = idmap.find( ids );
                if ( idit != idmap.end() )
                {
                    val.replace( p, ids.size()+2, (*idit).second );
                    q = p + (*idit).second.size();
                }
                p = val.find('&', q);
                q = val.find(';', p);
            }
            // convert encoded byte-string to unicode-string using encoding
            Read( theNode->_value, val, _locale );
        }
        theNode->_attributes = attributes;
        theNode->_parent_node = parent_id;
        theNode->_source = _source;
        theNode->_line = xmlLine;
    }


    XMLParser::Nodes XMLIterator::SubTree()
    {
        XMLParser::Nodes nodes;
        std::list< XMLIterator > it_stack;
        it_stack.push_back( *this );

        while ( !it_stack.empty() )
        {
            XMLIterator it = *it_stack.rbegin();
            it_stack.pop_back();

            // remember last node
            nodes.insert( std::make_pair( it.Key(), it.Node() ));

            for ( it.StepInto(); !!it; ++it ) // scan each child
            {
                it_stack.push_back(it); // remember each child node for future scan
            }
        }
        return nodes;
    }

};//namespace Useless
