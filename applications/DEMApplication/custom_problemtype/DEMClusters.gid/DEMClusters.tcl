## GiD events --------------------------------------------------------------------------------------------------------------------------------------------------

proc InitGIDProject { dir } {

    # Create buttons for calculate mass, center of mass and inertia later on.
    # if { [GidUtils::IsTkDisabled] eq 0} {
    #     GiDMenu::Create "Sphere Cluster Creation" PRE
    #     GiDMenu::InsertOption "Sphere Cluster Creation" [list "Inertia"] 0 PRE "GidOpenConditions \"Inertia\"" "" ""
    #     GiDMenu::UpdateMenus
    # }

    # Load the application scripts
    set scripts_dir [file join $dir .. .. ]
    # set tcl_filename [file join $scripts_dir scripts initptype.tcl]
    # if {[catch {source $tcl_filename} msg]} {
    #     WarnWinText $msg
    #     return 1
    # }
}


proc BeforeMeshGeneration {elementsize} {
    W "execute BeforeMeshGeneration"
}

proc AfterMeshGeneration {fail} {
    W "execute AfterMeshGeneration"
    # ExtractSurfaceTriangles 
}

proc ExtractSurfaceTriangles { } {
    # List of triangles defined by: its vertex and vertex normals
    #                               faces: ordered vertex ids

    # GenerateOBJFile
}

proc BeforeRunCalculation { batfilename basename dir problemtypedir gidexe args } {

    # Write file
    source [file join $problemtypedir file.tcl]
}

proc GenerateOBJFile { } {
    # Analyze the format of the OBJ and generate the file in GID
    # The format of the OBJ file is as follows:

    # block 1:surface vertex and vertex normal 
    # block 2: faces (with ordered vertex):

    # v 987.823009 -583.341002 122.360997
    # vn 0.329248 0.150250 0.932213
    # v 979.430974 -499.442995 88.674001
    # vn 0.689488 0.597651 0.409169
    # f 18//18 10//10 9//9 
    # f 18//18 9//9 17//17 
    # f 19//19 11//11 10//10 



    # SUR custom files can also be used with the following format:
    # The format of the SUR file is as follows:
    # -The number of vertices
    # -List of vertices with XYZ for position and XYZ for normal
    # -The number of triangles
    # -List of triangles with i0, i1, i2 (indices of the vertices - zero base)
    #                        n0, n1, n2 (neighbouring triangles, n0 shares edge
    #                                    from i0 to i1 etc.)
}



proc ExecuteSphereTreeAlgorithm { objfilename args} {
    #makeTreeMedial -branch NS -depth 1 -testerLevels 2 -numCover 10000 -minCover 5 -initSpheres 1000 -minSpheres 200 -erFact 2 -verify -nopause -eval -expand -merge -burst -optimise balance -balExcess 0.001 -maxOptLevel 100 file_name.obj
    # multiple args to be defined: algorithm,...

}

proc CleanSPHFile { sphfilename } {
    # Delete some lines and columns. 
    # Follow criteria to delete invalid spheres

}

proc GenerateClusterFile { sphfilename mshfilename} {
    # Look for a way to edit, compile and execute mesh_to_clu_converter.cpp with the custom filenames
    # Create cpp executable able to generate cluster file directly from inputs.
    # Or pass always the same generic sph and msh names. Generate generic cluster filename.

}

