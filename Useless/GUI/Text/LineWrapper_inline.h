
template< class WordStorage >
LineWrapper<WordStorage>::LineWrapper( const WordStorage &word_storage, int width, const Text &text )
{
    using TextInfo::Word;

    int  num_words  = word_storage.GetNumWords();
    Text separators = word_storage.GetSeparators();
    
    int i, row_width=0, row_height=0; _width=width; _height=0;
    Row row;

    typedef typename WordStorage::ConstIterator ConstIterator;
    ConstIterator word_iterator = word_storage.ConstBegin();

    for ( i=0; i<num_words; ++i )
    {
        Word word = *word_iterator++;

        row_height = (row_height>word.height)? row_height : word.height;
        row.height = row_height;

        if ( row_width + word.width <= width  || row_width<1 )
        {
            row_width += word.width;
            row.width = row_width;
            row.words.Insert(i);
        }
        else
        {
            _height += row.height;
            row_width  = word.width;
            row_height = 0;
            _rows.Insert(row);
            row = Row();
            row.width = word.width;
            row.height = word.height;
            row.words.Insert(i);
        }

        UniCode sepa = text[word.end];
        if ( sepa==separators[TextInfo::NEW_LINE] )
        {
            _height += row_height;
            row_width = 0;
            row_height = 0;
            _rows.Insert(row);
            row = Row();
        }
        else if ( sepa==separators[TextInfo::TAB] )
        {
            row_width += word.separator_width;
        }
        else
        {
            row_width += word.separator_width;
        }
    }

    if (!row.words.Empty())
    {
        _rows.Insert(row);
        _height += row.height;
    }
}
