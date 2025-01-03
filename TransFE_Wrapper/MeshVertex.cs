//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class MeshVertex : MeshEntity {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  private bool swigCMemOwnDerived;

  internal MeshVertex(global::System.IntPtr cPtr, bool cMemoryOwn) : base(TransFE_NativeWrapperPINVOKE.MeshVertex_SWIGSmartPtrUpcast(cPtr), true) {
    swigCMemOwnDerived = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MeshVertex obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  protected override void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwnDerived) {
          swigCMemOwnDerived = false;
          TransFE_NativeWrapperPINVOKE.delete_MeshVertex(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
      base.Dispose(disposing);
    }
  }

  public MeshVertex() : this(TransFE_NativeWrapperPINVOKE.new_MeshVertex(), true) {
  }

  public MeshEdgePtrVec Edges() {
    MeshEdgePtrVec ret = new MeshEdgePtrVec(TransFE_NativeWrapperPINVOKE.MeshVertex_Edges(swigCPtr), false);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MeshEdge getEdge(int n) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.MeshVertex_getEdge(swigCPtr, n);
    MeshEdge ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdge(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void addEdge(MeshEdge arg0) {
    TransFE_NativeWrapperPINVOKE.MeshVertex_addEdge(swigCPtr, MeshEdge.getCPtr(arg0));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public uint numEdges() {
    uint ret = TransFE_NativeWrapperPINVOKE.MeshVertex_numEdges(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override int get_dimensions() {
    int ret = TransFE_NativeWrapperPINVOKE.MeshVertex_get_dimensions(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public override NodePtrVec getNodes() {
    NodePtrVec ret = new NodePtrVec(TransFE_NativeWrapperPINVOKE.MeshVertex_getNodes(swigCPtr), true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}
