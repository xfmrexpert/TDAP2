//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class MeshEdgePtrVec : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<MeshEdge>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MeshEdgePtrVec(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MeshEdgePtrVec obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(MeshEdgePtrVec obj) {
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

  ~MeshEdgePtrVec() {
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
          TransFEPINVOKE.delete_MeshEdgePtrVec(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public MeshEdgePtrVec(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MeshEdge element in c) {
      this.Add(element);
    }
  }

  public MeshEdgePtrVec(global::System.Collections.Generic.IEnumerable<MeshEdge> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MeshEdge element in c) {
      this.Add(element);
    }
  }

  public bool IsFixedSize {
    get {
      return false;
    }
  }

  public bool IsReadOnly {
    get {
      return false;
    }
  }

  public MeshEdge this[int index]  {
    get {
      return getitem(index);
    }
    set {
      setitem(index, value);
    }
  }

  public int Capacity {
    get {
      return (int)capacity();
    }
    set {
      if (value < 0 || (uint)value < size())
        throw new global::System.ArgumentOutOfRangeException("Capacity");
      reserve((uint)value);
    }
  }

  public int Count {
    get {
      return (int)size();
    }
  }

  public bool IsSynchronized {
    get {
      return false;
    }
  }

  public void CopyTo(MeshEdge[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(MeshEdge[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, MeshEdge[] array, int arrayIndex, int count)
  {
    if (array == null)
      throw new global::System.ArgumentNullException("array");
    if (index < 0)
      throw new global::System.ArgumentOutOfRangeException("index", "Value is less than zero");
    if (arrayIndex < 0)
      throw new global::System.ArgumentOutOfRangeException("arrayIndex", "Value is less than zero");
    if (count < 0)
      throw new global::System.ArgumentOutOfRangeException("count", "Value is less than zero");
    if (array.Rank > 1)
      throw new global::System.ArgumentException("Multi dimensional array.", "array");
    if (index+count > this.Count || arrayIndex+count > array.Length)
      throw new global::System.ArgumentException("Number of elements to copy is too large.");
    for (int i=0; i<count; i++)
      array.SetValue(getitemcopy(index+i), arrayIndex+i);
  }

  public MeshEdge[] ToArray() {
    MeshEdge[] array = new MeshEdge[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<MeshEdge> global::System.Collections.Generic.IEnumerable<MeshEdge>.GetEnumerator() {
    return new MeshEdgePtrVecEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new MeshEdgePtrVecEnumerator(this);
  }

  public MeshEdgePtrVecEnumerator GetEnumerator() {
    return new MeshEdgePtrVecEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class MeshEdgePtrVecEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<MeshEdge>
  {
    private MeshEdgePtrVec collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public MeshEdgePtrVecEnumerator(MeshEdgePtrVec collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public MeshEdge Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (MeshEdge)currentObject;
      }
    }

    // Type-unsafe IEnumerator.Current
    object global::System.Collections.IEnumerator.Current {
      get {
        return Current;
      }
    }

    public bool MoveNext() {
      int size = collectionRef.Count;
      bool moveOkay = (currentIndex+1 < size) && (size == currentSize);
      if (moveOkay) {
        currentIndex++;
        currentObject = collectionRef[currentIndex];
      } else {
        currentObject = null;
      }
      return moveOkay;
    }

    public void Reset() {
      currentIndex = -1;
      currentObject = null;
      if (collectionRef.Count != currentSize) {
        throw new global::System.InvalidOperationException("Collection modified.");
      }
    }

    public void Dispose() {
        currentIndex = -1;
        currentObject = null;
    }
  }

  public void Clear() {
    TransFEPINVOKE.MeshEdgePtrVec_Clear(swigCPtr);
  }

  public void Add(MeshEdge x) {
    TransFEPINVOKE.MeshEdgePtrVec_Add(swigCPtr, MeshEdge.getCPtr(x));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = TransFEPINVOKE.MeshEdgePtrVec_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = TransFEPINVOKE.MeshEdgePtrVec_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    TransFEPINVOKE.MeshEdgePtrVec_reserve(swigCPtr, n);
  }

  public MeshEdgePtrVec() : this(TransFEPINVOKE.new_MeshEdgePtrVec__SWIG_0(), true) {
  }

  public MeshEdgePtrVec(MeshEdgePtrVec other) : this(TransFEPINVOKE.new_MeshEdgePtrVec__SWIG_1(MeshEdgePtrVec.getCPtr(other)), true) {
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public MeshEdgePtrVec(int capacity) : this(TransFEPINVOKE.new_MeshEdgePtrVec__SWIG_2(capacity), true) {
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  private MeshEdge getitemcopy(int index) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshEdgePtrVec_getitemcopy(swigCPtr, index);
    MeshEdge ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdge(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private MeshEdge getitem(int index) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshEdgePtrVec_getitem(swigCPtr, index);
    MeshEdge ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdge(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, MeshEdge val) {
    TransFEPINVOKE.MeshEdgePtrVec_setitem(swigCPtr, index, MeshEdge.getCPtr(val));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(MeshEdgePtrVec values) {
    TransFEPINVOKE.MeshEdgePtrVec_AddRange(swigCPtr, MeshEdgePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public MeshEdgePtrVec GetRange(int index, int count) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshEdgePtrVec_GetRange(swigCPtr, index, count);
    MeshEdgePtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdgePtrVec(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, MeshEdge x) {
    TransFEPINVOKE.MeshEdgePtrVec_Insert(swigCPtr, index, MeshEdge.getCPtr(x));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, MeshEdgePtrVec values) {
    TransFEPINVOKE.MeshEdgePtrVec_InsertRange(swigCPtr, index, MeshEdgePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    TransFEPINVOKE.MeshEdgePtrVec_RemoveAt(swigCPtr, index);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    TransFEPINVOKE.MeshEdgePtrVec_RemoveRange(swigCPtr, index, count);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public static MeshEdgePtrVec Repeat(MeshEdge value, int count) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshEdgePtrVec_Repeat(MeshEdge.getCPtr(value), count);
    MeshEdgePtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshEdgePtrVec(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    TransFEPINVOKE.MeshEdgePtrVec_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    TransFEPINVOKE.MeshEdgePtrVec_Reverse__SWIG_1(swigCPtr, index, count);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, MeshEdgePtrVec values) {
    TransFEPINVOKE.MeshEdgePtrVec_SetRange(swigCPtr, index, MeshEdgePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

}
