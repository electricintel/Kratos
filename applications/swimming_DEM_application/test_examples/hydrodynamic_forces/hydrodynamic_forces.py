from __future__ import print_function, absolute_import, division #makes KratosMultiphysics backward compatible with python 2.6 and 2.7
import math
import copy
from KratosMultiphysics import *
from KratosMultiphysics.DEMApplication import *
from KratosMultiphysics.SwimmingDEMApplication import *
from KratosMultiphysics.MeshingApplication import *
import swimming_DEM_gid_output
import ProjectParameters
import random

def AddVariables(model_part, pp):
    AddNodalVariables(model_part)
    AddAndInitializeProcessInfoVariables(model_part, pp)
        
def InitializeVariables(model_part, pp):
    InitializeNodalVariables(model_part, pp)
    AddAndInitializeProcessInfoVariables(model_part, pp)       

def InitializeNodalVariables(model_part, pp):
    for node in model_part.Nodes:
        node.X = pp.coor_x
        node.Y = pp.coor_y
        node.Z = pp.coor_z
        
        node.SetSolutionStepValue(DISPLACEMENT_X, pp.displ_x)
        node.SetSolutionStepValue(DISPLACEMENT_Y, pp.displ_y)
        node.SetSolutionStepValue(DISPLACEMENT_Z, pp.displ_z)
        
        node.SetSolutionStepValue(VELOCITY_X, pp.velocity_x)
        node.SetSolutionStepValue(VELOCITY_Y, pp.velocity_y)
        node.SetSolutionStepValue(VELOCITY_Z, pp.velocity_z)
              
        node.SetSolutionStepValue(PARTICLE_ROTATION_ANGLE_X, pp.rot_angle_x)
        node.SetSolutionStepValue(PARTICLE_ROTATION_ANGLE_Y, pp.rot_angle_y)
        node.SetSolutionStepValue(PARTICLE_ROTATION_ANGLE_Z, pp.rot_angle_z)     
        
        node.SetSolutionStepValue(EULER_ANGLES_X, pp.euler_angle_x)
        node.SetSolutionStepValue(EULER_ANGLES_Y, pp.euler_angle_y)
        node.SetSolutionStepValue(EULER_ANGLES_Z, pp.euler_angle_z)          
        
        node.SetSolutionStepValue(ANGULAR_VELOCITY_X, pp.ang_vel_x)
        node.SetSolutionStepValue(ANGULAR_VELOCITY_Y, pp.ang_vel_y)
        node.SetSolutionStepValue(ANGULAR_VELOCITY_Z, pp.ang_vel_z)
               
        node.SetSolutionStepValue(TOTAL_FORCES_X, pp.tota_force_x)
        node.SetSolutionStepValue(TOTAL_FORCES_Y, pp.tota_force_y)
        node.SetSolutionStepValue(TOTAL_FORCES_Z, pp.tota_force_z)       
        
        node.SetSolutionStepValue(FLUID_VEL_PROJECTED_X, pp.fluid_velocity_x)
        node.SetSolutionStepValue(FLUID_VEL_PROJECTED_Y, pp.fluid_velocity_y)
        node.SetSolutionStepValue(FLUID_VEL_PROJECTED_Z, pp.fluid_velocity_z)        
        
        node.SetSolutionStepValue(FLUID_ACCEL_PROJECTED_X, pp.fluid_acceleration_x)
        node.SetSolutionStepValue(FLUID_ACCEL_PROJECTED_Y, pp.fluid_acceleration_y)
        node.SetSolutionStepValue(FLUID_ACCEL_PROJECTED_Z, pp.fluid_acceleration_z)        
        
        node.SetSolutionStepValue(FLUID_VORTICITY_PROJECTED_X, pp.fluid_vorticity_x)
        node.SetSolutionStepValue(FLUID_VORTICITY_PROJECTED_Y, pp.fluid_vorticity_y)
        node.SetSolutionStepValue(FLUID_VORTICITY_PROJECTED_Z, pp.fluid_vorticity_z)
        
        node.SetSolutionStepValue(PRESSURE_GRAD_PROJECTED_X, pp.pressure_gradient_x)
        node.SetSolutionStepValue(PRESSURE_GRAD_PROJECTED_Y, pp.pressure_gradient_y)
        node.SetSolutionStepValue(PRESSURE_GRAD_PROJECTED_Z, pp.pressure_gradient_z)
                    
        
        node.SetSolutionStepValue(RADIUS, pp.radius)
        node.SetSolutionStepValue(PARTICLE_SPHERICITY, pp.sphericity)
        node.SetSolutionStepValue(SQRT_OF_MASS, pp.sqrt_of_mass)
        node.SetSolutionStepValue(PARTICLE_MOMENT_OF_INERTIA, pp.inertia)
        node.SetSolutionStepValue(FLUID_FRACTION_PROJECTED, pp.fluid_fraction)
        node.SetSolutionStepValue(FLUID_DENSITY_PROJECTED, pp.fluid_density)
        node.SetSolutionStepValue(FLUID_VISCOSITY_PROJECTED, pp.kinematic_viscosity)
        node.SetSolutionStepValue(POWER_LAW_N, pp.power_law_n)
        node.SetSolutionStepValue(POWER_LAW_K, pp.power_law_k)    
        node.SetSolutionStepValue(YIELD_STRESS, pp.yield_stress)   
        node.SetSolutionStepValue(SHEAR_RATE_PROJECTED, pp.shear_rate_projected)
           
    
