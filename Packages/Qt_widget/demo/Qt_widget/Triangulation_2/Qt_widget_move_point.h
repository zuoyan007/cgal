// ============================================================================
//
// Copyright (c) 1997-2000 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//	
// ----------------------------------------------------------------------------
//
// file          : include/CGAL/IO/Qt_widget_MovePoint.h
// package       : QT_widget
// author(s)     : Radu Ursu
// release       : 
// release_date  : 
//
// coordinator   : Laurent Rineau <rineau@clipper.ens.fr>
//
// ============================================================================


#ifndef CGAL_QT_WIDGET_MOVEPOINT_H
#define CGAL_QT_WIDGET_MOVEPOINT_H


#include <CGAL/IO/Qt_widget.h>
#include <CGAL/IO/Qt_widget_layer.h>


#include <qobject.h>
#include <qpopupmenu.h>
#include <qcursor.h>



class Qt_widget_movepoint_helper : public CGAL::Qt_widget_layer
{
  Q_OBJECT
  public:
  virtual void delete_vertexi(){};
  virtual void move_vertexi(){};
  virtual void change_weighti(){};


public slots:
  void stateChanged(int i);
  void delete_vertex();
  void move_vertex();
  void change_weight();
};

template <class T>
class Qt_widget_movepoint : public Qt_widget_movepoint_helper
{
public:

  typedef typename T::Point			      Point;
  typedef typename T::Segment			    Segment;
  typedef typename T::Face_handle		  Face_handle;
  typedef typename T::Vertex_handle		Vertex_handle;
  typedef typename T::Geom_traits::FT	FT;
protected:
  FT						first_x, first_y;
  FT						x2, y2;
  bool					wasrepainted;
  bool					on_first;

  Vertex_handle			current_v;	//the vertex that will be process
  Point					    old_point;
  T						      *dt;
  QPopupMenu				*popup;
public:
  Qt_widget_movepoint() : wasrepainted(true), on_first(FALSE)
  {};
  void set_Delaunay (T *t) {dt = t;}
private:
  QCursor oldcursor;

  void draw(){
    wasrepainted = true;
  };
  void mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton && on_first)
    {
      on_first = FALSE;
    }
    if(e->button() == Qt::RightButton)
    {
	    if (dt->dimension()<2) return;
	    FT x, y;
      widget->x_real(e->x(), x);
      widget->y_real(e->y(), y);
      Point p(x, y);
      Vertex_handle v = dt->nearest_vertex(p);
      RasterOp old = widget->rasterOp();	//save the initial raster mode
      widget->setRasterOp(XorROP);
      widget->lock();
          *widget << CGAL::GREEN << CGAL::PointSize (7) 
              << CGAL::PointStyle (CGAL::DISC);
      if(!wasrepainted)
        *widget << old_point;
      *widget << v->point();
      widget->unlock();
      widget->setRasterOp(old);
      popup->popup(widget->mapToGlobal(e->pos()));
      old_point = v->point();
      current_v = v;
      wasrepainted = FALSE;
      on_first = FALSE;
    }	
  };
  void mouseMoveEvent(QMouseEvent *e)
  {
  if(on_first)
  {
    FT x, y;
    widget->x_real(e->x(), x);
    widget->y_real(e->y(), y);
		
    *widget << CGAL::GREEN << CGAL::PointSize (5)
            << CGAL::PointStyle (CGAL::DISC);
    if(!wasrepainted)
      *widget << old_point;
    *widget << Point(x, y);
    dt->remove(current_v);
    current_v = dt->insert(Point(x, y));
    widget->redraw();	//redraw the scenes
    old_point = Point(x, y);
  }
};
  void activating()
  {
    oldcursor = widget->cursor();
    widget->setCursor(crossCursor);
    wasrepainted = TRUE;
    popup = new QPopupMenu( widget, 0);
    popup->insertItem("Delete Point", this, SLOT(delete_vertex()));
    popup->insertItem("Move Point", this,  SLOT(move_vertex()));
  };

  void deactivating()
  {
    widget->setCursor(oldcursor);
  };
  void delete_vertexi(){
    dt->remove(current_v);
    widget->redraw();	//redraw the scenes
  };
  void move_vertexi(){
    on_first = TRUE;
  };
};//end class 


template <class T>
class Qt_widget_move_weightedpoint : public Qt_widget_movepoint_helper
{
public:

  typedef typename T::Point			      Point;
  typedef typename T::Segment			    Segment;
  typedef typename T::Face_handle		  Face_handle;
  typedef typename T::Vertex_handle		Vertex_handle;
  typedef typename T::Geom_traits     GT;
  typedef typename CGAL::Kernel_traits<Point>::Kernel::FT FT;
protected:
  FT						    first_x, first_y;
  FT						    x2, y2;
  bool					    wasrepainted;
  bool					    on_first; //true if right mouse button was pressed
  bool              move_button_pressed; //true if the popup's move button 
                                         //was pressed
  bool              change_weight_pressed; //true if the popup's 
                                           //change_weight button was pressed

