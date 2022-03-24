%{
#ifndef _WIN32
#include <vtkPythonUtil.h>
#define VTK_TYPEMAP_IN(x, y) vtkPythonUtil::GetPointerFromObject(x, y)
#define VTK_TYPEMAP_OUT(x) vtkPythonUtil::GetObjectFromPointer(x)
#else
#define VTK_TYPEMAP_IN(x, y) 0
#define VTK_TYPEMAP_OUT(x) 0
#endif
%}

//Done to be able to use c++ overlaoded function using vtkUnstructuredGrid* or vtkPolyData*
%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) vtkUnstructuredGrid*
{
    $1 =  VTK_TYPEMAP_IN($input, "vtkUnstructuredGrid") ? 1 : 0;
}

%typemap(typecheck, precedence=SWIG_TYPECHECK_POINTER) vtkPolyData*
{
    $1 =  VTK_TYPEMAP_IN($input, "vtkPolyData") ? 1 : 0;
}

%typemap(typecheck) VTK_TYPEMAP_IN = vtkUnstructuredGrid *;
%typemap(typecheck) VTK_TYPEMAP_IN = vtkPolyData *;

%typemap(out) vtkPolyData*
{
    PyImport_ImportModule("vtk");
    $result =  VTK_TYPEMAP_OUT((vtkPolyData*)$1);
}

%typemap(in) vtkPolyData*
{
    $1 = (vtkPolyData*) VTK_TYPEMAP_IN($input, "vtkPolyData");
    if ($1 == nullptr)
    {
        SWIG_fail;
    }
}

%typemap(out) vtkUnstructuredGrid*
{
    PyImport_ImportModule("vtk");
    $result =  VTK_TYPEMAP_OUT((vtkUnstructuredGrid*)$1);
}


%typemap(in) vtkUnstructuredGrid*
{
    $1 = (vtkUnstructuredGrid*) VTK_TYPEMAP_IN($input, "vtkUnstructuredGrid");
    if ($1 == nullptr)
    {
        SWIG_fail;
    }
}
