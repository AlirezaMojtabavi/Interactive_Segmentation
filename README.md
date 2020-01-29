# Interactive_Segmentation
My Master Thesis in Tehran University Of Medical Sciences
This project segment Glioma tumor in MRI T1+C Images
It Starts with segment boundaries(edema area in tumor) by seeding user and levelset method, finally morphological method adding Surrounded region to segmented region
![first test](sample/Media2.avi)


For now, Segmentation 3D is Ready to use

## Installing

- download VTK-8.1 and ITK-4.13 and cmake-3.16.0
- config and Generate VTK by cmake(unchecked shared-libs and select openGL not openGl2) 
- open vtk.sln with visual studio(Run as administrator) and build Install with configuration Release 
- config and Generate ITK by cmake(unchecked shared-libs, check module_ITKVTKGlue and select vtk directory) 
- open itk.sln with visual studio(Run as administrator) and build Install with configuration Release 
- open Segmentaion3D or Segmentation2D and set libs and header files of vtk and itk

## Running the tests

- set image directory in main function (MRI Images for Segmentation3D for example "input3D" file)
- set upper and lower threshold of area(s) that you want to segment
- run the project

### Usage

when project is running :
- if (key == "c") // contrast -> by draging mouse you can adjust contrast and brightness
- if (key == "b") // brush -> by clicking Left Button you can initialaze algorithm before running algorithm
- if (key == "r") // manual refine -> not working yet
- if (key == "p") // manual erase -> not working yet
- if (key == "g") // go - run -> by clicking Left Button you can initialaze and run algorithm
- if (key == "o") // remove seed -> by clicking Left Button you can remove last seed and run algorithm
