#include "UselessPch.h"
#include "Useless/GUI/Widgets/GraphWidget.h"

////////////////////////////////////////////////////////////////////////////////
// ----------------------------- NAMESPACE USELESS ----------------------------- //
////////////////////////////////////////////////////////////////////////////////
namespace Useless
{
	////////////////////////////////////////////////////////////////////////////////
	// ----------------- STATIC MEMBER VARIABLES INITIALISATION ----------------- //
	////////////////////////////////////////////////////////////////////////////////
	GraphWidget::tpe_color      GraphWidget::fld_DefaultGraphColor = MakeRGB(0,0,0);
	GraphWidget::tpe_graph_type GraphWidget::fld_DefaultGraphType  = GraphWidget::GT_POINTS;

	////////////////////////////////////////////////////////////////////////////////
	// ---------------------- CONSTRUCTORS AND DESTRUCTOR ----------------------- //
	////////////////////////////////////////////////////////////////////////////////
	GraphWidget::GraphWidget(const Dim2i&           par_GraphDimensions,
						           tpe_color        par_GraphColor  /*= fld_DefaultGraphColor*/, 
						           tpe_graph_type   par_GraphType   /*= fld_DefaultGraphType )*/ )
    {
		fld_GraphColor      = par_GraphColor;
		fld_GraphDimensions = par_GraphDimensions;
		fld_GraphType       = par_GraphType;
		fld_ValueBounds.AutoBounds = true;
        SetDimensions( fld_GraphDimensions.x, fld_GraphDimensions.y );
    }
    
    //**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | GraphWidget
	//* Method Type:  | constructor, inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * This constructor is useful to create graph of data
	  *  vector's elements in the function of their index.
	  *  On the X-axis are located values of data vector's index.
	  *  They increment from \p 0 to <i>vector's length</i> and
	  *  are located on the graph at regular intervals. On the
	  *  Y-axis are located vector elements' values. <br>
	  * Graph's size depend's on \p par_GraphDimensions 
	  *  parameter. X- and Y- axis scale also depends on the
	  *  graph's dimensions. Max value of index will be located
	  *  at right bound of graph. Max value of vector's elements
	  *  will be located at top bound of graph. Min value of
	  *  index and vector's elements will be located at left and
	  *  bottom bound of graph. <br> Because of above, to scale
	  *  for example Y-axis to 50% of actual size you have only
	  *  to call \p Resize method, set new height value to half
	  *  of old value, left width unchanged and refresh the 
	  *  widget. <br>
	  * \param par_YVector - reference to standard \p vector
	  *  containing graph's data.
	  * \param par_GraphDimensions - reference to Dim2i object
	  *  containing size of graph's area.
	  * \param par_GraphColor - optional parameter which
	  *  determine color of drawn graph. If not specified,
	  *  default color value will be used (it is stored in 
	  *  \p fld_DefaultGraphColor field and can be set by
	  *  \p SetDefaultGraphColor public, static method).
	  * \param par_GraphType - optional parameter which
	  *  determine graph's type. If not specified, default value
	  *  will be used (is is stored in \p fld_DefaultGraphType
	  *  field and can be set by \p SetDefaultGraphType static,
	  *  public method). For now, two types of graph are
	  *  planned: <ul> <li>points (\p GT_POINTS ) - in which
	  *  single points are drawn at suitable locations</li>
	  *  <li>connected points (\p GT_LINES ) - in which points
	  *  of graph are connected by lines.
	  *///******************************************************
	GraphWidget::GraphWidget( const tpe_input_vector& par_YVector, 
							  const Dim2i&            par_GraphDimensions,
									tpe_color         par_GraphColor, 
									tpe_graph_type    par_GraphType )
	{
		fld_YVector = par_YVector;
		fld_XVector.clear();
		fld_GraphPoints.clear();
		
		fld_GraphColor      = par_GraphColor;
		fld_GraphDimensions = par_GraphDimensions;
		fld_GraphType       = par_GraphType;
		fld_ValueBounds.AutoBounds = true;

        SetDimensions( fld_GraphDimensions.x, fld_GraphDimensions.y );
		InitialiseXVector();
		CalculateGraphPoints();
	} // GraphWidget::GraphWidget

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | GraphWidget
	//* Method Type:  | constructor, inline
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * This constructor is useful to create graph of collection
	  *  of points. On X-axis are located values got from
	  *  \p par_XVector. On Y-axis are located suitable values
     *  got from \p par_YVector.
	  * Graph's size depend's on \p par_GraphDimensions 
	  *  parameter. X- and Y- axis scale also depends on the
	  *  graph's dimensions. Max value of index will be located
	  *  at right bound of graph. Max value of vector's elements
	  *  will be located at top bound of graph. Min value of
	  *  index and vector's elements will be located at left and
	  *  bottom bound of graph. <br> Because of above, to scale
	  *  for example Y-axis to 50% of actual size you have only
	  *  to call \p Resize method, set new height value to half
	  *  of old value, left width unchanged and refresh the 
	  *  widget. <br>
	  * \param par_XVector - reference to standard \p vector
	  *  containing x parts of graph's points.
	  * \param par_YVector - reference to standard \p vector
	  *  containing y parts of graph's points.
	  * \param par_GraphDimensions - reference to Dim2i object
	  *  containing size of graph's area.
	  * \param par_GraphColor - optional parameter which
	  *  determine color of drawn graph. If not specified,
	  *  default color value will be used (it is stored in 
	  *  \p fld_DefaultGraphColor field and can be set by
	  *  \p SetDefaultGraphColor public, static method).
	  * \param par_GraphType - optional parameter which
	  *  determine graph's type. If not specified, default value
	  *  will be used (is is stored in \p fld_DefaultGraphType
	  *  field and can be set by \p SetDefaultGraphType static,
	  *  public method). For now, two types of graph are
	  *  planned: <ul> <li>points (\p GT_POINTS ) - in which
	  *  single points are drawn at suitable locations</li>
	  *  <li>connected points (\p GT_LINES ) - in which points
	  *  of graph are connected by lines.
	  *///******************************************************
	GraphWidget::GraphWidget( const tpe_input_vector& par_XVector, 
									  const tpe_input_vector& par_YVector,
									  const Dim2i&            par_GraphDimensions, 
											  tpe_color         par_GraphColor, 
											  tpe_graph_type    par_GraphType )
	{
		fld_YVector = par_YVector;
		fld_XVector = par_XVector;
		fld_GraphPoints.clear();
		fld_GraphColor      = par_GraphColor;
		fld_GraphDimensions = par_GraphDimensions;
		fld_GraphType       = par_GraphType;
		fld_ValueBounds.AutoBounds = true;
        SetDimensions( fld_GraphDimensions.x, fld_GraphDimensions.y );
		CalculateGraphPoints();
	} // GraphWidget::GraphWidget

