//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class Node : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  private bool swigCMemOwnBase;

  internal Node(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwnBase = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(Node obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~Node() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwnBase) {
          swigCMemOwnBase = false;
          TransFE_NativeWrapperPINVOKE.delete_Node(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public Node() : this(TransFE_NativeWrapperPINVOKE.new_Node(), true) {
  }

  public double x() {
    double ret = TransFE_NativeWrapperPINVOKE.Node_x(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double y() {
    double ret = TransFE_NativeWrapperPINVOKE.Node_y(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double z() {
    double ret = TransFE_NativeWrapperPINVOKE.Node_z(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void pt(SWIGTYPE_p_point in_pt) {
    TransFE_NativeWrapperPINVOKE.Node_pt__SWIG_0(swigCPtr, SWIGTYPE_p_point.getCPtr(in_pt));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public SWIGTYPE_p_point pt() {
    SWIGTYPE_p_point ret = new SWIGTYPE_p_point(TransFE_NativeWrapperPINVOKE.Node_pt__SWIG_1(swigCPtr), false);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint getID() {
    uint ret = TransFE_NativeWrapperPINVOKE.Node_getID(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void setID(uint in_ID) {
    TransFE_NativeWrapperPINVOKE.Node_setID(swigCPtr, in_ID);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public DOFPtrVec getDOFs() {
    DOFPtrVec ret = new DOFPtrVec(TransFE_NativeWrapperPINVOKE.Node_getDOFs(swigCPtr), true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public DOF newDOF() {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.Node_newDOF(swigCPtr);
    DOF ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOF(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public MeshEntity getParent() {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.Node_getParent(swigCPtr);
    MeshEntity ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEntity(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void setParent(MeshEntity in_parent) {
    TransFE_NativeWrapperPINVOKE.Node_setParent(swigCPtr, MeshEntity.getCPtr(in_parent));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

}
