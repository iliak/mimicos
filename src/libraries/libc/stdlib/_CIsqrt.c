//extern "C"
//{


float __declspec(naked) _CIsqrt()
{
   _asm {
      fsqrt
      ret
   };
};


//};

