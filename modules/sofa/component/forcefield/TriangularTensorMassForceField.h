/*******************************************************************************
*       SOFA, Simulation Open-Framework Architecture, version 1.0 beta 1       *
*                (c) 2006-2007 MGH, INRIA, USTL, UJF, CNRS                     *
*                                                                              *
* This library is free software; you can redistribute it and/or modify it      *
* under the terms of the GNU Lesser General Public License as published by the *
* Free Software Foundation; either version 2.1 of the License, or (at your     *
* option) any later version.                                                   *
*                                                                              *
* This library is distributed in the hope that it will be useful, but WITHOUT  *
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or        *
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License  *
* for more details.                                                            *
*                                                                              *
* You should have received a copy of the GNU Lesser General Public License     *
* along with this library; if not, write to the Free Software Foundation,      *
* Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA.           *
*                                                                              *
* Contact information: contact@sofa-framework.org                              *
*                                                                              *
* Authors: J. Allard, P-J. Bensoussan, S. Cotin, C. Duriez, H. Delingette,     *
* F. Faure, S. Fonteneau, L. Heigeas, C. Mendoza, M. Nesme, P. Neumann,        *
* and F. Poyer                                                                 *
*******************************************************************************/
#ifndef SOFA_COMPONENT_FORCEFIELD_TRIANGULARTENSORMASSFORCEFIELD_H
#define SOFA_COMPONENT_FORCEFIELD_TRIANGULARTENSORMASSFORCEFIELD_H

#if !defined(__GNUC__) || (__GNUC__ > 3 || (_GNUC__ == 3 && __GNUC_MINOR__ > 3))
#pragma once
#endif

#include <sofa/core/componentmodel/behavior/ForceField.h>
#include <sofa/component/MechanicalObject.h>
#include <sofa/core/VisualModel.h>
#include <sofa/component/topology/MeshTopology.h>
#include <sofa/defaulttype/Vec.h>
#include <sofa/defaulttype/Mat.h>
#include <sofa/component/topology/TriangleData.h>
#include <sofa/component/topology/EdgeData.h>


namespace sofa
{

namespace component
{


namespace forcefield
{

using namespace sofa::defaulttype;
using namespace sofa::component::topology;


template<class DataTypes>
class TriangularTensorMassForceField : public core::componentmodel::behavior::ForceField<DataTypes>, public core::VisualModel
{
public:
    typedef core::componentmodel::behavior::ForceField<DataTypes> Inherited;
    typedef typename DataTypes::VecCoord VecCoord;
    typedef typename DataTypes::VecDeriv VecDeriv;
    typedef typename DataTypes::Coord    Coord   ;
    typedef typename DataTypes::Deriv    Deriv   ;
    typedef typename Coord::value_type   Real    ;


    class Mat3 : public fixed_array<Deriv,3>
    {
    public:
        Deriv operator*(const Deriv& v)
        {
            return Deriv((*this)[0]*v,(*this)[1]*v,(*this)[2]*v);
        }
        Deriv transposeMultiply(const Deriv& v)
        {
            return Deriv(v[0]*((*this)[0])[0]+v[1]*((*this)[1])[0]+v[2]*((*this)[2][0]),
                    v[0]*((*this)[0][1])+v[1]*((*this)[1][1])+v[2]*((*this)[2][1]),
                    v[0]*((*this)[0][2])+v[1]*((*this)[1][2])+v[2]*((*this)[2][2]));
        }
    };

protected:


    class EdgeRestInformation
    {
    public:
        Mat3 DfDx; /// the edge stiffness matrix

        EdgeRestInformation()
        {
        }
    };

    EdgeData<EdgeRestInformation> edgeInfo;

    TriangleSetTopology<DataTypes> * _mesh;
    VecCoord _initialPoints;///< the intial positions of the points

    bool updateMatrix;

    DataField<Real> f_poissonRatio;
    DataField<Real> f_youngModulus;
    DataField<Real> f_dampingRatio;

    Real lambda;  /// first Lame coefficient
    Real mu;    /// second Lame coefficient
public:

    TriangularTensorMassForceField();

    TriangleSetTopology<DataTypes> *getTriangularTopology() const {return _mesh;}

    virtual ~TriangularTensorMassForceField();

    virtual double getPotentialEnergy(const VecCoord& x);

    virtual void init();
    virtual void addForce(VecDeriv& f, const VecCoord& x, const VecDeriv& v);
    virtual void addDForce(VecDeriv& df, const VecDeriv& dx);

    virtual Real getLambda() const { return lambda;}
    virtual Real getMu() const { return mu;}

    // handle topological changes
    virtual void handleTopologyChange();

    // -- VisualModel interface
    void draw();
    void initTextures() { };
    void update() { };
    /// compute lambda and mu based on the Young modulus and Poisson ratio
    void updateLameCoefficients();



protected :

    EdgeData<EdgeRestInformation> &getEdgeInfo() {return edgeInfo;}

    static void TriangularTMEdgeCreationFunction(int edgeIndex, void* param,
            EdgeRestInformation &ei,
            const Edge& ,  const std::vector< unsigned int > &,
            const std::vector< double >&);

    static void TriangularTMTriangleCreationFunction(const std::vector<unsigned int> &triangleAdded,
            void* param, vector<EdgeRestInformation> &edgeData);
    static void TriangularTMTriangleDestructionFunction ( const std::vector<unsigned int> &triangleAdded,
            void* param, vector<EdgeRestInformation> &edgeData);

};
} //namespace forcefield

} // namespace Components


} // namespace Sofa



#endif /* _TriangularTensorMassForceField_H_ */
