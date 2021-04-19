#if !defined __INCLUDED_USELESS_GRAPHWIDGET_H__
#define __INCLUDED_USELESS_GRAPHWIDGET_H__

/*
 * $Id: GraphWidget.h,v 1.8 2002/09/13 16:22:15 koolas Exp $
 *
 *  NAME
 *      GraphWidget
 *
 *  PURPOSE
 *      Definition of GraphWidget.
 *
 *  AUTHOR(S)
 *      Wally (wally@iavmb.pl)
 *
 *  
 */

#include "Useless/System/Types.h"
#include "Useless/Graphic/Point.h"
#include "Useless/Graphic/Rect.h"
#include "Useless/GUI/Widgets/Widget.h"
#include "Useless/Graphic/Pixel/Pixel.h"

namespace Useless
{
	//! Class %GraphWidget defines graph container.
	/*! \ingroup Widgets
	 *  \p #include \p "Useless/GUI/Widgets/GraphWidget.h" <br> <br>
	 *  
	 *  Graph %widget provides methods to draw simple graph on transparent background.
	 *   It can draw any set of points or mathematical function in any color and scale it.
	 *  \todo Add GT_LINES graph type in Repaint(...) method.
	 */
	class USELESS_API GraphWidget : virtual public Widget
	{
		//======================================*
		// Internal types definitions:          |
		//======================================*
	public:
		    // Changed by Szyman
        typedef double                       tpe_data_type;
        //typedef float                        tpe_data_type;
		typedef std::vector<tpe_data_type>   tpe_input_vector;
		typedef enum { GT_POINTS, GT_LINES } tpe_graph_type;
		typedef NormalRGB                    tpe_color;

	protected:
		//! Element of output vector.
		typedef Point<int>                   tpe_point;
		//! Internal vector used to store the calculation results.
		typedef std::vector<tpe_point>       tpe_output_vector;
		//! Structure containing data about graph's bounds.
		typedef struct
				  {
					  tpe_data_type MaxX;
					  tpe_data_type MinX;
					  tpe_data_type MaxY;
					  tpe_data_type MinY;
					  bool          AutoBounds;
				  }                            sct_value_bounds;

		
	public:
		//======================================*
		// Constructors and destructor:         |
		//======================================*
        //! Constructor. graph's dimensions required. Graph's color and type are optional.
		GraphWidget(     const Dim2i& par_GraphDimensions,
						 tpe_color par_GraphColor = fld_DefaultGraphColor, 
						 tpe_graph_type par_GraphType = fld_DefaultGraphType );
        
        //! Constructor. Y-values vector and graph's dimensions required. Graph's color and type are optional.
		GraphWidget( const tpe_input_vector& par_YVector, const Dim2i& par_GraphDimensions,
						 tpe_color par_GraphColor = fld_DefaultGraphColor, 
						 tpe_graph_type par_GraphType = fld_DefaultGraphType );

		//! Constructor. X-values vector, Y-values vector and graph's dimensions required. Graph's color and type are optional.
		GraphWidget( const tpe_input_vector& par_XVector, const tpe_input_vector& par_YVector,
						 const Dim2i& par_GraphDimensions, tpe_color par_GraphColor = fld_DefaultGraphColor, 
						 tpe_graph_type par_GraphType = fld_DefaultGraphType );

		//! Destructor.
		virtual ~GraphWidget() {};

		//======================================*
		// Public methods:                      |
		//======================================*
		//! Called when widget should be painted/repainted.
		virtual void          Repaint( const WidgetPainter &painter );
		//! Changes graph's dimensions.
		virtual void          Resize( int w, int h );
		//! Sets default value of new graph's color.
		static tpe_color      SetDefaultGraphColor( const tpe_color par_color ) { return (fld_DefaultGraphColor = par_color); };
		//! Sets default value of new graph's type.
		static tpe_graph_type SetDefaultGraphType( const tpe_graph_type par_type ) { return (fld_DefaultGraphType = par_type); };
		//! Sets graph's color.
		void                  SetGraphColor( tpe_color par_NewColor ) { fld_GraphColor = par_NewColor; SetDirty(); };
		//! Sets graph's type.
		void                  SetGraphType( tpe_graph_type par_NewType ) { fld_GraphType = par_NewType; SetDirty(); };
		//! Sets graph's input data.
		void                  SetInputData( tpe_input_vector& par_YVector );
		//! Sets graph's input data.
		void                  SetInputData( tpe_input_vector& par_XVector, tpe_input_vector& par_YVector );
		//! Sets bounds of x- and y- values.
		void                  SetValueBounds( tpe_data_type par_MinX, tpe_data_type par_MaxX, 
														  tpe_data_type par_MinY, tpe_data_type par_MaxY );
		//! Value bounds will be automaticaly determined.
		void                  UseAutoBounds();

