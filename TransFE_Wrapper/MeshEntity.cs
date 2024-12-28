//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class MeshEntity : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  private bool swigCMemOwnBase;

  internal MeshEntity(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwnBase = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MeshEntity obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  ~MeshEntity() {
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
          TransFE_NativeWrapperPINVOKE.delete_MeshEntity(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public virtual int get_dimensions() {
    int ret = TransFE_NativeWrapperPINVOKE.MeshEntity_get_dimensions(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public Node getNode() {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.MeshEntity_getNode(swigCPtr);
    Node ret = (cPtr == global::System.IntPtr.Zero) ? null : new Node(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void setNode(Node in_node) {
    TransFE_NativeWrapperPINVOKE.MeshEntity_setNode(swigCPtr, Node.getCPtr(in_node));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public SWIGTYPE_p_GeomEntity getClassification() {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.MeshEntity_getClassification(swigCPtr);
    SWIGTYPE_p_GeomEntity ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_GeomEntity(cPtr, false);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void setClassification(SWIGTYPE_p_GeomEntity arg0) {
    TransFE_NativeWrapperPINVOKE.MeshEntity_setClassification(swigCPtr, SWIGTYPE_p_GeomEntity.getCPtr(arg0));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public int getClassificationID() {
    int ret = TransFE_NativeWrapperPINVOKE.MeshEntity_getClassificationID(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public virtual NodePtrVec getNodes() {
    NodePtrVec ret = new NodePtrVec(TransFE_NativeWrapperPINVOKE.MeshEntity_getNodes(swigCPtr), true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public uint getID() {
    uint ret = TransFE_NativeWrapperPINVOKE.MeshEntity_getID(swigCPtr);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void setID(uint in_ID) {
    TransFE_NativeWrapperPINVOKE.MeshEntity_setID(swigCPtr, in_ID);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

}
