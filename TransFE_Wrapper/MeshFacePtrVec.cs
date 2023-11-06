//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class MeshFacePtrVec : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IEnumerable<MeshFace>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal MeshFacePtrVec(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(MeshFacePtrVec obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(MeshFacePtrVec obj) {
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

  ~MeshFacePtrVec() {
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
          TransFEPINVOKE.delete_MeshFacePtrVec(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public MeshFacePtrVec(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MeshFace element in c) {
      this.Add(element);
    }
  }

  public MeshFacePtrVec(global::System.Collections.Generic.IEnumerable<MeshFace> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (MeshFace element in c) {
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

  public MeshFace this[int index]  {
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

  public void CopyTo(MeshFace[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(MeshFace[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, MeshFace[] array, int arrayIndex, int count)
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

  public MeshFace[] ToArray() {
    MeshFace[] array = new MeshFace[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<MeshFace> global::System.Collections.Generic.IEnumerable<MeshFace>.GetEnumerator() {
    return new MeshFacePtrVecEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new MeshFacePtrVecEnumerator(this);
  }

  public MeshFacePtrVecEnumerator GetEnumerator() {
    return new MeshFacePtrVecEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class MeshFacePtrVecEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<MeshFace>
  {
    private MeshFacePtrVec collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public MeshFacePtrVecEnumerator(MeshFacePtrVec collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public MeshFace Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (MeshFace)currentObject;
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
    TransFEPINVOKE.MeshFacePtrVec_Clear(swigCPtr);
  }

  public void Add(MeshFace x) {
    TransFEPINVOKE.MeshFacePtrVec_Add(swigCPtr, MeshFace.getCPtr(x));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  private uint size() {
    uint ret = TransFEPINVOKE.MeshFacePtrVec_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = TransFEPINVOKE.MeshFacePtrVec_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    TransFEPINVOKE.MeshFacePtrVec_reserve(swigCPtr, n);
  }

  public MeshFacePtrVec() : this(TransFEPINVOKE.new_MeshFacePtrVec__SWIG_0(), true) {
  }

  public MeshFacePtrVec(MeshFacePtrVec other) : this(TransFEPINVOKE.new_MeshFacePtrVec__SWIG_1(MeshFacePtrVec.getCPtr(other)), true) {
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public MeshFacePtrVec(int capacity) : this(TransFEPINVOKE.new_MeshFacePtrVec__SWIG_2(capacity), true) {
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  private MeshFace getitemcopy(int index) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFacePtrVec_getitemcopy(swigCPtr, index);
    MeshFace ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshFace(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private MeshFace getitem(int index) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFacePtrVec_getitem(swigCPtr, index);
    MeshFace ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshFace(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, MeshFace val) {
    TransFEPINVOKE.MeshFacePtrVec_setitem(swigCPtr, index, MeshFace.getCPtr(val));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(MeshFacePtrVec values) {
    TransFEPINVOKE.MeshFacePtrVec_AddRange(swigCPtr, MeshFacePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public MeshFacePtrVec GetRange(int index, int count) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFacePtrVec_GetRange(swigCPtr, index, count);
    MeshFacePtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshFacePtrVec(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, MeshFace x) {
    TransFEPINVOKE.MeshFacePtrVec_Insert(swigCPtr, index, MeshFace.getCPtr(x));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, MeshFacePtrVec values) {
    TransFEPINVOKE.MeshFacePtrVec_InsertRange(swigCPtr, index, MeshFacePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    TransFEPINVOKE.MeshFacePtrVec_RemoveAt(swigCPtr, index);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    TransFEPINVOKE.MeshFacePtrVec_RemoveRange(swigCPtr, index, count);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public static MeshFacePtrVec Repeat(MeshFace value, int count) {
    global::System.IntPtr cPtr = TransFEPINVOKE.MeshFacePtrVec_Repeat(MeshFace.getCPtr(value), count);
    MeshFacePtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new MeshFacePtrVec(cPtr, true);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    TransFEPINVOKE.MeshFacePtrVec_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    TransFEPINVOKE.MeshFacePtrVec_Reverse__SWIG_1(swigCPtr, index, count);
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, MeshFacePtrVec values) {
    TransFEPINVOKE.MeshFacePtrVec_SetRange(swigCPtr, index, MeshFacePtrVec.getCPtr(values));
    if (TransFEPINVOKE.SWIGPendingException.Pending) throw TransFEPINVOKE.SWIGPendingException.Retrieve();
  }

}
