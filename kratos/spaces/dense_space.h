//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Riccardo Rossi
//



#if !defined(KRATOS_DENSE_SPACE_H_INCLUDED )
#define  KRATOS_DENSE_SPACE_H_INCLUDED



// System includes
#include <string>
#include <iostream>
#include <cstddef>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/ublas_interface.h"


namespace Kratos
{

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
///@name Kratos Classes
///@{

/// Short class definition.
/** Detail class definition.
*/
template<class TDataType>
class DenseSpace
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of DenseSpace
    KRATOS_CLASS_POINTER_DEFINITION(DenseSpace);

    typedef TDataType DataType;

    typedef matrix<TDataType> MatrixType;

    typedef vector<TDataType> VectorType;

    typedef std::size_t IndexType;

    typedef typename boost::shared_ptr< TMatrixType > MatrixPointerType;
    typedef typename boost::shared_ptr< TVectorType > VectorPointerType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructor.
    DenseSpace() {}

    /// Destructor.
    virtual ~DenseSpace() {}


    ///@}
    ///@name Operators
    ///@{


    ///@}
    ///@name Operations
    ///@{
    static MatrixPointerType CreateEmptyMatrixPointer()
    {
        return MatrixPointerType( new TMatrixType() );
    }
    static VectorPointerType CreateEmptyVectorPointer()
    {
        return VectorPointerType( new TVectorType() );
    }


    /// return size of vector rV
    static IndexType Size(VectorType& rV)
    {
        return rV.size();
    }

    /// return number of rows of rM
    static IndexType Size1(MatrixType& rM)
    {
        return rM.size1();
    }

    /// return number of columns of rM
    static IndexType Size2(MatrixType& rM)
    {
        return rM.size2();
    }

    /// rXi = rMij
    static void GetColumn(unsigned int j, MatrixType& rM, VectorType& rX)
    {
        rX = column(rM, j);
    }

    /// rMij = rXi
    static void SetColumn(unsigned int j, MatrixType& rM, VectorType& rX)
    {
        rX = row(rM, j);
    }

    /// rY = rX
    static void Copy(VectorType& rX, VectorType& rY)
    {
        rY.assign(rX);
    }

    /// rX * rY
    static double Dot(VectorType& rX, VectorType& rY)
    {
        return inner_prod(rX, rY);
    }

    /// ||rX||2
    static double TwoNorm(VectorType& rX)
    {
        return sqrt(norm_2(rX);
    }


                static void Mult(MatrixType& rA, VectorType& rX, VectorType& rY)
    {
        axpy_prod(rA, rX, rY, true);
    }

    static void TransposeMult(MatrixType& rA, VectorType& rX, VectorType& rY)
    {
        axpy_prod(rX, rA, rY, true);
    } // rY = rAT * rX

    static void ScaleAndAdd(double A, VectorType& rX, double B, VectorType& rY, VectorType& rZ)  // rZ = (A * rX) + (B * rY)
    {
        typename VectorType::iterator x_iterator = rX.begin();
        typename VectorType::iterator y_iterator = rY.begin();
        typename VectorType::iterator z_iterator = rZ.begin();
        typename VectorType::iterator end_iterator = rX.end();

        while(x_iterator != end_iterator)
            *z_iterator++ = (A * *x_iterator++) + (B * *y_iterator++);
    }

    static void ScaleAndAdd(double A, VectorType& rX, double B, VectorType& rY) // rY = (A * rX) + (B * rY)
    {
        typename VectorType::iterator x_iterator = rX.begin();
        typename VectorType::iterator y_iterator = rY.begin();
        typename VectorType::iterator end_iterator = rX.end();

        double c = B - double(1.00);

        while(x_iterator != end_iterator)
        {
            *y_iterator += (A * *x_iterator++) + (c * *y_iterator);
            y_iterator++;
        }
    }


    /// rA[i] * rX
    static double RowDot(unsigned int i, MatrixType& rA, VectorType& rX)
    {
        return Dot(rA[i], rX);
    }


    /// rX = A
    static void Set(VectorType& rX, TDataType A)
    {
        std::fill(rX.begin(), rX.end(), A);
    }


    ///@}
    ///@name Access
    ///@{


    ///@}
    ///@name Inquiry
    ///@{


    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    virtual std::string Info() const;

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const;

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const;


    ///@}
    ///@name Friends
    ///@{


    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{


    ///@}
    ///@name Protected member Variables
    ///@{


    ///@}
    ///@name Protected Operators
    ///@{


    ///@}
    ///@name Protected Operations
    ///@{


    ///@}
    ///@name Protected  Access
    ///@{


    ///@}
    ///@name Protected Inquiry
    ///@{


    ///@}
    ///@name Protected LifeCycle
    ///@{


    ///@}

private:
    ///@name Static Member Variables
    ///@{


    ///@}
    ///@name Member Variables
    ///@{


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{


    ///@}
    ///@name Private  Access
    ///@{


    ///@}
    ///@name Private Inquiry
    ///@{


    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    DenseSpace& operator=(DenseSpace const& rOther);

    /// Copy constructor.
    DenseSpace(DenseSpace const& rOther);


    ///@}

}; // Class DenseSpace

///@}

///@name Type Definitions
///@{


///@}
///@name Input and output
///@{


/// input stream function
inline std::istream& operator >> (std::istream& rIStream,
                                  DenseSpace& rThis);

/// output stream function
inline std::ostream& operator << (std::ostream& rOStream,
                                  const DenseSpace& rThis)
{
    rThis.PrintInfo(rOStream);
    rOStream << std::endl;
    rThis.PrintData(rOStream);

    return rOStream;
}
///@}


}  // namespace Kratos.

#endif // KRATOS_DENSE_SPACE_H_INCLUDED  defined 