	////////////////////////////////////////////////////////////////////////////////
	// ----------------------------- PUBLIC METHODS ----------------------------- //
	////////////////////////////////////////////////////////////////////////////////
	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | Repaint
	//* Method Type:  | virtual
	//* Method Scope: | public
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method paints the graph's points calculated by the 
	  *  \p CalculateGraphPoints method on widget's surface.
	  * \sa CalculateGraphPoints
	  *///******************************************************
	void GraphWidget::Repaint( const WidgetPainter &painter )
	{
		//***************************************
		// Prepare support variables:           *
		//***************************************
		unsigned int i; // Used in draw loop;

		//assert( fld_GraphPoints.size()>=2 );
		if ( fld_GraphPoints.size() < 2 ) return;


		//***************************************
		// Draw graph in GT_POINTS style:       *
		//***************************************
		if ( fld_GraphType == GT_POINTS )
		{
			for ( i = 0; i < fld_GraphPoints.size(); i++ )
			{
				Pos P( fld_GraphPoints[i].x,
					   fld_GraphDimensions.y - fld_GraphPoints[i].y - 1 );
				painter.PutPixel( P, fld_GraphColor );
			}
		}

		//***************************************
		// Draw graph in GT_LINES style:        *
		//***************************************
		if ( fld_GraphType == GT_LINES )
		{   
            std::vector< Pos > vertices;
            Pos previous_p( fld_GraphPoints[0].x,
                            fld_GraphDimensions.y - fld_GraphPoints[0].y - 1 );
            vertices.push_back( previous_p );
            int y_sum=0;
            int p_count=0;

			for ( i = 1; i < fld_GraphPoints.size(); i++ )
			{
				Pos p( fld_GraphPoints[i].x,
					   fld_GraphDimensions.y - fld_GraphPoints[i].y - 1 );
                
                y_sum += p.y; ++p_count;

                if ( (previous_p-p).Sqr() > 7*7 )// avoid bad overlaping effect
                {
                    p.y = y_sum/p_count;
                    vertices.push_back(p);
                    previous_p=p; y_sum=0; p_count=0;
                }
			}
			painter.PaintLine( vertices, fld_GraphColor, 1, 1 );
		}
	}; // GraphWidget::Repaint

