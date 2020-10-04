//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya
//

// System includes

// External includes

// Project includes
#include "includes/model_part.h"

// Include base h
#include "utilities/assemble_utilities.h"

namespace Kratos
{
template<>
ModelPart::NodesContainerType& AssembleUtilities::GetContainer<ModelPart::NodesContainerType>(
    ModelPart& rModelPart)
{
    return rModelPart.Nodes();
}

template<>
ModelPart::ElementsContainerType& AssembleUtilities::GetContainer<ModelPart::ElementsContainerType>(
    ModelPart& rModelPart)
{
    return rModelPart.Elements();
}

template<>
ModelPart::ConditionsContainerType& AssembleUtilities::GetContainer<ModelPart::ConditionsContainerType>(
    ModelPart& rModelPart)
{
    return rModelPart.Conditions();
}

} // namespace Kratos