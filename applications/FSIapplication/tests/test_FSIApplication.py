# import Kratos
from KratosMultiphysics import *
from KratosMultiphysics.FSIApplication import *

# Import Kratos "wrapper" for unittests
import KratosMultiphysics.KratosUnittest as KratosUnittest

# Import the tests o test_classes to create the suits
## SMALL TESTS
from SmallTests import FSIProblemEmulatorTest as TFSIProblemEmulatorTest
from KratosExecuteConvergenceAcceleratorTest import KratosExecuteConvergenceAcceleratorTest as TConvergenceAcceleratorTest
from KratosExecuteConvergenceAcceleratorSpringTest import KratosExecuteConvergenceAcceleratorSpringTest as TConvergenceAcceleratorSpringTest
from variable_redistribution_test import VariableRedistributionTest
from non_conformant_one_side_map_test import NonConformantOneSideMapTest

## NIGTHLY TESTS

## VALIDATION TESTS
from ValidationTests import MokBenchmarkTest as TMokBenchmark

def AssembleTestSuites():
    ''' Populates the test suites to run.

    Populates the test suites to run. At least, it should populate the suites:
    "small", "nighlty" and "all"

    Return
    ------

    suites: A dictionary of suites
        The set of suites with its test_cases added.
    '''
    suites = KratosUnittest.KratosSuites

    # Create a test suit with the selected tests (Small tests):
    smallSuite = suites['small']
    smallSuite.addTest(TConvergenceAcceleratorTest('test_aitken_accelerator'))
    smallSuite.addTest(TConvergenceAcceleratorTest('test_mvqn_accelerator'))
    smallSuite.addTest(TConvergenceAcceleratorTest('test_mvqn_recusive_accelerator'))
    smallSuite.addTest(TConvergenceAcceleratorTest('test_accelerator_with_jacobian'))
    smallSuite.addTest(TFSIProblemEmulatorTest('test_execution'))
    smallSuite.addTest(TConvergenceAcceleratorSpringTest('test_aitken_accelerator_constant_forces'))
    smallSuite.addTest(TConvergenceAcceleratorSpringTest('test_aitken_accelerator_variable_stiffness'))
    smallSuite.addTest(NonConformantOneSideMapTest('test2D_1'))
    smallSuite.addTest(NonConformantOneSideMapTest('test2D_2'))
    smallSuite.addTest(NonConformantOneSideMapTest('test3D_1'))
    smallSuite.addTest(NonConformantOneSideMapTest('test3D_two_faces'))
    smallSuite.addTest(VariableRedistributionTest('testLinearFunction'))
    smallSuite.addTest(VariableRedistributionTest('testSharpCorners'))
    smallSuite.addTest(VariableRedistributionTest('testVector'))
    smallSuite.addTest(VariableRedistributionTest('testQuadratic'))
    smallSuite.addTest(VariableRedistributionTest('testNodalArea'))

    # Create a test suit with the selected tests plus all small tests
    nightSuite = suites['nightly']
    nightSuite.addTests(smallSuite)

    # For very long tests that should not be in nighly and you can use to validate
    validationSuite = suites['validation']
    validationSuite.addTest(TMokBenchmark('test_execution'))

    # Create a test suit that contains all the tests
    allSuite = suites['all']
    allSuite.addTests(nightSuite)

    return suites

if __name__ == '__main__':
    KratosUnittest.runTests(AssembleTestSuites())
