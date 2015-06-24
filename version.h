#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "06";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] =  "15.06";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 1;
	static const long BUILD  = 166;
	static const long REVISION  = 971;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 406;
	#define RC_FILEVERSION 1,1,166,971
	#define RC_FILEVERSION_STRING "1, 1, 166, 971\0"
	static const char FULLVERSION_STRING [] = "1.1.166.971";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 66;
	

}
#endif //VERSION_H
