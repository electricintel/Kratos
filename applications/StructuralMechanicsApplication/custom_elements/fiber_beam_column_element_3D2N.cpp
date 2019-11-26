// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:     BSD License
//           license: structural_mechanics_application/license.txt
//
//  Main authors: Mahmoud Zidan
//

// System includes
#include<iostream>

// External includes

// Project includes
#include "custom_elements/fiber_beam_column_element_3D2N.hpp"
#include "includes/define.h"
#include "structural_mechanics_application_variables.h"
#include "custom_utilities/structural_mechanics_element_utilities.h"
#include "includes/checks.h"


#include "custom_constitutive/uniaxial_fiber_beam_column_steel_material_law.hpp"
#include "custom_constitutive/uniaxial_fiber_beam_column_concrete_material_law.hpp"
#include "custom_constitutive/uniaxial_fiber_beam_column_concrete_material_law2.hpp"

namespace Kratos {

///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

///@}
///@name  Enum's
///@{

///@}
///@name  Functions
///@{

///@}
///@name Life Cycle
///@{

/// Constructor
FiberBeamColumnElement3D2N::FiberBeamColumnElement3D2N(IndexType NewId)
    : Element(NewId) {}

/// Constructor using an array of nodes
FiberBeamColumnElement3D2N::FiberBeamColumnElement3D2N(IndexType NewId, const NodesArrayType& rThisNodes)
    : Element(NewId, rThisNodes) {}

/// Constructor using Geometry
FiberBeamColumnElement3D2N::FiberBeamColumnElement3D2N(IndexType NewId, GeometryType::Pointer pGeometry)
    : Element(NewId, pGeometry) {}

/// Constructor using Properties
FiberBeamColumnElement3D2N::FiberBeamColumnElement3D2N(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties)
    : Element(NewId, pGeometry, pProperties) {}

/// Copy Constructor
FiberBeamColumnElement3D2N::FiberBeamColumnElement3D2N(FiberBeamColumnElement3D2N const& rOther)
    : Element(rOther) {}

/// Destructor
FiberBeamColumnElement3D2N::~FiberBeamColumnElement3D2N() {}

///@}
///@name Operators
///@{

/// Assigment Operator
FiberBeamColumnElement3D2N & FiberBeamColumnElement3D2N::operator=(FiberBeamColumnElement3D2N const& rOther)
{
    BaseType::operator=(rOther);
    Flags::operator=(rOther);
    return *this;
}

///@}
///@name Operations
///@{

Element::Pointer FiberBeamColumnElement3D2N::Create(
    IndexType NewId,
    NodesArrayType const& rThisNodes,
    PropertiesType::Pointer pProperties) const
{
    KRATOS_TRY
    return Kratos::make_intrusive<FiberBeamColumnElement3D2N>(NewId, GetGeometry().Create(rThisNodes), pProperties);
    KRATOS_CATCH("")
}

Element::Pointer FiberBeamColumnElement3D2N::Create(
    IndexType NewId,
    GeometryType::Pointer pGeom,
    PropertiesType::Pointer pProperties) const
{
    KRATOS_TRY
    return Kratos::make_intrusive<FiberBeamColumnElement3D2N>(NewId, pGeom, pProperties);
    KRATOS_CATCH("")
}

Element::Pointer FiberBeamColumnElement3D2N::Clone(
    IndexType NewId,
    NodesArrayType const& rThisNodes) const
{
    KRATOS_TRY
    return Kratos::make_intrusive<FiberBeamColumnElement3D2N>(NewId, GetGeometry().Create(rThisNodes), pGetProperties());
    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::EquationIdVector(
    EquationIdVectorType& rResult,
    ProcessInfo& rCurrentProcessInfo)
{

    if (rResult.size() != msGlobalSize) {
        rResult.resize(msGlobalSize, false);
    }

    const unsigned int global_size_per_node = 6;
    for (int i = 0; i < msNumberOfNodes; ++i) {
        int index = i * global_size_per_node;
        rResult[index] = GetGeometry()[i].GetDof(DISPLACEMENT_X).EquationId();
        rResult[index + 1] = GetGeometry()[i].GetDof(DISPLACEMENT_Y).EquationId();
        rResult[index + 2] = GetGeometry()[i].GetDof(DISPLACEMENT_Z).EquationId();
        rResult[index + 3] = GetGeometry()[i].GetDof(ROTATION_X).EquationId();
        rResult[index + 4] = GetGeometry()[i].GetDof(ROTATION_Y).EquationId();
        rResult[index + 5] = GetGeometry()[i].GetDof(ROTATION_Z).EquationId();
    }
}

void FiberBeamColumnElement3D2N::GetDofList(
    DofsVectorType& rElementalDofList,
    ProcessInfo& rCurrentProcessInfo)
{

    if (rElementalDofList.size() != msGlobalSize) {
        rElementalDofList.resize(msGlobalSize);
    }

    const unsigned int global_size_per_node = 6;
    for (int i = 0; i < msNumberOfNodes; ++i) {
        int index = i * global_size_per_node;
        rElementalDofList[index] = GetGeometry()[i].pGetDof(DISPLACEMENT_X);
        rElementalDofList[index + 1] = GetGeometry()[i].pGetDof(DISPLACEMENT_Y);
        rElementalDofList[index + 2] = GetGeometry()[i].pGetDof(DISPLACEMENT_Z);
        rElementalDofList[index + 3] = GetGeometry()[i].pGetDof(ROTATION_X);
        rElementalDofList[index + 4] = GetGeometry()[i].pGetDof(ROTATION_Y);
        rElementalDofList[index + 5] = GetGeometry()[i].pGetDof(ROTATION_Z);
    }
}

void FiberBeamColumnElement3D2N::Initialize()
{
    KRATOS_TRY

    // reserve memory without using a default constructor
    const unsigned int number_of_sections = GetProperties()[NUMBER_OF_SECTIONS];
    mSections.reserve(number_of_sections);

    const GeometryData::IntegrationPointsArrayType integration_points = GetGeometry().IntegrationPoints(GetIntegrationMethod());

    for (unsigned int i = 0; i < number_of_sections; ++i) {
        mSections.push_back( FiberBeamColumnSection( i+1, integration_points[i], pGetProperties() ));
    }

    Matrix concrete_fibers_data = GetProperties()[CONCRETE_FIBERS_DATA];
    Matrix steel_fibers_data = GetProperties()[STEEL_FIBERS_DATA];

    for (unsigned int i = 0; i < number_of_sections; ++i) {

        std::vector<FiberBeamColumnUniaxialFiber> fibers;
        fibers.reserve(concrete_fibers_data.size1() + steel_fibers_data.size1());

        for (unsigned int k = 0; k < concrete_fibers_data.size1(); ++k) {
            auto p_material = Kratos::make_shared<UniaxialFiberBeamColumnConcreteMaterialLaw>(pGetProperties());
            p_material->Confine();
            fibers.push_back( FiberBeamColumnUniaxialFiber( 0,
                concrete_fibers_data(k, 0),
                concrete_fibers_data(k, 1),
                concrete_fibers_data(k, 2),
                p_material) );
        }

        for (unsigned int k = 0; k < steel_fibers_data.size1(); ++k) {
            auto p_material = Kratos::make_shared<UniaxialFiberBeamColumnSteelMaterialLaw>(pGetProperties());
            fibers.push_back( FiberBeamColumnUniaxialFiber( 0,
                steel_fibers_data(k, 0),
                steel_fibers_data(k, 1),
                steel_fibers_data(k, 2),
                p_material) );
        }

        mSections[i].SetFibers(fibers);
        mSections[i].Initialize();
    }

    CalculateTransformationMatrix();
    CalculateElementLocalStiffnessMatrix();

    KRATOS_CATCH("")
}

GeometryData::IntegrationMethod FiberBeamColumnElement3D2N::GetIntegrationMethod() const
{
    const IndexType number_of_sections = GetProperties()[NUMBER_OF_SECTIONS];
    switch (number_of_sections){
        case 2: return GeometryData::GI_GAUSS_2;
        case 3: return GeometryData::GI_GAUSS_3;
        case 4: return GeometryData::GI_GAUSS_4;
        // case 5: return GeometryData::GI_GAUSS_5;
        // case 6: return GeometryData::GI_EXTENDED_GAUSS_1;
        // case 7: return GeometryData::GI_EXTENDED_GAUSS_2;
        // case 8: return GeometryData::GI_EXTENDED_GAUSS_3;
        // case 9: return GeometryData::GI_EXTENDED_GAUSS_4;
        // case 10:return GeometryData::GI_EXTENDED_GAUSS_5;
        default:
        {
            KRATOS_ERROR << "NUMBER_OF_SECTIONS can only be in range [2, 10]."
            << "The given NUMBER_OF_SECTIONS is: " << number_of_sections << std::endl;
            break;
        }
    }
}

void FiberBeamColumnElement3D2N::FinalizeNonLinearIteration(ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY

    mDeformationPreviousIteration = mDeformationCurrentIteration;
    mDeformationPreviousIncerements = mDeformationCurrentIncerements;

    Vector deformation_current_step = ZeroVector(msGlobalSize);
    GetValuesVector(deformation_current_step, rCurrentProcessInfo[STEP]);
    mDeformationCurrentIteration = prod(Matrix(trans(mTransformationMatrix)), deformation_current_step);

    mDeformationCurrentIncerements = mDeformationCurrentIteration - mConvergedDeformations;
    Vector chng_def_incr = mDeformationCurrentIncerements - mDeformationPreviousIncerements;

    // check if change in deformation increments is nearly zero
    // if so, no need to do the element loop
    // unsigned int check = 0;
    // for (unsigned int i = 0; i < msLocalSize; ++i) {
    //     if (std::abs(chng_def_incr[i]) < std::numeric_limits<double>::epsilon()) {
    //         check++;
    //     }
    // }
    // if (check != msLocalSize) {
    // }
    unsigned int max_iterations = 100;
    Vector chng_force_incr = ZeroVector(msLocalSize);
    for (unsigned int j = 1; j <= max_iterations; ++j)
    {
        if (j == 1){
            noalias(chng_force_incr) = prod(mLocalStiffnessMatrix, chng_def_incr);
        } else {
            noalias(chng_force_incr) = -1.0 * prod(mLocalStiffnessMatrix, mDeformationResiduals);
        }

        mForceIncerements += chng_force_incr;
        mForces = mConvergedForces + mForceIncerements;

        bool converged = true;
        for (FiberBeamColumnSection& r_section : mSections){
            converged *= r_section.StateDetermination(chng_force_incr);
        }
        CalculateElementLocalStiffnessMatrix();

        if (converged) {
            KRATOS_INFO("FiberBeamColumnElement3D2N")
            << "Element equilibrium achieved in " << j << " iterations." << std::endl;
            break;
        } else {
            std::fill(mDeformationResiduals.begin(), mDeformationResiduals.end(), 0.0);
            // integrate over sections
            const double reference_length = StructuralMechanicsElementUtilities::CalculateReferenceLength3D2N(*this);
            for (FiberBeamColumnSection& r_section : mSections) {
                mDeformationResiduals += r_section.GetGlobalDeformationResiduals() * reference_length/2.0 * r_section.GetWeight();
            }
        }

        if (j == max_iterations) {
            KRATOS_WARNING("FiberBeamColumnElement3D2N")
            << "Maximum number of iterations in element equilibrum loop reached." << std::endl;
        }
    }

    // CalculateElementLocalStiffnessMatrix();
    for (FiberBeamColumnSection& r_section : mSections) {
        r_section.ResetResidual();
    }

    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::GetValuesVector(Vector& rValues, int Step)
{
    KRATOS_TRY

    const unsigned int global_size_per_node = 6;
    for (int i = 0; i < msNumberOfNodes; ++i)
    {
        int index = i * global_size_per_node;
        const auto& disp = GetGeometry()[i].FastGetSolutionStepValue(DISPLACEMENT);
        const auto& rot = GetGeometry()[i].FastGetSolutionStepValue(ROTATION);
        rValues[index] = disp[0];
        rValues[index + 1] = disp[1];
        rValues[index + 2] = disp[2];
        rValues[index + 3] = rot[0];
        rValues[index + 4] = rot[1];
        rValues[index + 5] = rot[2];
    }

    KRATOS_CATCH("")
}

// void FiberBeamColumnElement3D2N::ElementLoop(ProcessInfo& rCurrentProcessInfo)
// {
//     KRATOS_TRY
//     KRATOS_CATCH("")
// }

// void FiberBeamColumnElement3D2N::CalculateDeformationResiduals()
// {
//     KRATOS_TRY
//     std::fill(mDeformationResiduals.begin(), mDeformationResiduals.end(), 0.0);
//     // integrate over sections
//     const double reference_length = StructuralMechanicsElementUtilities::CalculateReferenceLength3D2N(*this);
//     for (FiberBeamColumnSection& r_section : mSections) {
//         r_section.CalculateResiduals();
//         mDeformationResiduals += r_section.GetGlobalDeformationResiduals() * reference_length/2 * r_section.GetWeight();
//     }
//     KRATOS_CATCH("")
// }

void FiberBeamColumnElement3D2N::FinalizeSolutionStep(ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY

    mConvergedForces = mForces;
    mForceIncerements = ZeroVector(msLocalSize);
    // mDeformationResiduals = ZeroVector(msLocalSize);
    mConvergedDeformations = mDeformationCurrentIteration;
    mDeformationCurrentIncerements = ZeroVector(msLocalSize);

    for (FiberBeamColumnSection& r_section : mSections){
        r_section.FinalizeSolutionStep();
    }

    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::CalculateLocalSystem(
    MatrixType& rLeftHandSideMatrix,
    VectorType& rRightHandSideVector,
    ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY;
    CalculateRightHandSide(rRightHandSideVector,rCurrentProcessInfo);
    CalculateLeftHandSide(rLeftHandSideMatrix,rCurrentProcessInfo);

    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::CalculateLeftHandSide(
    MatrixType& rLeftHandSideMatrix,
    ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY;
    // resizing the matrices + create memory for LHS
    rLeftHandSideMatrix = ZeroMatrix(msGlobalSize, msGlobalSize);
    // creating LHS
    BoundedMatrix<double,msGlobalSize,msLocalSize> aux_matrix = prod(mTransformationMatrix, mLocalStiffnessMatrix);
    noalias(rLeftHandSideMatrix) += prod(aux_matrix, trans(mTransformationMatrix));
    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::CalculateElementLocalStiffnessMatrix()
{
    KRATOS_TRY

    // allocate memory
    Matrix local_flexibility = ZeroMatrix(msLocalSize, msLocalSize);

    // integrate over sections to get flexibility matrix
    const double reference_length = StructuralMechanicsElementUtilities::CalculateReferenceLength3D2N(*this);
    for (FiberBeamColumnSection& r_section : mSections) {
        local_flexibility += r_section.GetGlobalFlexibilityMatrix() * reference_length/2 * r_section.GetWeight();
    }

    // invert to get stiffness matrix
    double det_flexibility = MathUtils<double>::Det(local_flexibility);
    MathUtils<double>::InvertMatrix(local_flexibility, mLocalStiffnessMatrix, det_flexibility);

    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::CalculateRightHandSide(
    VectorType& rRightHandSideVector,
    ProcessInfo& rCurrentProcessInfo)
{
    KRATOS_TRY
    rRightHandSideVector = ZeroVector(msGlobalSize);
    noalias(rRightHandSideVector) -= prod(mTransformationMatrix, mForces);
    KRATOS_CATCH("")
}

void FiberBeamColumnElement3D2N::CalculateTransformationMatrix()
{
    KRATOS_TRY
    Matrix local_cs = CreateInitialLocalCoordSys();
    double L0 = StructuralMechanicsElementUtilities::CalculateReferenceLength3D2N(*this);

    // forces 1st node
    for (int i = 0; i < 3; ++i ) {
        mTransformationMatrix(i, 0) += local_cs(1, i) / L0;
        mTransformationMatrix(i, 1) += local_cs(1, i) / L0;
        mTransformationMatrix(i, 2) -= local_cs(2, i) / L0;
        mTransformationMatrix(i, 3) -= local_cs(2, i) / L0;
        mTransformationMatrix(i, 4) -= local_cs(0, i);
    }

    // moments 1st node
    // mTransformationMatrix(3, 0) += local_cs(2, 0);
    // mTransformationMatrix(4, 0) += local_cs(2, 1);
    // mTransformationMatrix(5, 0) += local_cs(2, 2);
    // mTransformationMatrix(3, 2) += local_cs(1, 0);
    // mTransformationMatrix(4, 2) += local_cs(1, 1);
    // mTransformationMatrix(5, 2) += local_cs(1, 2);
    for ( int i=3, j=0; i<6 && j<3; ++i, ++j ) {
        mTransformationMatrix(i, 0) += local_cs(2, j);
        mTransformationMatrix(i, 2) += local_cs(1, j);
    }

    // forces 2nd node
    for ( int i=6, j=0; i<9 && j<3; ++i, ++j ) {
        mTransformationMatrix(i, 0) -= local_cs(1, j) / L0;
        mTransformationMatrix(i, 1) -= local_cs(1, j) / L0;
        mTransformationMatrix(i, 2) += local_cs(2, j) / L0;
        mTransformationMatrix(i, 3) += local_cs(2, j) / L0;
        mTransformationMatrix(i, 4) += local_cs(0, j);
    }

    // moments 2nd node
    for ( int i=9, j=0; i<12 && j<3; ++i, ++j ) {
        mTransformationMatrix(i, 1) += local_cs(2, j);
        mTransformationMatrix(i, 3) += local_cs(1, j);
    }

    KRATOS_CATCH("")
}

Matrix FiberBeamColumnElement3D2N::CreateInitialLocalCoordSys() const
{
    KRATOS_TRY
    Matrix local_cs = ZeroMatrix(msDimension, msDimension);

    const double numerical_limit = std::numeric_limits<double>::epsilon();
    array_1d<double, msDimension> direction_vector_x = ZeroVector(msDimension);
    array_1d<double, msDimension> direction_vector_y = ZeroVector(msDimension);
    array_1d<double, msDimension> direction_vector_z = ZeroVector(msDimension);
    array_1d<double, msDimension*msNumberOfNodes> reference_coordinates = ZeroVector(msDimension*msNumberOfNodes);

    reference_coordinates[0] = GetGeometry()[0].X0();
    reference_coordinates[1] = GetGeometry()[0].Y0();
    reference_coordinates[2] = GetGeometry()[0].Z0();
    reference_coordinates[3] = GetGeometry()[1].X0();
    reference_coordinates[4] = GetGeometry()[1].Y0();
    reference_coordinates[5] = GetGeometry()[1].Z0();

    for (unsigned int i = 0; i < msDimension; ++i) {
        direction_vector_x[i] =
            (reference_coordinates[i + msDimension] - reference_coordinates[i]);
    }

    array_1d<double, msDimension> global_z = ZeroVector(msDimension);
    global_z[2] = 1.0;
    array_1d<double, msDimension> v2 = ZeroVector(msDimension);
    array_1d<double, msDimension> v3 = ZeroVector(msDimension);

    double vector_norm;
    vector_norm = MathUtils<double>::Norm(direction_vector_x);
    if (vector_norm > numerical_limit) {
        direction_vector_x /= vector_norm;
    }

    if (std::abs(direction_vector_x[2] - 1.00) < numerical_limit) {
            v2[1] = 1.0;
            v3[0] = -1.0;
    } else if (std::abs(direction_vector_x[2] + 1.00) < numerical_limit) {
        v2[1] = 1.0;
        v3[0] = 1.0;
    } else {
        MathUtils<double>::UnitCrossProduct(v2, global_z, direction_vector_x);
        MathUtils<double>::UnitCrossProduct(v3, direction_vector_x, v2);
    }

    for (int i = 0; i < msDimension; ++i) {
        local_cs(i, 0) = direction_vector_x[i];
        local_cs(i, 1) = v2[i];
        local_cs(i, 2) = v3[i];
    }

    return local_cs;
    KRATOS_CATCH("")
}

// int FiberBeamColumnElement3D2N::Check(const ProcessInfo& rCurrentProcessInfo)
// {
//     KRATOS_TRY

//     KRATOS_ERROR_IF(this->Id() < 1) <<"FiberBeamColumnElement3D2N found with Id 0 or negative" << std::endl;

//     KRATOS_ERROR_IF(this->GetGeometry().Area() <= 0) << "On FiberBeamColumnElement3D2N -> "
//         << this->Id() <<  "; Area cannot be less than or equal to 0" << std::endl;

//     return 0;

//     KRATOS_CATCH("");
// }

///@}
///@name Access
///@{


///@}
///@name Inquiry
///@{


///@}
///@name Input and output
///@{

std::string FiberBeamColumnElement3D2N::Info() const {
    std::stringstream buffer;
    buffer << "FiberBeamColumnElement3D2N #" << Id();
    return buffer.str();
}

void FiberBeamColumnElement3D2N::PrintInfo(std::ostream& rOStream) const {
    rOStream << "FiberBeamColumnElement3D2N #" << Id();
}

void FiberBeamColumnElement3D2N::PrintData(std::ostream& rOStream) const {
    pGetGeometry()->PrintData(rOStream);
}


void FiberBeamColumnElement3D2N::save(Serializer& rSerializer) const
{
    KRATOS_SERIALIZE_SAVE_BASE_CLASS(rSerializer, Element);
    rSerializer.save("mSections", mSections);
    rSerializer.save("mTransformationMatrix", mTransformationMatrix);
    rSerializer.save("mLocalStiffnessMatrix", mLocalStiffnessMatrix);
    rSerializer.save("mDeformationPreviousIteration", mDeformationPreviousIteration);
    rSerializer.save("mDeformationCurrentIteration", mDeformationCurrentIteration);
    rSerializer.save("mDeformationPreviousIncerements", mDeformationPreviousIncerements);
    rSerializer.save("mDeformationCurrentIncerements", mDeformationCurrentIncerements);
    // rSerializer.save("mChangeDeformationIncerements", mChangeDeformationIncerements);
    rSerializer.save("mConvergedDeformations", mConvergedDeformations);
    rSerializer.save("mDeformationResiduals", mDeformationResiduals);
    // rSerializer.save("mChangeForceIncerements", mChangeForceIncerements);
    rSerializer.save("mForceIncerements", mForceIncerements);
    rSerializer.save("mForces", mForces);
    rSerializer.save("mConvergedForces", mConvergedForces);
}
void FiberBeamColumnElement3D2N::load(Serializer& rSerializer)
{
    KRATOS_SERIALIZE_LOAD_BASE_CLASS(rSerializer, Element);
    rSerializer.load("mSections", mSections);
    rSerializer.load("mTransformationMatrix", mTransformationMatrix);
    rSerializer.load("mLocalStiffnessMatrix", mLocalStiffnessMatrix);
    rSerializer.load("mDeformationPreviousIteration", mDeformationPreviousIteration);
    rSerializer.load("mDeformationCurrentIteration", mDeformationCurrentIteration);
    rSerializer.load("mDeformationPreviousIncerements", mDeformationPreviousIncerements);
    rSerializer.load("mDeformationCurrentIncerements", mDeformationCurrentIncerements);
    // rSerializer.load("mChangeDeformationIncerements", mChangeDeformationIncerements);
    rSerializer.load("mConvergedDeformations", mConvergedDeformations);
    rSerializer.load("mDeformationResiduals", mDeformationResiduals);
    // rSerializer.load("mChangeForceIncerements", mChangeForceIncerements);
    rSerializer.load("mForceIncerements", mForceIncerements);
    rSerializer.load("mForces", mForces);
    rSerializer.load("mConvergedForces", mConvergedForces);
}

} // namespace Kratos.
