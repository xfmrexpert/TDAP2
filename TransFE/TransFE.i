/* File : TransFE.i */
%module TransFE
%include "std_string.i"
%include "typemaps.i"
%include "cpointer.i"
%include "stl.i"
%include "windows.i"
%include <std_shared_ptr.i>
%include <std_vector.i>

%{
#include "feprog.h"
#include "MeshDB/mesh.h"
#include "MeshDB/meshentity.h"
#include "MeshDB/meshvertex.h"
#include "MeshDB/meshedge.h"
#include "MeshDB/meshface.h"
#include "MeshDB/node.h"
#include "dof.h"
%}

%shared_ptr(Mesh);
%shared_ptr(DOF);
%shared_ptr(Node);
%shared_ptr(MeshEntity);
%shared_ptr(MeshVertex);
%shared_ptr(MeshEdge);
%shared_ptr(MeshFace);
%shared_ptr(MeshRegion);

%template(NodePtrVec) std::vector<shared_ptr<Node>>;
%template(DOFPtrVec) std::vector<shared_ptr<DOF>>;
%template(MeshFacePtrVec) std::vector<shared_ptr<MeshFace>>;
%template(MeshEdgePtrVec) std::vector<shared_ptr<MeshEdge>>;


%include "feprog.h"
%include "MeshDB/mesh.h"
%include "MeshDB/meshentity.h"
%include "MeshDB/meshvertex.h"
%include "MeshDB/meshedge.h"
%include "MeshDB/meshface.h"
%include "MeshDB/node.h"
%include "dof.h"
