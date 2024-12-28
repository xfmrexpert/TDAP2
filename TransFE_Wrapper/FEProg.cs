//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class FEProg : global::System.IDisposable {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal FEProg(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(FEProg obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(FEProg obj) {
    if (obj != null) {
      if (!obj.swigCMemOwn)
        throw new global::System.ApplicationException("Cannot release ownership as memory is not owned");
      global::System.Runtime.InteropServices.HandleRef ptr = obj.swigCPtr;
      obj.swigCMemOwn = false;
      obj.Dispose();
      return ptr;
    } else {
      return new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
    }
  }

  ~FEProg() {
    Dispose(false);
  }

  public void Dispose() {
    Dispose(true);
    global::System.GC.SuppressFinalize(this);
  }

  protected virtual void Dispose(bool disposing) {
    lock(this) {
      if (swigCPtr.Handle != global::System.IntPtr.Zero) {
        if (swigCMemOwn) {
          swigCMemOwn = false;
          TransFE_NativeWrapperPINVOKE.delete_FEProg(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public SWIGTYPE_p_MagAxiStaticAnalysis theAnalysis {
    set {
      TransFE_NativeWrapperPINVOKE.FEProg_theAnalysis_set(swigCPtr, SWIGTYPE_p_MagAxiStaticAnalysis.getCPtr(value));
    } 
    get {
      global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.FEProg_theAnalysis_get(swigCPtr);
      SWIGTYPE_p_MagAxiStaticAnalysis ret = (cPtr == global::System.IntPtr.Zero) ? null : new SWIGTYPE_p_MagAxiStaticAnalysis(cPtr, false);
      return ret;
    } 
  }

  public FEProg() : this(TransFE_NativeWrapperPINVOKE.new_FEProg(), true) {
  }

  public Mesh run_FEA(string filename, int formulation) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.FEProg_run_FEA(swigCPtr, filename, formulation);
    Mesh ret = (cPtr == global::System.IntPtr.Zero) ? null : new Mesh(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

}
