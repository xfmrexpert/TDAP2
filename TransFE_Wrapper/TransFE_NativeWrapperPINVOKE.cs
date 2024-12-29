//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


class TransFE_NativeWrapperPINVOKE {

  protected class SWIGExceptionHelper {

    public delegate void ExceptionDelegate(string message);
    public delegate void ExceptionArgumentDelegate(string message, string paramName);

    static ExceptionDelegate applicationDelegate = new ExceptionDelegate(SetPendingApplicationException);
    static ExceptionDelegate arithmeticDelegate = new ExceptionDelegate(SetPendingArithmeticException);
    static ExceptionDelegate divideByZeroDelegate = new ExceptionDelegate(SetPendingDivideByZeroException);
    static ExceptionDelegate indexOutOfRangeDelegate = new ExceptionDelegate(SetPendingIndexOutOfRangeException);
    static ExceptionDelegate invalidCastDelegate = new ExceptionDelegate(SetPendingInvalidCastException);
    static ExceptionDelegate invalidOperationDelegate = new ExceptionDelegate(SetPendingInvalidOperationException);
    static ExceptionDelegate ioDelegate = new ExceptionDelegate(SetPendingIOException);
    static ExceptionDelegate nullReferenceDelegate = new ExceptionDelegate(SetPendingNullReferenceException);
    static ExceptionDelegate outOfMemoryDelegate = new ExceptionDelegate(SetPendingOutOfMemoryException);
    static ExceptionDelegate overflowDelegate = new ExceptionDelegate(SetPendingOverflowException);
    static ExceptionDelegate systemDelegate = new ExceptionDelegate(SetPendingSystemException);

    static ExceptionArgumentDelegate argumentDelegate = new ExceptionArgumentDelegate(SetPendingArgumentException);
    static ExceptionArgumentDelegate argumentNullDelegate = new ExceptionArgumentDelegate(SetPendingArgumentNullException);
    static ExceptionArgumentDelegate argumentOutOfRangeDelegate = new ExceptionArgumentDelegate(SetPendingArgumentOutOfRangeException);

    [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="SWIGRegisterExceptionCallbacks_TransFE_NativeWrapper")]
    public static extern void SWIGRegisterExceptionCallbacks_TransFE_NativeWrapper(
                                ExceptionDelegate applicationDelegate,
                                ExceptionDelegate arithmeticDelegate,
                                ExceptionDelegate divideByZeroDelegate, 
                                ExceptionDelegate indexOutOfRangeDelegate, 
                                ExceptionDelegate invalidCastDelegate,
                                ExceptionDelegate invalidOperationDelegate,
                                ExceptionDelegate ioDelegate,
                                ExceptionDelegate nullReferenceDelegate,
                                ExceptionDelegate outOfMemoryDelegate, 
                                ExceptionDelegate overflowDelegate, 
                                ExceptionDelegate systemExceptionDelegate);

