//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
// ==============================================================================
//  ChimeraApplication
//
//  License:         BSD License
//                   license: ChimeraApplication/license.txt
//
//  Authors:        Aditya Ghantasala, https://github.com/adityaghantasala
// 					Navaneeth K Narayanan
//					Rishith Ellath Meethal
// ==============================================================================
//
// System includes

#if defined(KRATOS_PYTHON)
// External includes

// Project includes
#include "includes/define_python.h"
#include "chimera_application.h"
#include "chimera_application_variables.h"
#include "custom_python/add_custom_processes_to_python.h"
#include "custom_python/add_custom_utilities_to_python.h"
#include "custom_python/add_custom_strategies_to_python.h"


namespace Kratos
{

namespace Python
{

PYBIND11_MODULE(KratosChimeraApplication, m)
{

	namespace py = pybind11;
    py::class_<KratosChimeraApplication,
		KratosChimeraApplication::Pointer,
		KratosApplication >(m,"KratosChimeraApplication")
		.def(py::init<>())
		;

	AddCustomProcessesToPython(m);
	AddCustomUtilitiesToPython(m);
	AddCustomStrategiesToPython(m);

    KRATOS_REGISTER_IN_PYTHON_FLAG(m, CHIMERA_INTERNAL_BOUNDARY);

	KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ROTATIONAL_ANGLE);
	KRATOS_REGISTER_IN_PYTHON_VARIABLE(m, ROTATIONAL_VELOCITY);
}

}  // namespace Python.

}  // namespace Kratos.

#endif // KRATOS_PYTHON defined
