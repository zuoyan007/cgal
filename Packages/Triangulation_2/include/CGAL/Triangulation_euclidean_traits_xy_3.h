// ============================================================================
//
// Copyright (c) 1997 The CGAL Consortium
//
// This software and related documentation is part of an INTERNAL release
// of the Computational Geometry Algorithms Library (CGAL). It is not
// intended for general use.
//
// ----------------------------------------------------------------------------
//
// release       :
// release_date  :
//
// file          : include/CGAL/Triangulation_euclidean_traits_xy_3.h
// source        : web/Triangulation_euclidean_traits_xy_3.fw
// revision      : $Revision$
// revision_date : $Date$
// author(s)     : Herve Bronnimann and Andreas Fabri
//
// coordinator   : Herve Bronnimann  <Herve.Bronnimann@sophia.inria.fr>
//
// ============================================================================


#ifndef CGAL_TRIANGULATION_EUCLIDEAN_TRAITS_XY_3_H
#define CGAL_TRIANGULATION_EUCLIDEAN_TRAITS_XY_3_H

#include <CGAL/triangulation_assertions.h>
#include <CGAL/Distance_2.h>
#include <CGAL/Triangulation_vertex.h>
#include <CGAL/Triangulation_face.h>
#include <CGAL/Triangulation_face_circulator.h>
#include <CGAL/Triangulation_edge_circulator.h>
#include <CGAL/Triangulation_vertex_circulator.h>

#include <CGAL/Point_3.h>
#include <CGAL/Segment_3.h>
#include <CGAL/Triangle_3.h>
#include <CGAL/predicates_on_ftC2.h>

template < class R >
class CGAL_Triangulation_euclidean_traits_xy_3 {
public:
    typedef CGAL_Triangulation_euclidean_traits_xy_3<R> Traits;
    typedef R Rep;
    typedef CGAL_Point_3<R>  Point;
    typedef CGAL_Segment_3<R> Segment;
    typedef CGAL_Triangle_3<R> Triangle;
    typedef CGAL_Line_3<R>   Line;
    typedef CGAL_Ray_3<R>    Ray;
    typedef CGAL_Direction_3<R> Direction;
    
    typedef CGAL_Triangulation_vertex<Point> Vertex;
    typedef CGAL_Triangulation_face<Vertex> Face;
    typedef typename Vertex::Vertex_handle Vertex_handle;
    typedef typename Face::Face_handle Face_handle;
    
      static typename Rep::FT x(const Point &p) { return p.x(); }
      static typename Rep::FT y(const Point &p) { return p.y(); }
    
    CGAL_Comparison_result compare_x(const Point &p, const Point &q) const
      {
        return CGAL_compare(x(p), x(q));
      }
    CGAL_Comparison_result compare_y(const Point &p, const Point &q) const
      {
        return CGAL_compare(y(p), y(q));
      }
    bool compare(const Point &p, const Point &q) const
      {
        return (x(p)==x(q)) &&  (y(p)==y(q));
      }
    
    CGAL_Orientation orientation(const Point &p,
                                 const Point &q,
                                 const Point &r) const
      {
        return CGAL_orientationC2(x(p), y(p), x(q), y(q), x(r), y(r));
      }
    
    
    CGAL_Orientation extremal(const Point &p,
                              const Point &q,
                              const Point &r) const
      {
        if (p==q) return CGAL_COLLINEAR;
        if (p==r) return CGAL_COLLINEAR;
        if (r==q) return CGAL_COLLINEAR;
    
        return CGAL_orientationC2(x(p), y(p), x(q), y(q), x(r), y(r));
      }
    
    CGAL_Oriented_side side_of_oriented_circle(const Point &p,
                                               const Point &q,
                                               const Point &r,
                                               const Point &s) const
      {
        if (p==s) return CGAL_ON_ORIENTED_BOUNDARY;
        if (q==s) return CGAL_ON_ORIENTED_BOUNDARY;
        if (r==s) return CGAL_ON_ORIENTED_BOUNDARY;
    
        return CGAL_side_of_oriented_circleC2(x(p), y(p),
                                              x(q), y(q),
                                              x(r), y(r),
                                              x(s), y(s));
      }
    
    
    class Distance : public CGAL_Distance_2<Traits> {
    public:
        Distance(const Point& p0,
                 const Traits* traits = NULL)
            : CGAL_Distance_2<Traits>(p0, traits) { }
    
    
        Distance(const Point& p0,
                 const Point& p1,
                 const Traits* traits = NULL)
            : CGAL_Distance_2<Traits>(p0,p1,traits) { }
    
        Distance(const Point& p0,
                 const Point& p1,
                 const Point& p2,
                 const Traits* traits = NULL)
            : CGAL_Distance_2<Traits>(p0,p1,p2,traits) { }
    
        CGAL_Comparison_result
        compare() const
        {
          Point p0 = get_point(0);
          Point p1 = get_point(1);
          Point p2 = get_point(2);
          return CGAL_cmp_dist_to_pointC2(x(p0),y(p0),x(p1),y(p1),x(p2),y(p2));
        }
    };
    
};


#endif // CGAL_TRIANGULATION_EUCLIDEAN_TRAITS_XY_3_H
