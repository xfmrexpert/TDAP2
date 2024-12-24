/* File : TransFE.i */
%module TransFE
%include "std_string.i"
%include "typemaps.i"
%include "cpointer.i"
%include "stl.i"
%include "windows.i"
%include <std_shared_ptr.i>
%include <std_unique_ptr.i>
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
%unique_ptr(Node);
%unique_ptr(MeshFace);
%unique_ptr(MeshEdge);

%typemap(out) std::vector<std::unique_ptr<DOF>> {
    std::vector<DOF*> rawPtrs;
    for (const auto& item : $1) {
        rawPtrs.push_back(item.get());
    }
    $result = rawPtrs;
}

%typemap(out) std::vector<std::unique_ptr<Node>> {
    std::vector<Node*> rawPtrs;
    for (const auto& item : $1) {
        rawPtrs.push_back(item.get());
    }
    $result = rawPtrs;
}

%typemap(out) std::vector<std::unique_ptr<MeshEdge>> {
    std::vector<MeshEdge*> rawPtrs;
    for (const auto& item : $1) {
        rawPtrs.push_back(item.get());
    }
    $result = rawPtrs;
}

%typemap(out) std::vector<std::unique_ptr<MeshFace>> {
    std::vector<MeshFace*> rawPtrs;
    for (const auto& item : $1) {
        rawPtrs.push_back(item.get());
    }
    $result = rawPtrs;
}

%template(NodePtrVec) std::vector<Node*>;
%template(DOFPtrVec) std::vector<DOF*>;
%template(MeshFacePtrVec) std::vector<MeshFace*>;
%template(MeshEdgePtrVec) std::vector<MeshEdge*>;


%include "feprog.h"
%include "MeshDB/mesh.h"
%include "MeshDB/meshentity.h"
%include "MeshDB/meshvertex.h"
%include "MeshDB/meshedge.h"
%include "MeshDB/meshface.h"
%include "MeshDB/node.h"
%include "dof.h"
