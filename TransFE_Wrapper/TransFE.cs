//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.3.0
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class TransFE {
  public static uint ndof {
    set {
      TransFEPINVOKE.ndof_set(value);
    } 
    get {
      uint ret = TransFEPINVOKE.ndof_get();
      return ret;
    } 
  }

  public static readonly int DOF_Free = TransFEPINVOKE.DOF_Free_get();
  public static readonly int DOF_Zero = TransFEPINVOKE.DOF_Zero_get();
  public static readonly int DOF_Fixed = TransFEPINVOKE.DOF_Fixed_get();
}
