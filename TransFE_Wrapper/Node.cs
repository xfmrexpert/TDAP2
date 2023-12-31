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
          TransFEPINVOKE.delete_Node(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public double x() {
    double ret = TransFEPINVOKE.Node_x(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double y() {
    double ret = TransFEPINVOKE.Node_y(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public double z() {
    double ret = TransFEPINVOKE.Node_z(swigCPtr);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void pt(SWIGTYPE_p_point in_pt) {
    TransFEPINVOKE.Node_pt__SWIG_0(swigCPtr, SWIGTYPE_p_point.getCPtr(in_pt));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public SWIGTYPE_p_point pt() {
    SWIGTYPE_p_point ret = new SWIGTYPE_p_point(TransFEPINVOKE.Node_pt__SWIG_1(swigCPtr), true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public long ID {
    set {
      TransFEPINVOKE.Node_ID_set(swigCPtr, value);
      if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    } 
    get {
      long ret = TransFEPINVOKE.Node_ID_get(swigCPtr);
      if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
      return ret;
    } 
  }

  public Node() : this(TransFEPINVOKE.new_Node(), true) {
  }

  public DOFPtrVec getDOFs() {
    DOFPtrVec ret = new DOFPtrVec(TransFEPINVOKE.Node_getDOFs(swigCPtr), false);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public DOF newDOF() {
    global::System.IntPtr cPtr = TransFEPINVOKE.Node_newDOF(swigCPtr);
    DOF ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOF(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}