    [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="SWIGRegisterExceptionArgumentCallbacks_TransFE_NativeWrapper")]
    public static extern void SWIGRegisterExceptionCallbacksArgument_TransFE_NativeWrapper(
                                ExceptionArgumentDelegate argumentDelegate,
                                ExceptionArgumentDelegate argumentNullDelegate,
                                ExceptionArgumentDelegate argumentOutOfRangeDelegate);

    static void SetPendingApplicationException(string message) {
      SWIGPendingException.Set(new global::System.ApplicationException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingArithmeticException(string message) {
      SWIGPendingException.Set(new global::System.ArithmeticException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingDivideByZeroException(string message) {
      SWIGPendingException.Set(new global::System.DivideByZeroException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingIndexOutOfRangeException(string message) {
      SWIGPendingException.Set(new global::System.IndexOutOfRangeException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingInvalidCastException(string message) {
      SWIGPendingException.Set(new global::System.InvalidCastException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingInvalidOperationException(string message) {
      SWIGPendingException.Set(new global::System.InvalidOperationException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingIOException(string message) {
      SWIGPendingException.Set(new global::System.IO.IOException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingNullReferenceException(string message) {
      SWIGPendingException.Set(new global::System.NullReferenceException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingOutOfMemoryException(string message) {
      SWIGPendingException.Set(new global::System.OutOfMemoryException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingOverflowException(string message) {
      SWIGPendingException.Set(new global::System.OverflowException(message, SWIGPendingException.Retrieve()));
    }
    static void SetPendingSystemException(string message) {
      SWIGPendingException.Set(new global::System.SystemException(message, SWIGPendingException.Retrieve()));
    }

    static void SetPendingArgumentException(string message, string paramName) {
      SWIGPendingException.Set(new global::System.ArgumentException(message, paramName, SWIGPendingException.Retrieve()));
    }
    static void SetPendingArgumentNullException(string message, string paramName) {
      global::System.Exception e = SWIGPendingException.Retrieve();
      if (e != null) message = message + " Inner Exception: " + e.Message;
      SWIGPendingException.Set(new global::System.ArgumentNullException(paramName, message));
    }
    static void SetPendingArgumentOutOfRangeException(string message, string paramName) {
      global::System.Exception e = SWIGPendingException.Retrieve();
      if (e != null) message = message + " Inner Exception: " + e.Message;
      SWIGPendingException.Set(new global::System.ArgumentOutOfRangeException(paramName, message));
    }

    static SWIGExceptionHelper() {
      SWIGRegisterExceptionCallbacks_TransFE_NativeWrapper(
                                applicationDelegate,
                                arithmeticDelegate,
                                divideByZeroDelegate,
                                indexOutOfRangeDelegate,
                                invalidCastDelegate,
                                invalidOperationDelegate,
                                ioDelegate,
                                nullReferenceDelegate,
                                outOfMemoryDelegate,
                                overflowDelegate,
                                systemDelegate);

      SWIGRegisterExceptionCallbacksArgument_TransFE_NativeWrapper(
                                argumentDelegate,
                                argumentNullDelegate,
                                argumentOutOfRangeDelegate);
    }
  }

  protected static SWIGExceptionHelper swigExceptionHelper = new SWIGExceptionHelper();

  public class SWIGPendingException {
    [global::System.ThreadStatic]
    private static global::System.Exception pendingException = null;
    private static int numExceptionsPending = 0;
    private static global::System.Object exceptionsLock = null;

    public static bool Pending {
      get {
        bool pending = false;
        if (numExceptionsPending > 0)
          if (pendingException != null)
            pending = true;
        return pending;
      } 
    }

    public static void Set(global::System.Exception e) {
      if (pendingException != null)
        throw new global::System.ApplicationException("FATAL: An earlier pending exception from unmanaged code was missed and thus not thrown (" + pendingException.ToString() + ")", e);
      pendingException = e;
      lock(exceptionsLock) {
        numExceptionsPending++;
      }
    }

    public static global::System.Exception Retrieve() {
      global::System.Exception e = null;
      if (numExceptionsPending > 0) {
        if (pendingException != null) {
          e = pendingException;
          pendingException = null;
          lock(exceptionsLock) {
            numExceptionsPending--;
          }
        }
      }
      return e;
    }

    static SWIGPendingException() {
      exceptionsLock = new global::System.Object();
    }
  }


  protected class SWIGStringHelper {

    public delegate string SWIGStringDelegate(string message);
    static SWIGStringDelegate stringDelegate = new SWIGStringDelegate(CreateString);

    [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="SWIGRegisterStringCallback_TransFE_NativeWrapper")]
    public static extern void SWIGRegisterStringCallback_TransFE_NativeWrapper(SWIGStringDelegate stringDelegate);

    static string CreateString(string cString) {
      return cString;
    }

    static SWIGStringHelper() {
      SWIGRegisterStringCallback_TransFE_NativeWrapper(stringDelegate);
    }
  }

  static protected SWIGStringHelper swigStringHelper = new SWIGStringHelper();


  static TransFE_NativeWrapperPINVOKE() {
  }


  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_FEProg_theAnalysis_set")]
  public static extern void FEProg_theAnalysis_set(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_FEProg_theAnalysis_get")]
  public static extern global::System.IntPtr FEProg_theAnalysis_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_FEProg")]
  public static extern global::System.IntPtr new_FEProg();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_FEProg")]
  public static extern void delete_FEProg(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_FEProg_run_FEA")]
  public static extern global::System.IntPtr FEProg_run_FEA(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_Mesh")]
  public static extern global::System.IntPtr new_Mesh();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_Mesh")]
  public static extern void delete_Mesh(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_newRegion")]
  public static extern global::System.IntPtr Mesh_newRegion(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_newFace")]
  public static extern global::System.IntPtr Mesh_newFace(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_newEdge")]
  public static extern global::System.IntPtr Mesh_newEdge(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_newVertex")]
  public static extern global::System.IntPtr Mesh_newVertex(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_newNode")]
  public static extern global::System.IntPtr Mesh_newNode(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getRegion")]
  public static extern global::System.IntPtr Mesh_getRegion(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getFace")]
  public static extern global::System.IntPtr Mesh_getFace(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getEdge")]
  public static extern global::System.IntPtr Mesh_getEdge(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getVertex")]
  public static extern global::System.IntPtr Mesh_getVertex(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getNode")]
  public static extern global::System.IntPtr Mesh_getNode(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getFaces")]
  public static extern global::System.IntPtr Mesh_getFaces(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getNodes")]
  public static extern global::System.IntPtr Mesh_getNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_numRegions")]
  public static extern uint Mesh_numRegions(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_numFaces")]
  public static extern uint Mesh_numFaces(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_numEdges")]
  public static extern uint Mesh_numEdges(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_numVertexes")]
  public static extern uint Mesh_numVertexes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_numNodes")]
  public static extern uint Mesh_numNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_readMesh")]
  public static extern void Mesh_readMesh(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_readAttributes")]
  public static extern void Mesh_readAttributes(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_getGeomEntity")]
  public static extern global::System.IntPtr Mesh_getGeomEntity(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Mesh_writeMesh")]
  public static extern void Mesh_writeMesh(global::System.Runtime.InteropServices.HandleRef jarg1, string jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_MeshEntity")]
  public static extern void delete_MeshEntity(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_get_dimensions")]
  public static extern int MeshEntity_get_dimensions(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_getNode")]
  public static extern global::System.IntPtr MeshEntity_getNode(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_setNode")]
  public static extern void MeshEntity_setNode(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_getClassification")]
  public static extern global::System.IntPtr MeshEntity_getClassification(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_setClassification")]
  public static extern void MeshEntity_setClassification(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_getClassificationID")]
  public static extern int MeshEntity_getClassificationID(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_getNodes")]
  public static extern global::System.IntPtr MeshEntity_getNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_getID")]
  public static extern uint MeshEntity_getID(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEntity_setID")]
  public static extern void MeshEntity_setID(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_MeshVertex")]
  public static extern global::System.IntPtr new_MeshVertex();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_MeshVertex")]
  public static extern void delete_MeshVertex(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_Edges")]
  public static extern global::System.IntPtr MeshVertex_Edges(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_getEdge")]
  public static extern global::System.IntPtr MeshVertex_getEdge(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_addEdge")]
  public static extern void MeshVertex_addEdge(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_numEdges")]
  public static extern uint MeshVertex_numEdges(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_get_dimensions")]
  public static extern int MeshVertex_get_dimensions(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_getNodes")]
  public static extern global::System.IntPtr MeshVertex_getNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_MeshEdge")]
  public static extern global::System.IntPtr new_MeshEdge();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_MeshEdge")]
  public static extern void delete_MeshEdge(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_getVertex")]
  public static extern global::System.IntPtr MeshEdge_getVertex(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_Faces")]
  public static extern global::System.IntPtr MeshEdge_Faces(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_getFace")]
  public static extern global::System.IntPtr MeshEdge_getFace(global::System.Runtime.InteropServices.HandleRef jarg1, int jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_addVertex")]
  public static extern void MeshEdge_addVertex(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2, int jarg3);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_addFace")]
  public static extern void MeshEdge_addFace(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_otherVertex")]
  public static extern global::System.IntPtr MeshEdge_otherVertex(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_isConnected__SWIG_0")]
  public static extern bool MeshEdge_isConnected__SWIG_0(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_isConnected__SWIG_1")]
  public static extern bool MeshEdge_isConnected__SWIG_1(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_getNodes")]
  public static extern global::System.IntPtr MeshEdge_getNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_getVertices")]
  public static extern global::System.IntPtr MeshEdge_getVertices(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_get_dimensions")]
  public static extern int MeshEdge_get_dimensions(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_MeshFace")]
  public static extern global::System.IntPtr new_MeshFace();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_MeshFace")]
  public static extern void delete_MeshFace(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_Edges")]
  public static extern global::System.IntPtr MeshFace_Edges(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_getEdge")]
  public static extern global::System.IntPtr MeshFace_getEdge(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_getRegion")]
  public static extern global::System.IntPtr MeshFace_getRegion(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_addEdge")]
  public static extern void MeshFace_addEdge(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_addRegion")]
  public static extern void MeshFace_addRegion(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_numEdges")]
  public static extern uint MeshFace_numEdges(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_area")]
  public static extern double MeshFace_area(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_get_dimensions")]
  public static extern int MeshFace_get_dimensions(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_getNodes")]
  public static extern global::System.IntPtr MeshFace_getNodes(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_getVertices")]
  public static extern global::System.IntPtr MeshFace_getVertices(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_MeshEdges_set")]
  public static extern void MeshFace_MeshEdges_set(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_MeshEdges_get")]
  public static extern global::System.IntPtr MeshFace_MeshEdges_get(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_new_Node")]
  public static extern global::System.IntPtr new_Node();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_delete_Node")]
  public static extern void delete_Node(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_x")]
  public static extern double Node_x(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_y")]
  public static extern double Node_y(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_z")]
  public static extern double Node_z(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_pt__SWIG_0")]
  public static extern void Node_pt__SWIG_0(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_pt__SWIG_1")]
  public static extern global::System.IntPtr Node_pt__SWIG_1(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_getID")]
  public static extern uint Node_getID(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_setID")]
  public static extern void Node_setID(global::System.Runtime.InteropServices.HandleRef jarg1, uint jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_getParent")]
  public static extern global::System.IntPtr Node_getParent(global::System.Runtime.InteropServices.HandleRef jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_Node_setParent")]
  public static extern void Node_setParent(global::System.Runtime.InteropServices.HandleRef jarg1, global::System.Runtime.InteropServices.HandleRef jarg2);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_ndof_set")]
  public static extern void ndof_set(uint jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_ndof_get")]
  public static extern uint ndof_get();

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshVertex_SWIGSmartPtrUpcast")]
  public static extern global::System.IntPtr MeshVertex_SWIGSmartPtrUpcast(global::System.IntPtr jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshEdge_SWIGSmartPtrUpcast")]
  public static extern global::System.IntPtr MeshEdge_SWIGSmartPtrUpcast(global::System.IntPtr jarg1);

  [global::System.Runtime.InteropServices.DllImport("TransFE_NativeWrapper", EntryPoint="CSharp_MeshFace_SWIGSmartPtrUpcast")]
  public static extern global::System.IntPtr MeshFace_SWIGSmartPtrUpcast(global::System.IntPtr jarg1);
}
