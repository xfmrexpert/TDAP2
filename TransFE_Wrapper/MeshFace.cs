//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class MeshFace : MeshEntity {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  private bool swigCMemOwnDerived;

  internal MeshFace(global::System.IntPtr cPtr, bool cMemoryOwn) : base(TransFEPINVOKE.MeshFace_SWIGSmartPtrUpcast(cPtr), true) {
    swigCMemOwnDerived = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MeshFace obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwnDerived) {
          swigCMemOwnDerived = false;
          TransFEPINVOKE.delete_MeshFace(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public MeshFace() : this(TransFEPINVOKE.new_MeshFace(), true) {
  }

  public MeshEdgePtrVec Edges() {
    MeshEdgePtrVec ret = new MeshEdgePtrVec(TransFEPINVOKE.MeshFace_Edges(swigCPtr), false);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MeshEdge getEdge(uint n) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFace_getEdge(swigCPtr, n);
    MeshEdge ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdge(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_MeshRegion getRegion(uint n) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFace_getRegion(swigCPtr, n);
    SWIGTYPE_p_MeshRegion ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_MeshRegion(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void addEdge(MeshEdge new_edge) {
    TransFEPINVOKE.MeshFace_addEdge(swigCPtr, MeshEdge.getCPtr(new_edge));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void addRegion(SWIGTYPE_p_MeshRegion new_region) {
    TransFEPINVOKE.MeshFace_addRegion(swigCPtr, SWIGTYPE_p_MeshRegion.getCPtr(new_region));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public uint numEdges() {
    uint ret = TransFEPINVOKE.MeshFace_numEdges(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double area() {
    double ret = TransFEPINVOKE.MeshFace_area(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int get_dimensions() {
    int ret = TransFEPINVOKE.MeshFace_get_dimensions(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public NodePtrVec getNodes() {
    NodePtrVec ret = new NodePtrVec(TransFEPINVOKE.MeshFace_getNodes(swigCPtr), true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public SWIGTYPE_p_std__vectorT_MeshVertex_const_p_t getVertices() {
    SWIGTYPE_p_std__vectorT_MeshVertex_const_p_t ret = new SWIGTYPE_p_std__vectorT_MeshVertex_const_p_t(TransFEPINVOKE.MeshFace_getVertices(swigCPtr), true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MeshEdgePtrVec MeshEdges {
    set {
      TransFEPINVOKE.MeshFace_MeshEdges_set(swigCPtr, MeshEdgePtrVec.getCPtr(value));
      if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      global::System.IntPtr cPtr = TransFEPINVOKE.MeshFace_MeshEdges_get(swigCPtr);
      MeshEdgePtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdgePtrVec(cPtr, false);
      if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

}
