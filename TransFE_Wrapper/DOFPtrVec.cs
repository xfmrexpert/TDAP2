//------------------------------------------------------------------------------
// <auto-generated />
//
// This file was automatically generated by SWIG (https://www.swig.org).
// Version 4.1.1
//
// Do not make changes to this file unless you know what you are doing - modify
// the SWIG interface file instead.
//------------------------------------------------------------------------------


public class DOFPtrVec : global::System.IDisposable, global::System.Collections.IEnumerable, global::System.Collections.Generic.IList<DOF>
 {
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  internal DOFPtrVec(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  internal static global::System.Runtime.InteropServices.HandleRef getCPtr(DOFPtrVec obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }

  internal static global::System.Runtime.InteropServices.HandleRef swigRelease(DOFPtrVec obj) {
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

  ~DOFPtrVec() {
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
          TransFE_NativeWrapperPINVOKE.delete_DOFPtrVec(swigCPtr);
        }
        swigCPtr = new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero);
      }
    }
  }

  public DOFPtrVec(global::System.Collections.IEnumerable c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (DOF element in c) {
      this.Add(element);
    }
  }

  public DOFPtrVec(global::System.Collections.Generic.IEnumerable<DOF> c) : this() {
    if (c == null)
      throw new global::System.ArgumentNullException("c");
    foreach (DOF element in c) {
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

  public DOF this[int index]  {
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

  public void CopyTo(DOF[] array)
  {
    CopyTo(0, array, 0, this.Count);
  }

  public void CopyTo(DOF[] array, int arrayIndex)
  {
    CopyTo(0, array, arrayIndex, this.Count);
  }

  public void CopyTo(int index, DOF[] array, int arrayIndex, int count)
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

  public DOF[] ToArray() {
    DOF[] array = new DOF[this.Count];
    this.CopyTo(array);
    return array;
  }

  global::System.Collections.Generic.IEnumerator<DOF> global::System.Collections.Generic.IEnumerable<DOF>.GetEnumerator() {
    return new DOFPtrVecEnumerator(this);
  }

  global::System.Collections.IEnumerator global::System.Collections.IEnumerable.GetEnumerator() {
    return new DOFPtrVecEnumerator(this);
  }

  public DOFPtrVecEnumerator GetEnumerator() {
    return new DOFPtrVecEnumerator(this);
  }

  // Type-safe enumerator
  /// Note that the IEnumerator documentation requires an InvalidOperationException to be thrown
  /// whenever the collection is modified. This has been done for changes in the size of the
  /// collection but not when one of the elements of the collection is modified as it is a bit
  /// tricky to detect unmanaged code that modifies the collection under our feet.
  public sealed class DOFPtrVecEnumerator : global::System.Collections.IEnumerator
    , global::System.Collections.Generic.IEnumerator<DOF>
  {
    private DOFPtrVec collectionRef;
    private int currentIndex;
    private object currentObject;
    private int currentSize;

    public DOFPtrVecEnumerator(DOFPtrVec collection) {
      collectionRef = collection;
      currentIndex = -1;
      currentObject = null;
      currentSize = collectionRef.Count;
    }

    // Type-safe iterator Current
    public DOF Current {
      get {
        if (currentIndex == -1)
          throw new global::System.InvalidOperationException("Enumeration not started.");
        if (currentIndex > currentSize - 1)
          throw new global::System.InvalidOperationException("Enumeration finished.");
        if (currentObject == null)
          throw new global::System.InvalidOperationException("Collection modified.");
        return (DOF)currentObject;
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
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_Clear(swigCPtr);
  }

  public void Add(DOF x) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_Add(swigCPtr, DOF.getCPtr(x));
  }

  private uint size() {
    uint ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_size(swigCPtr);
    return ret;
  }

  private uint capacity() {
    uint ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_capacity(swigCPtr);
    return ret;
  }

  private void reserve(uint n) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_reserve(swigCPtr, n);
  }

  public DOFPtrVec() : this(TransFE_NativeWrapperPINVOKE.new_DOFPtrVec__SWIG_0(), true) {
  }

  public DOFPtrVec(DOFPtrVec other) : this(TransFE_NativeWrapperPINVOKE.new_DOFPtrVec__SWIG_1(DOFPtrVec.getCPtr(other)), true) {
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public DOFPtrVec(int capacity) : this(TransFE_NativeWrapperPINVOKE.new_DOFPtrVec__SWIG_2(capacity), true) {
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  private DOF getitemcopy(int index) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.DOFPtrVec_getitemcopy(swigCPtr, index);
    DOF ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOF(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private DOF getitem(int index) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.DOFPtrVec_getitem(swigCPtr, index);
    DOF ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOF(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  private void setitem(int index, DOF val) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_setitem(swigCPtr, index, DOF.getCPtr(val));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public void AddRange(DOFPtrVec values) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_AddRange(swigCPtr, DOFPtrVec.getCPtr(values));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public DOFPtrVec GetRange(int index, int count) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.DOFPtrVec_GetRange(swigCPtr, index, count);
    DOFPtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOFPtrVec(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Insert(int index, DOF x) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_Insert(swigCPtr, index, DOF.getCPtr(x));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public void InsertRange(int index, DOFPtrVec values) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_InsertRange(swigCPtr, index, DOFPtrVec.getCPtr(values));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveAt(int index) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_RemoveAt(swigCPtr, index);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public void RemoveRange(int index, int count) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_RemoveRange(swigCPtr, index, count);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public static DOFPtrVec Repeat(DOF value, int count) {
    global::System.IntPtr cPtr = TransFE_NativeWrapperPINVOKE.DOFPtrVec_Repeat(DOF.getCPtr(value), count);
    DOFPtrVec ret = (cPtr == global::System.IntPtr.Zero) ? null : new DOFPtrVec(cPtr, true);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
    return ret;
  }

  public void Reverse() {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_Reverse__SWIG_0(swigCPtr);
  }

  public void Reverse(int index, int count) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_Reverse__SWIG_1(swigCPtr, index, count);
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public void SetRange(int index, DOFPtrVec values) {
    TransFE_NativeWrapperPINVOKE.DOFPtrVec_SetRange(swigCPtr, index, DOFPtrVec.getCPtr(values));
    if (TransFE_NativeWrapperPINVOKE.SWIGPendingException.Pending) throw TransFE_NativeWrapperPINVOKE.SWIGPendingException.Retrieve();
  }

  public bool Contains(DOF value) {
    bool ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_Contains(swigCPtr, DOF.getCPtr(value));
    return ret;
  }

  public int IndexOf(DOF value) {
    int ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_IndexOf(swigCPtr, DOF.getCPtr(value));
    return ret;
  }

  public int LastIndexOf(DOF value) {
    int ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_LastIndexOf(swigCPtr, DOF.getCPtr(value));
    return ret;
  }

  public bool Remove(DOF value) {
    bool ret = TransFE_NativeWrapperPINVOKE.DOFPtrVec_Remove(swigCPtr, DOF.getCPtr(value));
    return ret;
  }

}
