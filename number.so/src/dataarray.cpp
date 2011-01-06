#include "number.so.h"

namespace CAS
{
  DataArray::DataArray(int length)
  {
    Length = length;
    Data = new UInt32[length];
    RefCount = 1;
  };

  DataArray::DataArray( const DataArray& value )
  {
    Length = value.Length;
    Data = new UInt32[Length];
    for (int i = 0 ; i < Length ; i++)
      {
	Data[i] = value.Data[i];
      }
    RefCount = 1;
  };

  DataArray::~DataArray()
  {
    delete []Data;
    Data = NULL;
  };
};
