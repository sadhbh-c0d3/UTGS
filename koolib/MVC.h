/********************************************************************
 *                                                                  *
 * THIS FILE IS PART OF THE KooLAS C++ Template & Class Library     *
 * CALLED KooLib SOURCE CODE.                                       *
 * USE, DISTRIBUTION AND REPRODUCTION OF THIS LIBRARY SOURCE IS     *
 * GOVERNED BY A BSD-STYLE SOURCE LICENSE INCLUDED WITH THIS SOURCE *
 * IN 'COPYING'. PLEASE READ THESE TERMS BEFORE DISTRIBUTING.       *
 *                                                                  *
 * THE KooLib SOURCE CODE IS (C) COPYRIGHT 2004                     *
 * by the Sadhbh Code (https://github.com/sadhbh-c0d3)              *
 *                                                                  *
 ********************************************************************

  function:
  last mod: $Id: MVC.h,v 1.2 2004/06/03 18:17:32 koolas Exp $

 ********************************************************************/


#ifndef __INCLUDED_MVC_H__
#define __INCLUDED_MVC_H__

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#define MVC_DUMB_METHOD( _retType, _proto ) \
    _retType _proto { assert(! "Dumb method: "#_proto); return _retType(); }

#define MVC_VIRTUAL_METHOD( _retType, _proto ) \
    public: virtual _retType _proto = 0;

#define MVC_PRIVATE_METHOD( _retType, _proto ) \
    private: _retType _proto { assert(! "Private method: "#_proto); return _retType(); }

#define MVC_REGULAR_METHOD( _retType, _proto ) \
    _retType _proto;

#define MVC_PROTO_QueryInterface( _typeName ) \
    boost::shared_ptr< _typeName > QueryInterface( MVC::Type< _typeName > )

#define MVC_VIRTUAL_METHOD_QueryInterface( _typeName ) \
    MVC_VIRTUAL_METHOD( boost::shared_ptr< _typeName >, QueryInterface( MVC::Type< _typeName > ))


namespace MVC {

    template< class _Type > struct Type {};

    template< class _Interface > struct Model;
    template< class _Interface > struct View;
    
    template< class _Interface > struct PublicView : _Interface
    {
    };
    
    template< class _Interface > struct PublicModel : _Interface
    {
    };

    template< class _Interface > struct Model : virtual PublicModel< _Interface >
    {
        boost::weak_ptr< View< _Interface > > _view;
        boost::shared_ptr< View< _Interface > > GetView()
        {
            return boost::shared_ptr< View< _Interface > >( _view );
        }
        bool IsViewAttached() const { return !_view.expired(); }
        virtual void OnViewAttached() {}
    };

    template< class _Interface > struct View : virtual PublicView< _Interface >
    {
        boost::weak_ptr< Model< _Interface > > _model; 
        boost::shared_ptr< Model< _Interface > > GetModel()
        {
            return boost::shared_ptr< Model< _Interface > >( _model );
        }
        bool IsModelAttached() const { return !_model.expired(); }
        virtual void OnModelAttached() {}
    };

    template< class _Interface > struct Controller
    {
        boost::shared_ptr< Model< _Interface > > _model;
        boost::shared_ptr< View< _Interface > > _view;

        void SetModel( boost::shared_ptr< Model< _Interface > > model )
        {
            _model = model;
            if ( !!_view )
            {
                _view->_model = _model;
                _model->_view = _view;
                _view->OnModelAttached();
                _model->OnViewAttached();
            }
        }
        
        void SetView( boost::shared_ptr< View< _Interface > > view )
        {
            _view = view;
            if ( !!_model )
            {
                _model->_view = _view;
                _view->_model = _model;
                _view->OnModelAttached();
                _model->OnViewAttached();
            }
        }

        void SetModel( Model< _Interface > *model )
        {
            _model.reset( model );
            if ( !!_view )
            {
                _view->_model = _model;
                _model->_view = _view;
                _view->OnModelAttached();
                _model->OnViewAttached();
            }
        }
        
        void SetView( View< _Interface > *view )
        {
            _view.reset( view );
            if ( !!_model )
            {
                _model->_view = _view;
                _view->_model = _model;
                _view->OnModelAttached();
                _model->OnViewAttached();
            }
        }
    };

    template< class _Interface > struct CreateView
    {
        static boost::shared_ptr< View< _Interface > > Apply();
    };

    template< class _Interface > struct CreateModel
    {
        static boost::shared_ptr< Model< _Interface > > Apply();
    };

    template< class _Interface >
    struct Interface
    {
        boost::shared_ptr< _Interface > _ptr;
        template< class _Object >
            Interface( _Object object )
                : _ptr( object->QueryInterface( MVC::Type< _Interface >() ))
            {}

        boost::shared_ptr< _Interface > operator ->() const { return _ptr; }
        _Interface & operator * () const { return *_ptr; }
    };


};//MVC

#endif//__INCLUDED_MVC_H__