	////////////////////////////////////////////////////////////////////////////////
	// ---------------------------- PROTECTED METHODS --------------------------- //
	////////////////////////////////////////////////////////////////////////////////
	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | CalculateGraphPoints
	//* Method Type:  | -
	//* Method Scope: | protected
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method projects entire graph (or its part if values 
	  *  bounds are specified) on graph widget's area. <br>
	  * Method should be called after 
	  *///******************************************************
	void GraphWidget::CalculateGraphPoints()
	{
		unsigned int     loc_PointIndex; // Index of vector element.
		unsigned int     loc_VectorSize; // Size of data vectors.
		tpe_data_type    loc_XMax; // Maximum value in x-values vector.
		tpe_data_type    loc_XMin; // Minimum value in x-values vector.
		tpe_data_type    loc_YMax; // Maximum value in y-values vector.
		tpe_data_type    loc_YMin; // Minimum value in y-values vector.
		double           loc_XCoefficient; // 
		double           loc_YCoefficient; //
		Useless::Point<int> loc_tmp_point;
		
		//***************************************
		// Prepare support variables:           *
		//***************************************
		//======================================*
		// If graph points vector isn't empty,  |
		//  clear it.                           |
		//======================================*
		if ( fld_GraphPoints.empty() == false )
		{
			fld_GraphPoints.clear();
		}

		//======================================*
		// Determine common vector size. If     |
		//  fld_XVector's and fld_YVector's     |
		//  sizes are different, lower will be  |
		//  assumpted.                          |
		//======================================*
		loc_VectorSize = fld_YVector.size();
		
		if ( loc_VectorSize > fld_XVector.size() )
		{ 
			loc_VectorSize = fld_XVector.size();
		}
		if ( loc_VectorSize < 2 )
		{	
			return;
		}

		//======================================*
		// Determine boundary values of data:   |
		//======================================*
		if ( fld_ValueBounds.AutoBounds == true )
		{
			loc_XMax = fld_XVector[0];
			loc_XMin = fld_XVector[0];
			loc_YMax = fld_YVector[0];
			loc_YMin = fld_YVector[0];
			for ( loc_PointIndex = 1; loc_PointIndex < loc_VectorSize; loc_PointIndex++ )
			{
				if ( fld_XVector[loc_PointIndex] > loc_XMax ) loc_XMax = fld_XVector[loc_PointIndex];
				if ( fld_XVector[loc_PointIndex] < loc_XMin ) loc_XMin = fld_XVector[loc_PointIndex];
				if ( fld_YVector[loc_PointIndex] > loc_YMax ) loc_YMax = fld_YVector[loc_PointIndex];
				if ( fld_YVector[loc_PointIndex] < loc_YMin ) loc_YMin = fld_YVector[loc_PointIndex];
			}
		} // if ( fld_ValueBounds.AutoBounds == true )
		else
		{
			loc_XMax = fld_ValueBounds.MaxX;
			loc_XMin = fld_ValueBounds.MinX;
			loc_YMax = fld_ValueBounds.MaxY;
			loc_YMin = fld_ValueBounds.MinY;
		} // else of ( fld_ValueBounds.AutoBounds == true )

		//======================================*
		// Determine coefficients:              |
		//======================================*
		loc_XCoefficient = (double)(fld_GraphDimensions.x - 1) / ( loc_XMax - loc_XMin );
		loc_YCoefficient = (double)(fld_GraphDimensions.y - 1) / ( loc_YMax - loc_YMin );

		//***************************************
		// Calculate points:                    *
		//***************************************
		for ( loc_PointIndex = 0; loc_PointIndex < loc_VectorSize; loc_PointIndex++ )
		{
			loc_tmp_point.x = Round(( fld_XVector[loc_PointIndex] - loc_XMin ) * loc_XCoefficient);
			loc_tmp_point.y = Round(( fld_YVector[loc_PointIndex] - loc_YMin ) * loc_YCoefficient);
			fld_GraphPoints.push_back( loc_tmp_point );
		}
	} // GraphWidget::CalculateGraphPoints

	//**********************************************************
	//* Class Name:   | Useless::GraphWidget
	//* Method Name:  | InitialiseXVector
	//* Method Type:  | -
	//* Method Scope: | protected
	//*               | 
	//*_DoxyGen:______|_________________________________________
	 /*!
	  * Method initialises X-values vector as vector of size
	  *  equal to size of Y-values vector with values equal to
	  *  their index. (first value == 0, second value == 1 etc.).
	  * Method is called by constructor, when X-values vector
	  *  was not specified.
	  *///******************************************************
	void GraphWidget::InitialiseXVector()
	{
		tpe_data_type loc_CurrentValue;
		tpe_data_type loc_IncrementValue;
		int           loc_index;
		unsigned int  loc_VectorSize;

		//***************************************
		// Prepare support variables:           *
		//***************************************
		if ( fld_XVector.empty() == false )
		{
			fld_XVector.clear();
		}

		loc_VectorSize = fld_YVector.size();
		if ( loc_VectorSize == 0 )
		{
			return;
		}

		loc_IncrementValue = (tpe_data_type)1;

		//***************************************
		// Initialise x-values vector:          *
		//***************************************
		loc_CurrentValue = (tpe_data_type)0.0;
		for ( loc_index = 0; loc_index < loc_VectorSize; loc_index++, loc_CurrentValue+=loc_IncrementValue )
		{
			fld_XVector.push_back( (tpe_data_type)loc_CurrentValue );
		}
	} // GraphWidget::InitialiseXVector
}; // namespace Useless
