#pragma once

#ifdef NEO_EXPORT
//export
#define NEO_API __declspec(dllexport)
#else
//import
#define NEO_API __declspec(dllimport)
#endif // NEXPORT