  Vertex_handle			current_v;	//the vertex that will be processed
  Point					    old_point;  //contains the old vertex that should be removed
  T						      *t;         //pointer to regular triangulation being used
  QPopupMenu				*popup;     //the popup being displayed when right mouse 
                                //button is pressed
public:
  Qt_widget_move_weightedpoint() : wasrepainted(true), on_first(false)
            , move_button_pressed(false), change_weight_pressed(false) {};
  void set_triangulation (T *tr) {t = tr;}
  
  template < class TRIANGULATION > 
  Vertex_handle
  closest_vertex(const TRIANGULATION &T,
			      Face_handle f,
			      const Point& p)
  {
    Vertex_handle v ;
    typename Gt::Compare_distance_2 cmp =
      T.geom_traits().compare_distance_2_object();

    if( T.is_infinite(f)){
      int i = f->index(T.infinite_vertex());
      Point pcwi = f->vertex(f->cw(i))->point();
      Point pccwi = f->vertex(f->ccw(i))->point();
      v =  cmp(p, pcwi, pccwi) == CGAL::SMALLER ? f->vertex(f->cw(i)) :
                                                  f->vertex(f->ccw(i));
    }
    else{ 
      v = f->vertex(0);
      if (cmp(p, f->vertex(1)->point(), v->point()) == CGAL::SMALLER) 
        v = f->vertex(1);
      if (cmp(p, f->vertex(2)->point(), v->point()) == CGAL::SMALLER) 
        v = f->vertex(2);
    }
    return v;
  } 
private:
  QCursor oldcursor;

  void 
  draw(){
    wasrepainted = true;
  }

  void 
  mousePressEvent(QMouseEvent *e)
  {
    if(e->button() == Qt::LeftButton && on_first)
    {
      on_first = FALSE;
    }
    if(e->button() == Qt::RightButton)
    {
	    if (t->dimension()<2) return;
	    FT x, y;
      widget->x_real(e->x(), x);
      widget->y_real(e->y(), y);

	    Point p(x, y);
      Face_handle f = t->locate(p);
	    Vertex_handle v = closest_vertex(*t, f, p);
	    RasterOp old = widget->rasterOp();	//save the initial raster mode
	    widget->setRasterOp(XorROP);
	    widget->lock();
        *widget << CGAL::GREEN << CGAL::PointSize (7) 
              << CGAL::PointStyle (CGAL::DISC);
        if(!wasrepainted)
          *widget << old_point;
        *widget << v->point();
      widget->unlock();
      widget->setRasterOp(old);
      popup->popup(widget->mapToGlobal(e->pos()));
      old_point = v->point();
      current_v = v;
      wasrepainted = FALSE;
      on_first = FALSE;
    }	
  }
  
  void 
  mouseMoveEvent(QMouseEvent *e)
  {
    if(on_first)
    {
      if(move_button_pressed){
        FT x, y;
        widget->x_real(e->x(), x);
        widget->y_real(e->y(), y);
    		
        *widget << CGAL::GREEN << CGAL::PointSize (5)
                << CGAL::PointStyle (CGAL::DISC);
        if(!wasrepainted)
          *widget << old_point;
        *widget << Point(x, y);
        double wght = current_v->point().weight();
        t->remove(current_v);
        current_v = t->insert(Gt::Weighted_point(Point(x, y), wght));
        widget->redraw();	//redraw the scenes
        old_point = Point(x, y);
      } else if(change_weight_pressed){
        FT x, y;
        widget->x_real(e->x(), x);
        widget->y_real(e->y(), y);
        double wght = current_v->point().weight();
        t->remove(current_v);
        current_v = t->insert(Gt::Weighted_point(Point(x, y), wght));
        widget->redraw();	//redraw the scenes
        old_point = Point(x, y);
      }
    }
  }

  void 
  activating()
  {
    oldcursor = widget->cursor();
    widget->setCursor(crossCursor);
    wasrepainted = TRUE;
    popup = new QPopupMenu( widget, 0);
    popup->insertItem("Delete Vertex", this, SLOT(delete_vertex()));
    popup->insertItem("Move Vertex", this,  SLOT(move_vertex()));
    popup->insertItem("Change Weight", this, SLOT(change_weight()));
  }

  void 
  deactivating()
  {
    widget->setCursor(oldcursor);
  }
  
  void 
  delete_vertexi(){
    t->remove(current_v);
    widget->redraw();	//redraw the scenes
  }
  
  void 
  move_vertexi(){
    on_first = true;
    move_button_pressed = true;
    widget->cursor().setPos(widget->mapToGlobal(QPoint(old_point.x(), old_point.y())));
  }
  
  void 
  change_weighti(){
    on_first = true;
    change_weight_pressed = true;
  }
};//end class 



#endif // CGAL_QT_WIDGET_GET_SEGMENT_H

