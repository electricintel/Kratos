//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Jordi Cotela
//

#if !defined(KRATOS_QSVMSDEMCOUPLED_DATA_H)
#define KRATOS_QSVMSDEMCOUPLED_DATA_H

#include "../FluidDynamicsApplication/fluid_dynamics_application_variables.h"
#include "../FluidDynamicsApplication/custom_utilities/fluid_element_data.h"
#include "../FluidDynamicsApplication/custom_utilities/qsvms_data.h"
#include "utilities/element_size_calculator.h"
#include "includes/checks.h"

namespace Kratos {

///@addtogroup SwimmingDEMApplication
///@{

///@name Kratos classes
///@{

template< size_t TDim, size_t TNumNodes, bool TElementIntegratesInTime = false>
class KRATOS_API(SWIMMING_DEM_APPLICATION) QSVMSDEMCoupledData : public QSVMSData<TDim, TNumNodes, TElementIntegratesInTime>
{
public:

///@name Type Definitions
///@{

using NodalScalarData = typename FluidElementData<TDim,TNumNodes, false>::NodalScalarData;
using NodalVectorData = typename FluidElementData<TDim,TNumNodes, false>::NodalVectorData;

///@}
///@name Public Members
///@{

NodalScalarData FluidFraction;
NodalScalarData FluidFractionRate;

NodalVectorData FluidFractionGradient;
NodalVectorData Acceleration;
NodalVectorData BodyForce;

double ElementSize;

///@}
///@name Public Operations
///@{

void Initialize(const Element& rElement, const ProcessInfo& rProcessInfo) override
{
    // Base class Initialize manages constitutive law parameters
    QSVMSData<TDim, TNumNodes, TElementIntegratesInTime>::Initialize(rElement,rProcessInfo);
    const Geometry< Node<3> >& r_geometry = rElement.GetGeometry();
    this->FillFromNodalData(FluidFraction, FLUID_FRACTION, r_geometry);
    this->FillFromNodalData(FluidFractionRate, FLUID_FRACTION_RATE, r_geometry);
    this->FillFromNodalData(FluidFractionGradient, FLUID_FRACTION_GRADIENT, r_geometry);
    this->FillFromNodalData(Acceleration, ACCELERATION, r_geometry);
    this->FillFromNodalData(BodyForce,BODY_FORCE,r_geometry);

    ElementSize = ElementSizeCalculator<TDim,TNumNodes>::MinimumElementSize(r_geometry);
}

static int Check(const Element& rElement, const ProcessInfo& rProcessInfo)
{


    const Geometry< Node<3> >& r_geometry = rElement.GetGeometry();
    for (unsigned int i = 0; i < TNumNodes; i++)
    {
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(FLUID_FRACTION, r_geometry[i]);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(FLUID_FRACTION_RATE, r_geometry[i]);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(FLUID_FRACTION_GRADIENT, r_geometry[i]);
        KRATOS_CHECK_VARIABLE_IN_NODAL_DATA(BODY_FORCE,r_geometry[i]);
    }
    QSVMSData<TDim, TNumNodes, TElementIntegratesInTime>::Check(rElement, rProcessInfo);

    return 0;
}

///@}

};

///@}

///@}

}

#endif