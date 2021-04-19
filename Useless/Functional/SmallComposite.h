#ifndef __INLCUDED__USELESS_SMALL_COMPOSITE_H__
#define __INLCUDED__USELESS_SMALL_COMPOSITE_H__

namespace Useless {

    /*! SmallComposite creates functional-binary-tree
     *  Use it to create RT function which evaluates some expression
     *  For example: (A | B) & (C ^ D)
     *  This expression will be evaluated everythime from the begining
     *  so it depends on RT values of A,B,C and D which may change
     *  during program run
     -------------------------------------------------------------------*/
    template< class T > 
        class SmallComposite
        {
            public:
                typedef T Type;

                SmallComposite(): _node_1(0), _node_2(0) {}
                SmallComposite( Type x ): _node_1(0), _node_2(0), _value(x) {}
                SmallComposite( const SmallComposite &s )
                    : _value(s._value), _node_1(s._node_1), _node_2(s._node_2)
                    {
                        const_cast<SmallComposite&>(s)._node_1 = 0;
                        const_cast<SmallComposite&>(s)._node_2 = 0;
                    }

                ~SmallComposite()
                {
                    if (_node_1) { delete _node_1; _node_1=0; }
                    if (_node_2) { delete _node_2; _node_2=0; }
                }

                template< class P1, class P2, class Val, class Parms >
                    bool Eval( P1 &p1, P2 p2, Val x, Parms &parms )
                    {
                        bool do_continue = p2.Eval(x, parms );
                        do_continue = p1.Eval( p2(), parms );
                        return do_continue;
                    }

                template< class Pred > bool ApplyPred( Pred p )
                {
                    return ApplyPred( p, (int)0 );
                }

                template< class Pred, class Params > bool ApplyPred( Pred p, const Params &params )
                {
                    if ( _node_1 && _node_2 )
                    {
                        if ( p.Eval( _node_1->ApplyPred( p.Lhs(), params ), params ) )
                        {
                            p.Eval( _node_2->ApplyPred( p.Rhs(), params ), params );
                        }
                    }
                    else if ( _node_1 )
                    {
                        if ( Eval( p, p.Rhs(), _value, params ) )  
                        {
                            p.Eval( _node_1->ApplyPred( p.Lhs(), params ), params );
                        }
                    }
                    else
                    {
                        p.Eval( _value, params );
                    }
                    return p();
                }

                template< class SmallCompositeAdapter >
                    static SmallCompositeAdapter
                    Concatenate( SmallCompositeAdapter p1, SmallCompositeAdapter p2 )
                    {
                        if	( !p1._node_1 ) { p1._node_1 = new SmallCompositeAdapter(p2); return p1; }
                        else if ( !p2._node_1 ) { p2._node_1 = new SmallCompositeAdapter(p1); return p2; }
                        else
                        {
                            SmallCompositeAdapter adapter;
                            adapter._node_1 = new SmallCompositeAdapter(p1);
                            adapter._node_2 = new SmallCompositeAdapter(p2);
                            return adapter;
                        }	
                    }

                Type            _value;
                SmallComposite *_node_1;
                SmallComposite *_node_2;
        };

    /* Terminator Predicate NONE
       --------------------------------------*/
    class Pred_NONE
    {
        public:
            Pred_NONE(): _result(0) {}
            bool operator ()() { return _result; }
            template< typename T > bool Eval( bool v, const T &p )
            {
                _result=v; return true; 
            }

            Pred_NONE Lhs() { return Pred_NONE(); }
            Pred_NONE Rhs() { return Pred_NONE(); }

        private:
            bool _result;
    };

    /* Cumulative predicate AND
       --------------------------------------*/
    template< typename LHS=Pred_NONE, typename RHS=Pred_NONE >
        class Pred_AND
        {
            public:
                Pred_AND(): _result(1) {}
                bool operator ()() { return _result; }
                template< typename T > bool Eval( bool v, const T &p ) 
                {
                    return _result &= v;
                }

                LHS Lhs() { return LHS(); }
                RHS Rhs() { return RHS(); }

            private:
                bool _result;
        };

    /* Cumulative predicate OR
       --------------------------------------*/
    template< typename LHS=Pred_NONE, typename RHS=Pred_NONE >
        class Pred_OR
        {
            public:
                Pred_OR(): _result(0) {}
                bool operator ()() { return _result; }
                template< typename T > bool Eval( bool v, const T &p ) 
                {
                    return !(_result |= v);
                }

                LHS Lhs() { return LHS(); }
                RHS Rhs() { return RHS(); }

            private:
                bool _result;
        };

    /* Cumulative predicate XOR
       --------------------------------------*/
    template< typename LHS=Pred_NONE, typename RHS=Pred_NONE >
        class Pred_XOR
        {
            public:
                Pred_XOR(): _result(0) {}
                bool operator ()() { return _result; }
                template< typename T > bool Eval( bool v, const T &p ) 
                {
                    _result = (_result && !v) || (!_result && v);
                    return true; 
                }

                LHS Lhs() { return LHS(); }
                RHS Rhs() { return RHS(); }

            private:
                bool _result;
        };

    /* Dummy USER Predicate
       --------------------------------------*/
    class Pred_USER
    {
        public:
            bool operator ()() { return _result; }

            Pred_NONE Lhs() { return Pred_NONE(); }
            Pred_NONE Rhs() { return Pred_NONE(); }

        protected:
            bool _result;
    };

};//namespace Useless
#endif//__INLCUDED__USELESS_SMALL_COMPOSITE_H__
