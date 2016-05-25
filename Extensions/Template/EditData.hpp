/* EditData.hpp
 * This is where you control what data
 * you want to have at edittime. You
 * are responsible for serializing and
 * deserializing (saving/loading) the
 * data to/from the SerializedED structure
 * as if it were a file on the hard drive.
 * It doesn't need to be efficient; this
 * is just at edittime and once at the
 * start of the runtime.
 */

struct EditData final
{
	/* MyString, MyInt, MyArray_t, MyArray
	 * Example data
	 */
//	stdtstring MyString;
//	int MyInt;
//	typedef std::vector<float> MyArray_t;
//	MyArray_t MyArray;

	/* <default constructor>
	 * This is where you provide default values for
	 * your editdata. This constructor is used
	 * when your extension is first created and
	 * default values are needed.
	 */
	EditData() // : MyString(_T("Hello, world!")), MyInt(1337)
	{
		//MyArray.push_back(3.1415926f);
	}

	/* <copy constructor>
	 * As a convenience in other parts of your code,
	 * you should copy data from another instance
	 * of the EditData class. Make sure you deep-copy
	 * dynamically allocated memory e.g. with pointers.
	 */
	EditData(const EditData &from) // : MyString(from.MyString), MyInt(from.MyInt), MyArray(from.MyArray)
	{
		//
	}

	/* operator=
	 * This is essentially the same as the copy
	 * constructor above, except you are working
	 * with an instance that is already
	 * constructed.
	 */
	EditData &operator=(const EditData &from)
	{
//		MyString = from.MyString;
//		MyInt = from.MyInt;
//		MyArray = from.MyArray;
	}

#ifndef RUN_ONLY
	/* Serialize
	 * This is where you need to "write" data
	 * to SerializedED like a file. Make sure
	 * you can read the data back in the
	 * constructor below!
	 */
	bool Serialize(mv *mV, SerializedED *&SED) const
	{
		//Create an instance of EDOStream, a helper class
		EDOStream os (mV, SED);

		//Write the data you need to save in binary format
		//(you can use text format, but binary is recommended)
//		os.write_string(MyString); //works for c-strings too
//		os.write_value(MyInt); //only works for primitives!
//		os.write_value(MyArray.size()); //need to know how many to load later
//		os.write_sequence(MyArray.begin(), MyArray.end()); //works for c-style arrays too

		//That's it! EDOStream automatically stores the data in your extension's editdata
		return true; //return false in the event of an error
	}
#endif

	/* <constructor>
	 * This is the primary constructor for the
	 * EditData class. Here you will have to
	 * "read" SerializedED like a file and
	 * load back everything that gets saved
	 * above in Serialize. Make sure you check
	 * the version information first, this
	 * constructor is used to update from
	 * older versions of your editdata as well.
	 */
	EditData(SerializedED *SED)
	{
		if(SED->Header.extVersion == 0) //older version
		{
			//code to update from an older version
		}
		else if(SED->Header.extVersion == 1) //current version
		{
			//Create an instance of EDIStream, a helper class
			EDIStream is (SED);
			//Read back the data in the same format that you stored it above
//			MyString = is.read_string();
//			MyInt = is.read_value<int>(); //need to specify the type here
//			MyArray_t::size_type MyArray_size = is.read_value<MyArray_t::size_type>();
//			for(MyArray_t::size_type i = 0; i < MyArray_size; ++i)
//			{
//				MyArray.push_back(is.read_value<MyArray_t::value_type>());
//			}
		}
		else //the version is newer than current
		{
			//Either try to load the data anyway assuming your
			//future self was smart enough to keep the data in
			//the same format with new data at the end, or
			//make an error dialog and load some default data.
//			MessageBox(NULL, _T("The MFA you are trying to load was saved")
//			                 _T("with a newer version of this extension."),
//			                 _T("Error Loading My Extension"), MB_OK);
		}
	}

	/* <destructor>
	 * If you grabbed any memory e.g. with new,
	 * make sure to e.g. delete it in here.
	 */
	~EditData()
	{
		//
	}
};
