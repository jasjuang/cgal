// Copyright (c) 2015 INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
//
// Author(s)     : Sven Oesau, Yannick Verdie, Clément Jamin, Pierre Alliez
//

#ifndef CGAL_SHAPE_DETECTION_3_PLANE_SHAPE_H
#define CGAL_SHAPE_DETECTION_3_PLANE_SHAPE_H

#include "Shape_base.h"

/*!
 \file Plane_shape.h
 */
namespace CGAL {
    /*!
     \ingroup PkgPointSetShapeDetection3
     \brief Plane_shape implements Shape_base. The plane is represented by the normal vector and the distance to the origin.
     */
  template <class Sd_traits>
  class Plane_shape : public Shape_base<Sd_traits> {
  public:
    /// \cond SKIP_IN_MANUAL
    typedef typename Sd_traits::Input_iterator Input_iterator;
     ///< random access iterator for input data.
    typedef typename Sd_traits::Point_pmap Point_pmap;
     ///< property map to access the location of an input point.
    typedef typename Sd_traits::Normal_pmap Normal_pmap;
     ///< property map to access the unoriented normal of an input point.
    typedef typename Sd_traits::Geom_traits::FT FT; ///< number type.
    typedef typename Sd_traits::Geom_traits::Point_3 Point; ///< point type.
    typedef typename Sd_traits::Geom_traits::Vector_3 Vector;
    /// \endcond

    typedef typename Sd_traits::Geom_traits::Plane_3 Plane;///< plane type for conversion operator.

  public:
    Plane_shape() : Shape_base<Sd_traits>() {}

    /*!
      Conversion operator to Plane_3 type.
     */
    operator Plane() const {
      return Plane(m_normal.x(), m_normal.y(), m_normal.z(), m_d);
    }
            
    /*!
      Normal vector of the plane.
     */
    Vector plane_normal() const {
      return m_normal;
    }
    
    /// \cond SKIP_IN_MANUAL
    /*!
      Computes squared Euclidean distance from query point to the shape.
     */
    FT squared_distance(const Point &_p) const {
      FT d = (_p - m_point_on_primitive) * m_normal;
      return d * d;
    }

    
    /*!
      Helper function to write the plane equation and
      number of assigned points into a string.
     */
    std::string info() const {
      std::stringstream sstr;
      sstr << "Type: plane (" << m_normal.x() << ", " << m_normal.y() 
        << ", " << m_normal.z() << ")x - " << m_d << "= 0"
        << " #Pts: " << this->m_indices.size();

      return sstr.str();
    }
    /// \endcond

  protected:
      /// \cond SKIP_IN_MANUAL
    virtual void create_shape(const std::vector<std::size_t> &indices) {
      Point p1 = this->point(indices[0]);
      Point p2 = this->point(indices[1]);
      Point p3 = this->point(indices[2]);

      m_normal = CGAL::cross_product(p1 - p2, p1 - p3);

      m_normal = m_normal * (1.0 / sqrt(m_normal.squared_length()));
      m_d = -(p1[0] * m_normal[0] + p1[1] * m_normal[1] + p1[2] * m_normal[2]);

      //check deviation of the 3 normal
      Vector l_v;
      for (std::size_t i = 0;i<3;i++) {
        l_v = this->normal(indices[i]);

        if (abs(l_v * m_normal)
            < this->m_normal_threshold * sqrt(l_v.squared_length())) {
          this->m_isValid = false;
          return;
        }

        m_point_on_primitive = p1;
        m_base1 = CGAL::cross_product(p1 - p2, m_normal);
        m_base1 = m_base1 * (1.0 / sqrt(m_base1.squared_length()));

        m_base2 = CGAL::cross_product(m_base1, m_normal);
        m_base2 = m_base2 * (1.0 / sqrt(m_base2.squared_length()));
      }
    }

    virtual void parameters(const std::vector<std::size_t>& indices,
                            std::vector<std::pair<FT, FT> >& parameterSpace,
                            FT min[2],
                            FT max[2]) const {
      // Transform first point before to initialize min/max
      Vector p = (this->point(indices[0]) - m_point_on_primitive);
      FT u = p * m_base1;
      FT v = p * m_base2;
      parameterSpace[0] = std::pair<FT, FT>(u, v);
      min[0] = max[0] = u;
      min[1] = max[1] = v;

      for (std::size_t i = 1;i<indices.size();i++) {
        p = (this->point(indices[i]) - m_point_on_primitive);
        u = p * m_base1;
        v = p * m_base2;
        min[0] = (std::min<FT>)(min[0], u);
        max[0] = (std::max<FT>)(max[0], u);
        min[1] = (std::min<FT>)(min[1], v);
        max[1] = (std::max<FT>)(max[1], v);
        parameterSpace[i] = std::pair<FT, FT>(u, v);
      }
    }
    
    virtual void squared_distance(const std::vector<std::size_t> &indices,
                                  std::vector<FT> &dists) {
      for (std::size_t i = 0;i<indices.size();i++) {
        const FT d = (this->point(indices[i]) - m_point_on_primitive) * m_normal;
        dists[i] = d * d;
      }
    }

    virtual void cos_to_normal(const std::vector<std::size_t> &indices, 
                               std::vector<FT> &angles) const {
      for (std::size_t i = 0;i<indices.size();i++) {
        angles[i] = abs(this->normal(indices[i]) * m_normal);
      }
    }

    FT cos_to_normal(const Point &_p, const Vector &_n) const{
      return abs(_n * m_normal);
    } 
    
    virtual std::size_t required_samples() const {
      return 3;
    }

    virtual bool supports_connected_component() const {
      return true;
    }

    virtual bool wraps_u() const {
      return false;
    }

    virtual bool wraps_v() const {
      return false;
    }

  private:
    Point m_point_on_primitive;
    Vector m_base1, m_base2, m_normal;
    FT m_d;
    /// \endcond
  };
}
#endif