def AddVariables(model_part):
    # COMMON
    model_part.AddNodalSolutionStepVariable(VELOCITY)
    model_part.AddNodalSolutionStepVariable(DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(TOTAL_FORCES)
    model_part.AddNodalSolutionStepVariable(GROUP_ID)   

    # KINEMATIC
    model_part.AddNodalSolutionStepVariable(DELTA_DISPLACEMENT)
    model_part.AddNodalSolutionStepVariable(PARTICLE_ROTATION_ANGLE)
    model_part.AddNodalSolutionStepVariable(ANGULAR_VELOCITY)
    
    # FORCES
    model_part.AddNodalSolutionStepVariable(ELASTIC_FORCES)
    model_part.AddNodalSolutionStepVariable(DAMP_FORCES)
    model_part.AddNodalSolutionStepVariable(PARTICLE_MOMENT)
    model_part.AddNodalSolutionStepVariable(EXTERNAL_APPLIED_FORCE)

    # BASIC PARTICLE PROPERTIES
    model_part.AddNodalSolutionStepVariable(RADIUS)
    model_part.AddNodalSolutionStepVariable(PARTICLE_SPHERICITY)
    model_part.AddNodalSolutionStepVariable(SQRT_OF_MASS)
    model_part.AddNodalSolutionStepVariable(PRINCIPAL_MOMENTS_OF_INERTIA)
    model_part.AddNodalSolutionStepVariable(CHARACTERISTIC_LENGTH)
    model_part.AddNodalSolutionStepVariable(PARTICLE_DENSITY)

    # ROTATION RELATED PROPERTIES
    model_part.AddNodalSolutionStepVariable(PARTICLE_MOMENT_OF_INERTIA)
    model_part.AddNodalSolutionStepVariable(PARTICLE_ROTATION_DAMP_RATIO)
    model_part.AddNodalSolutionStepVariable(ROLLING_FRICTION)

    # OTHER PROPERTIES
    model_part.AddNodalSolutionStepVariable(PARTICLE_MATERIAL)   # Colour defined in GiD

    # LOCAL AXIS
    model_part.AddNodalSolutionStepVariable(EULER_ANGLES)

    # FLAGS
    model_part.AddNodalSolutionStepVariable(GROUP_ID)            # Differencied groups for plotting, etc..
    
    # ONLY VISUALIZATION
    model_part.AddNodalSolutionStepVariable(EXPORT_ID)
    #model_part.AddNodalSolutionStepVariable(EXPORT_GROUP_ID)
    
    # SWIMMING
    model_part.AddNodalSolutionStepVariable(REYNOLDS_NUMBER)
    model_part.AddNodalSolutionStepVariable(PRESSURE_GRAD_PROJECTED)
    model_part.AddNodalSolutionStepVariable(HYDRODYNAMIC_FORCE)
    model_part.AddNodalSolutionStepVariable(FLUID_VEL_PROJECTED)
    model_part.AddNodalSolutionStepVariable(FLUID_ACCEL_PROJECTED)
    model_part.AddNodalSolutionStepVariable(FLUID_FRACTION_PROJECTED)
    model_part.AddNodalSolutionStepVariable(FLUID_DENSITY_PROJECTED)
    model_part.AddNodalSolutionStepVariable(FLUID_VISCOSITY_PROJECTED)
    model_part.AddNodalSolutionStepVariable(BUOYANCY)
    model_part.AddNodalSolutionStepVariable(DRAG_FORCE)
    model_part.AddNodalSolutionStepVariable(VIRTUAL_MASS_FORCE)
    model_part.AddNodalSolutionStepVariable(LIFT_FORCE)    
    model_part.AddNodalSolutionStepVariable(FLUID_VORTICITY_PROJECTED)
    model_part.AddNodalSolutionStepVariable(SHEAR_RATE_PROJECTED)
    model_part.AddNodalSolutionStepVariable(FLUID_ACCEL_PROJECTED)
    model_part.AddNodalSolutionStepVariable(DISTANCE)
    model_part.AddNodalSolutionStepVariable(POWER_LAW_N)
    model_part.AddNodalSolutionStepVariable(POWER_LAW_K)
    model_part.AddNodalSolutionStepVariable(POWER_LAW_K)
    model_part.AddNodalSolutionStepVariable(YIELD_STRESS)   
    
def AddAndInitializeProcessInfoVariables(model_part, pp):    
    # SIMULATION FLAGS
    
    model_part.ProcessInfo.SetValue(VIRTUAL_MASS_OPTION, 0)
    model_part.ProcessInfo.SetValue(CRITICAL_TIME_OPTION, 0)
    model_part.ProcessInfo.SetValue(ROTATION_OPTION, 1)       

    # GLOBAL MATERIAL PROPERTIES
    model_part.ProcessInfo.SetValue(NODAL_MASS_COEFF, pp.nodal_mass_coeff)

    # PRINTING VARIABLES

    model_part.ProcessInfo.SetValue(FORCE_CALCULATION_TYPE, 0)
    model_part.ProcessInfo.SetValue(DAMP_TYPE, 0)
    model_part.ProcessInfo.SetValue(ROLLING_FRICTION_OPTION, 0)
    model_part.ProcessInfo.SetValue(PRINT_EXPORT_ID, 1)

    # TIME RELATED PARAMETERS
    model_part.ProcessInfo.SetValue(DELTA_TIME, pp.delta_time)
    
    # SWIMMING
    model_part.ProcessInfo.SetValue(BUOYANCY_FORCE_TYPE, pp.buoyancy_force_type)
    model_part.ProcessInfo.SetValue(DRAG_FORCE_TYPE, pp.drag_force_type)
    model_part.ProcessInfo.SetValue(VIRTUAL_MASS_FORCE_TYPE, pp.virtual_mass_force_type)
    model_part.ProcessInfo.SetValue(LIFT_FORCE_TYPE, pp.lift_force_type)
    model_part.ProcessInfo.SetValue(FLUID_MODEL_TYPE, pp.fluid_model_type)
    model_part.ProcessInfo.SetValue(MANUALLY_IMPOSED_DRAG_LAW_OPTION, pp.manually_imposed_drag_law_option)
    model_part.ProcessInfo.SetValue(DRAG_MODIFIER_TYPE, pp.drag_modifier_type)
    model_part.ProcessInfo.SetValue(INIT_DRAG_FORCE, pp.initial_drag_force)
    model_part.ProcessInfo.SetValue(DRAG_LAW_SLOPE, pp.drag_law_slope)
    model_part.ProcessInfo.SetValue(POWER_LAW_TOLERANCE, pp.power_law_tol)    
    model_part.ProcessInfo.SetValue(GRAVITY_X, pp.gravity_x)
    model_part.ProcessInfo.SetValue(GRAVITY_Y, pp.gravity_y)
    model_part.ProcessInfo.SetValue(GRAVITY_Z, pp.gravity_z)
    
           
    model_part.ProcessInfo.SetValue(CASE_OPTION, 1)
    model_part.ProcessInfo.SetValue(TRIHEDRON_OPTION, 0)
    model_part.ProcessInfo.SetValue(BOUNDING_BOX_OPTION, 0)
    model_part.ProcessInfo.SetValue(FIX_VELOCITIES_FLAG, 0)
    model_part.ProcessInfo.SetValue(NEIGH_INITIALIZED, 0);
    model_part.ProcessInfo.SetValue(TOTAL_CONTACTS, 0);
    model_part.ProcessInfo.SetValue(CLEAN_INDENT_OPTION, 0)
    model_part.ProcessInfo.SetValue(ACTIVATE_SEARCH, 1)  # needed in the basic for the continuum.
    
def AddDofs(model_part):

    for node in model_part.Nodes:
        node.AddDof(VELOCITY_X, REACTION_X)
        node.AddDof(VELOCITY_Y, REACTION_Y)
        node.AddDof(VELOCITY_Z, REACTION_Z)
        node.AddDof(ANGULAR_VELOCITY_X, REACTION_X)
        node.AddDof(ANGULAR_VELOCITY_Y, REACTION_Y)
        node.AddDof(ANGULAR_VELOCITY_Z, REACTION_Z)

def PadWithSpacesa(lines, lines_to_print):
    widths = GetColumsMaxWidths(lines, 2)

    for line in lines:
        i = 0
        
        for i_entry in range(0, len(line)):
            line[i_entry] = line[i_entry].ljust(widths[i_entry])
    
    i = 0
    
    for line in lines:
        line_to_print = ""
        
        for entry in line:
            line_to_print += entry
                
        lines_to_print += [line_to_print]
    
    return sum(widths)    

def PadWithSpaces(lines):    
    widths = GetColumsMaxWidths(lines, 2)

    for line in lines:
        
        for i_entry in range(0, len(line)):
            line[i_entry] = line[i_entry].ljust(widths[i_entry])     
   
    aux = copy.deepcopy(lines)
    
    del lines[:]
    
    for line_aux in aux:
        line = ""
        
        for entry in line_aux:
            line += entry
         
        lines += [line]
   
    return sum(widths)       
    
def GetColumsMaxWidths(lines, margin):
    widths = []
    
    if (not len(lines)):
        return widths    
        
    for entry in lines[0]:
        widths += [1]
    
    for line in lines:
        i = 0
        
        for entry in line:
            
            if (len(str(entry)) > widths[i]):
                widths[i] = len(str(entry))              
                
            i += 1
        
    for i in range(0, len(widths)):
        widths[i] += margin

    return widths
        
def Distance(v1, v2):
    return math.sqrt((v1[0] - v2 [0]) ** 2 + (v1[1] - v2 [1]) ** 2 + (v1[2] - v2 [2]) ** 2)

def Normalize(v, length = 1.0):
    
    if (v[0] == 0.0 and v[1] == 0.0 and v[1] == 0.0):
        return
    
    else:
        norm = math.sqrt(v[0] ** 2 + v[1] ** 2 + v[2] ** 2)
        v[0] *= length * norm
        v[1] *= length * norm
        v[2] *= length * norm
        
    
def PrintResults(title, tests):
    lines = []
    lines += [["Test Number", "Target", "Calculated", "Error", "Veredict", "Description"]]
    
    i_test = 0
    
    for test in tests:
        lines += [[str(i_test)] + test.string_results]
        i_test += 1

    total_width = PadWithSpaces(lines)    
    print()
    print ("=" * total_width)
    print (title)
    print ("-" * total_width)
    print (lines[0])
    print ("-" * total_width)
    print (lines[1])
    print (lines[2])
    print (lines[3])
    print ("=" * total_width)
    print()        

class Benchmark:
    def __init__(self):
        pass
    
    tests = []
    
    @staticmethod
    def PrintResults(title, tests):
        lines = []
        lines += [["Test Number", "Target", "Calculated", "Error", "Veredict", "Description"]]
        
        i_test = 0
        
        for test in tests:
            if (test.has_results):
                lines += [[str(i_test)] + test.string_results]            
                i_test += 1

        total_width = PadWithSpaces(lines)    
        print()
        print ("=" * total_width)
        print (title)
        print ("-" * total_width)
        print (lines[0])
        print ("-" * total_width)
        
        for i in range(1, i_test + 1):
            print (lines[i])

        print ("=" * total_width)
        print()
    
    
class BuoyancyBenchmark(Benchmark):    
    title = "Buoyancy force test results"
    tests = []
    
    @staticmethod
    def PrintResults():
        Benchmark.PrintResults(BuoyancyBenchmark.title, BuoyancyBenchmark.tests)
        
    @staticmethod
    def ConvertToStrings(results):
         
        if results[3]:
            word = "OK"
        else:
            word = "Failed"
            
        return [str(results[0]), str(results[1]), str(results[2]), word, str(results[4])]
    
    def __init__(self, model_part, pp, buoyancy_force_type, drag_force_type, pressure_gradient, target_buoyancy, description):
        self.pp = copy.deepcopy(pp) 
        self.buoyancy_tol = 10e-5 * self.pp.sqrt_of_mass

        self.pp.virtual_mass_force_type = 0
        self.pp.lift_force_type = 0
        self.pp.pressure_gradient_x = pressure_gradient[0]
        self.pp.pressure_gradient_y = pressure_gradient[1]
        self.pp.pressure_gradient_z = pressure_gradient[2]
        self.pp.drag_force_type = drag_force_type
        self.pp.buoyancy_force_type = buoyancy_force_type
        
        self.target_buoyancy = target_buoyancy
        self.description = description
        self.has_results = False
        
        BuoyancyBenchmark.tests += [self]
        Benchmark.tests += [self]
     
    def Test(self, model_part, benchmark_utils):
        InitializeVariables(model_part, self.pp)
        benchmark_utils.ComputeHydrodynamicForces(model_part)

        for node in model_part.Nodes:
            buoyancy = node.GetSolutionStepValue(BUOYANCY)
                  
        error = Distance(buoyancy, self.target_buoyancy)
        
        if (error < self.buoyancy_tol):
            veredict = True
            
        else:
            veredict = False
            
        self.results = [self.target_buoyancy, buoyancy, error, veredict, self.description] 
        self.string_results = BuoyancyBenchmark.ConvertToStrings(self.results)
        self.has_results = True

class DragBenchmark(Benchmark):
    title = "Drag force test results"
    tests = []
    
    @staticmethod
    def PrintResults():
        Benchmark.PrintResults(DragBenchmark.title, DragBenchmark.tests)
        
    @staticmethod
    def ConvertToStrings(results):
         
        if results[3]:
            word = "OK"
        else:
            word = "Failed"
            
        return [str(results[0]), str(results[1]), str(results[2]), word, str(results[4])]
        
    def __init__(self, model_part, pp, drag_force_type, velocity, viscosity, fluid_fraction, target_drag, description):
        self.pp = copy.deepcopy(pp) 
        self.drag_tol = 10e-5 * self.pp.sqrt_of_mass
        self.target_drag = target_drag
        
        self.pp.buoyancy_force_type = 0
        self.pp.drag_force_type = drag_force_type 
        self.pp.virtual_mass_force_type = 0
        self.pp.lift_force_type = 0      
        self.pp.velocity_x = velocity[0]
        self.pp.velocity_y = velocity[1]
        self.pp.velocity_z = velocity[2]        
        self.pp.fluid_fraction = fluid_fraction
        self.pp.kinematic_viscosity = viscosity
        
        self.target_drag = target_drag
        self.description = description
        self.has_results = False
        
        DragBenchmark.tests += [self]
        Benchmark.tests += [self]
        
    def Test(self, model_part, benchmark_utils):
        InitializeVariables(model_part, self.pp)
        benchmark_utils.ComputeHydrodynamicForces(model_part)

        for node in model_part.Nodes:
            drag = node.GetSolutionStepValue(DRAG_FORCE)
                  
        error = Distance(drag, self.target_drag)
        
        if (error < self.drag_tol):
            veredict = True
            
        else:
            veredict = False
            
        self.results = [self.target_drag, drag, error, veredict, self.description] 
        self.string_results = DragBenchmark.ConvertToStrings(self.results)
        self.has_results = True
    
pp = ProjectParameters.Parameters()
model_part = ModelPart("OneBallModelPart")
AddVariables(model_part)

model_part_io_solid = ModelPartIO("hydrodynamic_forces", True)
model_part_io_solid.ReadModelPart(model_part)
model_part.SetBufferSize(1)
AddDofs(model_part)

benchmark_utils = BenchmarkUtils()

# Buoyancy

pressure_gradient = Array3()
pressure_gradient[0] = random.random()
pressure_gradient[1] = random.random()
pressure_gradient[2] = random.random()
Normalize(pressure_gradient)

fluid_vel = Array3()
fluid_vel[0] = 1.0
fluid_vel[1] = -2.0
fluid_vel[2] = 3.0

buoyancy_target_0 = Array3()
buoyancy_target_0[0] = 0.0
buoyancy_target_0[1] = 0.0
buoyancy_target_0[2] = 0.0

buoyancy_target_1 = Array3()
buoyancy_target_1_norm = 4/3 * math.pi
buoyancy_target_1[0] = - buoyancy_target_1_norm * pressure_gradient[0]
buoyancy_target_1[1] = - buoyancy_target_1_norm * pressure_gradient[1]
buoyancy_target_1[2] = - buoyancy_target_1_norm * pressure_gradient[2]

buoyancy_target_2 = Array3()
buoyancy_target_2[0] = 0.0
buoyancy_target_2[1] = 0.0
buoyancy_target_2[2] = 9.81 * 4/3 * math.pi

buoyancy_test_0 = BuoyancyBenchmark(model_part, pp, 0, 1, pressure_gradient, buoyancy_target_0, "Inactive")
buoyancy_test_1 = BuoyancyBenchmark(model_part, pp, 1, 1, pressure_gradient, buoyancy_target_1, "Standard")
buoyancy_test_2 = BuoyancyBenchmark(model_part, pp, 1, 2, pressure_gradient, buoyancy_target_2, "Weatherford: hydrostatic buoyancy")

buoyancy_test_0.Test(model_part, benchmark_utils)
buoyancy_test_1.Test(model_part, benchmark_utils)
buoyancy_test_2.Test(model_part, benchmark_utils)

BuoyancyBenchmark.PrintResults()

# Drag

viscosity = 10e-2
fluid_fraction = 1.0
fluid_vel = Array3()
fluid_vel[0] = random.random()
fluid_vel[1] = random.random()
fluid_vel[2] = random.random()
Normalize(fluid_vel)

drag_target_0 = Array3()
drag_target_0[0] = 0.0
drag_target_0[1] = 0.0
drag_target_0[2] = 0.0

drag_target_1 = fluid_vel
fluid_vel_norm = 0.01
drag_target_1_norm = 6 * math.pi * viscosity * fluid_vel_norm
Normalize(fluid_vel, fluid_vel_norm)
Normalize(drag_target_1, drag_target_1_norm)
drag_target_1[0] = fluid_vel[0]
drag_target_1[1] = fluid_vel[1]
drag_target_1[2] = fluid_vel[2]

drag_test_0 = DragBenchmark(model_part, pp, 0, fluid_vel, viscosity, fluid_fraction, drag_target_0, "Inactive")
drag_test_1 = DragBenchmark(model_part, pp, 1, fluid_vel, viscosity, fluid_fraction, drag_target_1, "Stokes")

drag_test_2 = DragBenchmark(model_part, pp, 2, fluid_vel, viscosity, fluid_fraction, drag_target_0, "Inactive")
drag_test_3 = DragBenchmark(model_part, pp, 3, fluid_vel, viscosity, fluid_fraction, drag_target_0, "Inactive")
drag_test_4 = DragBenchmark(model_part, pp, 4, fluid_vel, viscosity, fluid_fraction, drag_target_0, "Inactive")
drag_test_5 = DragBenchmark(model_part, pp, 5, fluid_vel, viscosity, fluid_fraction, drag_target_0, "Inactive")

drag_test_0.Test(model_part, benchmark_utils)
drag_test_1.Test(model_part, benchmark_utils)

DragBenchmark.PrintResults()
