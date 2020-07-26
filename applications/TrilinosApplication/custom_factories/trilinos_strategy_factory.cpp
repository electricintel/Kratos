//  KRATOS  _____     _ _ _
//         |_   _| __(_) (_)_ __   ___  ___
//           | || '__| | | | '_ \ / _ \/ __|
//           | || |  | | | | | | | (_) \__
//           |_||_|  |_|_|_|_| |_|\___/|___/ APPLICATION
//
//  License:             BSD License
//                                       Kratos default license: kratos/license.txt
//
//  Main authors:   Vicente Mataix Ferrandiz
//

// System includes

// External includes

// Project includes
#include "includes/define.h"
#include "custom_factories/trilinos_factory.h"

// Strategies

namespace Kratos
{
void RegisterTrilinosStrategies()
{
    // TODO: To be added
};

template class KratosComponents<TrilinosSolvingStrategyType>;
} // Namespace Kratos