	protected:
		//======================================*
		// Protected fields:                    |
		//======================================*
		//! Stores graph's color value.
		tpe_color         fld_GraphColor;
		//! Stores graph's dimensions. Must be specified during creation/initialisation.
		/*! \p Dim2i is defined in \p "Point.h" as \code typedef Point<int> Dim2i; \endcode */
		Dim2i             fld_GraphDimensions;
		//! Stores graph points vector.
		tpe_output_vector fld_GraphPoints;
		//! Stores graph's type.
		tpe_graph_type    fld_GraphType;
		//! Stores bounds of x- and y-values.
		sct_value_bounds  fld_ValueBounds;
		//! Stores X-values vector.
		tpe_input_vector  fld_XVector;
		//! Stores Y-values vector. Must be specified during creation/initialisation.
		tpe_input_vector  fld_YVector;

		//--------------------------------------+
		// Static protected fields:             |
		//--------------------------------------+
		//! Stores default value of new graph's color.
		static tpe_color      fld_DefaultGraphColor;
		//! Stores default value of new graph's type.
		static tpe_graph_type fld_DefaultGraphType;

		//======================================*
		// Protected methods:                   |
		//======================================*
		//! Performs all major before-display calculations.
		void CalculateGraphPoints();
		//! Defaultly initialises X-values vector.
		void InitialiseXVector();

	private:
		int Round( double x ) { return ( x-(double)((int)x) >= (double)0.5 ? (int)x + 1 : (int)x ); };
	}; // class BasicGraphWidget

////////////////////////////////////////////////////////////////////////////////
// --------------------- INLINE METHODS IMPLEMENTATION ---------------------- //
////////////////////////////////////////////////////////////////////////////////
	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | Resize
	//* Method Type:  | virtual, inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method changes graph widget's dimensions, recalculates
	  *  graph's points and then refreshes the widget.
	  * \param w - new width of the graph;
	  * \param h - new height of the graph;
	  *///******************************************************
	inline void GraphWidget::Resize( int w, int h )
	{
		fld_GraphDimensions.x = w;
		fld_GraphDimensions.y = h;
		CalculateGraphPoints();
		SetDimensions(w,h);
	} // GraphWidget::Resize

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | SetInputData
	//* Method Type:  | inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
     *///******************************************************
	inline void GraphWidget::SetInputData( tpe_input_vector& par_YVector )
	{
		fld_YVector = par_YVector;
		InitialiseXVector();
		CalculateGraphPoints();
	} // GraphWidget::SetInputData

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | SetInputData
	//* Method Type:  | inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
     *///******************************************************
	inline void GraphWidget::SetInputData( tpe_input_vector& par_XVector, tpe_input_vector& par_YVector )
	{
		fld_XVector = par_XVector;
		fld_YVector = par_YVector;
		CalculateGraphPoints();
	} // GraphWidget::SetInputData

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | SetValueBounds
	//* Method Type:  | inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method allows to set bounds of data values to be 
	  *  displayed in the graph area. We should use it, if for
	  *  example we want to display only one period of
	  *  periodical functions, function's positive part only or
	  *  its any other rectangular part. Of course 'function'
	  *  doesn't only mean function of type y=f(x). <br>
	  * It is important to remember that such rectangular area  
	  *  of entire graph will be scaled to graph's dimmensions.
	  *  It can be said that value bounds work like zoom. <br>
	  * To draw entire graph after use of SetValueBounds method,
	  * To make changes visible, widget should be set dirt.
	  *  UseAutoBounds method should be called.
	  * \param par_MinX - minimal value of x-vector's element,
	  *  displayed on the graph;
	  * \param par_MaxX - maximal value of x-vector's element,
	  *  displayed on the graph;
	  * \param par_MinY - minimal value of y-vector's element,
	  *  displayed on the graph;
	  * \param par_MaxY - maximal value of y-vector's element,
	  *  displayed on the graph;
	  * \sa UseAutoBounds
	  *///******************************************************
	inline void GraphWidget::SetValueBounds( tpe_data_type par_MinX, tpe_data_type par_MaxX, 
														tpe_data_type par_MinY, tpe_data_type par_MaxY )
	{
		assert( par_MaxX > par_MinX );
		assert( par_MaxY > par_MinY );

		fld_ValueBounds.MaxX       = par_MaxX;
		fld_ValueBounds.MinX       = par_MinX;
		fld_ValueBounds.MaxY       = par_MaxY;
		fld_ValueBounds.MinY       = par_MinY;
		fld_ValueBounds.AutoBounds = false;

		CalculateGraphPoints();
	} // GraphWidget::SetValueBounds

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | UseAutoBounds
	//* Method Type:  | inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method causes graph to be drawn auto-scaled. Auto-scaled
	  *  mode is default after object initialisation and can be
	  *  changed to "custom-scaled" mode by use of 
	  *  SetValueBounds method. <br>
	  * To make changes visible, widget should be set dirt.
	  * \sa SetValueBounds
	  *///******************************************************
	inline void GraphWidget::UseAutoBounds()
	{
		fld_ValueBounds.AutoBounds = true;

		CalculateGraphPoints();
	} // GraphWidget::UseAutoBounds

}; // namespace Useless

#endif // !defined __INCLUDED_USELESS_BASICGRAPHWIDGET_H__
