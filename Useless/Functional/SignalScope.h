#ifndef __INCLUDED_USELESS_SIGNAL_SCOPE_H__
#define __INCLUDED_USELESS_SIGNAL_SCOPE_H__

namespace Useless {

    struct SignalScope
    {
        private:
            struct Item
            {
                Item( const Signal::FuncID &fid, Item *next )
                    : _scopedFID( fid )
                    , _next( next )
                    {}

                ~Item()
                {
                    delete _next;
                }

                Signal::UntieOnDestruction  _scopedFID;
                Item *_next;
            };

            Item *_head;

            SignalScope& operator = ( const SignalScope &);
            SignalScope( const SignalScope & );

        public:
            SignalScope()
                : _head(0)
                {}

            ~SignalScope()
            {
                delete _head;
            }

            void add( const Signal::FuncID &fid )
            {
                _head = new Item( fid, _head );
            }
    };

}; //Useless

#endif//__INCLUDED_USELESS_SIGNAL_SCOPE_H__
